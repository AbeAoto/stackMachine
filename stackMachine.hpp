#include <string>
#include <vector>
#include <stack>

#pragma once

class StackMachine
{
public:
    // コンストラクタ
    StackMachine() : _programCounter(0) {};

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

    void Jump(unsigned short label);

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
        LABEL,
        JUMP,
    };

    const unsigned short GetOpecodeFromInstruction(const unsigned int instruction) const;
    const short GetOperandFromInstruction(const unsigned int instruction) const;

    const bool IsLabelsContain(const unsigned short label) const;
    const unsigned short GetLabeledAddress(const unsigned short label) const;

    void JampAddressValidCheck() const;

    std::vector<unsigned int> _instructions;     /// 命令列
    std::vector<unsigned int> _labels;         /// ラベルとそのアドレスが入っている
    std::stack<int> _stack;                      /// スタック

    unsigned int _programCounter;
    const unsigned short _opecodeBytes = 16;
    const unsigned short _operandBytes = 16;

    const unsigned short _labelNameBytes = 16;
    const unsigned short _labelAddressBytes = 16;
};
