#include "Serialization.hh"

Sery::Stream& operator>>(Sery::Stream& stream, glm::vec3& vec)
{
  stream >> vec.x >> vec.y >> vec.z;
  return stream;
}

Sery::Stream& operator>>(Sery::Stream& stream, Triangle& triangle)
{
  stream >> triangle.normal;
  stream >> triangle.v[0] >> triangle.v[1] >> triangle.v[2];
  stream >> triangle.attribute;
  return stream;
}