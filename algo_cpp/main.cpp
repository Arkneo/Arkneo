#include "Triangle.hh"
#include "AngleGen.hh"
#include <iostream>

int		main()
{
  Triangle	t1;
  Triangle	t2;
  float		angle;
  AngleGen	*angleGen = new AngleGen();

  t1.v[0].x = -1.0;
  t1.v[0].y = -1.0;
  t1.v[0].z = 1.0;
  t1.v[1].x = 0.0;
  t1.v[1].y = 0.0;
  t1.v[1].z = 0.0;
  t1.v[2].x = 0.0;
  t1.v[2].y = 0.0;
  t1.v[2].z = 2.0;
  t2.v[0].x = 1.0;
  t2.v[0].y = -1.0;
  t2.v[0].z = 1.0;
  t2.v[1].x = 0.0;
  t2.v[1].y = 0.0;
  t2.v[1].z = 0.0;
  t2.v[2].x = 0.0;
  t2.v[2].y = 0.0;
  t2.v[2].z = 2.0;
  angle = angleGen->get_angle(t1, t2);
  std::cout << "angle : " << angle << std::endl;
  delete(angleGen);
  return (0);
}
