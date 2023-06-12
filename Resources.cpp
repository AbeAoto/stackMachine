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
