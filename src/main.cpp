#define SERY_GENERATE_IMPLEMENTATION
#include "Sery_Amalgamated.hh"
#undef SERY_GENERATE_IMPLEMENTATION

#include "STL.hh"

#include <string>

int main(int ac, char** av)
{
  std::string filePath{"./Files/cubeafacette.stl"};

  if (ac > 1)
    filePath = av[1];

  STL stl(filePath);
  stl.setupNeighbors();
  
  return 0;
}