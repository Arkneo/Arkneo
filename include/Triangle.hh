#ifndef ARKNEO_TRIANGLE_HH_
#define ARKNEO_TRIANGLE_HH_

#include "Sery_Amalgamated.hh"

#include <glm/vec3.hpp>
#include <array>

/**
 *  @brief  Structure repr�sentant un triangle 3D
 *          d�fini par trois points
 */
struct Triangle
{
  struct NeighborData
  {
    size_t    index;
    Triangle* triangle;
    float     length;
    float     angle;
  };

  /**
   *  @brief  Un triangle a au maximum trois voisins.
   *          Le premier membre de la paire est le voisin.
   *          Le second membre de la pairse est la longueur du segment
   *          partag� avec le voisin.
   */
  std::array<NeighborData, 3> neighbors;
  glm::vec3     normal;         /**< Vecteur normal du triangle */
  glm::vec3     v[3];           /**< Les trois vertex du triangle */
  Sery::uint16  attribute;      /**< Attribute extrait du fichier STL. Ignor�. */
  size_t        lastIndex = 0;  /**< Dernier index de neighbors disponible */
  bool		    match = false;
};

#endif // ARKNEO_TRIANGLE_HH_
