/*
** from_points_to_dist.c for from_points_to_dist.c in /home/jojo/Documents/arkneo
** 
** Made by Joseph Queuche
** Login   <joseph.queuche@epitech.eu>
** 
** Started on  Mon Feb  6 15:29:40 2017 Joseph Queuche
** Last update Tue Feb 14 11:27:10 2017 Joseph Queuche
*/

#include <math.h>

typedef struct		s_my_triangle
{
  // POINT A
  float			Xa;
  float			Ya;
  float			Za;
  // POINT B
  float			Xb;
  float			Yb;
  float			Zb;
  // POINT C
  float			Xc;
  float			Yc;
  float			Zc;
  // COTÉS
  float			Dist_max;
  float			Dist_middle;
  float			Dist_min;
}			t_my_triangle;

float	au_carre(float X)
{
  return (X * X);
}

void	put_in_struct(float dist_A, float dist_B, float dist_C, t_my_triangle *tri)
{
  if (dist_A <= dist_B && dist_A <= dist_C)
    {
      tri->Dist_min = dist_A;
      dist_A = 0;
    }
  else if (dist_B <= dist_A && dist_B <= dist_C)
    {
      tri->Dist_min = dist_B;
      dist_B = 0;
    }
  else
    {
      tri->Dist_min = dist_C;
      dist_C = 0;
    }
  if (dist_A >= dist_B && dist_A >= dist_C)
    {
      tri->Dist_max = dist_A;
      dist_A = 0;
    }
  else if (dist_B >= dist_A && dist_B >= dist_C)
    {
      tri->Dist_max = dist_B;
      dist_B = 0;
    }
  else
    {
      tri->Dist_max = dist_C;
      dist_C = 0;
    }
  if (dist_A == 0 && dist_B == 0)
    tri->Dist_middle = dist_C;
  if (dist_B == 0 && dist_C == 0)
    tri->Dist_middle = dist_A;
  if (dist_A == 0 && dist_C == 0)
    tri->Dist_middle = dist_B;
}

float	calc_dist(float X1, float Y1, float Z1, float X2, float Y2, float Z2)
{
  float	dist;

  dist = sqrt(au_carre((X1 - X2)) + au_carre((Y1 - Y2)) + au_carre((Z1 - Z2)));
  return (dist);
}

void	from_points_to_dist(t_my_triangle *tri)
{
  float	dist_A;
  float	dist_B;
  float	dist_C;

  dist_A = calc_dist(tri->Xa, tri->Ya, tri->Za, tri->Xb, tri->Yb, tri->Zb);
  dist_B = calc_dist(tri->Xb, tri->Yb, tri->Zb, tri->Xc, tri->Yc, tri->Zc);
  dist_C = calc_dist(tri->Xa, tri->Ya, tri->Za, tri->Xc, tri->Yc, tri->Zc);
  put_in_struct(dist_A, dist_B, dist_C, tri);
}

float	take_opo(float cote, t_my_triangle *tri)
{
  //check si la distance de 2 des 3 points est la même que coté et renvois le 3é point
  return (opo_cote);
}

float	take_middle(float cote, t_my_triangle *tri)
{
  //trouve les 2 points qui forment le coté (en regardant la distance formée par ces 2 points et en comparant aux 3 coté du triangle) : (X1, Y1, Z1) (X2, Y2, Z2)
  // et calcule (Xm, Ym, Zm)
  // Xm = (X1 + X2) / 2;
  // Ym = (Y1 + Y2) / 2;
  // Zm = (Z1 + Z2) / 2;
  return (middle);
}

float	calc_angle(float opo_cote, float middle, float opo_next_cote)
{
  float	angle;

  // opo_cote (Xa, Ya, Za)
  // opo_next_cote (Xb, Yb, Zb)
  // middle (Xm, Ym, Zm)
  angle = acos(sqrt((Xb - Xm)^2 * (Xa - Xm)^2 + (Yb - Ym)^2 * (Ya - Ym)^2 + (Zb - Zm)^2 * (Za - Zm)^2) / sqrt((Xb - Xm)^2 + (Yb - Ym)^2 + (Zb - Zm)^2));
  return (angle);
}

float	calc_angle(float cote, t_my_triangle *tri)
{
  float	opo_cote;
  float	middle;
  float opo_next_cote;
  float	angle;

  opo_cote = take_opo(cote, tri);
  opo_next_cote = take_opo(cote, tri->next_by_cote);
  middle = take_middle(cote, tri);
  angle = calc_angle(opo_cote, middle, opo_next_cote);
}
