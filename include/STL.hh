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
   *  @brief  Lit le fichier STL path et stocke les données en interne.
   */
  void  parseFile(const std::string& path);

  /**
   *  @brief  Charge les voisins depuis le fichier spécifié
   */
  void  loadNeighborsFromFile(const std::string& path);

  /**
   *  @brief  Calcule les voisins en parcourant les triangles
   */
  void  calculateNeighbors(const std::string& path = "");
  
  /**
   *  @brief  Stocke les voisins dans un fichier
   */
  void  serializeNeighbors(const std::string& path) const;

  /**
   *  @brief  Parcoure tous les triangles et met les voisins à jour.
   *          Range les voisins de chaque triangle par longueur croissante.
   */
  void  setupNeighbors();
  const std::vector<Triangle>  &getTriangle();

private:
  std::vector<Triangle> _triangles;
  std::array<uint8, 80> _header;
  std::string           _path;
};

#endif // ARKNEO_STL_HH_
