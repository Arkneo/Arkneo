#define SERY_GENERATE_IMPLEMENTATION
#include "Sery_Amalgamated.hh"
#undef SERY_GENERATE_IMPLEMENTATION

#include "STL.hh"
#include "Viewer.hh"
#include "BruteForcer.hpp"
#include <string>
#include <chrono>
#include <thread>

int main(int ac, char** av)
{
  std::string filePath1{"./Files/boule_disco1/demibouleafacette.stl"};
  std::string filePath2{"./Files/boule_disco1/demibouleafacette2.stl"};

  if (ac > 2)
  {
    filePath1 = av[1];
    filePath2 = av[2];
  }

  STL stl1(filePath1);
  stl1.setupNeighbors();

  STL stl2(filePath2);
  stl2.setupNeighbors();

  const std::vector<Triangle> &tri1 = stl1.getTriangle();
  const std::vector<Triangle> &tri2 = stl2.getTriangle();

  const std::vector<t_res>	&res = BruteForcer(tri1.front(), tri2.front());

  Viewer        viewer1(stl1.getTriangle(), 1);
  Viewer        viewer2(stl2.getTriangle(), 2);

  viewer1.buildView();
  viewer2.buildView();

  return 0;
}
