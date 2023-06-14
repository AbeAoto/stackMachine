#include "StackMachine.hpp"
#include <iostream>

int main(void)
{
    StackMachine* sm = new StackMachine("./ExampleOperation.txt");

    std::cout << "ExampleOperation.txt start." << std::endl;
    sm->DoInstructions();
    
    return 0;
}
