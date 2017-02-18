#ifndef ARKNEO_SERIALIZATION_HH_
#define ARKNEO_SERIALIZATION_HH_

#include "Triangle.hh"

/**
 *  @brief  Extrait un glm::vec3 depuis un Sery::Stream
 */
Sery::Stream& operator>>(Sery::Stream& stream, glm::vec3& vec);

/**
 *  @brief  Extrait un Triangle depuis un Sery::Stream
 */
Sery::Stream& operator>>(Sery::Stream& stream, Triangle& triangle);

/**
 *  @brief  Extrait un std::array de taille
 *          variable depuis un Sery::Stream
 */
template <class T, std::size_t N>
Sery::Stream& operator>>(Sery::Stream& stream, std::array<T, N>& array)
{
  for (auto i = 0; i < N; ++i)
    stream >> array[i];
  return stream;
}

#endif // ARKNEO_SERIALIZATION_HH_