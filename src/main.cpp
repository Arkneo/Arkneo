#define SERY_GENERATE_IMPLEMENTATION
#include "Sery_Amalgamated.hh"
#undef SERY_GENERATE_IMPLEMENTATION

#include "STL.hh"

#include <string>
#include <chrono>

int main(int ac, char** av)
{
  std::string filePath{"./Files/cubeafacette.stl"};

  if (ac > 1)
    filePath = av[1];

  auto tStart = std::chrono::high_resolution_clock::now();
  STL stl(filePath);
  auto tEnd = std::chrono::high_resolution_clock::now();
  std::cout << "Parsing done in " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() << "ms.\n";
  tStart = std::chrono::high_resolution_clock::now();
  stl.setupNeighbors();
  tEnd = std::chrono::high_resolution_clock::now();
  std::cout << "Neighbors set up in " <<
    std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count() << "ms.\n";

  std::cin.get();
  return 0;
}