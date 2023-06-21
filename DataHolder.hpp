#pragma once

class _DataHolder {};

template <typename T>
class DataHolder : _DataHolder
{
public:
  DataHolder(T &data) : _data(data) {}
  T& GetData() const {  return _data;  }
  void SetData(T &data) {  _data = data;  }
private:
  T _data;
};
