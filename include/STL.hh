#ifndef ARKNEO_STL_HH_
#define ARKNEO_STL_HH_

#include "Triangle.hh"

#include <vector>
#include <array>

/**
 *  @brief  Classe qui contient les données
 *          parsées depuis un fichier STL
 */
class STL
{
  using uint8 = Sery::uint8; /**< Typedef pour simplicité */

public:
  /**
   *  @brief  Appelle parseFile si path n'est pas vide.
   */
  STL(const std::string& path = "");

public:
  /**
   *  @brief  Lis le fichier STL path et stocke les données en interne.
   */
  void  parseFile(const std::string& path);

  /**
   *  @brief  Parcoure tous les triangles et met les voisins à jour.
   *          Range les voisins de chaque triangle par longueur croissante.
   */
  void  setupNeighbors();

private:
  std::vector<Triangle> _triangles;
  std::array<uint8, 80> _header;
};

#endif // ARKNEO_STL_HH_