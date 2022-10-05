#include "hash_table.h"

namespace s21 {

HashTable::HashTable() : hash_size_(kDefaultHashSize), num_of_structs_(0), a_(kDefaultAMultiplyer) {
  hash_table_container.resize(hash_size_);
  std::pair<std::string, struct data> default_pair = {"", DefaultStruct()};
  for (auto it : hash_table_container) {
    it.push_back(default_pair);
  }
}

HashTable::~HashTable() {
  Clear();
  cycle = false;
}

bool HashTable::SetData(std::string key, data data) {
  if (Exists(key) == false) {
    unsigned long index = HashFunction(key);
    num_of_structs_++;
    unsigned long size = hash_table_container.size();
    if (num_of_structs_ > size / 2) {
      num_of_structs_--;
      auto tmp = CopyDataToSimpleVector();
      Clear();
      hash_table_container.resize(size * M_PI);
      a_ = a_ * 2 - 1;
      for (auto element : tmp) {
        SetData(element.first, element.second);
      }
    }
    long t = (long)time(NULL);
    if (data.time_life > 0) data.create_time = t;
    std::pair<std::string, struct data> TheData = {key, data};
    hash_table_container[index].push_back(TheData);
    return true;
  } else {
    return false;
  }
}

data HashTable::GetData(const std::string &key) {
  if (Exists(key) == false) {
    throw std::invalid_argument("invalid key");
  }
  unsigned long index = HashFunction(key);
  std::string tmp;
  int i = 0;
  for (; i < int(hash_table_container[index].size()); ++i) {
    tmp = hash_table_container[index][i].first;
    if (tmp == key) break;
  }
  if (tmp != key) {
    throw std::invalid_argument("No such key in hash table");
  }
  return hash_table_container[index][i].second;
}

bool HashTable::Exists(const std::string &key) {
  unsigned long index = HashFunction(key);
  for (unsigned long i = 0; i < hash_table_container[index].size(); ++i) {
    std::string tmp = hash_table_container[index][i].first;
    if (tmp == key) return true;
  }
  return false;
}

bool HashTable::DelData(const std::string &key) {
  unsigned long index = HashFunction(key);
  if (Exists(key) == false) {
    return false;
  }
  unsigned long i = 0;
  for (; i < hash_table_container[index].size(); ++i) {
    std::string tmp = hash_table_container[index][i].first;
    if (tmp == key) break;
  }
  hash_table_container[index].erase(hash_table_container[index].begin() + i);
  num_of_structs_--;
  return true;
}

void HashTable::UpdateData(const std::string &key, data data) {
  if (Exists(key) == false) {
    throw std::invalid_argument("No data with such key. Cannot update.");
  }
  unsigned long index = HashFunction(key);
  struct data updated = GetData(key);
  if (data.name != "-") updated.name = data.name;
  if (data.surname != "-") updated.surname = data.surname;
  if (data.year_of_birth != -1) updated.year_of_birth = data.year_of_birth;
  if (data.city != "-") updated.city = data.city;
  if (data.coins != -1) updated.coins = data.coins;
  if (data.time_life != -1) {
    updated.time_life = data.time_life;
    updated.create_time = time(NULL);
  }
  unsigned long i = 0;
  for (; i < hash_table_container[index].size(); ++i) {
    std::string tmp = hash_table_container[index][i].first;
    if (tmp == key) hash_table_container[index][i].second = updated;
  }
}

std::vector<std::string> HashTable::ReturnKeys() {
  std::vector<std::string> keys_vec;
  for (auto key : hash_table_container) {
    int i = key.size();
    while (i > 0) {
      --i;
      keys_vec.push_back(key[i].first);
    }
  }
  return keys_vec;
}

void HashTable::RenameKeys(const std::string &cur_keys, const std::string &new_keys) {
  unsigned long index = HashFunction(cur_keys);
  if (Exists(cur_keys) == false) {
    throw std::invalid_argument("Cannot replace data, no such key in table");
  }
  unsigned long i = 0;
  for (; i < hash_table_container[index].size(); ++i) {
    std::string tmp = hash_table_container[index][i].first;
    if (tmp == cur_keys) break;
  }
  data tmp_data = hash_table_container[index][i].second;
  DelData(cur_keys);
  SetData(new_keys, tmp_data);
}

long HashTable::GetTTL(const std::string &key) {
  if (Exists(key) == false) {
    throw std::invalid_argument("Value deleted by time or never existed");
  }
  long timeL = GetData(key).time_life;
  long timeC = GetData(key).create_time;
  if (timeL > 0 && (((long)time(NULL) - timeC) < timeL)) {
    timeL -= time(NULL) - timeC;
  }
  return timeL;
}

std::vector<std::string> HashTable::FindKeys(data data) {
  std::vector<std::string> result;
  for (auto it : hash_table_container) {
    for (auto itt : it) {
      if ((itt.second.name == data.name || data.name == "-") &&
          (itt.second.surname == data.surname || data.surname == "-") &&
          (itt.second.year_of_birth == data.year_of_birth ||
           data.year_of_birth == -1) &&
          (itt.second.city == data.city || data.city == "-") &&
          (itt.second.coins == data.coins || data.coins == -1)) {
        result.push_back(itt.first);
      }
    }
  }
  return result;
}

void HashTable::ShowAll() {
  int num_of_record = 0;
  PrintHeader();
  for (auto cell : hash_table_container) {
    for (auto record : cell) {
      PrintItem(++num_of_record, record.second);
    }
  }
  std::cout << "\u001b[1;35;1;1m";
}

void HashTable::PrintHeader() {
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

void HashTable::PrintItem(unsigned long index, data data) {
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

unsigned long HashTable::LoadData(const std::string &filePath) {
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
    data tmp = {result[1], result[2], std::stoi(result[3]), result[4], std::stol(result[5])};
    SetData(key, tmp);
    ++countStrings;
  }
  ifs.close();
  return countStrings;
}

unsigned long HashTable::SaveData(const std::string &filePath) {
  std::ofstream ofs(filePath);

  if (!ofs.is_open()) {
    throw std::runtime_error("Can`t write file");
  }
  unsigned long countStrings = 0;
  auto vec = CopyDataToSimpleVector();

  for (auto it : vec) {
    ofs << it.first << " " << it.second.surname << " " << it.second.name << " "
        << it.second.year_of_birth << " " << it.second.city << " "
        << it.second.coins << std::endl;
    ++countStrings;
  }
  ofs.close();
  return countStrings;
}

unsigned long HashTable::HashFunction(std::string key) {
  unsigned long hash = std::hash<std::string>{}(key) % hash_size_;
  return hash;
}

std::vector<std::pair<std::string, data>>
HashTable::CopyDataToSimpleVector() {
  std::vector<std::pair<std::string, data>> new_vec;
  for (auto lvl1 : hash_table_container) {
    std::copy(lvl1.begin(), lvl1.end(), std::back_inserter(new_vec));
  }
  return new_vec;
}

std::vector<std::string> HashTable::SplitString(std::string line) {
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

void HashTable::DeleteOldNode() {
  for (auto lvl1 : hash_table_container) {
    unsigned long i = 0;
    for (; i < lvl1.size(); ++i) {
      if ((lvl1[i].second.time_life > 0) &&
          ((long)time(NULL) - lvl1[i].second.create_time) >=
              (unsigned long)lvl1[i].second.time_life) {
        try {
          DelData(lvl1[i].first);
        } catch (std::exception &e) {
          std::cout << e.what() << std::endl;
        }
      }
    }
  }
}

void HashTable::Clear() {
  hash_table_container.clear();
  hash_table_container.resize(kDefaultHashSize);
}
}  // namespace s21
