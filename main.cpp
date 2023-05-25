#include "stackMachine.hpp"
#include "LexicalAnalyzer.hpp"
#include <utility>

int main(void)
{
    LexicalAnalyzer la("./TestOperation copy.txt");
    std::pair<Token, std::string> p = la.GetToken();
    std::cout << p.first << " " << p.second << std::endl;
    p = la.GetToken();
    std::cout << p.first << " " << p.second << std::endl;

    return 0;
}
