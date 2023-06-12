#include "stackMachine.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

void StackMachine::DoInstructions()
{
  _resources->SetProgramCounter(0);

  while (true) {
    bool readNewLine = ((int)_instructions.size() - 1 <= (int)_resources->GetProgramCounter());
    // 新規読み込み判定
    if (readNewLine)
    {
      _instructions.push_back(_inputMgr->GetLineInstruction());
    }

    const unsigned int instructionIdx = 0;
    std::string opString = _instructions[_resources->GetProgramCounter()][instructionIdx];

    // ラベルの新規登録
    if(opString.back() == ':')
    {
      if (opString.size() - 1 <= 0)
      {
        std::cerr << "[err] Label Name is not required.   Line : " << _resources->GetProgramCounter() <<  std::endl;
        exit(1);
      }

      if (readNewLine)
      {
        opString.pop_back();
        _resources->SetLabel(opString, _resources->GetProgramCounter());
      }

      _resources->IncrementProgramCounter();
      continue;
    }

    OPECODES op = StringToOpecodes(_instructions[_resources->GetProgramCounter()][instructionIdx]);
    switch (op)
    {
    case OPECODES::PUSH:     Push(_instructions[_resources->GetProgramCounter()]);  break;
    case OPECODES::POP:      Pop();  break;
    case OPECODES::SETLOCAL: SetLocal(_instructions[_resources->GetProgramCounter()]);  break;
    case OPECODES::GETLOCAL: GetLocal(_instructions[_resources->GetProgramCounter()]);  break;
    case OPECODES::ADD:      Add();  break;
    case OPECODES::SUB:      Sub();  break;
    case OPECODES::MUL:      Mul();  break;
    case OPECODES::DIV:      Div();  break;
    case OPECODES::PRINT:    Print();  break;
    case OPECODES::JUMP:     Jump(_instructions[_resources->GetProgramCounter()]);  break;
    case OPECODES::JPEQ0:    Jpeq0(_instructions[_resources->GetProgramCounter()]);  break;
    case OPECODES::GT:       Gt();  break;
    case OPECODES::LT:       Lt();  break;
    case OPECODES::LOGNOT:   LogNot();  break;
    case OPECODES::END:      return;
    }
    _resources->IncrementProgramCounter();
  }
}

void StackMachine::Push(std::vector<std::string> inst)
{
  int num = std::stoi(inst[1]);
  _stack.push(num);
}

void StackMachine::Pop()
{
  _stack.pop();
}

void StackMachine::SetLocal(std::vector<std::string> inst)
{
  // テーブルサイズ調整
  if (_variables[_callStackDepth].size() <= _blockDepth)
  {
    _variables[_callStackDepth].resize(_blockDepth+1);
  }

  std::string variableName = inst[1];
  std::map<std::string, int>* varMap = &_variables[_callStackDepth][_blockDepth];
  (*varMap)[variableName] = _stack.top();
}

void StackMachine::GetLocal(std::vector<std::string> inst)
{
  std::string variableName = inst[1];
  for (int blockDepth = _blockDepth; 0 <= blockDepth; blockDepth--) {

    // その階層で変数が宣言されていない場合スキップ
    if (_variables[_callStackDepth].size() <= blockDepth)
      continue;

    std::map<std::string, int> varMap = _variables[_callStackDepth][blockDepth];
    auto varInfo = _variables[_callStackDepth][blockDepth].find(variableName);

    // 要素が存在していた場合
    if (varInfo != _variables[_callStackDepth][blockDepth].end())
    {
      _stack.push(varInfo->second);
      return;
    }
  }

  std::cerr << "[err] variable \"" << variableName
            << "\" is not declared.   Line : " << _resources->GetProgramCounter() <<  std::endl;
  exit(1);
}

// void StackMachine::Call(const unsigned short func)
// {
//     _callStack.push(_resources->GetProgramCounter());
//     Jump(func);
// }

void StackMachine::Add()
{
  // もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2)
  {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 + op2);
}

void StackMachine::Sub()
{
  // もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2)
  {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 - op2);
}

