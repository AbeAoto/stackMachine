#include <string>
#include <vector>
#include <stack>

#pragma once

class StackMachine
{
public:
    // コンストラクタ
    StackMachine(){};

    // デストラクタ
    ~StackMachine(){};

    void parseFile(std::string fileName);

    void push(int num);

    void pop();

    void add();

    void sub();

    void mul();

    void div();

    void print();

private:
    enum OPECODES {
        PUSH,
        POP,
        ADD,
        SUB,
        MUL,
        DIV,
        PRINT,
    };

    const unsigned short GetOpecodeFromInstruction(const unsigned int instruction) const;
    const short GetOperandFromInstruction(const unsigned int instruction) const;

    std::vector<unsigned int> _instructions;   /// 命令列
    std::stack<int> _stack;                      /// スタック

    const unsigned short opecodeBytes = 16;
    const unsigned short operandBytes = 16;
};
