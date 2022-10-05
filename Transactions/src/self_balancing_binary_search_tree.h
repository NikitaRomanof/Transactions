#pragma once

#include "base.h"
#include "s21_map.h"

namespace s21 {

class SelfBalancingBinarySearchTree : public Base {
 public:
  SelfBalancingBinarySearchTree() {}
  ~SelfBalancingBinarySearchTree() {}
  SelfBalancingBinarySearchTree(SelfBalancingBinarySearchTree const&) = delete;
  SelfBalancingBinarySearchTree(SelfBalancingBinarySearchTree&&) = delete;
  SelfBalancingBinarySearchTree& operator=(
      SelfBalancingBinarySearchTree const&) = delete;
  SelfBalancingBinarySearchTree& operator=(SelfBalancingBinarySearchTree&&) =
      delete;
  bool SetData(std::string key, data data);
  data GetData(const std::string& key);
  bool Exists(const std::string& key);
  bool DelData(const std::string& key);
  void UpdateData(const std::string& key, data data);
  std::vector<std::string> ReturnKeys();
  void RenameKeys(const std::string &cur_keys, const std::string &new_keys);
  long GetTTL(const std::string& key);
  std::vector<std::string> FindKeys(data data);
  void ShowAll();
  unsigned long LoadData(const std::string& filePath);
  unsigned long SaveData(const std::string& filePath);
  void exportTreeToDot(const std::string& filePath);
  void Clear();
  std::vector<std::string> SplitString(std::string line);
  void DeleteOldNode();

 private:
  Map<std::string, data> m_base;
  void PrintItem(unsigned long index, data data);
  void PrintHeader();
};
}  // namespace s21