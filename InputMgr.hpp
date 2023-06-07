#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class InputMgr
{
public:
  InputMgr(std::string fileName);
  ~InputMgr();
  std::vector<std::string> GetLineInstruction();

private:
  const std::string _fileName;
  std::ifstream _inputFileStream;
};
