#include "consol.h"

#include <algorithm>

namespace s21 {

void Consol::start() {
  coutStart();
  menu();
}

void Consol::coutStart() {
  std::cout << style1 << "\n*******************************************" << end
            << std::endl;
  std::cout << style2 << "                TRANSACTIONS             " << end
            << std::endl;
  std::cout << style1 << "*******************************************" << end
            << std::endl;
}

void Consol::coutMenu() {
  std::cout << style1 << "*********************************************" << end
            << std::endl;
  std::cout << style2 << "                   MENU                      " << end
            << std::endl;
  std::cout << style1 << "*********************************************" << end1
            << std::endl;
  std::cout << style3
            << " 1. ----> Hash table                                  " << end
            << std::endl;
  std::cout << style3
            << " 2. ----> Binary search tree                          " << end
            << std::endl;
  std::cout << style3
            << " 0. ----> Exit from the program                       " << end1
            << std::endl;
}

void Consol::menu() {
  while (true) {
    coutMenu();
    int ch;
    std::cin >> ch;
    if (ch == 1) {
      try {
        p_ctrl = new Controller(1);
      } catch (const std::exception& e) {
        std::cout << style4 << e.what() << end << std::endl;
      }
      if (p_ctrl != nullptr) {
        internalMenu();
        delete p_ctrl;
        p_ctrl = nullptr;
      }
    } else if (ch == 2) {
      try {
        p_ctrl = new Controller(2);
      } catch (const std::exception& e) {
        std::cout << style4 << e.what() << end << std::endl;
      }
      if (p_ctrl != nullptr) {
        internalMenu();
        delete p_ctrl;
        p_ctrl = nullptr;
      }
    } else if (ch == 0) {
      return;
    } else {
      std::cout << style4 << "\nError: incorrect input, try again " << end
                << std::endl;
    }
  }
}

void Consol::set() {
  std::cout << end;
  std::string line;
  std::getline(std::cin >> std::ws, line);
  std::vector<std::string> result = p_ctrl->split(line);
  try {
    if (result.size() == 6) {
      if (p_ctrl->set(result[0], result[1], result[2], std::stoul(result[3]),
                      result[4], std::stol(result[5]))) {
        std::cout << style3 << "> OK" << end2 << std::endl;
      } else {
        std::cout << style4 << "> ERROR" << end2 << std::endl;
      }
    } else if (result.size() == 8 && result[6] == "EX") {
      if (p_ctrl->set(result[0], result[1], result[2], std::stol(result[3]),
                      result[4], std::stol(result[5]), std::stol(result[7]))) {
        std::cout << style3 << "> OK" << end2 << std::endl;
      } else {
        std::cout << style4 << "> ERROR" << end2 << std::endl;
      }
    } else {
      std::cout << style4 << "\nError: incorrect input into SET function "
                << end2 << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << style4 << e.what() << '\n'
              << "\nError: incorrect input into SET function " << end2
              << std::endl;
  }
}

void Consol::get() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  std::pair<std::string, data> rez = p_ctrl->get(line);
  if (rez.first.size() > 1) {
    std::cout << style4 << "> (null)" << end2 << std::endl;
  } else {
    std::cout << style3
              << "> " + rez.second.surname + " " + rez.second.name + " "
              << rez.second.year_of_birth << " " + rez.second.city + " "
              << rez.second.coins << end2 << std::endl;
  }
}

void Consol::Exists() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  if (p_ctrl->Exists(line) == true) {
    std::cout << style3 << "> true" << end2 << std::endl;
  } else {
    std::cout << style4 << "> false" << end2 << std::endl;
  }
}

void Consol::del() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  if (p_ctrl->del(line) == true) {
    std::cout << style3 << "> true" << end2 << std::endl;
  } else {
    std::cout << style4 << "> false" << end2 << std::endl;
  }
}

