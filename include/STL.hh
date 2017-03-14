#ifndef ARKNEO_STL_HH_
#define ARKNEO_STL_HH_

#include "Triangle.hh"

#include <vector>
#include <array>

/**
 *  @brief  Classe qui contient les donn�es
 *          pars�es depuis un fichier STL
 */
class STL
{
  using uint8 = Sery::uint8; /**< Typedef pour simplicit� */

public:
  /**
   *  @brief  Appelle parseFile si path n'est pas vide.
   */
  STL(const std::string& path = "");

public:
  /**
   *  @brief  Lit le fichier STL path et stocke les donn�es en interne.
   */
  void  parseFile(const std::string& path);

  /**
   *  @brief  Charge les voisins depuis le fichier sp�cifi�
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
   *  @brief  Parcoure tous les triangles et met les voisins � jour.
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
