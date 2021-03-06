#ifndef BRUTEFORCER_HPP_
# define BRUTEFORCER_HPP_

# define RES_MIN 99.9f
# define NBR_RES 10

#include <vector>
#include "Triangle.hh"

typedef struct	s_res
{
  float		tot;
  Triangle	*tri1;
  Triangle	*tri2;
}		t_res;

std::vector<t_res> const &	BruteForcer(Triangle, Triangle);

#endif
