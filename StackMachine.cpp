#include "StackMachine.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

void StackMachine::DoInstructions()
{
  _resources->SetProgramCounter(0);

  while (true) {
    bool readNewLine = ((int)_resources->GetInstructionsSize() - 1 < (int)_resources->GetProgramCounter());
    // 新規読み込み判定
    if (readNewLine)
    {
      _resources->SetInstruction(_inputMgr->GetLineInstruction(), _resources->GetProgramCounter());
    }

    const unsigned int instructionIdx = 0;
    std::vector<std::string> inst = _resources->LoadInstruction(_resources->GetProgramCounter());
    std::string opString = inst[instructionIdx];

    // ラベルの新規登録
    if(opString.back() == ':')
    {
      // エラー処理
      if (opString.size() - 1 <= 0)
      {
        std::cerr << "[err] Label Name is not required.   Line : " << _resources->GetProgramCounter() <<  std::endl;
        exit(1);
      }

      // 新規ラベル登録
      if (readNewLine)
      {
        opString.pop_back();
        _resources->SetLabel(opString, _resources->GetProgramCounter());
      }

      // 後方JUMPだった場合のラベルサーチ処理
      if (opString == _searchingLabelName)
      {
        _isSearchingJumpLabel = false;
        _searchingLabelName = "";
      }

      _resources->IncrementProgramCounter();
      continue;
    }

    OPECODES op = StringToOpecodes(opString);

    // ラベルサーチ中だった場合命令は実行せず読み飛ばす
    if (_isSearchingJumpLabel)
    {
      if (op == OPECODES::END)
      {
          std::cerr << "[err] The program has reached the endpoint but JUMP label is not found.  LABEL name: "
                  << _searchingLabelName << std::endl;
        exit(1);
      }
      _resources->IncrementProgramCounter();
      continue;
    }

    switch (op)
    {
    case OPECODES::PUSH:     Push(inst);  break;
    case OPECODES::POP:      Pop();  break;
    case OPECODES::SETLOCAL: SetLocal(inst);  break;
    case OPECODES::GETLOCAL: GetLocal(inst);  break;
    case OPECODES::ADD:      Add();  break;
    case OPECODES::SUB:      Sub();  break;
    case OPECODES::MUL:      Mul();  break;
    case OPECODES::DIV:      Div();  break;
    case OPECODES::PRINT:    Print();  break;
    case OPECODES::JUMP:     Jump(inst);  break;
    case OPECODES::JPEQ0:    Jpeq0(inst);  break;
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
  _resources->PushStack(num);
}

void StackMachine::Pop()
{
  _resources->PopStack();
}

void StackMachine::SetLocal(std::vector<std::string> inst)
{
  _resources->SetLocalVariable(inst[1], _resources->TopStack());
}

void StackMachine::GetLocal(std::vector<std::string> inst)
{
  _resources->PushStack(_resources->GetLocalVariableValue(inst[1]));
}

void StackMachine::Add()
{
  if (_resources->GetStackSize() < 2)
  {
    std::cerr << "[err]Add operation needs 2 or more stack elements.  Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op1 = _resources->TopStack();
  _resources->PopStack();
  int op2 = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(op1 + op2);
}

void StackMachine::Sub()
{
  if (_resources->GetStackSize() < 2)
  {
    std::cerr << "[err]Sub operation needs 2 or more stack elements.  Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op1 = _resources->TopStack();
  _resources->PopStack();
  int op2 = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(op1 - op2);
}

void StackMachine::Mul()
{
  if (_resources->GetStackSize() < 2)
  {
    std::cerr << "[err]Mul operation needs 2 or more stack elements.  Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op1 = _resources->TopStack();
  _resources->PopStack();
  int op2 = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(op1 * op2);
}

void StackMachine::Div()
{
  if (_resources->GetStackSize() < 2)
  {
    std::cerr << "[err]Div operation needs 2 or more stack elements.  Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  // (先/後)の順にしたいのでオペランド取得順が逆になっている
  int op2 = _resources->TopStack();
  _resources->PopStack();
  int op1 = _resources->TopStack();
  _resources->PopStack();

  if (op2 == 0)
  {
    std::cerr << "[err] divided 0 : " << op1 << "/" << op2 << std::endl;
    exit(1);
  }

  _resources->PushStack(op1 / op2);
}

void StackMachine::Print()
{
  std::cout << _resources->TopStack() << std::endl;
}

void StackMachine::Jump(std::vector<std::string> inst)
{
  if (inst.size() <= 1)
  {
    std::cerr << "[err] You need to assign jump destination label.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  unsigned int newPC = _resources->GetLabeledAddress(inst[1]);
  if (newPC == -1)  // ラベルがまだ登録されていない場合
  {
    _isSearchingJumpLabel = true;
    _searchingLabelName = inst[1];
  }
  else
  {
    _resources->SetProgramCounter(_resources->GetLabeledAddress(inst[1]));
  }
}

void StackMachine::Jpeq0(std::vector<std::string> inst)
{
  if (inst.size() <= 1)
  {
    std::cerr << "[err] You need to assign jump destination label.   Line : " << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  // スタックが空の場合のエラー
  if (_resources->GetStackSize() <= 0)
  {
    std::cerr << "[err] Stack is empty with operation JPEQ0.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  bool needToJump = (_resources->TopStack() == 0);
  _resources->PopStack();

  if (needToJump)
  {
    Jump(inst);
  }
}

void StackMachine::Gt()
{
  if (_resources->GetStackSize() <= 1)
  {
    std::cerr << "[err] Stack needs at least 2 elements with operation GT.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op1 = _resources->TopStack();
  _resources->PopStack();
  int op2 = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(op1 > op2);
}

void StackMachine::Lt()
{
  if (_resources->GetStackSize() <= 1)
  {
    std::cerr << "[err] Stack needs at least 2 elements with operation LT.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  int op2 = _resources->TopStack();
  _resources->PopStack();
  int op1 = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(op1 < op2);
}

void StackMachine::LogNot()
{
  // スタックが空の場合のエラー
  if (_resources->GetStackSize() <= 0)
  {
    std::cerr << "[err] Stack is empty with operation LOGNOT.   Line : "
              << _resources->GetProgramCounter() << std::endl;
    exit(1);
  }

  bool stackTop = _resources->TopStack();
  _resources->PopStack();
  _resources->PushStack(!stackTop);
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
