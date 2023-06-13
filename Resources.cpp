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

void Resources::SetProgramCounter(const unsigned int num)
{
  _programCounter = num;
}

unsigned int Resources::GetProgramCounter() const
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
