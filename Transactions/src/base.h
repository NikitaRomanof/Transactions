#pragma once

#include <string>
#include <vector>
#include <iomanip>
namespace s21 {
struct data {
  std::string surname;
  std::string name;
  int year_of_birth = -1;
  std::string city;
  long coins = -1;
  long time_life = 0;
  // 0 дефолтное значение, если время жизни не ограниченно
  // -1 - если значение не указанно

  unsigned long create_time = 0;
};

class Base {
 public:
  Base(){};
  virtual ~Base(){};

  virtual bool SetData(std::string key, data data) = 0;
  virtual data GetData(const std::string &key) = 0;
  virtual bool Exists(const std::string &key) = 0;
  virtual bool DelData(const std::string &key) = 0;
  virtual void UpdateData(const std::string &key, data data) = 0;
  virtual std::vector<std::string> ReturnKeys() = 0;
  virtual void RenameKeys(const std::string &cur_keys, const std::string &new_keys) = 0;
  virtual long GetTTL(const std::string &key) = 0;
  virtual std::vector<std::string> FindKeys(data data) = 0;
  virtual void ShowAll() = 0;
  virtual unsigned long LoadData(const std::string &filePath) = 0;
  virtual unsigned long SaveData(const std::string &filePath) = 0;
  virtual void Clear() = 0;
  virtual std::vector<std::string> SplitString(std::string line) = 0;
  virtual void DeleteOldNode() = 0;
};
}  // namespace s21