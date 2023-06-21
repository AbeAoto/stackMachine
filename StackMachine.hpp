﻿#include <string>
#include <vector>
#include <stack>
#include <map>
#include <limits.h>
#include <iostream>
#include "InputMgr.hpp"
#include "Resources.hpp"

#pragma once

enum OPECODES {
  PUSH,
  POP,
  SETLOCAL,
  GETLOCAL,
  ALLOCARR,
  SETARR,
  GETARR,
  FREEARR,
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
  GT,
  LT,
  LOGNOT,
  END,
};

class StackMachine
{
public:
  // コンストラクタ・デストラクタ
  StackMachine(std::string fileName)
  {
    _inputMgr = new InputMgr(fileName);
    _resources = Resources::GetInstance();
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
  void SetLocal(std::vector<std::string> inst);
  void GetLocal(std::vector<std::string> inst);
  void AllocateLocalArray(std::vector<std::string> inst);
  void SetLocalArrayAt(std::vector<std::string> inst);
  void GetLocalArrayAt(std::vector<std::string> inst);
  void FreeArray(std::vector<std::string> inst);
  void Call(const unsigned short func);
  void Add();
  void Sub();
  void Mul();
  void Div();
  void Print();
  void Jump(std::vector<std::string> inst);
  void Jpeq0(std::vector<std::string> inst);
  void Gt();
  void Lt();
  void LogNot();

  // String ⇔ オペコードenum変換
  OPECODES StringToOpecodes(std::string instruction);

  // 補助クラス
  InputMgr* _inputMgr;
  Resources* _resources;
};


