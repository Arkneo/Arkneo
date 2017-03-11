#ifndef VIEWER__
#define VIEWER__

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Triangle.hh"



class				Viewer
{
public :
  Viewer(const std::vector<Triangle> &triangles, int type);
  ~Viewer();
  
  int				buildView();

private:
  int				createWindow();
  void				init();
  void				setBuffer();
  void				setZoom(float deltaTime);
  void				setAttributes();
  void				generateRotation1(float deltaTime);
  void				generateRotation2(float deltaTime);
  
  const std::vector<Triangle>	&_triangles;
  int				_bufferSize;
  int				_type;
  GLFWwindow			*window;

  /*Position and Orientation for Rotation*/
  glm::vec3			Position;
  glm::vec3			Orientation;

  /* position, angles Fov for zoom */
  glm::vec3			position;
  float				horizontalAngle;
  float				verticalAngle;
  float				initialFoV;

  /* speed for FPS */
  float				speed;
  
  GLuint			VertexArrayID;
  GLuint			programID;
  GLuint			MatrixID;
  GLuint			ViewMatrixID;
  GLuint			ModelMatrixID;
  
  GLuint			vertexbuffer;
  GLuint			colorbuffer;
  
  glm::mat4			ProjectionMatrix;
  glm::mat4			ViewMatrix;
};

#endif //VIEWER_
