#define SERY_GENERATE_IMPLEMENTATION
#include "Sery_Amalgamated.hh"

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

#include <string>
#include <vector>
#include <array>
#include <fstream>

struct Triangle
{
  std::array<std::pair<Triangle*, float>, 3> neighbors;
  glm::vec3     normal;
  glm::vec3     v[3];
  Sery::uint16  attribute;
  size_t        lastIndex = 0;
};

Sery::Stream& operator >> (Sery::Stream& stream, glm::vec3& vec)
{
  stream >> vec.x >> vec.y >> vec.z;
  return stream;
}

Sery::Stream& operator >> (Sery::Stream& stream, Triangle& triangle)
{
  stream >> triangle.normal;
  stream >> triangle.v[0] >> triangle.v[1] >> triangle.v[2];
  stream >> triangle.attribute;
  return stream;
}

template <class T, std::size_t N>
Sery::Stream& operator >> (Sery::Stream& stream, std::array<T, N>& array)
{
  for (auto i = 0; i < N; ++i)
    stream >> array[i];
  return stream;
}

int main(int ac, char** av)
{
  std::array<Sery::uint8, 80> header;
  std::string                 filePath{"./Files/cubeafacette.stl"};
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

  for (size_t i = 0; i < triangles.size(); ++i)
  {
    Triangle& tri = triangles.at(i);
    if (tri.lastIndex == 3)
      continue;
    for (size_t j = i + 1; j < triangles.size(); ++j)
    {
      Triangle& tri2 = triangles.at(j);
      if (tri2.lastIndex == 3)
        continue;
      const glm::vec3* vOld = nullptr;
      float dist = 0;
      for (const glm::vec3& v1 : tri.v)
        for (const glm::vec3& v2 : tri2.v)
          if (v1 == v2)
          {
            if (!vOld)
              vOld = &v1;
            else
              dist = glm::distance(*vOld, v1);
          }
      if (dist > 0)
      {
        std::cout << "Found neighbor ! Dist = " << dist << "\n";
        tri.neighbors[tri.lastIndex].first = &tri2;
        tri.neighbors[tri.lastIndex].second = dist;
        tri.lastIndex++;
        tri2.neighbors[tri2.lastIndex].first = &tri;
        tri2.neighbors[tri2.lastIndex].second = dist;
        tri2.lastIndex++;
        if (tri.lastIndex == 3)
          break;
      }
    }
  }

  for (Triangle& tri : triangles)
  {
    std::sort(tri.neighbors.begin(), tri.neighbors.end(), [](auto a, auto b) {return a.second < b.second;});
    if (tri.lastIndex != 3) // A triangle doesn't have three neighbors
      std::cout << "tri.lastIndex != 3 : " << tri.lastIndex << "\n";
  }

  std::cout << "Parsing done (" << triangles.size() << ").\n";
  //std::cin.get();
  return 0;
}