#include <cmath>
#include <algorithm>
#include "BruteForcer.hpp"

bool				sortResult(const t_res &elem1,
					   const t_res &elem2)
{
  return (elem1.tot < elem2.tot);
}

bool				verifyNeighbor1(Triangle *tri)
{
  if (tri->neighbors[0].triangle->alreadyPassed1 &&
      tri->neighbors[1].triangle->alreadyPassed1 &&
      tri->neighbors[2].triangle->alreadyPassed1)
    return (true);
  return (false);
}

bool				verifyNeighbor2(Triangle *tri)
{
  if (tri->neighbors[0].triangle->alreadyPassed2 &&
      tri->neighbors[1].triangle->alreadyPassed2 &&
      tri->neighbors[2].triangle->alreadyPassed2)
    return (true);
  return (false);
}

void	ForcerRec(Triangle *tri1,
		  Triangle *tri2,
		  int &tot)
{
  //printf("DEBUG\n%f\n", tri2->neighbors[0].length);
  if (verifyNeighbor2(tri1) && verifyNeighbor2(tri2))
    return ;
  tri1->alreadyPassed2 = true;
  tri2->alreadyPassed2 = true;
  for (int i = 0; i < 3; i++)
    {
      double diff = std::abs(tri1->neighbors[i].angle - tri2->neighbors[i].angle);
      double res = 100 - (diff / 180) *100;
      //printf("%f %i\n", res, i);
      if (res > RES_MIN)
	{
	  tot += res;
	  if (tri1->neighbors[i].triangle->alreadyPassed2 == false && tri2->neighbors[i].triangle->alreadyPassed2 == false)
	    ForcerRec(tri1->neighbors[i].triangle, tri2->neighbors[i].triangle, tot);
	}
    }
}

void	FirstRec(Triangle *tri1, Triangle *tri2, std::vector<t_res> &res)
{
  static int	tot = 0;
  static int	debug = 0;

  //printf("DEBUG\n%f\n", tri2->neighbors[0].length);
  if (verifyNeighbor1(tri1) && verifyNeighbor1(tri2))
    return ;
  tri1->alreadyPassed1 = true;
  tri2->alreadyPassed1 = true;
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
	  std::sort(res.begin(), res.end(), sortResult);
	  if (res.size() > NBR_RES)
	    res.pop_back();
	    }
      //printf("index: %i\n", debug++);
      if (tri2->neighbors[i].triangle->alreadyPassed1 == false)
	FirstRec(tri1, tri2->neighbors[i].triangle, res);
    }
}

void	AllTri1(Triangle *tri1, Triangle *tri2, std::vector<t_res> &res)
{
  FirstRec(tri1, tri2, res);
  for (int i = 0; i < 3; i++)
    {
      if (tri1->neighbors[i].triangle->alreadyPassed1 == false)
	AllTri1(tri1->neighbors[i].triangle, tri2, res);
    }
}

void	setColor(std::vector<t_res> &res)
{
  int len = res.size();
  float colorValue = 1.0f;

  for (int i = 0; i < len; i++)
    {
      if (i <= len / 3)
	{
	  res[i].tri1->color = {0.0f, 0.0f, colorValue};
	  res[i].tri2->color = {0.0f, 0.0f, colorValue};
	}
      if (i <= len / 2)
	{
	  res[i].tri1->color = {0.0f, colorValue, 0.0f};
	  res[i].tri2->color = {0.0f, colorValue, 0.0f};
	}
      else
	{
	  res[i].tri1->color = {colorValue, 0.0f, 0.0f};
	  res[i].tri2->color = {colorValue, 0.0f, 0.0f};
	}
    }
}

void	rec_color(Triangle *tri1, Triangle *tri2)
{
  tri1->color = {1.0f, 0.0f, 0.0f};
  tri2->color = {1.0f, 0.0f, 0.0f};
  for (int i = 0; i < 3; i++)
    {
      double diff = std::abs(tri1->neighbors[i].angle - tri2->neighbors[i].angle);
      double res = 100 - (diff / 180) *100;
      if (res > RES_MIN)
        {
	  if (tri1->neighbors[i].triangle->color[0] != 1.0f && tri2->neighbors[i].triangle->color[0] != 1.0f)
	    rec_color(tri1->neighbors[i].triangle, tri2->neighbors[i].triangle);
        }
    }
}

void	color_all(std::vector<t_res> &res)
{
  rec_color(res[0].tri1, res[0].tri2);
}

std::vector<t_res> const &	BruteForcer(Triangle tri1, Triangle tri2)
{
  std::vector<t_res>		res;

  AllTri1(&tri1, &tri2, res);
  color_all(res);
  return (res);
}
