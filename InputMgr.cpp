#include "InputMgr.hpp"
#include <sstream>

InputMgr::InputMgr(std::string fileName) : _fileName(fileName)
{
    _inputFileStream.open(_fileName);

    // オープンエラー処理
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

    // 一行読み込む
    std::string buf;
    std::getline(_inputFileStream, buf);

    // 空白で区切って vector に詰める
    std::stringstream ss{buf};
    std::string tmpWord;
    while (getline(ss, tmpWord, ' ')) {
        res.push_back(tmpWord);
    }

    return res;
}
