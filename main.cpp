#include "stackMachine.hpp"
#include <iostream>

int main(void)
{
    StackMachine* sm = new StackMachine();

    std::cout << "ExampleOperationFor.txt start." << std::endl;
    sm->ParseFile("./ExampleOperationFor.txt");
    sm->DoInstructions();

    delete sm;

    StackMachine* sm2 = new StackMachine();

    std::cout << "ExampleOperationFunction.txt start." << std::endl;
    sm2->ParseFile("./ExampleOperationFuntion.txt");
    sm2->DoInstructions();

    delete sm2;

    return 0;
}
