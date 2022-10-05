#pragma once

#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "base.h"

namespace s21 {

class HashTable : public Base {
 public:
  HashTable();
  ~HashTable();
  HashTable(const HashTable &other) = delete;
  HashTable(HashTable &&other) = delete;
  HashTable &operator=(const HashTable &other) = delete;

  bool SetData(std::string key, data data);            // insert_or_assign
  data GetData(const std::string &key);                // at or []
  bool Exists(const std::string &key);                 // contains
  bool DelData(const std::string &key);                // erase
  void UpdateData(const std::string &key, data data);  // []
  std::vector<std::string> ReturnKeys();
  void RenameKeys(const std::string &cur_keys, const std::string &new_keys);
  long GetTTL(const std::string &key);
  std::vector<std::string> FindKeys(data data);
  void ShowAll();
  unsigned long LoadData(const std::string &filePath);
  unsigned long SaveData(const std::string &filePath);
  void DeleteOldNode();
  void Clear();
  std::vector<std::string> SplitString(std::string line);

 private:
  // methods
  void PrintItem(unsigned long index, data data);
  void PrintHeader();
  unsigned long HashFunction(std::string key);
  void HashResize();
  std::vector<std::pair<std::string, data>> CopyDataToSimpleVector();

  data DefaultStruct() {
    data new_data{"", "", 0, "", 0};
    return new_data;
  }
  // fields
  const unsigned int kDefaultHashSize = 13;
  const unsigned int kDefaultAMultiplyer = 7;
  unsigned int hash_size_;       // размер хеш-таблицы
  unsigned int num_of_structs_;  // счетчик структур
  unsigned int a_;
  bool cycle = true;
  std::vector<std::vector<std::pair<std::string, data>>> hash_table_container;
};
}  // namespace s21