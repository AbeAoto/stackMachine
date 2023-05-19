#include "stackMachine.hpp"
#include <iostream>
#include <fstream>

void StackMachine::parseFile(std::string fileName)
{
  std::ifstream inputFile(fileName);

  // オープンエラー処理
  if (!inputFile.is_open()) {
    std::cerr << "Could not open the file : " << fileName << std::endl;
    exit(1);
  }

  // 命令分岐(多分あんまりよくない実装)
  std::string word;
  while (inputFile >> word) {

    if (word == "PUSH") {
      inputFile >> word;
      push(std::stoi(word));
    }
    else if (word == "POP") {
      pop();
    }
    else if (word == "ADD") {
      add();
    }
    else if (word == "SUB") {
      sub();
    }
    else if (word == "MUL") {
      mul();
    }
    else if (word == "DIV") {
      div();
    }
    else if (word == "PRINT") {
      print();
    }
    else {
      std::cerr << "This operation is not supported!" << std::endl;
    }
  }

  inputFile.close();
}

void StackMachine::push(int num)
{
  _stack.push(num);
}

void StackMachine::add()
{
  //もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2) {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 + op2);
}

void StackMachine::sub()
{
  //もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2) {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 - op2);
}

void StackMachine::mul()
{
  //もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2) {
    exit(1);
  }

  int op1 = _stack.top();
  _stack.pop();
  int op2 = _stack.top();
  _stack.pop();
  _stack.push(op1 * op2);
}

void StackMachine::div()
{
  //もうちょっとちゃんとした例外投げた方がいい
  if (_stack.size() < 2) {
    exit(1);
  }

  // (先/後)の順にしたいのでオペランド取得順が逆になっている
  int op2 = _stack.top();
  _stack.pop();
  int op1 = _stack.top();
  _stack.pop();

  if (op2 == 0) {
    std::cerr << "[err] divided 0 : " << op1 << "/" << op2 << std::endl;
    exit(1);
  }

  _stack.push(op1 / op2);
}

void StackMachine::print()
{
  std::cout << _stack.top() << std::endl;
}

void StackMachine::pop()
{
  _stack.pop();
}
