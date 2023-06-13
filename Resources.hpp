#include <string>
#include <map>

#pragma once

class Resources
{
// singleton用途
private:
  static Resources* _singleton;

  // コンストラクタ
  Resources() : _programCounter(0) {};

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

  // プログラムカウンタ関連
  void IncrementProgramCounter();
  void SetProgramCounter(const unsigned int num);
  unsigned int GetProgramCounter() const;

  // ラベル関連
  unsigned int GetLabeledAddress(const std::string labelName) const;
  void SetLabel(const std::string labelName, const unsigned int address);

  // ブロック・コールスタック関連
  void IncreaseBlockDepth();
  void DecreaseBlockDepth();
  const unsigned int GetBlockDepth() const;
// リソース宣言
private:
  unsigned int _programCounter;
  unsigned int _blockDepth;
  std::map<std::string, unsigned int> _labels;  /// ラベルとそのアドレスが入っている
};