void StackMachine::Mul()
{
  // もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2)
  {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 * op2);
}

void StackMachine::Div()
{
  // もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2)
  {
    exit(1);
  }

  // (先/後)の順にしたいのでオペランド取得順が逆になっている
  int op2 = _stack.top();
  _stack.pop();
  int op1 = _stack.top();
  _stack.pop();

  if (op2 == 0)
  {
    std::cerr << "[err] divided 0 : " << op1 << "/" << op2 << std::endl;
    exit(1);
  }

  _stack.push(op1 / op2);
}

void StackMachine::Print()
{
  std::cout << _stack.top() << std::endl;
}

void StackMachine::Jump(std::vector<std::string> inst)
{
  if (inst.size() <= 1)
  {
    std::cerr << "[err] You need to assign jump destination label.   Line : " << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  _resources->SetProgramCounter(_resources->GetLabeledAddress(inst[1]));
}

void StackMachine::Jpeq0(std::vector<std::string> inst)
{
  if (inst.size() <= 1)
  {
    std::cerr << "[err] You need to assign jump destination label.   Line : " << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  // スタックが空の場合のエラー
  if (_stack.size() <= 0)
  {
    std::cerr << "[err] Stack is empty with operation JPEQ0.   Line : " << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  bool needToJump = (_stack.top() == 0);
  _stack.pop();

  if (needToJump)
  {
    Jump(inst);
  }
}

void StackMachine::Gt()
{
  if (_stack.size() <= 1)
  {
    std::cerr << "[err] Stack needs at least 2 elements with operation GT.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op2 = _stack.top();
  _stack.pop();
  int op1 = _stack.top();
  _stack.pop();

  _stack.push((op2 > op1));
}

void StackMachine::Lt()
{
  if (_stack.size() <= 1)
  {
    std::cerr << "[err] Stack needs at least 2 elements with operation LT.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op2 = _stack.top();
  _stack.pop();
  int op1 = _stack.top();
  _stack.pop();

  _stack.push((op2 < op1));
}

void StackMachine::LogNot()
{
  // スタックが空の場合のエラー
  if (_stack.size() <= 0)
  {
    std::cerr << "[err] Stack is empty with operation LOGNOT.   Line : " << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  bool stackTop = _stack.top();
  _stack.pop();
  _stack.push(!stackTop);
}

const unsigned short StackMachine::GetOpecodeFromInstruction(const unsigned int instruction) const
{
  return (instruction & ~((1 << _operandBytes) - 1)) >> _operandBytes;
}

const short StackMachine::GetOperandFromInstruction(const unsigned int instruction) const
{
  return instruction & ((1 << _operandBytes) - 1);
}

unsigned short StackMachine::stringToHash(const std::string& str)
{
  unsigned short hash = 0;
  for (char ch : str) {
    hash = (hash << 1) ^ ch;
  }
  return hash;
}

OPECODES StackMachine::StringToOpecodes(std::string instruction)
{
  if (instruction ==  "PUSH")          return OPECODES::PUSH;
  else if (instruction == "POP")       return OPECODES::POP;
  else if (instruction == "SETLOCAL")  return OPECODES::SETLOCAL;
  else if (instruction == "GETLOCAL")  return OPECODES::GETLOCAL;
  else if (instruction == "CALL")      return OPECODES::CALL;
  else if (instruction == "ADD")       return OPECODES::ADD;
  else if (instruction == "SUB")       return OPECODES::SUB;
  else if (instruction == "MUL")       return OPECODES::MUL;
  else if (instruction == "DIV")       return OPECODES::DIV;
  else if (instruction == "PRINT")     return OPECODES::PRINT;
  else if (instruction == "JUMP")      return OPECODES::JUMP;
  else if (instruction == "JPEQ0")     return OPECODES::JPEQ0;
  else if (instruction == "GT")        return OPECODES::GT;
  else if (instruction == "LT")        return OPECODES::LT;
  else if (instruction == "LOGNOT")    return OPECODES::LOGNOT;
  else                                 return OPECODES::END;
}
