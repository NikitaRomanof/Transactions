#pragma once

#include <cmath>
#include <iostream>
#include <limits>

// #include "s21_matrix_oop.h"

namespace s21 {
template <class K, class T>
class Map {
 public:
  using key_type = K;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = size_t;

  /*______________________Inner class NodeT___________________________*/
 private:
  class NodeT {
   public:
    value_type _kV;
    NodeT *_pParent;
    NodeT *_pRight;
    NodeT *_pLeft;
    int _balance;
    int _rightDeph;
    int _leftDeph;

    explicit NodeT(const_reference kV, NodeT *pParent = nullptr,
                   NodeT *pRight = nullptr, NodeT *pLeft = nullptr)
        : _kV(kV),
          _pParent(pParent),
          _pRight(pRight),
          _pLeft(pLeft),
          _balance(0),
          _rightDeph(0),
          _leftDeph(0) {}
  };

  /*________________Private fields of the map class _________________*/

  NodeT *_head;
  size_type _size;

  /*______________________Inner class Iterator_______________________*/
 public:
  class MapIterator {
   public:
    NodeT *_curParent;
    NodeT *_curIter;

    MapIterator() : _curParent(nullptr), _curIter(nullptr) {}

    MapIterator(const MapIterator &other) {
      *this = other;
    }

     MapIterator &operator=(const MapIterator &other) {
      _curParent = other._curParent;
      _curIter = other._curIter;
      return *this;
     }

    explicit MapIterator(const Map &map) {
      _curParent = nullptr;
      _curIter = map._head;
    }

    MapIterator &operator++();
    MapIterator &operator--();
    value_type &operator*() {
      if (_curIter == nullptr) {
        throw std::out_of_range("ERROR, in operator*, iterator is nullptr");
      }
      return _curIter->_kV;
    }
    bool operator!=(const MapIterator &other) {
      return _curIter != other._curIter;
    }

    bool operator==(const MapIterator &other) {
      return _curIter == other._curIter;
    }

    void changeIterPos(NodeT *other) {
      _curIter = other;
      if (other != nullptr) {
        _curParent = other->_pParent;
      } else {
        _curParent = nullptr;
      }
    }
  };

  /*____________________Inner class CONST_Iterator___________________*/
  class MapConstIterator : public MapIterator {
   public:
    explicit MapConstIterator(NodeT *curIter = nullptr,
                              NodeT *curParent = nullptr)
        : MapIterator(curIter, curParent) {}

    MapConstIterator() : MapIterator() {}

    MapConstIterator(const MapConstIterator &other) {
      this->_curParent = other._curParent;
      this->_curIter = other._curIter;
    }

    explicit MapConstIterator(const Map &map) : MapIterator(map) {}

    const_reference operator*() const {
      if (this->_curIter == nullptr) {
        throw std::out_of_range("ERROR, in operator*, iterator is nullptr");
      }
      return this->_curIter->_kV;
    }
  };
  using iterator = MapIterator;
  using const_iterator = MapConstIterator;
  using pair_type = std::pair<iterator, bool>;

  /*______________________Map Member functions______________________*/
 public:
  Map();
  Map(std::initializer_list<value_type> const &items);
  Map(const Map &m);
  Map(Map &&m);
  ~Map();
  Map &operator=(Map &&l);
  Map &operator=(const Map &l);

  void printTree(NodeT *curNode = nullptr);

  /*______________________Element access________________________*/
  T &at(const K &key);
  T &operator[](const K &key);
  std::pair<int, int> getBalance(const K &key);

 private:
  T &_AtHelper(NodeT *curNode, const K &key, bool flagAt);
  //

  /*______________________Iterators_______________________________*/
 public:
  iterator begin();
  iterator end();
  const_iterator cbegin();
  const_iterator cend();

  /*______________________Capacity_______________________________*/
  bool empty();
  size_type size();
  size_type max_size();

  /*______________________Modifiers_______________________________*/

  void Clear();
  pair_type insert(const_reference value);
  pair_type insert(const K &key, const T &obj);
  pair_type insert_or_assign(const K &key, const T &obj);
  void erase(iterator pos);
  void swap(Map &other);
  void merge(Map &other);

 private:
  void _Add(value_type a, iterator &pos, NodeT *curNode = nullptr);
  void _Helper_Clear(NodeT *&curNode);
  void _CopyNode(NodeT *&curNode, NodeT *otherCurNode, NodeT *curParent);
  void _MergeHelper(NodeT *&otherCurNode, Map &buf);
  void _EraseNoParent(iterator pos);
  void _EraseTwoParent(iterator pos);
  void _EraseRightOrLeftParent(iterator pos, NodeT *buf);
  void _EraseHead(iterator pos);

  /*______________________Lookup__________________________________*/

 public:
  bool contains(const K &key);

  /*______________________Bonus____________________________________*/
  template <class... Args>
  pair_type emplace(Args &&...args);
  /*______________________Function tree___________________________*/
 private:
  void smallLeftTurn(NodeT *curNode);
  void smallRightTurn(NodeT *curNode);
  void bigLeftTurn(NodeT *curNode);
  void bigRightTurn(NodeT *curNode);
  void balancing(NodeT *curNode);
  int depthMax(NodeT *curNode);
  void changeBalance(NodeT *curNode);
  unsigned long rCount = 0;
};
}  // namespace s21

#include "s21_map.inl"
