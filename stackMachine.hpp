#include <string>
#include <vector>
#include <stack>

#pragma once

class StackMachine
{
public:
    // コンストラクタ
    StackMachine() : programCounter(0) {};

    // デストラクタ
    ~StackMachine(){};

    void ParseFile(std::string fileName);

    void Push(int num);

    void Pop();

    void Add();

    void Sub();

    void Mul();

    void Div();

    void Print();

    void DoInstructions();

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

    unsigned int programCounter;
    const unsigned short opecodeBytes = 16;
    const unsigned short operandBytes = 16;
};
