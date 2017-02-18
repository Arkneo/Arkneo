/*
** angle.h for angle.h in /home/jojo/Documents/arkneo/calc_angle
** 
** Made by Joseph Queuche
** Login   <joseph.queuche@epitech.eu>
** 
** Started on  Sat Feb 18 18:08:56 2017 Joseph Queuche
** Last update Sat Feb 18 18:30:05 2017 Joseph Queuche
*/

#ifndef ANGLE_H_
# define ANGLE_H_

typedef struct          s_vec3
{
  float                 x;
  float                 y;
  float                 z;
}                       vec3;

typedef struct          s_my_triangle
{
  vec3                  v[3];
}                       t_my_triangle;

int	match(vec3, t_my_triangle *);
vec3	*find_point_cote(t_my_triangle *, t_my_triangle *);
vec3	take_middle(vec3 *);
float	get_angle(t_my_triangle *, t_my_triangle *);
float	calc_angle(vec3, vec3, vec3);

#endif /* !ANGLE_H_ */
