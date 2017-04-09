#include "AngleGen.hh"
#include <cmath>
#include <iostream>

namespace
{
    void				find_point_cote(const Triangle &t1, const Triangle &t2, std::vector<glm::vec3> &point_cote)
    {
      if (match(t1.v[0], t2))
        point_cote.push_back(t1.v[0]);
      if (match(t1.v[1], t2))
        point_cote.push_back(t1.v[1]);
      if (match(t1.v[2], t2))
        point_cote.push_back(t1.v[2]);
    }

    bool				match(const glm::vec3 &point, const Triangle &t)
    {
      if (point.x == t.v[0].x && point.y == t.v[0].y &&
          point.z == t.v[0].z)
        return (true);
      if (point.x == t.v[1].x && point.y == t.v[1].y &&
          point.z == t.v[1].z)
        return (true);
      if (point.x == t.v[2].x && point.y == t.v[2].y &&
          point.z == t.v[2].z)
        return (true);
      return (false);
    }

    void				take_opo(const std::vector<glm::vec3> &point_cote, const Triangle &t, glm::vec3 &point)
    {
      if ((t.v[0].x != point_cote.at(0).x || t.v[0].y != point_cote.at(0).y
           || t.v[0].z != point_cote.at(0).z) && (t.v[0].x != point_cote.at(1).x
    					      || t.v[0].y != point_cote.at(1).y || t.v[0].z != point_cote.at(1).z))
        point = t.v[0];
      if ((t.v[1].x != point_cote.at(0).x || t.v[1].y != point_cote.at(0).y
           || t.v[1].z != point_cote.at(0).z) && (t.v[1].x != point_cote.at(1).x
    					      || t.v[1].y != point_cote.at(1).y || t.v[1].z != point_cote.at(1).z))
        point = t.v[1];
      if ((t.v[2].x != point_cote.at(0).x || t.v[2].y != point_cote.at(0).y
           || t.v[2].z != point_cote.at(0).z) && (t.v[2].x != point_cote.at(1).x
    					      || t.v[2].y != point_cote.at(1).y || t.v[2].z != point_cote.at(1).z))
        point = t.v[2];
    }

    void				take_middle(const std::vector<glm::vec3> &point_cote, glm::vec3 &M)
    {
      M.x = (point_cote.at(0).x + point_cote.at(1).x) / 2;
      M.y = (point_cote.at(0).y + point_cote.at(1).y) / 2;
      M.z = (point_cote.at(0).z + point_cote.at(1).z) / 2;
    }

    float				calc_angle(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &M)
    {
      float				angle;
      float				prodsca;
      float				normes;
      glm::vec3			MA;
      glm::vec3			MB;

      MA.x = A.x - M.x;
      MA.y = A.y - M.y;
      MA.z = A.z - M.z;
      MB.x = B.x - M.x;
      MB.y = B.y - M.y;
      MB.z = B.z - M.z;
      prodsca = MA.x * MB.x + MA.y * MB.y + MA.z * MB.z;
      normes = sqrt(au_carre(MA.x) + au_carre(MA.y) + au_carre(MA.z))
        * sqrt(au_carre(MB.x) + au_carre(MB.y) + au_carre(MB.z));
      angle = acos(prodsca / normes);
      angle = angle / M_PI * 180;
      return (angle);
    }

    float				au_carre(float X)
    {
      return (X * X);
    }
}

float               getAngle(const Triangle &t1, const Triangle &t2)
{
  float             angle;
  glm::vec3         A;
  glm::vec3         B;
  glm::vec3         M;
  std::vector<glm::vec3>    point_cote;

  find_point_cote(t1, t2, point_cote);
  take_opo(point_cote, t1, A);
  take_opo(point_cote, t2, B);
  take_middle(point_cote, M);
  std::cout << "A : " << A.x << ", " << A.y << ", " << A.z << std::endl;
  std::cout << "B : " << B.x << ", " << B.y << ", " << B.z << std::endl;
  std::cout << "M : " << M.x << ", " << M.y << ", " << M.z << std::endl;
  angle = calc_angle(A, B, M);
  return angle;
}
