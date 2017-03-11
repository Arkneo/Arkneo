// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include "Viewer.hh"
#include "shader.hpp"

Viewer::Viewer(const std::vector<Triangle> &triangles, int type) :
  _triangles(triangles), _type(type)
{
  _bufferSize = triangles.size() * 9;
  Position = glm::vec3(0, 0, 0);
  position = glm::vec3(0, 0, 5);
  horizontalAngle = 0.00f;
  verticalAngle = 3.14f;
  initialFoV = 45.0f;
  speed = 3.0f;
}

Viewer::~Viewer()
{
}

/* Initialise GLFW, open window and create its OpenGl context. Init Glew.
*/
int			Viewer::createWindow()
{
  if( !glfwInit())
    {
      fprintf( stderr, "Failed to initialize GLFW\n");
      getchar();
      return -1;
    }
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow( 1024, 768, "ARKNEO STL Viewer", NULL, NULL);
  if( window == NULL )
    {
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.Try the 2.1 version of the tutorials.\n" );
      getchar();
      glfwTerminate();
      return -1;
    }
  glfwMakeContextCurrent(window);
  
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
    {
      fprintf(stderr, "Failed to initialize GLEW\n");
      getchar();
      glfwTerminate();
      return -1;
    }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glEnable(GL_CULL_FACE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  return 0;
}

/* Create and compile our GLSL program from the shaders. Get a handle for our "MVP" uniform.
*/
void			Viewer::init()
{
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  programID = LoadShaders( "./src/TransformVertexShader.vertexshader", "./src/ColorFragmentShader.fragmentshader" );
  MatrixID = glGetUniformLocation(programID, "MVP");
  ViewMatrixID = glGetUniformLocation(programID, "V");
  ModelMatrixID = glGetUniformLocation(programID, "M");
}

/* Set vertex buffer and color buffer. Bind buffers to opengl*/
void			Viewer::setBuffer()
{
  std::vector<glm::vec3>	g_vertex_buffer_data;
  for(const Triangle& tri : _triangles)
    {
      for (int count = 0; count < 3; ++count)
	{
	  glm::vec3 vertex;
	  vertex.x = tri.v[count].x;
	  vertex.y = tri.v[count].y;
	  vertex.z = tri.v[count].z;
	  g_vertex_buffer_data.push_back(vertex);
	}
    }

  float color = 0.850f;
  std::vector<glm::vec3>	 g_color_buffer_data;
  for (const Triangle &tri : _triangles)
    {
      if (tri.match)
        {
          for (int count = 0; count < 3; ++count) {
	    glm::vec3 vertex;
	    vertex.x = 1.000f;
	    vertex.y = 0.000f;
	    vertex.z = 0.000f;
	    g_color_buffer_data.push_back(vertex);
          }
        }
      else
        {
	  if (color == 0.850f)
            color = 1.000;
          else
	  color = 0.850;
          for (int count = 0; count < 9; ++count)
	    {
	      glm::vec3 vertex;
	      vertex.x = color;
	      vertex.y = color;
	      vertex.z = color;
	      g_color_buffer_data.push_back(vertex);
	    }
        }
    }

  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_vertex_buffer_data.size() * sizeof(glm::vec3), &g_vertex_buffer_data[0], GL_STATIC_DRAW);

  glGenBuffers(1, &colorbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data.size() * sizeof(glm::vec3), &g_color_buffer_data[0], GL_STATIC_DRAW);
}

void			Viewer::setZoom(float deltaTime)
{
  // Direction : Spherical coordinates to Cartesian coordinates conversion                                                                                                                   
  glm::vec3 direction(
		      cos(verticalAngle) * sin(horizontalAngle),
		      sin(verticalAngle),
		      cos(verticalAngle) * cos(horizontalAngle)
		      );

  if (_type == 1)
    {
      if (glfwGetKey( window, GLFW_KEY_O ) == GLFW_PRESS){
	position += direction * deltaTime * speed;
      }
      if (glfwGetKey( window, GLFW_KEY_L ) == GLFW_PRESS){
	position -= direction * deltaTime * speed;
      }
    }
  else
    {
      if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS){
        position += direction * deltaTime * speed;
      }
      if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS){
        position -= direction * deltaTime * speed;
      }
    }
  float FoV = initialFoV;
  
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units                                                                                                
  ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
  ViewMatrix = glm::lookAt(
			   position,
			   position+direction,
			   glm::vec3( 0, 1, 0 )
			   );
}

/* set vertex and color attributes */
void			Viewer::setAttributes()
{
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,       
			(void*)0 
			);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
  glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,       
			(void*)0 
			);
 }

/* Rotation keys for first stl */
void			Viewer::generateRotation1(float deltaTime)
{
  if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS)
    Orientation.y -= 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    Orientation.y += 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_UP) == GLFW_PRESS)
    Orientation.x += 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS)
    Orientation.x -= 3.14159f/2.0f * deltaTime;
}

/* Rotataion keys for second stl */
void		        Viewer::generateRotation2(float deltaTime)
{
  if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS)
    Orientation.y -= 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS)
    Orientation.y += 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_W) == GLFW_PRESS)
    Orientation.x += 3.14159f/2.0f * deltaTime;
  if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS)
    Orientation.x -= 3.14159f/2.0f * deltaTime;
}

int			Viewer::buildView()
{
  double		currentTime;
  float			deltaTime;

  std::cout << "inside buildView" << std::endl;
  if (createWindow() < 0)
    return -1;
  init();
  setBuffer();

  double lastTime = glfwGetTime();
  double lastFrameTime = lastTime;
  int nbFrames = 0;

  do{
    currentTime = glfwGetTime();
    deltaTime = (float)(currentTime - lastFrameTime);
    lastFrameTime = currentTime;
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 )
      {
	nbFrames = 0;
	lastTime += 1.0;
      }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);
    setZoom(deltaTime);
    setAttributes();
    if (_type == 1)
      generateRotation1(deltaTime);
    else
      generateRotation2(deltaTime);

    // Build the model matrix                                                                                                  
    glm::mat4 RotationMatrix = glm::eulerAngleYXZ(Orientation.y, Orientation.x, Orientation.z);
    glm::mat4 TranslationMatrix = translate(glm::mat4(), Position);
    glm::mat4 ScalingMatrix = scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;
    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

   // Send our transformation to the currently bound shader,                                                                  
   // in the "MVP" uniform                                                                                                    
   glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
   glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
   glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
    
    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, _bufferSize);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    
  }
  // Check if the ESC key was pressed or the window was closed
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
	 glfwWindowShouldClose(window) == 0 );
  
  // Cleanup VBO and shader
  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &colorbuffer);
  glDeleteProgram(programID);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();
  
  return 0;
}
