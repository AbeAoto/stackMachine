#include <string>
#include <map>

#pragma once

class Resources
{
// singleton�p�r
private:
  static Resources* _singleton;

  // �R���X�g���N�^
  Resources() : _programCounter(0) {};

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

  // �v���O�����J�E���^�֘A
  void IncrementProgramCounter();
  void SetProgramCounter(const unsigned int num);
  unsigned int GetProgramCounter() const;

  // ���x���֘A
  unsigned int GetLabeledAddress(const std::string labelName) const;
  void SetLabel(const std::string labelName, const unsigned int address);

  // �u���b�N�E�R�[���X�^�b�N�֘A
  void IncreaseBlockDepth();
  void DecreaseBlockDepth();
  const unsigned int GetBlockDepth() const;
// ���\�[�X�錾
private:
  unsigned int _programCounter;
  unsigned int _blockDepth;
  std::map<std::string, unsigned int> _labels;  /// ���x���Ƃ��̃A�h���X�������Ă���
};
