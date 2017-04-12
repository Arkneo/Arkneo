#ifndef BRUTEFORCER_HPP_
# define BRUTEFORCER_HPP_

# define RES_MIN 90.0f
# define NBR_RES 10

typedef struct	s_res
{
  int		tot;
  Triangle	*tri1;
  Triangle	*tri2;
}		t_res;

std::vector<t_res> const &	BruteForcer(const Triangle &, const Triangle &);

#endif
