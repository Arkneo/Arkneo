#define SERY_GENERATE_IMPLEMENTATION
#include "Sery_Amalgamated.hh"

#include "glm/vec3.hpp"

#include <string>
#include <vector>
#include <array>
#include <fstream>

struct Triangle
{
  glm::vec3     normal;
  glm::vec3     v1;
  glm::vec3     v2;
  glm::vec3     v3;
  Sery::uint16  attribute;
};

Sery::Stream& operator>>(Sery::Stream& stream, glm::vec3& vec)
{
  stream >> vec.x >> vec.y >> vec.z;
  return stream;
}

Sery::Stream& operator>>(Sery::Stream& stream, Triangle& triangle)
{
  stream >> triangle.normal;
  stream >> triangle.v1 >> triangle.v2 >> triangle.v3;
  stream >> triangle.attribute;
  return stream;
}

template <class T, std::size_t N>
Sery::Stream& operator>>(Sery::Stream& stream, std::array<T, N>& array)
{
  for (auto i = 0; i < N; ++i)
    stream >> array[i];
  return stream;
}

int main(int ac, char** av)
{
  std::array<Sery::uint8, 80> header;
  std::string                 filePath{"./Files/OctoLanternCombined_NoHole.stl"};
  Sery::Buffer                buffer;
  Sery::Stream                stream(buffer, Sery::LittleEndian);
  Sery::uint32                trianglesCount;
  std::vector<Triangle>       triangles;

  if (ac > 1)
    filePath = av[1];

  std::ifstream file(filePath, std::ofstream::binary);
  Sery::readToBuffer([&file] (char* buf, int size) {
    file.read(buf, size);
  }, buffer, 84);

  stream >> header >> trianglesCount;
  std::cout << trianglesCount << " triangles." << "\n";

  triangles.reserve(trianglesCount);

  Sery::readToBuffer([&file] (char* buf, int size) {
    file.read(buf, size);
  }, buffer, 50 * trianglesCount);
  file.close();
  
  Triangle triangle;
  while (trianglesCount--)
  {
    stream >> triangle;
    triangles.push_back(triangle);
  }
  std::cout << "Parsing done.\n";
  std::cin.get();
  return 0;
}