#include "STL.hh"
#include "Serialization.hh"
#include "AngleGen.hh"

#include <glm/geometric.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

STL::STL(const std::string& path)
  : _path(path)
{
  if (!path.empty())
    parseFile(path);
}

const std::vector<Triangle>	&STL::getTriangle()
{
  return _triangles;
}

void STL::parseFile(const std::string& path)
{
  Sery::Buffer  buffer;
  Sery::Stream  stream(buffer, Sery::LittleEndian);
  Sery::uint32  trianglesCount;

  std::ifstream file(path, std::ofstream::binary);

  if (!file.is_open())
  {
    std::cout << "[ERROR] Impossible d'ouvrir le fichier `" << path << "`.\n";
    return;
  }
  auto          lambda = [&file] (char* buf, int size) {
    file.read(buf, size);
  };
  Sery::readToBuffer(lambda, buffer, 84);

  stream >> _header >> trianglesCount;
  std::cout << "[DEBUG] " << trianglesCount << " triangles dans le fichier.\n";
  _triangles.reserve(trianglesCount);

  Sery::readToBuffer(lambda, buffer, 50 * trianglesCount);
  file.close();

  Triangle triangle;
  while (trianglesCount--)
  {
    stream >> triangle;
    _triangles.push_back(triangle);
  }
  std::cout << "[DEBUG] Parsing terminé.\n";
}

void  STL::loadNeighborsFromFile(const std::string& path)
{
  Sery::Buffer  buffer;
  Sery::Stream  stream(buffer, Sery::BigEndian);

  std::ifstream file(path, std::ofstream::binary);

  if (!file.is_open())
  {
    std::cout << "[ERROR] Impossible d'ouvrir le fichier `" << path << "`.\n";
    return;
  }
  auto          lambda = [&file] (char* buf, int size) {
    file.read(buf, size);
  };

  size_t size = sizeof(size_t) + 3 * sizeof(Triangle::NeighborData);
  Sery::readToBuffer(lambda, buffer, size * _triangles.size());
  file.close();

  for (Triangle& tri : _triangles)
  {
    stream >> tri.lastIndex;
    stream >> tri.neighbors[0].index >> tri.neighbors[0].length >> tri.neighbors[0].angle;
    tri.neighbors[0].triangle = &_triangles.at(tri.neighbors[0].index);

    stream >> tri.neighbors[1].index >> tri.neighbors[1].length >> tri.neighbors[1].angle;
    tri.neighbors[1].triangle = &_triangles.at(tri.neighbors[1].index);

    stream >> tri.neighbors[2].index >> tri.neighbors[2].length >> tri.neighbors[2].angle;
    tri.neighbors[2].triangle = &_triangles.at(tri.neighbors[2].index);
  }
}

void  STL::serializeNeighbors(const std::string& path) const
{
  Sery::Buffer  buffer;
  Sery::Stream  stream(buffer, Sery::BigEndian);

  for (const Triangle& tri : _triangles)
  {
    stream << tri.lastIndex;
    stream << tri.neighbors[0].index << tri.neighbors[0].length << tri.neighbors[0].angle;
    stream << tri.neighbors[1].index << tri.neighbors[1].length << tri.neighbors[1].angle;
    stream << tri.neighbors[2].index << tri.neighbors[2].length << tri.neighbors[2].angle;
  }

  std::ofstream file(path, std::ofstream::binary);
  file.write(buffer.data(), buffer.size());
  file.close();
}

void  STL::calculateNeighbors(const std::string& path)
{
  for (size_t i = 0; i < _triangles.size(); ++i)
  {
    Triangle& tri = _triangles.at(i);

    // Si le triangle a déjà trois voisins,
    // on n'en trouvera pas d'autres.
    if (tri.lastIndex == 3)
      continue;
    for (size_t j = i + 1; j < _triangles.size(); ++j)
    {
      Triangle& tri2 = _triangles.at(j);
      if (tri2.lastIndex == 3)
        continue;

      // Dernier vertex qui a eu une correspondance.
      // Permet de calculer la distance si dist == 2
      const glm::vec3* vOld = nullptr;
      float dist = 0;

      // On "croise" tous les vertex des deux triangles
      // pour voir si on en trouve deux égaux
      for (const glm::vec3& v1 : tri.v)
      {
        for (const glm::vec3& v2 : tri2.v)
        {
          if (v1 == v2)
          {
            // Si vOld est null, c'est le premier vertex commun,
            // on le stocke pour après
            if (!vOld)
              vOld = &v1;
            else
              dist = glm::distance(*vOld, v1);
          }
        }
      }

      // Si on a trouvé un segment commun, on met à jour les voisins
      if (dist > 0)
      {
        float angle = AngleGen::getAngle(tri, tri2);
        tri.neighbors[tri.lastIndex].index = j;
        tri.neighbors[tri.lastIndex].triangle = &tri2;
        tri.neighbors[tri.lastIndex].length = dist;
        tri.neighbors[tri.lastIndex].angle = angle;
        tri.lastIndex++;
        tri2.neighbors[tri2.lastIndex].index = i;
        tri2.neighbors[tri2.lastIndex].triangle = &tri;
        tri2.neighbors[tri2.lastIndex].length = dist;
        tri2.neighbors[tri2.lastIndex].angle = angle;
        tri2.lastIndex++;

        // Si le triangle a trois voisins, on passe au suivant.
        if (tri.lastIndex == 3)
          break;
      }
    }
  }

  size_t i = 0;
  // On trie les voisins par longueur croissante
  for (Triangle& tri : _triangles)
  {
    std::sort(tri.neighbors.begin(), tri.neighbors.end(),
              [] (auto a, auto b) { return a.length < b.length; });
    ++i;
  }

  if (!path.empty())
    serializeNeighbors(path);
}

void STL::setupNeighbors()
{
  if (!_path.empty())
  {
    fs::path path(_path);
    fs::path neighborsPath(path.parent_path() / (path.stem().string() + ".arkneig"));
    if (fs::exists(neighborsPath))
    {
      std::cout << "Loading neighbors from `" << neighborsPath << "`\n";
      loadNeighborsFromFile(neighborsPath.string());
      std::cout << "Neighbors loading done\n";
      return;
    }
    else
    {
      std::cout << "Calculating and writing neighbors to `" << neighborsPath << "`\n";
      calculateNeighbors(neighborsPath.string());
      std::cout << "Neighbors calculation done, written.\n";
      return;
    }
  }
  else
    calculateNeighbors();
}
