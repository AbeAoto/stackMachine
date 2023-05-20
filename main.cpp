#include "stackMachine.hpp"

int main(void)
{
    StackMachine sm;
    sm.ParseFile("./TestOperation.txt");
    sm.DoInstructions();

    return 0;
}
