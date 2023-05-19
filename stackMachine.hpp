#include <string>
#include <stack>

#pragma once

class StackMachine
{
public:
  // コンストラクタ
  StackMachine() {};

  // デストラクタ
  ~StackMachine() {};

  void parseFile(std::string fileName);

  void push(int num);

  void add();

  void sub();

  void mul();

  void div();

  void print();

  void pop();

private:
  std::stack<int> _stack;
};
