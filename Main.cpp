#include "StackMachine.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "Number of arguments is invalid.\n" \
              << "It need to be 2" << std::endl;
  }

  std::string fileName = std::string(argv[1]);
  StackMachine* sm = new StackMachine(fileName);

  std::cout << fileName << " start." << std::endl;
  sm->DoInstructions();

  return 0;
}
