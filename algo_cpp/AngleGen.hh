#ifndef ARKNEO_ANGLEGEN_
#define ARKNEO_ANGLEGEN_

#include "Triangle.hh"
#include <vector>

class				AngleGen
{
public :
  AngleGen();
  ~AngleGen();
  
  float				get_angle(const Triangle &t1, const Triangle &t2);

private:
  void				find_point_cote(const Triangle &t1, const Triangle &t2, std::vector<glm::vec3> &point_cote);
  bool				match(const glm::vec3 &point, const Triangle &t);
  void				take_opo(const std::vector<glm::vec3> &point_cote, const Triangle &t, glm::vec3 &point);
  void				take_middle(const std::vector<glm::vec3> &point_cote, glm::vec3 &point);
  float				calc_angle(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &M);
  float				au_carre(float X);
};

#endif //ARKNEO_ANGLEGEN_