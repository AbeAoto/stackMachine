#include <string>
#include <stack>
#include <map>
#include <vector>

#pragma once

class Resources
{
// singleton�p�r
private:
  static Resources* _singleton;

  // �R���X�g���N�^
  Resources() : _programCounter(0), _callStackDepth(1), _blockDepth(0)
  {
    _arrays.resize(2);
    _variables.resize(2);
  };

  // �R�s�[�R���X�g���N�^
  Resources(const Resources& src)
  {
    _singleton = src._singleton;
  };

  // ������Z�q
  Resources& operator=(const Resources& src)
  {
    _singleton = src._singleton;
    return *this;
  }

public:
  // �f�X�g���N�^
  virtual ~Resources()
  {
    delete _singleton;
  }

// �O���C���^�[�t�F�[�X
public:
  // singleton�p�C���X�^���X�擾�֐�
  static Resources* GetInstance();

  // ���ߊ֘A
  const unsigned int GetInstructionsSize() const;
  void SetInstruction(std::vector<std::string> inst, const unsigned short pc);
  const std::vector<std::string> LoadInstruction(const unsigned short pc) const;

  // �ϐ��֘A
  void SetLocalVariable(std::string varName, int varVal);
  const int GetLocalVariableValue(std::string varName) const;

  // �z��֘A
  void AllocateLocalArray(std::string arrayName, int size);
  void SetLocalArrayAt(std::string arrayName, const unsigned int idx, int data);
  int GetLocalArrayAt(std::string arrayName, const unsigned int idx);
  void FreeLocalArray(std::string arrayName);

  // �v���O�����J�E���^�֘A
  void IncrementProgramCounter();
  void SetProgramCounter(const unsigned int num);
  const unsigned int GetProgramCounter() const;

  // ���x���֘A
  unsigned int GetLabeledAddress(const std::string labelName) const;
  void SetLabel(const std::string labelName, const unsigned int address);

  // �֐��֘A
  unsigned int GetFunctionAddress(const std::string funcName) const;
  void SetFunction(const std::string funcName, const unsigned int address);

  // �u���b�N�E�R�[���X�^�b�N�֘A
  void IncreaseBlockDepth();
  void DecreaseBlockDepth();
  const unsigned int GetBlockDepth() const;

  void IncreaseCallStackDepth();
  void DecreaseCallStackDepth();
  const unsigned int GetCallStackDepth() const;
  void PushCallStack(const unsigned int pc);
  const unsigned int GetCallStackTop();

  // �v�Z�X�^�b�N�֘A
  void PushStack(const int ele);
  void PopStack();
  const int TopStack() const;
  const unsigned int GetStackSize() const;

// ���\�[�X�錾
private:
  std::stack<int> _stack;
  std::vector<std::vector<std::string>> _instructions;
  std::vector<std::vector<std::map<std::string, int>>> _variables;
  std::vector<std::vector<std::map<std::string, std::vector<int>>>> _arrays;
  std::map<std::string, unsigned int> _labels;  /// ���x���Ƃ��̃A�h���X�������Ă���
  std::map<std::string, unsigned int> _functions;  /// �֐����Ƃ��̃A�h���X�������Ă���
  std::stack<unsigned int> _callStack;
  unsigned int _programCounter;
  unsigned int _blockDepth;
  unsigned int _callStackDepth;
};
