#include "self_balancing_binary_search_tree.h"

#include <fstream>
#include <iostream>

namespace s21 {

bool SelfBalancingBinarySearchTree::SetData(std::string key, data data) {
  bool rez = false;
  long t = (long)time(NULL);
  if (data.time_life > 0) data.create_time = t;
  try {
    rez = m_base.insert(key, data).second;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return rez;
}

data SelfBalancingBinarySearchTree::GetData(const std::string &key) {
  data rez = m_base.at(key);
  return rez;
}

bool SelfBalancingBinarySearchTree::Exists(const std::string &key) {
  return m_base.contains(key);
}

bool SelfBalancingBinarySearchTree::DelData(const std::string &key) {
  Map<std::string, data>::iterator it(m_base.begin());
  bool rez = false;
  while (it != m_base.end()) {
    if (it.operator*().first == key) {
      rez = true;
      m_base.erase(it);
      break;
    }
    ++it;
  }
  return rez;
}
void SelfBalancingBinarySearchTree::UpdateData(const std::string &key,
                                               data data) {
  if (data.surname == "-") data.surname = m_base.operator[](key).surname;
  if (data.name == "-") data.name = m_base.operator[](key).name;
  if (data.year_of_birth == -1)
    data.year_of_birth = m_base.operator[](key).year_of_birth;
  if (data.city == "-") data.city = m_base.operator[](key).city;
  if (data.coins == -1)
    data.coins = m_base.operator[](key).coins;
  if (data.time_life == -1) {
    data.time_life = m_base.operator[](key).time_life;
  } else {
    data.create_time = time(NULL);
  }
  m_base.at(key) = data;
}

std::vector<std::string> SelfBalancingBinarySearchTree::ReturnKeys() {
  std::vector<std::string> rez;
  Map<std::string, data>::iterator it(m_base.begin());
  while (it != m_base.end()) {
    rez.push_back(it.operator*().first);
    ++it;
  }
  return rez;
}
void SelfBalancingBinarySearchTree::RenameKeys(const std::string &cur_keys, const std::string &new_keys) {
  data curData = m_base.at(cur_keys);
  bool rez = DelData(cur_keys);
  if (rez == true) m_base.insert(new_keys, curData);
}

long SelfBalancingBinarySearchTree::GetTTL(const std::string &key) {
  long timeL = m_base.at(key).time_life;
  long timeC = m_base.at(key).create_time;
  if (timeL > 0 && (((long)time(NULL) - timeC) < timeL)) {
    timeL -= time(NULL) - timeC;
  }
  return timeL;
}

std::vector<std::string> SelfBalancingBinarySearchTree::FindKeys(data dt) {
  std::vector<std::string> rez;
  Map<std::string, data>::iterator it(m_base.begin());
  while (it != m_base.end()) {
    bool tmp = true;
    if (dt.city != "-")
      tmp = dt.city != it.operator*().second.city ? false : tmp;
    if (dt.name != "-")
      tmp = dt.name != it.operator*().second.name ? false : tmp;
    if (dt.surname != "-")
      tmp = dt.surname != it.operator*().second.surname ? false : tmp;
    if (dt.coins != -1)
      tmp = dt.coins != it.operator*().second.coins ? false : tmp;
    if (dt.year_of_birth != -1)
      tmp = dt.year_of_birth != it.operator*().second.year_of_birth ? false : tmp;
    if (tmp == true) rez.push_back(it.operator*().first);
    ++it;
  }
  return rez;
}

void SelfBalancingBinarySearchTree::ShowAll() {
  Map<std::string, data>::iterator it(m_base.begin());
  PrintHeader();
  if (m_base.size() > 0) {
    long count = 1;
    while (it != m_base.end()) {
      PrintItem(count++, it.operator*().second);
      ++it;
    }
  }
  std::cout << "\u001b[1;35;1;1m";
}

void SelfBalancingBinarySearchTree::PrintHeader() {
  std::cout << "\u001b[1;32;1;1m"
            << "> ";
  std::cout << std::left << std::setw(5) << std::setfill(' ') << "№   |";
  std::cout << std::setw(15) << std::setfill(' ') << "   Фамилия    |";
  std::cout << std::setw(15) << "     Имя      |";
  std::cout << std::setw(6) << " Год |";
  std::cout << std::setw(15) << "    Город     |";
  std::cout << std::setw(8) << "Количество коинов";
  std::cout << std::endl;
}

void SelfBalancingBinarySearchTree::PrintItem(unsigned long index, data data) {
    std::cout << "\u001b[1;32;1;1m"
            << "> ";
  std::cout << std::setw(5) << index;
  std::cout << std::setw(15) << data.surname;
  std::cout << std::setw(15) << data.name;
  std::cout << std::setw(6) << data.year_of_birth;
  std::cout << std::setw(15) << data.city;
  std::cout << std::setw(8) << data.coins;
  std::cout << std::endl;
}


unsigned long SelfBalancingBinarySearchTree::LoadData(
    const std::string &filePath) {
  std::ifstream ifs(filePath);
  if (!ifs.is_open()) {
    throw std::runtime_error("Can`t open file");
  }
  if (ifs.eof() == true) {
    throw std::runtime_error("Empty file");
  }
  std::string line;
  unsigned long countStrings = 0;
  while (getline(ifs, line)) {
    std::vector<std::string> result = SplitString(line);
    if (result.size() != 6) {
      throw std::runtime_error("Error data on file");
    }
    std::string key = result[0];
    data tmp = {result[1], result[2], std::stoi(result[3]), result[4],
                std::stol(result[5])};
    try {
      m_base.insert(key, tmp);
      ++countStrings;
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
  ifs.close();
  return countStrings;
}

unsigned long SelfBalancingBinarySearchTree::SaveData(
    const std::string &filePath) {
  std::ofstream ofs(filePath);
  if (!ofs.is_open()) {
    throw std::runtime_error("Can`t write file");
  }
  unsigned long countStrings = 0;
  Map<std::string, data>::iterator it(m_base.begin());
  while (it != m_base.end()) {
    ofs << it.operator*().first << " " << it.operator*().second.surname << " "
        << it.operator*().second.name << " "
        << it.operator*().second.year_of_birth << " "
        << it.operator*().second.city << " " << it.operator*().second.coins
        << std::endl;
    ++it;
    ++countStrings;
  }
  ofs.close();
  return countStrings;
}

void SelfBalancingBinarySearchTree::DeleteOldNode() {
  s21::Map<std::string, data>::MapIterator it = m_base.begin();
  int flag = 1;
  while (it._curIter != nullptr && it != m_base.end()) {
    if (it.operator*().second.time_life > 0 &&
        (((long)time(NULL) - it.operator*().second.create_time) >=
         (unsigned long)it.operator*().second.time_life)) {
      try {
        m_base.erase(it);
        it = m_base.begin();
        flag = 0;
      } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
      }
    }
    if (flag == 1) {
      ++it;
    } else {
      ++flag;
    }
  }
}

std::vector<std::string> SelfBalancingBinarySearchTree::SplitString(
    std::string line) {
  std::string splitters = "[ \f\n\r\t\v]";
  std::vector<std::string> result;
  std::string::size_type start(0), finish(0);
  while (finish != std::string::npos) {
    while (splitters.find_first_of(line[start]) != std::string::npos) ++start;
    if (start == std::string::npos) break;
    finish = line.find_first_of(splitters, start);
    result.emplace_back(line.substr(start, finish - start));
    if (finish == std::string::npos) break;
    start = finish;
  }
  return result;
}

void SelfBalancingBinarySearchTree::Clear() { m_base.Clear(); }

}  // namespace s21