void Consol::update() {
  std::cout << end;
  std::string line;
  std::getline(std::cin >> std::ws, line);
  std::vector<std::string> result = p_ctrl->split(line);
  try {
    if (result.size() >= 6) {
      result[3] = result[3] == "-" ? "-1" : result[3];
      result[5] = result[5] == "-" ? "-1" : result[5];
      long tmp = -1;
      if (result.size() == 8 && result[6] == "EX") tmp = std::stol(result[7]);
      if (p_ctrl->update(result[0], result[1], result[2], std::stoul(result[3]),
                         result[4], std::stol(result[5]), tmp)) {
        std::cout << style3 << "> OK" << end2 << std::endl;
      } else {
        std::cout << style4 << "> ERROR" << end2 << std::endl;
      }
    } else {
      std::cout << style4 << "\nError: incorrect input into UPDATE function "
                << end << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << style4 << e.what() << '\n'
              << "\nError: incorrect input into UPDATE function " << end2
              << std::endl;
  }
}

void Consol::keys() {
  std::vector<std::string> allKeys = p_ctrl->keys();
  for (size_t i = 1; i <= allKeys.size(); ++i) {
    std::cout << style3 << "> " << i << ") " << allKeys[i - 1] << end2
              << std::endl;
  }
}

void Consol::rename() {
  std::cout << end;
  std::string line;
  std::getline(std::cin >> std::ws, line);
  std::vector<std::string> result = p_ctrl->split(line);
  if (result.size() == 2) {
    if (p_ctrl->rename(result[0], result[1])) {
      std::cout << style3 << "> OK" << end2 << std::endl;
    } else {
      std::cout << style4 << "> ERROR" << end2 << std::endl;
    }
  } else {
    std::cout << style4 << "\nError: incorrect input into RENAME function "
              << end2 << std::endl;
  }
}

void Consol::ttl() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  long rez = p_ctrl->ttl(line);
  if (rez > -1) {
    std::cout << style3 << "> " << rez << end2 << std::endl;
  } else {
    std::cout << style4 << "> (null)" << end2 << std::endl;
  }
}

void Consol::find() {
  std::cout << end;
  std::string line;
  std::getline(std::cin >> std::ws, line);
  std::vector<std::string> result = p_ctrl->split(line);
  try {
    if (result.size() == 5) {
      result[2] = result[2] == "-" ? "-1" : result[2];
      result[4] = result[4] == "-" ? "-1" : result[4];
      std::vector<std::string> tmp =
          p_ctrl->find(result[0], result[1], std::stoul(result[2]), result[3],
                       std::stol(result[4]));
      if (tmp.size() > 0) {
        for (int i = 1; i <= int(tmp.size()); ++i) {
          std::cout << style3 << "> " << i << ") " << tmp[i - 1] << end2
                    << std::endl;
        }
      } else {
        std::cout << style4 << "> (null)" << end2 << std::endl;
      }
    } else {
      std::cout << style4 << "\nError: incorrect input into FIND function "
                << end2 << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << style4 << e.what() << '\n'
              << "\nError: incorrect input into FIND function " << end2
              << std::endl;
  }
}

void Consol::upload() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  std::pair<std::string, unsigned long> tmp = p_ctrl->upload(line);
  if (tmp.first.size() > 1) {
    std::cout << style4 << tmp.first << end2 << std::endl;
  } else {
    std::cout << style3 << "> OK " << tmp.second << end2 << std::endl;
  }
}

void Consol::exp() {
  std::cout << end;
  std::string line;
  std::cin >> line;
  std::pair<std::string, unsigned long> tmp = p_ctrl->exp(line);
  if (tmp.first.size() > 1) {
    std::cout << style4 << tmp.first << end2 << std::endl;
  } else {
    std::cout << style3 << "> OK " << tmp.second << end2 << std::endl;
  }
}

void Consol::internalMenu() {
  std::cout << style2 << " back ----> Exit from menu " << end2 << std::endl;
  while (true) {
    if (p_ctrl != nullptr) p_ctrl->deleteOld();
    std::string choose;
    std::cin >> choose;
    std::transform(choose.begin(), choose.end(), choose.begin(), ::toupper);
    if (choose == "SET") {
      set();
    } else if (choose == "GET") {
      get();
    } else if (choose == "EXISTS") {
      Exists();
    } else if (choose == "DEL") {
      del();
    } else if (choose == "UPDATE") {
      update();
    } else if (choose == "KEYS") {
      keys();
    } else if (choose == "RENAME") {
      rename();
    } else if (choose == "TTL") {
      p_ctrl->deleteOld();
      ttl();
    } else if (choose == "FIND") {
      find();
    } else if (choose == "SHOWALL") {
      p_ctrl->ShowAll();
    } else if (choose == "UPLOAD") {
      upload();
    } else if (choose == "EXPORT") {
      exp();
    } else if (choose == "BACK") {
      return;
    } else {
      std::cout << style4 << "\nError: incorrect input, try again " << end2
                << std::endl;
    }
  }
}

}  // namespace s21