/*
** main_test.c for main_test.c in /home/jojo/Documents/arkneo/calc_angle
** 
** Made by Joseph Queuche
** Login   <joseph.queuche@epitech.eu>
** 
** Started on  Sat Feb 18 18:10:30 2017 Joseph Queuche
** Last update Sat Feb 18 18:43:37 2017 Joseph Queuche
*/

#include <stdio.h>
#include <stdlib.h>

#include "angle.h"

vec3		vec;

int		main()
{
  t_my_triangle	*tri1;
  t_my_triangle	*tri2;
  float		angle;

  tri1 = malloc(sizeof(t_my_triangle));
  tri2 = malloc(sizeof(t_my_triangle));
  tri1->v[0].x = -1.0;
  tri1->v[0].y = -1.0;
  tri1->v[0].z = 1.0;
  tri1->v[1].x = 0.0;
  tri1->v[1].y = 0.0;
  tri1->v[1].z = 0.0;
  tri1->v[2].x = 0.0;
  tri1->v[2].y = 0.0;
  tri1->v[2].z = 2.0;
  tri2->v[0].x = 1.0;
  tri2->v[0].y = -1.0;
  tri2->v[0].z = 1.0;
  tri2->v[1].x = 0.0;
  tri2->v[1].y = 0.0;
  tri2->v[1].z = 0.0;
  tri2->v[2].x = 0.0;
  tri2->v[2].y = 0.0;
  tri2->v[2].z = 2.0;
  angle = get_angle(tri1, tri2);
  printf("angle : %f\n", angle);
  return (0);
}
