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
  Resources() : _programCounter(0), _callStackDepth(1) {};

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

  // プログラムカウンタ関連
  void IncrementProgramCounter();
  void SetProgramCounter(const unsigned int num);
  const unsigned int GetProgramCounter() const;

  // ラベル関連
  unsigned int GetLabeledAddress(const std::string labelName) const;
  void SetLabel(const std::string labelName, const unsigned int address);

  // ブロック・コールスタック関連
  void IncreaseBlockDepth();
  void DecreaseBlockDepth();
  const unsigned int GetBlockDepth() const;

  void IncreaseCallStackDepth();
  void DecreaseCallStackDepth();
  const unsigned int GetCallStackDepth() const;

  // 計算スタック関連
  void PushStack(const int ele);
  void PopStack();
  const int TopStack() const;
  const unsigned int GetStackSize() const;

// リソース宣言
private:
  std::stack<int> _stack;
  std::vector<std::vector<std::string>> _instructions;
  unsigned int _programCounter;
  unsigned int _blockDepth;
  unsigned int _callStackDepth;

  std::map<std::string, unsigned int> _labels;  /// ラベルとそのアドレスが入っている
};
