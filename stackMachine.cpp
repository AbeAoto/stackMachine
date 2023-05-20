#include "stackMachine.hpp"
#include <iostream>
#include <fstream>
#include <bitset>

void StackMachine::ParseFile(std::string fileName)
{
  std::ifstream inputFile(fileName);

  // オープンエラー処理
  if (!inputFile.is_open())
  {
    std::cerr << "Could not open the file : " << fileName << std::endl;
    exit(1);
  }

  // 命令分岐(多分あんまりよくない実装)
  std::string word;
  unsigned short instructionNum = 0;
  while (inputFile >> word)
  {
    unsigned short opecode = 0;
    short operand = 0;

    if (word == "PUSH")
    {
      inputFile >> word;
      opecode = static_cast<unsigned short>(OPECODES::PUSH);
      operand = static_cast<short>(std::stoi(word));
    }
    else if (word == "POP")
    {
      opecode = static_cast<unsigned short>(OPECODES::POP);
    }
    else if (word == "ADD")
    {
      opecode = static_cast<unsigned short>(OPECODES::ADD);
    }
    else if (word == "SUB")
    {
      opecode = static_cast<unsigned short>(OPECODES::SUB);
    }
    else if (word == "MUL")
    {
      opecode = static_cast<unsigned short>(OPECODES::MUL);
    }
    else if (word == "DIV")
    {
      opecode = static_cast<unsigned short>(OPECODES::DIV);
    }
    else if (word == "PRINT")
    {
      opecode = static_cast<unsigned short>(OPECODES::PRINT);
    }
    else if (word == "LABEL")
    {
      inputFile >> word;
      opecode = static_cast<unsigned short>(OPECODES::LABEL);
      operand = static_cast<unsigned short>(std::stoi(word));

      if (IsLabelsContain(operand)) {
        std::cerr << "Same label has already declared : " << operand << std::endl;
        exit(1);
      }

      unsigned int label = (operand << _labelAddressBytes) | (instructionNum & ((1 << _labelAddressBytes) - 1));
      _labels.push_back(label);
    }
    else
    {
      std::cerr << "This operation is not supported!" << std::endl;
      exit(1);
    }

    // オペコードとオペランドを命令列のフォーマットに変更
    unsigned int instruction = (opecode << _operandBytes) | (operand & ((1 << _operandBytes) - 1));
    _instructions.push_back(instruction);

    instructionNum += 1;
  }

  inputFile.close();
}

void StackMachine::DoInstructions()
{
  for (int i = 0; i < _labels.size(); i++) {
    std::cout << "labels[" << i << "] : " << std::bitset<32>(_labels[i]) << std::endl;
  }

  _programCounter = 0;
  const unsigned int instructionNum = _instructions.size();

  while(_programCounter < instructionNum) {

    const unsigned int instruction = _instructions[_programCounter];
    const OPECODES opecode = static_cast<OPECODES>(GetOpecodeFromInstruction(instruction));
    const short operand = GetOperandFromInstruction(instruction);

    switch(opecode) {
      case OPECODES::PUSH:  Push(operand);   break;
      case OPECODES::POP:   Pop();           break;
      case OPECODES::ADD:   Add();           break;
      case OPECODES::SUB:   Sub();           break;
      case OPECODES::MUL:   Mul();           break;
      case OPECODES::DIV:   Div();           break;
      case OPECODES::PRINT: Print();         break;
      case OPECODES::LABEL: break;
    }
    _programCounter++;
  }
}

void StackMachine::Push(int num)
{
  _stack.push(num);
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

void StackMachine::Pop()
{
  _stack.pop();
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
  for (int i = 0; i < _labels.size(); i++) {
    const unsigned short _label = (_labels[i] & ~((1 << _labelAddressBytes) - 1)) >> _labelAddressBytes;
    if (_label == label) {
      return true;
    }
  }

  return false;
}
