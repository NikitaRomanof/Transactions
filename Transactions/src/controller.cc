#include "controller.h"

#include "hash_table.h"
#include "self_balancing_binary_search_tree.h"

namespace s21 {
Controller::Controller(int type) {
  if (type == 1) {
    p_base = new HashTable();
  } else if (type == 2) {
    p_base = new SelfBalancingBinarySearchTree();
  } else {
    throw std::invalid_argument("Controller::Controller, invalid type");
  }
  if (p_base == nullptr) {
    throw std::invalid_argument("Controller::Controller, Base not found");
  }
}

Controller::~Controller() {
  if (p_base != nullptr) {
    p_base->Clear();
    p_base = nullptr;
  }
}

bool Controller::set(std::string key, std::string sName, std::string name,
                     int birth, std::string city, long coins, long time_life) {
  data tmp = {sName, name, birth, city, coins, time_life};
  bool rez;
  try {
    rez = p_base->SetData(key, tmp);
  } catch (std::exception &e) {
    rez = false;
  }
  return rez;
}

std::pair<std::string, data> Controller::get(const std::string &key) {
  std::pair<std::string, data> rez;
  try {
    rez = {"", p_base->GetData(key)};
  } catch (std::exception &e) {
    data tmp = {"-1", "-1", -1, "-1", -1};
    rez = {"> (null)", tmp};
  }
  return rez;
}

bool Controller::Exists(const std::string &key) { return p_base->Exists(key); }

bool Controller::del(const std::string &key) {
  bool rez;
  try {
    rez = p_base->DelData(key);
  } catch (std::exception &e) {
    rez = false;
  }
  return rez;
}

bool Controller::update(std::string key, std::string sName, std::string name,
                        int birth, std::string city, long coins,
                        long time_life) {
  bool rez;
  try {
    data tmp = {sName, name, birth, city, coins, time_life};
    p_base->UpdateData(key, tmp);
    rez = true;
  } catch (std::exception &e) {
    rez = false;
  }
  return rez;
}

std::vector<std::string> Controller::keys() { return p_base->ReturnKeys(); }

bool Controller::rename(std::string key1, std::string key2) {
  bool rez;
  try {
    p_base->RenameKeys(key1, key2);
    rez = true;
  } catch (std::exception &e) {
    rez = false;
  }
  return rez;
}

long Controller::ttl(const std::string &key) {
  long rez;
  try {
    rez = p_base->GetTTL(key);
  } catch (std::exception &e) {
    rez = -1;
  }
  return rez;
}

std::vector<std::string> Controller::find(std::string sName, std::string name,
                                          int birth, std::string city,
                                          long coins) {
  data tmp = {sName, name, birth, city, coins};
  return p_base->FindKeys(tmp);
}

void Controller::ShowAll() { p_base->ShowAll(); }

std::pair<std::string, unsigned long> Controller::upload(
    const std::string &filePath) {
  std::string error = "";
  unsigned long rez = 0;
  try {
    rez = p_base->LoadData(filePath);
  } catch (std::exception &e) {
    error = e.what();
  }
  return {error, rez};
}

std::pair<std::string, unsigned long> Controller::exp(
    const std::string &filePath) {
  std::string error = "";
  unsigned long rez = 0;
  try {
    rez = p_base->SaveData(filePath);
  } catch (std::exception &e) {
    error = e.what();
  }
  return {error, rez};
}
void Controller::Clear() {
  if (p_base != nullptr) p_base->Clear();
}
std::vector<std::string> Controller::split(std::string line) {
  return p_base->SplitString(line);
}

void Controller::deleteOld() {
  if (p_base != nullptr) {
    p_base->DeleteOldNode();
  }
}

//
}  // namespace s21