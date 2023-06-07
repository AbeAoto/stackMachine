#include "InputMgr.hpp"
#include <sstream>

InputMgr::InputMgr(std::string fileName) : _fileName(fileName)
{
    _inputFileStream.open(_fileName);

    // �I�[�v���G���[����
    if (!_inputFileStream.is_open())
    {
        std::cerr << "Could not open the file : " << fileName << std::endl;
        exit(1);
    }
}

InputMgr::~InputMgr()
{
    _inputFileStream.close();
}

std::vector<std::string> InputMgr::GetLineInstruction()
{
    std::vector<std::string> res;

    // ��s�ǂݍ���
    std::string buf;
    std::getline(_inputFileStream, buf);

    // �󔒂ŋ�؂��� vector �ɋl�߂�
    std::stringstream ss{buf};
    std::string tmpWord;
    while (getline(ss, tmpWord, ' ')) {
        res.push_back(tmpWord);
    }

    return res;
}
