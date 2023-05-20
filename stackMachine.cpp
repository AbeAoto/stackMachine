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
      opecode = static_cast<int>(OPECODES::POP);
    }
    else if (word == "ADD")
    {
      opecode = static_cast<int>(OPECODES::ADD);
    }
    else if (word == "SUB")
    {
      opecode = static_cast<int>(OPECODES::SUB);
    }
    else if (word == "MUL")
    {
      opecode = static_cast<int>(OPECODES::MUL);
    }
    else if (word == "DIV")
    {
      opecode = static_cast<int>(OPECODES::DIV);
    }
    else if (word == "PRINT")
    {
      opecode = static_cast<int>(OPECODES::PRINT);
    }
    else
    {
      std::cerr << "This operation is not supported!" << std::endl;
      exit(1);
    }

    // オペコードとオペランドを命令列のフォーマットに変更
    unsigned int instruction = (opecode << operandBytes) | (operand & ((1 << operandBytes) - 1));
    _instructions.push_back(instruction);
  }

  inputFile.close();
}

void StackMachine::DoInstructions()
{
  programCounter = 0;
  const unsigned int instructionNum = _instructions.size();

  while(programCounter < instructionNum) {

    const unsigned int instruction = _instructions[programCounter];
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
    }
    programCounter++;
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
  return (instruction & ~((1 << operandBytes) - 1)) >> operandBytes;
}

const short StackMachine::GetOperandFromInstruction(const unsigned int instruction) const
{
  return instruction & ((1 << operandBytes) - 1);
}
