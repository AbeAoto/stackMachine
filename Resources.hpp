#include <string>
#include <stack>
#include <map>
#include <vector>

#pragma once

class Resources
{
// singleton用途
private:
  static Resources* _singleton;

  // コンストラクタ
  Resources() : _programCounter(0), _callStackDepth(1), _blockDepth(0)
  {
    _arrays.resize(2);
    _variables.resize(2);
  };

  // コピーコンストラクタ
  Resources(const Resources& src)
  {
    _singleton = src._singleton;
  };

  // 代入演算子
  Resources& operator=(const Resources& src)
  {
    _singleton = src._singleton;
    return *this;
  }

public:
  // デストラクタ
  virtual ~Resources()
  {
    delete _singleton;
  }

// 外部インターフェース
public:
  // singleton用インスタンス取得関数
  static Resources* GetInstance();

  // 命令関連
  const unsigned int GetInstructionsSize() const;
  void SetInstruction(std::vector<std::string> inst, const unsigned short pc);
  const std::vector<std::string> LoadInstruction(const unsigned short pc) const;

  // 変数関連
  void SetLocalVariable(std::string varName, int varVal);
  const int GetLocalVariableValue(std::string varName) const;

  // 配列関連
  void AllocateLocalArray(std::string arrayName, int size);
  void SetLocalArrayAt(std::string arrayName, const unsigned int idx, int data);
  int GetLocalArrayAt(std::string arrayName, const unsigned int idx);
  void FreeLocalArray(std::string arrayName);

  // プログラムカウンタ関連
  void IncrementProgramCounter();
  void SetProgramCounter(const unsigned int num);
  const unsigned int GetProgramCounter() const;

  // ラベル関連
  unsigned int GetLabeledAddress(const std::string labelName) const;
  void SetLabel(const std::string labelName, const unsigned int address);

  // 関数関連
  unsigned int GetFunctionAddress(const std::string funcName) const;
  void SetFunction(const std::string funcName, const unsigned int address);

  // ブロック・コールスタック関連
  void IncreaseBlockDepth();
  void DecreaseBlockDepth();
  const unsigned int GetBlockDepth() const;

  void IncreaseCallStackDepth();
  void DecreaseCallStackDepth();
  const unsigned int GetCallStackDepth() const;
  void PushCallStack(const unsigned int pc);
  const unsigned int GetCallStackTop();

  // 計算スタック関連
  void PushStack(const int ele);
  void PopStack();
  const int TopStack() const;
  const unsigned int GetStackSize() const;

// リソース宣言
private:
  std::stack<int> _stack;
  std::vector<std::vector<std::string>> _instructions;
  std::vector<std::vector<std::map<std::string, int>>> _variables;
  std::vector<std::vector<std::map<std::string, std::vector<int>>>> _arrays;
  std::map<std::string, unsigned int> _labels;  /// ラベルとそのアドレスが入っている
  std::map<std::string, unsigned int> _functions;  /// 関数名とそのアドレスが入っている
  std::stack<unsigned int> _callStack;
  unsigned int _programCounter;
  unsigned int _blockDepth;
  unsigned int _callStackDepth;
};
