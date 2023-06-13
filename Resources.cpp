#include "Resources.hpp"
#include <iostream>

Resources* Resources::_singleton = NULL;

Resources* Resources::GetInstance()
{
  if (_singleton == NULL)
  {
    _singleton = new Resources();
  }
  return _singleton;
}

const unsigned int Resources::GetInstructionsSize() const
{
  return _instructions.size();
}

void Resources::SetInstruction(std::vector<std::string> inst, const unsigned short pc)
{
  if (pc < 0)
  {
    std::cerr << "[fatal][err]Invalid operation in SetInstruction().   Line : "
              << GetProgramCounter() << std::endl;
  }

  if (_instructions.size() <= pc)
  {
    _instructions.resize(pc+1);
  }

  _instructions[pc] = inst;
}

const std::vector<std::string> Resources::LoadInstruction(const unsigned short pc) const
{
  if (pc < 0 || _instructions.size() <= pc)
  {
    std::cerr << "[fatal][err]Invalid operation in LoadInstruction().   Line : "
              << GetProgramCounter() << std::endl;
  }

  return _instructions[pc];
}

void Resources::SetLocalVariable(std::string varName, int varVal)
{
  if (_variables[GetCallStackDepth()].size() <= GetBlockDepth())
  {
    _variables[GetCallStackDepth()].resize(GetBlockDepth()+1);
  }

  _variables[GetCallStackDepth()][GetBlockDepth()][varName] = varVal;
}

const int Resources::GetLocalVariableValue(std::string varName) const
{
  for (int blockDepth = GetBlockDepth(); 0 <= blockDepth; blockDepth--) {

    // ‚»‚ÌŠK‘w‚Å•Ï”‚ªéŒ¾‚³‚ê‚Ä‚¢‚È‚¢ê‡ƒXƒLƒbƒv
    if (_variables[GetCallStackDepth()].size() <= blockDepth)
      continue;

    std::map<std::string, int> varMap = _variables[GetCallStackDepth()][blockDepth];
    auto varInfo = _variables[GetCallStackDepth()][blockDepth].find(varName);

    // —v‘f‚ª‘¶Ý‚µ‚Ä‚¢‚½ê‡
    if (varInfo != varMap.end())
    {
      return varInfo->second;
    }
  }

  std::cerr << "[err] variable \"" << varName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

void Resources::SetProgramCounter(const unsigned int num)
{
  _programCounter = num;
}

const unsigned int Resources::GetProgramCounter() const
{
  return _programCounter;
}

void Resources::IncrementProgramCounter()
{
  _programCounter++;
}

unsigned int Resources::GetLabeledAddress(const std::string labelName) const
{
  auto newPC = _labels.find(labelName);

  // ƒ‰ƒxƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚©‚Á‚½ê‡‚ÌƒGƒ‰[
  if (newPC == _labels.end())
  {
    std::cerr << "[err] Label " << labelName << " is not declared.   Line : "
              << GetProgramCounter() << std::endl;
    exit(1);
  }

  return newPC->second;
}

void Resources::SetLabel(const std::string labelName, const unsigned int address)
{
  _labels[labelName] = address;
}

void Resources::IncreaseBlockDepth()
{
  _blockDepth++;
}

void Resources::DecreaseBlockDepth()
{
  if (_blockDepth == 0)
  {
    std::cerr << "[fatal][err] Block depth must be 0 or more."
              << "Report it to the programmer." << std::endl;
  }

  _blockDepth--;
}

const unsigned int Resources::GetBlockDepth() const
{
  return _blockDepth;
}

void Resources::IncreaseCallStackDepth()
{
  _callStackDepth++;
}

void Resources::DecreaseCallStackDepth()
{
  if (_callStackDepth == 0)
  {
    std::cerr << "[fatal][err] Call stack depth must be 0 or more."
              << "Report it to the programmer." << std::endl;
  }

  _callStackDepth--;
}

const unsigned int Resources::GetCallStackDepth() const
{
  return _callStackDepth;
}

void Resources::PushStack(const int ele)
{
  _stack.push(ele);
}

void Resources::PopStack()
{
  _stack.pop();
}

const int Resources::TopStack() const
{
  return _stack.top();
}

const unsigned int Resources::GetStackSize() const
{
  return _stack.size();
}
