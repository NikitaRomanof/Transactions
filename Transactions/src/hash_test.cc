#include <algorithm>
#include <chrono>
#include <random>
#include <set>
#include <thread>

#include "gtest/gtest.h"
#include "hash_table.h"

TEST(hashTest, setData_getData) {
  s21::HashTable a;
  std::string key1 = "student1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  a.SetData(key1, value1);
  ASSERT_EQ(a.GetData(key1).year_of_birth, 1995);
  a.Clear();
}

TEST(hashTest, renameKeys) {
  s21::HashTable a;
  std::string key1 = "student1";
  std::string key2 = "engineer1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  a.SetData(key1, value1);
  a.RenameKeys(key1, key2);
  ASSERT_EQ(a.GetData(key2).year_of_birth, 1995);
  EXPECT_ANY_THROW(a.GetData(key1).year_of_birth);
  a.Clear();
}

TEST(hashTest, exists) {
  s21::HashTable a;
  std::string key1 = "student1";
  std::string key2 = "engineer1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  a.SetData(key1, value1);
  ASSERT_TRUE(a.Exists(key1));
  ASSERT_FALSE(a.Exists(key2));
  a.Clear();
}

TEST(hashTest, delData) {
  s21::HashTable a;
  std::string key1 = "student1";
  std::string key2 = "engineer1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value2 = {"Nikolaev", "Ivan", 1985, "Naryan-mar", 202};
  a.SetData(key1, value1);
  a.SetData(key2, value2);
  a.DelData(key1);
  ASSERT_FALSE(a.Exists(key1));
  ASSERT_TRUE(a.Exists(key2));
  a.Clear();
}

TEST(hashTest, updateData) {
  s21::HashTable a;
  std::string key1 = "student1";
  std::string key2 = "engineer1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value1_1 = {"Ivanova", "Nataly", 1996, "Kyzyl-Orda", 1000000};
  s21::data value2 = {"Nikolaev", "Ivan", 1985, "Naryan-mar", 202};
  s21::data value2_1 = {"Nikolaeva", "Irina", 1987, "-", 2000000};
  a.SetData(key1, value1);
  a.SetData(key2, value2);
  a.UpdateData(key1, value1_1);
  a.UpdateData(key2, value2_1);
  ASSERT_EQ(a.GetData(key1).year_of_birth, 1996);
  ASSERT_EQ(a.GetData(key2).name, "Irina");
  a.Clear();
}

TEST(hashTest, returnKeys) {
  s21::HashTable a;
  std::vector<std::string> key = {"student1", "student2", "engineer1",
                                  "engineer2"};
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value2 = {"Ivanova", "Nataly", 1996, "Kyzyl-Orda", 1000000};
  s21::data value3 = {"Nikolaev", "Ivan", 1985, "Naryan-mar", 202};
  s21::data value4 = {"Nikolaeva", "Irina", 1987, "Sochi", 2000000};
  a.SetData(key[0], value1);
  a.SetData(key[1], value2);
  a.SetData(key[2], value3);
  a.SetData(key[3], value4);
  std::vector<std::string> result = a.ReturnKeys();
  for (int i = 0; i < int(result.size()); ++i) {
    ASSERT_NE(result.end(), std::find(result.begin(), result.end(), key[i]));
  }
  a.Clear();
}

TEST(hashTest, getTTL) {
  s21::HashTable a;
  std::string key1 = "student1";
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101, 2};
  a.SetData(key1, value1);
  std::this_thread::sleep_for(std::chrono::seconds{1});
  ASSERT_LE(a.GetTTL(key1), 1);
  std::this_thread::sleep_for(std::chrono::seconds{1});
  a.DeleteOldNode();
  EXPECT_THROW(a.GetTTL(key1), std::invalid_argument);
  a.Clear();
}

TEST(hashTest, findFull) {
  s21::HashTable a;
  std::vector<std::string> key = {"student1", "student2", "engineer1",
                                  "engineer2"};
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  a.SetData(key[0], value1);
  a.SetData(key[1], value1);
  a.SetData(key[2], value1);
  a.SetData(key[3], value1);
  s21::data search = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  std::vector<std::string> keys_found = a.FindKeys(search);
  ASSERT_EQ(keys_found.size(), 4);
  for (auto key_f : keys_found) {
    ASSERT_NE(key.end(), std::find(key.begin(), key.end(), key_f));
  }
  a.Clear();
}

