#include "STL.hh"
#include "Serialization.hh"

#include <glm/geometric.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>

STL::STL(const std::string& path)
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

void STL::setupNeighbors()
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
        tri.neighbors[tri.lastIndex].first = &tri2;
        tri.neighbors[tri.lastIndex].second = dist;
        tri.lastIndex++;
        tri2.neighbors[tri2.lastIndex].first = &tri;
        tri2.neighbors[tri2.lastIndex].second = dist;
        tri2.lastIndex++;

        // Si le triangle a trois voisins, on passe au suivant.
        if (tri.lastIndex == 3)
          break;
      }
    }
  }

  int i = 0;
  // On trie les voisins par longueur croissante
  for (Triangle& tri : _triangles)
    {
      std::sort(tri.neighbors.begin(), tri.neighbors.end(),
		[] (auto a, auto b) {return a.second < b.second; });
      if (i < _triangles.size() / 2)
	tri.match = true;
      ++i;
    }
}
