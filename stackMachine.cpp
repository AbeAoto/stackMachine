#include "stackMachine.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

void StackMachine::DoInstructions()
{
    _programCounter = 0;

    while (true) {
        if (_instructions2.size() - 1 <= _programCounter || _instructions2.size() == 0)
        {
            _instructions2.push_back(_inputMgr->GetLineInstruction());
        }

        const unsigned int instructionIdx = 0;
        OPECODES op = StringToOpecodes(_instructions2[_programCounter][instructionIdx]);
        std::cout << "op : " << op << std::endl;
        switch (op)
        {
        case OPECODES::PUSH:     Push(_instructions2[_programCounter]);  break;
        case OPECODES::POP:      Pop();  break;
        case OPECODES::SETLOCAL: SetLocal(_instructions2[_programCounter]);  break;
        case OPECODES::ADD:      Add();  break;
        case OPECODES::SUB:      Sub();  break;
        case OPECODES::MUL:      Mul();  break;
        case OPECODES::DIV:      Div();  break;
        case OPECODES::PRINT:    Print();  break;
        case OPECODES::END:      return;
        }
        _programCounter++;
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

void StackMachine::Store(const unsigned short dst)
{
    _variables[dst] = _stack.top();
}

void StackMachine::Load(const unsigned short src)
{
  // Push(_variables[src]);
}

void StackMachine::SetLocal(std::vector<std::string> inst)
{
  // テーブルサイズ調整
  if (_variables2[_callStackDepth].size() <= _blockDepth)
  {
      _variables2[_callStackDepth].resize(_blockDepth+1);
  }

  std::string variableName = inst[1];
  std::map<std::string, int> varMap = _variables2[_callStackDepth][_blockDepth];
  varMap[variableName] = _stack.top();
}

void StackMachine::Call(const unsigned short func)
{
    _callStack.push(_programCounter);
    Jump(func);
}

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

void StackMachine::Jump(unsigned short label)
{
    const unsigned short address = GetLabeledAddress(label);
    _programCounter = address;
}

void StackMachine::Jpeq0(unsigned short label)
{
    if (_stack.size() < 1)
    {
        exit(1);
    }

    int stackTop = _stack.top();
    _stack.pop();

    if (stackTop == 0)
    {
        Jump(label);
    }
}

const unsigned short StackMachine::GetOpecodeFromInstruction(const unsigned int instruction) const
{
    return (instruction & ~((1 << _operandBytes) - 1)) >> _operandBytes;
}

const short StackMachine::GetOperandFromInstruction(const unsigned int instruction) const
{
    return instruction & ((1 << _operandBytes) - 1);
}

const bool StackMachine::IsLabelsContain(const unsigned short label) const
{
    // すべてのbitが立っている(つまりUSHRT_MAX)だと未定義
    return (_labels[label] == USHRT_MAX) ? false : true;
}

const unsigned short StackMachine::GetLabeledAddress(const unsigned short label) const
{
    return (_labels[label] & ((1 << _labelAddressBytes) - 1));
}

void StackMachine::JampAddressValidCheck() const
{
    for (int i = 0; i < _instructions.size(); i++)
    {
        const OPECODES opecode = static_cast<OPECODES>(GetOpecodeFromInstruction(_instructions[i]));
        const unsigned short label = GetOperandFromInstruction(_instructions[i]);
        if ((opecode == OPECODES::JUMP || opecode == OPECODES::JPEQ0) && !IsLabelsContain(label))
        {
            std::cerr << "JUMP operand label is not valid. : [" << i << "] JUMP " << label << std::endl;
            exit(1);
        }
    }
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
    else if (instruction == "STORE")     return OPECODES::STORE;
    else if (instruction == "LOAD")      return OPECODES::LOAD;
    else if (instruction == "SETLOCAL")  return OPECODES::SETLOCAL;
    else if (instruction == "CALL")      return OPECODES::CALL;
    else if (instruction == "ADD")       return OPECODES::ADD;
    else if (instruction == "SUB")       return OPECODES::SUB;
    else if (instruction == "MUL")       return OPECODES::MUL;
    else if (instruction == "DIV")       return OPECODES::DIV;
    else if (instruction == "PRINT")     return OPECODES::PRINT;
    else if (instruction == "JUMP")      return OPECODES::JUMP;
    else if (instruction == "JPEQ0")     return OPECODES::JPEQ0;
    else                                 return OPECODES::END;
}