TEST(hashTest, findNotFull) {
  s21::HashTable a;
  std::vector<std::string> key = {"student1", "student2", "engineer1",
                                  "engineer2"};
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value2 = {"Ivanova", "Nataly", 1996, "Kyzyl-Orda", 1000000};
  a.SetData(key[0], value1);
  a.SetData(key[1], value1);
  a.SetData(key[2], value2);
  a.SetData(key[3], value1);
  s21::data search = {"Ivanov", "-", -1, "-", 101};
  std::vector<std::string> keys_found = a.FindKeys(search);
  ASSERT_EQ(keys_found.size(), 3);
  for (auto key_f : keys_found) {
    ASSERT_NE(key.end(), std::find(key.begin(), key.end(), key_f));
  }
  a.Clear();
}

TEST(hashTest, showAll) {
  s21::HashTable a;
  std::vector<std::string> key = {"student1", "student2", "engineer1",
                                  "engineer2"};
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value2 = {"Ivanova", "Nataly", 1996, "Kyzyl-Orda", 1000000};
  s21::data value3 = {"Nikolaev", "Ivan", 1985, "Naryan-mar", 202};
  s21::data value4 = {"Nikolaeva", "Irina", 1987, "Sochi", 2000000};
  a.SetData(key[0], value1);
  a.SetData(key[1], value2);
  a.SetData(key[2], value3);
  a.SetData(key[3], value4);
  a.ShowAll();
  a.Clear();
}

TEST(hashTest, saveData) {
  s21::HashTable a;
  std::vector<std::string> key = {"student1", "student2", "engineer1",
                                  "engineer2"};
  s21::data value1 = {"Ivanov", "Nikolay", 1995, "Syktyvkar", 101};
  s21::data value2 = {"Ivanova", "Nataly", 1996, "Kyzyl-Orda", 1000000};
  s21::data value3 = {"Nikolaev", "Ivan", 1985, "Naryan-mar", 202};
  s21::data value4 = {"Nikolaeva", "Irina", 1987, "Sochi", 2000000};
  a.SetData(key[0], value1);
  a.SetData(key[1], value2);
  a.SetData(key[2], value3);
  a.SetData(key[3], value4);
  a.SaveData("hash_table.txt");
  a.Clear();
}

TEST(hashTest, loadData) {
  s21::HashTable a;
  a.LoadData("hash_table.txt");
  ASSERT_EQ(a.GetData("engineer1").city, "Naryan-mar");
  ASSERT_EQ(a.GetData("student2").year_of_birth, 1996);
  ASSERT_EQ(a.GetData("engineer2").coins, 2000000);
  ASSERT_EQ(a.GetData("student1").surname, "Ivanov");
  a.Clear();
}

TEST(hashTest, saveData100000) {
  s21::HashTable a;
  for (int i = 0; i < 100000; ++i) {
    std::string ii = std::to_string(i);
    std::string key = "key" + ii;
    s21::data value = {"Ivanov" + ii, "Nikolay", 1995, ii, i};
    a.SetData(key, value);
  }
  a.SaveData("treeload.txt");
  a.Clear();
}

TEST(hashTest, loadData2) {
  s21::HashTable a;
  a.LoadData("treeload.txt");
  ASSERT_EQ(a.GetData("key0").city, "0");
  ASSERT_EQ(a.GetData("key1").year_of_birth, 1995);
  ASSERT_EQ(a.GetData("key2").coins, 2);
  ASSERT_EQ(a.GetData("key4").name, "Nikolay");
  ASSERT_EQ(a.GetData("key5").surname, "Ivanov5");
  a.Clear();
}

TEST(hashTest, Deleterandom) {
  s21::HashTable a;
  a.LoadData("treeload.txt");
  std::random_device rd;
  std::default_random_engine eng(rd());

  for (int i = 0; i < 50; ++i) {
    std::uniform_int_distribution<int> distr(i, 999999);
    std::string key = "key" + std::to_string(distr(eng));
    try {
      a.DelData(key);
    } catch (const std::exception& e) {
      //
    }
  }
  a.Clear();
}
TEST(hashTest, Delete1000) {
  s21::HashTable a;
  a.LoadData("treeload.txt");
  std::random_device rd;
  std::default_random_engine eng(rd());

  for (int i = 0; i < 1000;) {
    std::string key = "key" + std::to_string(i);
    a.DelData(key);
    i = i + 7;
  }
  a.Clear();
}

TEST(hashTest, Delete50000_51000) {
  s21::HashTable a;
  a.LoadData("treeload.txt");
  std::random_device rd;
  std::default_random_engine eng(rd());

  for (int i = 50000; i < 51000;) {
    std::string key = "key" + std::to_string(i);
    a.DelData(key);
    i = i + 7;
  }
  a.Clear();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
