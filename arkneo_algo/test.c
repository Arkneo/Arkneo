/*
** test.c for test.c in /home/jojo/Documents/arkneo
** 
** Made by Joseph Queuche
** Login   <joseph.queuche@epitech.eu>
** 
** Started on  Tue Feb 14 14:20:17 2017 Joseph Queuche
** Last update Tue Feb 14 23:20:16 2017 Joseph Queuche
*/

#include <math.h>
#include <stdio.h>

float   au_carre(float X)
{
  return (X * X);
}

int	main()
{
  float	Xa = 0;
  float	Xb = 1235;
  float	Xm = 1;
  float	Ya = 0;
  float	Yb = 123;
  float	Ym = 1;
  float	Za = 1;
  float	Zb = 2;
  float	Zm = 1;
  float	angle;
  float	prodsca;
  float	normes;
  float	MAx;
  float MAy;
  float MAz;
  float	MBx;
  float MBy;
  float MBz;

  MAx = Xa - Xm;
  MAy = Ya - Ym;
  MAz = Za - Zm;
  MBx = Xb - Xm;
  MBy = Yb - Ym;
  MBz = Zb - Zm;
  prodsca = MAx * MBx + MAy * MBy + MAz * MBz;
  normes = sqrt(au_carre(MAx) + au_carre(MAy) + au_carre(MAz))
    * sqrt(au_carre(MBx) + au_carre(MBy) + au_carre(MBz));
  printf("prodsca : %f\n", prodsca);
  printf("normes : %f\n", normes);
  angle = acos(prodsca / normes);
  angle = angle / M_PI * 180;
  printf("angle : %f\n", angle);
  return (0);
}
