#pragma once

#include "base.h"

namespace s21 {
//
class Controller {
 public:
  Controller(int type);
  ~Controller();
  Controller(Controller const &) = delete;
  Controller(Controller &&) = delete;
  Controller &operator=(Controller const &) = delete;
  Controller &operator=(Controller &&) = delete;

  bool set(std::string key, std::string sName, std::string name, int birth,
           std::string city, long coins, long time_life = 0);
  std::pair<std::string, data> get(const std::string &key);
  bool Exists(const std::string &key);
  bool del(const std::string &key);
  bool update(std::string key, std::string sName, std::string name, int birth,
              std::string city, long coins, long time_life = 0);
  std::vector<std::string> keys();
  bool rename(std::string key1, std::string key2);
  long ttl(const std::string &key);
  std::vector<std::string> find(std::string sName, std::string name, int birth,
                                std::string city, long coins);
  void ShowAll();
  std::pair<std::string, unsigned long> upload(const std::string &filePath);
  std::pair<std::string, unsigned long> exp(const std::string &filePath);
  void Clear();
  std::vector<std::string> split(std::string line);
  void deleteOld();

  //
  //

 private:
  Base *p_base = nullptr;
  std::vector<std::string> SplitString(std::string line);

  //
};

}  // namespace s21