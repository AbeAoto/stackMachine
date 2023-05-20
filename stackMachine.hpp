#include <string>
#include <vector>
#include <stack>
#include <limits.h>
#include <iostream>

#pragma once

class StackMachine
{
public:
    // コンストラクタ・デストラクタ
    StackMachine() : _programCounter(0)
    {
        _variables.reserve(USHRT_MAX+1);
        _labels.reserve(USHRT_MAX+1);
        for (int i = 0; i < USHRT_MAX+1; i++) {
            _labels[i] = USHRT_MAX;
        }
    };
    ~StackMachine(){};

    /// @brief ファイルから命令を読み込み、正常判定を行う。
    /// @param fileName  読み込みファイル名
    void ParseFile(std::string fileName);

    /// @brief 読み込んだ命令を実行する
    void DoInstructions();

private:
    enum OPECODES {
        PUSH,
        POP,
        STORE,
        LOAD,
        FUNC,
        CALL,
        RET,
        ADD,
        SUB,
        MUL,
        DIV,
        PRINT,
        LABEL,
        JUMP,
        JPEQ0,
        END,
    };

    // 命令関連
    void Push(int num);
    void Pop();
    void Store(const unsigned short dst);
    void Load(const unsigned short src);
    void Call(const unsigned short func);
    void Return();
    void Add();
    void Sub();
    void Mul();
    void Div();
    void Print();
    void Jump(unsigned short label);
    void Jpeq0(unsigned short label);

    // 命令→オペコード・オペランド変換
    const unsigned short GetOpecodeFromInstruction(const unsigned int instruction) const;
    const short GetOperandFromInstruction(const unsigned int instruction) const;

    // ラベル判定
    const bool IsLabelsContain(const unsigned short label) const;
    const unsigned short GetLabeledAddress(const unsigned short label) const;

    // ジャンプ判定
    void JampAddressValidCheck() const;

    // ハッシュ関連(ラベル・変数)
    unsigned short stringToHash(const std::string& str);

    // アーキテクチャ関連
    std::vector<unsigned int> _instructions;     /// 命令列
    std::vector<unsigned short> _labels;         /// ラベルとそのアドレスが入っている
    std::vector<unsigned short> _variables;      /// 変数ラベルとその値が入っている
    std::stack<int> _stack;                      /// スタック
    std::stack<unsigned short> _callStack;                  /// 関数のコールスタック
    unsigned int _programCounter;

    // 定数関連
    const unsigned short _opecodeBytes = 16;
    const unsigned short _operandBytes = 16;

    const unsigned short _labelNameBytes = 16;
    const unsigned short _labelAddressBytes = 16;
};
