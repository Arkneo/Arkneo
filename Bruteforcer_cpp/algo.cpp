#include <cmath>
#include "BruteForcer.hpp"

bool				sortResult(const t_res &elem1, const t_res &elem2)
{
  return (elem1.tot < elem2.tot);
}

bool				verifyNeighbor1(const Triangle &tri)
{
  return (tri.neighbor[0].alreadyPassed1
	  && tri.neighbor[1].alreadyPassed1
	  && tri.neighbor[2].alreadyPassed1);
}

bool				verifyNeighbor2(const Triangle &tri)
{
  return (tri.neighbor[0].alreadyPassed2
	  && tri.neighbor[1].alreadyPassed2
	  && tri.neighbor[2].alreadyPassed2);
}

void	ForcerRec(const Triangle &tri1,
		  const Triangle &tri2,
		  int &tot)
{
  if (verifyNeighbor2(tri1) && verifyNeighbor2(tri2))
    return ;
  tri1.alreadyPassed2 = true;
  tri2.alreadyPassed2 = true;
  for (int i = 0; i < 3; i++)
    {
      double diff = std::abs(tri1.angle[i] - tri2.amgle[i]);
      double res = 100 - (diff / 180) *100;

      if (res > RES_MIN)
	{
	  tot += res;
	  ForcerRec(tri1.neighbor[i], tri2.neighbor[i], tot);
	}
    }
}

void	FirstRec(const Triangle &tri1, const Triangle tri2, std::vector<t_res> &res)
{
  int				tot;

  if (verifyNeighbor1(tri1) && verifyNeighbor1(tri2))
    return ;
  tri1.alreadyPassed1 = true;
  tri2.alreadyPassed1 = true;
  for (int i = 0; i < 3; i++)
    {
      tot = 0;
      ForcerRec(tri1, tri2, tot);
      if (tot > 0)
	{
	  t_res	tmp;

	  tmp.tot = tot;
	  tmp.tri1 = tri1;
	  tmp.tri2 = tri2;

	  res.push_back(tmp);
	  res.sort(sortResult);
	  if (res.size() > NBR_RES)
	    res.pop_back();
	}
      FirstRec(tri1, tri2.neighbor[i], res);
    }
}

std::vector<t_res> const &	BruteForcer(const Triangle &tri1, const Triangle &tri2)
{
  std::vector<t_res>		res;

  FirstRec(tri1, tri2, res);
  return (res);
}
