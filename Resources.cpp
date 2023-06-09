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
  if (_variables.size() <= GetCallStackDepth())
  {
    _variables.resize(GetCallStackDepth()+1);
  }

  if (_variables[GetCallStackDepth()].size() <= GetBlockDepth())
  {
    _variables[GetCallStackDepth()].resize(GetBlockDepth()+1);
  }

  _variables[GetCallStackDepth()][GetBlockDepth()][varName] = varVal;
}

const int Resources::GetLocalVariableValue(std::string varName) const
{
  for (int blockDepth = GetBlockDepth(); 0 <= blockDepth; blockDepth--) {

    // その階層で変数が宣言されていない場合スキップ
    if (_variables[GetCallStackDepth()].size() <= blockDepth)
      continue;

    std::map<std::string, int> varMap = _variables[GetCallStackDepth()][blockDepth];
    auto varInfo = varMap.find(varName);

    // 要素が存在していた場合
    if (varInfo != varMap.end())
    {
      return varInfo->second;
    }
  }

  std::cerr << "[err] variable \"" << varName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

void Resources::AllocateLocalArray(std::string arrayName, int size)
{
  if (_arrays.size() <= GetCallStackDepth())
  {
    _arrays.resize(GetCallStackDepth()+1);
  }

  if (_arrays[GetCallStackDepth()].size() <= GetBlockDepth())
  {
    _arrays[GetCallStackDepth()].resize(GetBlockDepth()+1);
  }

  _arrays[GetCallStackDepth()][GetBlockDepth()][arrayName].resize(size);
}

void Resources::AllocateGlobalArray(std::string arrayName, int size)
{
  _arrays[0][0][arrayName].resize(size);
}


void Resources::SetLocalArrayAt(std::string arrayName, const unsigned int idx, int data)
{
  for (int blockDepth = GetBlockDepth(); 0 <= blockDepth; blockDepth--) {

    // その階層で変数が宣言されていない場合スキップ
    if (_arrays[GetCallStackDepth()].size() <= blockDepth)
      continue;

    auto mapEnd = _arrays[GetCallStackDepth()][blockDepth].end();
    auto arrInfo = _arrays[GetCallStackDepth()][blockDepth].find(arrayName);

    // 要素が存在していた場合
    if (arrInfo != mapEnd)
    {
      std::vector<int>* arr = &arrInfo->second;
      if (arr->size() <= idx)
      {
        std::cerr << "[err] Array idx " << idx << " is greater than array size "
                  << arr->size() << ".   Line : " << GetProgramCounter() <<  std::endl;

        exit(1);
      }

      arr->at(idx) = data;
      return;
    }
  }

  std::cerr << "[err] Array \"" << arrayName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

void Resources::SetGlobalArrayAt(std::string arrayName, const unsigned int idx, int data)
{
  auto mapEnd = _arrays[0][0].end();
  auto arrInfo = _arrays[0][0].find(arrayName);

  // 要素が存在していた場合の範囲外エラー処理
  if (arrInfo != mapEnd)
  {
    std::vector<int>* arr = &arrInfo->second;
    if (arr->size() <= idx)
    {
      std::cerr << "[err] Array idx " << idx << " is greater than array size "
                << arr->size() << ".   Line : " << GetProgramCounter() <<  std::endl;

      exit(1);
    }
    arr->at(idx) = data;
    return;
  }

  std::cerr << "[err] Array \"" << arrayName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;

  return;
}

int Resources::GetLocalArrayAt(std::string arrayName, const unsigned int idx)
{
  for (int blockDepth = GetBlockDepth(); 0 <= blockDepth; blockDepth--) {

    // その階層で変数が宣言されていない場合スキップ
    if (_arrays[GetCallStackDepth()].size() <= blockDepth)
      continue;

    auto mapEnd = _arrays[GetCallStackDepth()][blockDepth].end();
    auto arrInfo = _arrays[GetCallStackDepth()][blockDepth].find(arrayName);

    // 要素が存在していた場合
    if (arrInfo != mapEnd)
    {
      std::vector<int>* arr = &arrInfo->second;
      if (arr->size() <= idx)
      {
        std::cerr << "[err] Array idx " << idx << " is greater than array size "
                  << arr->size() << ".   Line : " << GetProgramCounter() <<  std::endl;

        exit(1);
      }

      return arr->at(idx);
    }
  }

  std::cerr << "[err] Array \"" << arrayName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

int Resources::GetGlobalArrayAt(std::string arrayName, const unsigned int idx)
{
  auto mapEnd = _arrays[0][0].end();
  auto arrInfo = _arrays[0][0].find(arrayName);

  // 要素が存在していた場合
  if (arrInfo != mapEnd)
  {
    std::vector<int>* arr = &arrInfo->second;
    if (arr->size() <= idx)
    {
      std::cerr << "[err] Array idx " << idx << " is greater than array size "
                << arr->size() << ".   Line : " << GetProgramCounter() <<  std::endl;

      exit(1);
    }

    return arr->at(idx);
  }

  std::cerr << "[err] Array \"" << arrayName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

void Resources::FreeLocalArray(std::string arrayName)
{
  for (int blockDepth = GetBlockDepth(); 0 <= blockDepth; blockDepth--) {

    // その階層で変数が宣言されていない場合スキップ
    if (_arrays[GetCallStackDepth()].size() <= blockDepth)
      continue;

    auto mapEnd = _arrays[GetCallStackDepth()][blockDepth].end();
    auto arrInfo = _arrays[GetCallStackDepth()][blockDepth].find(arrayName);

    // 要素が存在していた場合
    if (arrInfo != mapEnd)
    {
      _arrays[GetCallStackDepth()][blockDepth].erase(arrayName);

      return;
    }
  }

  std::cerr << "[err] Array \"" << arrayName
            << "\" is not declared.   Line : " << GetProgramCounter() <<  std::endl;
  exit(1);
}

void Resources::FreeGlobalArray(std::string arrayName)
{
  auto mapEnd = _arrays[0][0].end();
  auto arrInfo = _arrays[0][0].find(arrayName);

  // 要素が存在していた場合
  if (arrInfo != mapEnd)
  {
    _arrays[0][0].erase(arrayName);

    return;
  }

  std::cerr << "[err] Array \"" << arrayName
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

  // ラベルが登録されていなかった場合-1を返す
  return (newPC == _labels.end()) ? -1 : newPC->second;
}

void Resources::SetLabel(const std::string labelName, const unsigned int address)
{
  _labels[labelName] = address;
}

unsigned int Resources::GetFunctionAddress(const std::string funcName) const
{
  auto newPC = _functions.find(funcName);

  // ラベルが登録されていなかった場合-1を返す
  return (newPC == _labels.end()) ? -1 : newPC->second;
}

void Resources::SetFunction(const std::string funcName, const unsigned int address)
{
  _functions[funcName] = address;
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

void Resources::PushCallStack(const unsigned int pc)
{
  _callStack.push(pc);
}

const unsigned int Resources::GetCallStackTop()
{
  const unsigned int top = _callStack.top();
  _callStack.pop();
  return top;
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
