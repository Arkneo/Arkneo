/*
** angle.c for angle.c in /home/jojo/Documents/arkneo/calc_angle
** 
** Made by Joseph Queuche
** Login   <joseph.queuche@epitech.eu>
** 
** Started on  Sat Feb 18 18:25:59 2017 Joseph Queuche
** Last update Sat Feb 18 18:42:54 2017 Joseph Queuche
*/

#include <math.h>
#include <stdlib.h>

#include "angle.h"

extern vec3	vec;

float	au_carre(float X)
{
  return (X * X);
}

int	match(vec3 point, t_my_triangle *Triangle)
{
  if (point.x == Triangle->v[0].x && point.y == Triangle->v[0].y &&
      point.z == Triangle->v[0].z)
    return (1);
  if (point.x == Triangle->v[1].x && point.y == Triangle->v[1].y &&
      point.z == Triangle->v[1].z)
    return (1);
  if (point.x == Triangle->v[2].x && point.y == Triangle->v[2].y &&
      point.z == Triangle->v[2].z)
    return (1);
  return (0);
}

vec3	*find_point_cote(t_my_triangle *Triangle1, t_my_triangle *Triangle2)
{
  vec3	*point_cote;
  int	i;

  i = 0;
  if (!(point_cote = malloc(sizeof (*point_cote) * 3)))
    return (NULL);
  if (match(Triangle1->v[0], Triangle2) == 1)
    {
      point_cote[i] = Triangle1->v[0];
      i++;
    }
  if (match(Triangle1->v[1], Triangle2) == 1)
    {
      point_cote[i] = Triangle1->v[1];
      i++;
    }
  if (match(Triangle1->v[2], Triangle2) == 1)
    {
      point_cote[i] = Triangle1->v[2];
      i++;
    }
  return (point_cote);
}

vec3	take_opo(vec3 *point_cote, t_my_triangle *Triangle)
{
  if ((Triangle->v[0].x != point_cote[0].x || Triangle->v[0].y != point_cote[0].y
       || Triangle->v[0].z != point_cote[0].z) && (Triangle->v[0].x != point_cote[1].x
       || Triangle->v[0].y != point_cote[1].y || Triangle->v[0].z != point_cote[1].z))
    return (Triangle->v[0]);
  if ((Triangle->v[1].x != point_cote[0].x || Triangle->v[1].y != point_cote[0].y
       || Triangle->v[1].z != point_cote[0].z) && (Triangle->v[1].x != point_cote[1].x
       || Triangle->v[1].y != point_cote[1].y || Triangle->v[1].z != point_cote[1].z))
    return (Triangle->v[1]);
  if ((Triangle->v[2].x != point_cote[0].x || Triangle->v[2].y != point_cote[0].y
       || Triangle->v[2].z != point_cote[0].z) && (Triangle->v[2].x != point_cote[1].x
       || Triangle->v[2].y != point_cote[1].y || Triangle->v[2].z != point_cote[1].z))
    return (Triangle->v[2]);
}

vec3	take_middle(vec3 *point_cote)
{
  vec3	M;

  M.x = (point_cote[0].x + point_cote[1].x) / 2;
  M.y = (point_cote[0].y + point_cote[1].y) / 2;
  M.z = (point_cote[0].z + point_cote[1].z) / 2;
  return (M);
}

float	get_angle(t_my_triangle *Triangle1, t_my_triangle *Triangle2)
{
  float	angle;
  vec3	A;
  vec3	B;
  vec3	M;
  vec3	*point_cote;

  point_cote = find_point_cote(Triangle1, Triangle2);
  A = take_opo(point_cote, Triangle1);
  B = take_opo(point_cote, Triangle2);
  M = take_middle(point_cote);
  printf("A : %f, %f, %f\n", A.x, A.y, A.z);
  printf("B : %f, %f, %f\n", B.x, B.y, B.z);
  printf("M : %f, %f, %f\n", M.x, M.y, M.z);
  angle = calc_angle(A, B, M);
  return (angle);
}

float	calc_angle(vec3 A, vec3 B, vec3 M)
{
  float angle;
  float prodsca;
  float normes;
  vec3	MA;
  vec3	MB;

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
