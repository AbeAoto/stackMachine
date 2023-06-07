#include <string>
#include <vector>
#include <stack>
#include <map>
#include <limits.h>
#include <iostream>
#include "InputMgr.hpp"

#pragma once

enum OPECODES {
        PUSH,
        POP,
        STORE,
        LOAD,
        SETLOCAL,
        GETLOCAL,
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

class StackMachine
{
public:
    // コンストラクタ・デストラクタ
  StackMachine(std::string fileName) : _programCounter(0), _callStackDepth(1), _blockDepth(0)
    {
        _variables.reserve(USHRT_MAX+1);
        _labels.reserve(USHRT_MAX+1);
        for (int i = 0; i < USHRT_MAX+1; i++) {
            _labels[i] = USHRT_MAX;
        }
        _variables2.resize(2);
        std::cout << fileName << std::endl;
        _inputMgr = new InputMgr(fileName);
    };
    ~StackMachine(){  delete _inputMgr; };

    /// @brief ファイルから命令を読み込み、正常判定を行う。
    /// @param fileName  読み込みファイル名
    void ParseFile();

    /// @brief 読み込んだ命令を実行する
    void DoInstructions();

private:
    // 命令関連
    void Push(std::vector<std::string> inst);
    void Pop();
    void Store(const unsigned short dst);
    void Load(const unsigned short src);
    void SetLocal(std::vector<std::string> inst);
    void GetLocal(std::vector<std::string> inst);
    void Call(const unsigned short func);
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

    OPECODES StringToOpecodes(std::string instruction);

    // アーキテクチャ関連
    std::vector<std::vector<std::string>> _instructions2;
    std::vector<unsigned int> _instructions;     /// 命令列
    std::vector<unsigned short> _labels;         /// ラベルとそのアドレスが入っている
    std::vector<unsigned short> _variables;      /// 変数ラベルとその値が入っている
    std::vector<std::vector<std::map<std::string, int>>> _variables2;
    std::stack<int> _stack;                      /// スタック
    std::stack<unsigned short> _callStack;                  /// 関数のコールスタック
    unsigned int _programCounter;
    unsigned int _blockDepth;
    unsigned int _callStackDepth;

    // 定数関連
    const unsigned short _opecodeBytes = 16;
    const unsigned short _operandBytes = 16;

    const unsigned short _labelNameBytes = 16;
    const unsigned short _labelAddressBytes = 16;

    // 補助クラス
    InputMgr* _inputMgr;
};
