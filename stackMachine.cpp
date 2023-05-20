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

        // コメント行をとばす
        if (word.find_first_of('#') == 0) {
            continue;
        }

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
        else if (word == "STORE")
        {
            inputFile >> word;
            opecode = static_cast<unsigned short>(OPECODES::STORE);
            operand = static_cast<short>(std::stoi(word));
        }
        else if (word == "LOAD")
        {
            inputFile >> word;
            opecode = static_cast<unsigned short>(OPECODES::LOAD);
            operand = static_cast<short>(std::stoi(word));
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

            if (IsLabelsContain(operand))
            {
                std::cerr << "Same label has already declared : " << operand << std::endl;
                exit(1);
            }

            unsigned int label = (operand << _labelAddressBytes) | (instructionNum & ((1 << _labelAddressBytes) - 1));
            _labels.push_back(label);
        }
        else if (word == "JUMP")
        {
            inputFile >> word;
            opecode = static_cast<unsigned short>(OPECODES::JUMP);
            operand = static_cast<unsigned short>(std::stoi(word));
        }
        else if (word == "JPEQ0")
        {
            inputFile >> word;
            opecode = static_cast<unsigned short>(OPECODES::JPEQ0);
            operand = static_cast<unsigned short>(std::stoi(word));
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
    _programCounter = 0;
    const unsigned int instructionNum = _instructions.size();

    while (_programCounter < instructionNum)
    {

        const unsigned int instruction = _instructions[_programCounter];
        const OPECODES opecode = static_cast<OPECODES>(GetOpecodeFromInstruction(instruction));
        const short operand = GetOperandFromInstruction(instruction);

        switch (opecode)
        {
        case OPECODES::PUSH:
            Push(operand);
            break;
        case OPECODES::POP:
            Pop();
            break;
        case OPECODES::STORE:
            Store(operand);
            break;
        case OPECODES::LOAD:
            Load(operand);
            break;
        case OPECODES::ADD:
            Add();
            break;
        case OPECODES::SUB:
            Sub();
            break;
        case OPECODES::MUL:
            Mul();
            break;
        case OPECODES::DIV:
            Div();
            break;
        case OPECODES::PRINT:
            Print();
            break;
        case OPECODES::LABEL:
            break;
        case OPECODES::JUMP:
            Jump(operand);
            break;
        case OPECODES::JPEQ0:
            Jpeq0(operand);
            break;
        default:
            std::cerr << "This Opecode is not valid : " << opecode << std::endl;
            exit(1);
        }
        _programCounter++;
    }
}

void StackMachine::Push(int num)
{
    _stack.push(num);
}

void StackMachine::Pop()
{
    _stack.pop();
}

void StackMachine::Store(const unsigned short dst)
{
    _variables[dst] = _stack.top();
    _stack.pop();
}

void StackMachine::Load(const unsigned short src)
{
    Push(_variables[src]);
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
    for (int i = 0; i < _labels.size(); i++)
    {
        const unsigned short _label = (_labels[i] & ~((1 << _labelAddressBytes) - 1)) >> _labelAddressBytes;
        if (_label == label)
        {
            return true;
        }
    }

    return false;
}

const unsigned short StackMachine::GetLabeledAddress(const unsigned short label) const
{
    unsigned short address = -1;
    for (int i = 0; i < _labels.size(); i++)
    {
        const unsigned short _label = (_labels[i] & ~((1 << _labelAddressBytes) - 1)) >> _labelAddressBytes;
        if (_label == label)
        {
            address = (_labels[i] & ((1 << _labelAddressBytes) - 1));
            break;
        }
    }
    return address;
}

void StackMachine::JampAddressValidCheck() const
{
    for (int i = 0; i < _instructions.size(); i++)
    {
        const OPECODES opecode = static_cast<OPECODES>((_instructions[i] & ~((1 << _operandBytes) - 1)) >> _operandBytes);
        const unsigned short label = (_labels[i] & ((1 << _labelAddressBytes) - 1));
        if ((opecode == OPECODES::JUMP || opecode == OPECODES::JPEQ0) && !IsLabelsContain(label))
        {
            std::cerr << "JUMP operand label is not valid. : [" << i << "] JUMP " << label << std::endl;
            exit(1);
        }
    }
}
