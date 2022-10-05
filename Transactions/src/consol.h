#pragma once

#include <iostream>

#include "controller.h"

namespace s21 {
class Consol {
 public:
  void start();
  //

 private:
  void menu();
  void internalMenu();
  void coutStart();
  void coutMenu();
  void set();
  void get();
  void Exists();
  void del();
  void update();
  void keys();
  void rename();
  void ttl();
  void find();
  void upload();
  void exp();
  Controller *p_ctrl = nullptr;

  std::string end = "\u001b[0m";
  std::string end1 = "\u001b[0m\n";
  std::string end2 = "\u001b[1;35;1;1m\n";
  std::string style1 = "\u001b[1;35;1;1m";
  std::string style2 = "\u001b[1;33;1;1m";
  std::string style3 = "\u001b[1;32;1;1m";
  std::string style4 = "\u001b[1;31;1;1m";
};
}  // namespace s21
