namespace s21 {

/*______________________Map Member functions______________________*/
template <class K, class T>
Map<K, T>::Map() : _head(nullptr), _size(0) {}

template <class K, class T>
Map<K, T>::Map(std::initializer_list<value_type> const &items)
    : _head(nullptr), _size(0) {
  if (items.size() >= max_size()) {
    throw std::invalid_argument(
        "ERROR on MAP constructor initializer_list, items.size() >= max_size");
  }
  for (value_type value : items) {
    insert(value);
  }
}

template <class K, class T>
Map<K, T>::Map(const Map &m) : _head(nullptr), _size(0) {
  *this = m;
}

template <class K, class T>
Map<K, T>::Map(Map &&m) : _head(nullptr), _size(0) {
  *this = std::move(m);
}

template <class K, class T>
Map<K, T>::~Map() {
  Clear();
}

template <class K, class T>
Map<K, T> &Map<K, T>::operator=(Map &&l) {
  if (this == &l) {
    throw std::invalid_argument(
        "ERROR on operator=(Map &&l). The current and added Map are one "
        "object");
  }
  Clear();
  swap(l);
  return *this;
}

template <class K, class T>
Map<K, T> &Map<K, T>::operator=(const Map &l) {
  if (this != &l) {
    Clear();
    if (l._head != nullptr) {
      _CopyNode(_head, l._head, nullptr);
      _size = l._size;
    }
  }
  return *this;
}

/*______________________Element access__________________________*/

template <class K, class T>
T &Map<K, T>::at(const K &key) {
  return _AtHelper(_head, key, true);
}

template <class K, class T>
T &Map<K, T>::operator[](const K &key) {
  return _AtHelper(_head, key, false);
}

template <class K, class T>
T &Map<K, T>::_AtHelper(NodeT *curNode, const K &key, bool flagAt) {
  NodeT *buf = curNode;
  while (buf != nullptr) {
    if (buf->_kV.first == key) {
      return buf->_kV.second;
    } else if (buf->_kV.first > key) {
      buf = buf->_pLeft;
    } else {
      buf = buf->_pRight;
    }
  }
  if (flagAt == true) {
    throw std::out_of_range("ERROR if function at(). Key not found.");
  } else {
    pair_type pos = insert(key, T());
    return pos.first._curIter->_kV.second;
  }
}
template <class K, class T>
std::pair<int, int> Map<K, T>::getBalance(const K &key) {
  NodeT *buf = _head;
  std::pair<int, int> rez = {0, 0};
  while (buf != nullptr) {
    if (buf->_kV.first == key) {
      rez = {buf->_leftDeph, buf->_rightDeph};
      break;
    } else if (buf->_kV.first > key) {
      buf = buf->_pLeft;
    } else {
      buf = buf->_pRight;
    }
  }
  return rez;
}

/*______________________Iterators_______________________________*/
template <class K, class T>
typename Map<K, T>::iterator Map<K, T>::begin() {
  iterator iter(*this);
  if (_head != nullptr) {
    while (iter._curIter->_pLeft != nullptr) {
      iter._curIter = iter._curIter->_pLeft;
    }
    iter._curParent = iter._curIter->_pParent;
  } else {
    iter._curIter = nullptr;
    iter._curParent = nullptr;
  }
  return iter;
}

template <class K, class T>
typename Map<K, T>::iterator Map<K, T>::end() {
  iterator iter(*this);
  if (_head != nullptr) {
    while (iter._curIter->_pRight != nullptr) {
      iter._curIter = iter._curIter->_pRight;
    }
    iter._curParent = iter._curIter;
    iter._curIter = iter._curIter->_pRight;
  }
  return iter;
}

template <class K, class T>
typename Map<K, T>::const_iterator Map<K, T>::cbegin() {
  return begin();
}

template <class K, class T>
typename Map<K, T>::const_iterator Map<K, T>::cend() {
  return end();
}

/*______________________Modifiers_______________________________*/
template <class K, class T>
void Map<K, T>::Clear() {
  if (_head != nullptr) {
    _Helper_Clear(_head);
    _head = nullptr;
    _size = 0;
  }
}

template <class K, class T>
void Map<K, T>::_Helper_Clear(NodeT *&curNode) {
  if (curNode->_pRight != nullptr) {
    _Helper_Clear(curNode->_pRight);
  }
  if (curNode->_pLeft != nullptr) {
    _Helper_Clear(curNode->_pLeft);
  }
  delete curNode;
  curNode = nullptr;
}

template <class K, class T>
typename Map<K, T>::pair_type Map<K, T>::insert(const_reference value) {
  if (_size == max_size()) {
    throw std::out_of_range("ERROR on function add(), map if full");
  }
  size_type bufSize = _size;
  Map<K, T>::MapIterator iterPos;
  if (_head == nullptr) {
    _head = new NodeT(value);
    ++_size;
    iterPos.changeIterPos(_head);
  } else {
    _Add(value, iterPos, _head);
  }
  return std::pair<iterator, bool>{iterPos, _size != bufSize};
}

template <class K, class T>
typename Map<K, T>::pair_type Map<K, T>::insert(const K &key, const T &obj) {
  return insert({key, obj});
}

template <class K, class T>
typename Map<K, T>::pair_type Map<K, T>::insert_or_assign(const K &key,
                                                          const T &obj) {
  pair_type buf = insert(key, obj);
  if (buf.second == false) {
    buf.first._curIter->_kV.second = obj;
  }
  return buf;
}

template <class K, class T>
void Map<K, T>::_Add(value_type a, iterator &pos, NodeT *curNode) {
  if (curNode->_kV.first > a.first) {
    if (curNode->_pLeft == nullptr) {
      NodeT *bufL = new NodeT(a, curNode);
      curNode->_pLeft = bufL;
      balancing(curNode->_pLeft);
      ++_size;
      pos.changeIterPos(bufL);
    } else {
      _Add(a, pos, curNode->_pLeft);
    }
  } else if (curNode->_kV.first < a.first) {
    if (curNode->_pRight == nullptr) {
      NodeT *bufR = new NodeT(a, curNode);
      curNode->_pRight = bufR;
      balancing(curNode->_pRight);
      ++_size;
      pos.changeIterPos(bufR);
    } else {
      _Add(a, pos, curNode->_pRight);
    }
  } else {
    pos.changeIterPos(curNode);
    curNode->_kV.second = a.second;
  }
}

template <class K, class T>
void Map<K, T>::_CopyNode(NodeT *&curNode, NodeT *otherCurNode,
                          NodeT *curParent) {
  curNode = new NodeT(otherCurNode->_kV, curParent, nullptr, nullptr);
  curNode->_balance = otherCurNode->_balance;
  curNode->_leftDeph = otherCurNode->_leftDeph;
  curNode->_rightDeph = otherCurNode->_rightDeph;
  if (otherCurNode->_pRight != nullptr) {
    _CopyNode(curNode->_pRight, otherCurNode->_pRight, curNode);
  }
  if (otherCurNode->_pLeft != nullptr) {
    _CopyNode(curNode->_pLeft, otherCurNode->_pLeft, curNode);
  }
}

template <class K, class T>
void Map<K, T>::swap(Map &other) {
  if (this == &other) {
    throw std::invalid_argument(
        "ERROR on function swap(), passing an object to itself");
  }
  std::swap(_head, other._head);
  std::swap(_size, other._size);
}

template <class K, class T>
void Map<K, T>::merge(Map &other) {
  if (this != &other && other._head != nullptr) {
    if (_head == nullptr) {
      swap(other);
    } else {
      Map buf;
      _MergeHelper(other._head, buf);

      other = std::move(buf);
    }
  }
}

template <class K, class T>
void Map<K, T>::_MergeHelper(NodeT *&otherCurNode, Map &buf) {
  if (otherCurNode->_pRight != nullptr) {
    _MergeHelper(otherCurNode->_pRight, buf);
  }
  if (otherCurNode->_pLeft != nullptr) {
    _MergeHelper(otherCurNode->_pLeft, buf);
  }
  pair_type del = insert(otherCurNode->_kV);
  if (del.second == true) {
  } else {
    buf.insert(otherCurNode->_kV);
  }
  delete otherCurNode;
  otherCurNode = nullptr;
}

template <class K, class T>
void Map<K, T>::erase(iterator pos) {
  if (pos._curIter == nullptr) {
    throw std::invalid_argument(
        "ERROR on function erase(), iterator is nullptr");
  }

  if (pos._curParent != nullptr) {
    if (pos._curIter->_pRight == nullptr && pos._curIter->_pLeft == nullptr) {
      _EraseNoParent(pos);
    } else if (pos._curIter->_pLeft != nullptr &&
               pos._curIter->_pRight == nullptr) {
      NodeT *buf = pos._curIter->_pLeft;
      while (buf->_pRight != nullptr) {
        buf = buf->_pRight;
      }
      _EraseRightOrLeftParent(pos, buf);
    } else if (pos._curIter->_pRight != nullptr &&
               pos._curIter->_pLeft == nullptr) {
      NodeT *buf = pos._curIter->_pRight;
      while (buf->_pLeft != nullptr) {
        buf = buf->_pLeft;
      }
      _EraseRightOrLeftParent(pos, buf);
    } else {
      _EraseTwoParent(pos);
    }
  } else {
    _EraseHead(pos);
  }
  --_size;
  if (_size == 0) {
    pos._curIter = nullptr;
    pos._curParent = nullptr;
    _head = nullptr;
  }
}

template <class K, class T>
void Map<K, T>::_EraseHead(iterator pos) {
  if (pos._curIter->_pRight != nullptr && pos._curIter->_pLeft != nullptr) {
    NodeT *buf = pos._curIter->_pRight;
    while (buf->_pLeft != nullptr) {
      buf = buf->_pLeft;
    }
    NodeT *bufParent = buf->_pParent;
    NodeT *bufRight = buf->_pRight;
    buf->_pParent = nullptr;
    buf->_pLeft = pos._curIter->_pLeft;
    pos._curIter->_pLeft->_pParent = buf;
    if (bufParent == pos._curIter) {
      buf->_leftDeph = pos._curIter->_leftDeph;
      buf->_rightDeph = bufRight == nullptr ? 0 : 1;
      buf->_balance = buf->_leftDeph + buf->_rightDeph;
      _head = buf;
      if (buf->_balance == -2 && buf->_pLeft->_balance <= 0) {
        smallRightTurn(buf);
      } else if (buf->_balance == -2 && buf->_pLeft->_balance > 0) {
        bigRightTurn(buf);
      }
    } else {
      buf->_pRight = pos._curIter->_pRight;
      pos._curIter->_pRight->_pParent = buf;
      bufParent->_pLeft = nullptr;
      bufParent->_leftDeph = 0;
      _head = buf;
      if (bufParent->_pRight == nullptr) {
        bufParent->_rightDeph = 0;
        bufParent->_balance = 0;
        balancing(bufParent);
      } else {
        bufParent->_rightDeph -= 1;
        balancing(bufParent->_pRight);
      }
    }
    if (bufRight != nullptr) {
      insert(bufRight->_kV);
    }
  } else if ((pos._curIter->_pRight == nullptr &&
              pos._curIter->_pLeft != nullptr) ||
             (pos._curIter->_pLeft == nullptr &&
              pos._curIter->_pRight != nullptr)) {
    if (pos._curIter->_pRight == nullptr) {
      pos._curIter->_pLeft->_pParent = nullptr;
      _head = pos._curIter->_pLeft;

    } else {
      pos._curIter->_pRight->_pParent = nullptr;
      _head = pos._curIter->_pRight;
    }
  }

  delete pos._curIter;
  pos._curIter = nullptr;
}

template <class K, class T>
void Map<K, T>::_EraseNoParent(iterator pos) {
  if (pos._curParent->_pLeft == pos._curIter) {
    pos._curParent->_pLeft = nullptr;
  } else {
    pos._curParent->_pRight = nullptr;
  }
  delete pos._curIter;
  pos._curIter = nullptr;
  balancing(pos._curParent);
}

template <class K, class T>
void Map<K, T>::_EraseTwoParent(iterator pos) {
  NodeT *buf = pos._curIter->_pRight;
  while (buf->_pLeft != nullptr) {
    buf = buf->_pLeft;
  }
  //  изменяем связь родителя, указывая на буфер вместо удаляемого
  if (pos._curParent->_pLeft == pos._curIter) {
    pos._curParent->_pLeft = buf;
  } else {
    pos._curParent->_pRight = buf;
  }
  NodeT *bufParent = buf->_pParent;
  NodeT *bufRight = buf->_pRight;
  buf->_pParent = pos._curParent;
  buf->_pLeft = pos._curIter->_pLeft;
  pos._curIter->_pLeft->_pParent = buf;
  //  if для малых деревьев, кода родитель буфера равен удаляемому
  if (bufParent == pos._curIter) {
    buf->_leftDeph = pos._curIter->_leftDeph + 1;
    buf->_rightDeph = bufRight == nullptr ? 0 : 1;
    balancing(buf->_pLeft);
    //  else для больших деревьев, кода родитель буфера не равен удаляемому
  } else {
    buf->_pRight = pos._curIter->_pRight;
    pos._curIter->_pRight->_pParent = buf;
    bufParent->_pLeft = nullptr;
    bufParent->_leftDeph = 0;
    buf->_leftDeph = pos._curIter->_leftDeph;
    if (bufParent->_pRight == nullptr) {
      bufParent->_rightDeph = 0;
      bufParent->_balance = 0;
      balancing(bufParent);
    } else {
      bufParent->_rightDeph -= 1;
      balancing(bufParent->_pRight);
    }
  }
  delete pos._curIter;
  pos._curIter = nullptr;
  // если у буфера был сын справа его нужно удалить и по новой запушить
  if (bufRight != nullptr) {
    insert(bufRight->_kV);
  }
}

template <class K, class T>
void Map<K, T>::_EraseRightOrLeftParent(iterator pos, NodeT *buf) {
  if (pos._curParent->_pLeft == pos._curIter) {
    pos._curParent->_pLeft = buf;
  } else {
    pos._curParent->_pRight = buf;
  }
  buf->_pParent = pos._curParent;
  delete pos._curIter;
  pos._curIter = nullptr;
  buf->_balance = 0;
  buf->_pRight = 0;
  buf->_pLeft = 0;
  balancing(buf);
}

/*______________________Lookup__________________________________*/
template <class K, class T>
bool Map<K, T>::contains(const K &key) {
  NodeT *buf = _head;
  while (buf != nullptr) {
    if (buf->_kV.first == key) {
      return true;
    } else if (buf->_kV.first > key) {
      buf = buf->_pLeft;
    } else {
      buf = buf->_pRight;
    }
  }
  return false;
}

/*______________________Capacity_______________________________*/
template <class K, class T>
bool Map<K, T>::empty() {
  return _size == 0;
}
template <class K, class T>
size_t Map<K, T>::size() {
  return _size;
}
template <class K, class T>
size_t Map<K, T>::max_size() {
  return std::numeric_limits<size_type>::max() /
         (sizeof(NodeT) + sizeof(size_type) + sizeof(value_type) + sizeof(Map));
}

/*______________________Function tree______________________*/
template <class K, class T>
void Map<K, T>::smallLeftTurn(NodeT *curNode) {
  if (curNode == nullptr) {
    throw std::invalid_argument(
        "ERROR on function smallLeftTurn(), curNode = nullptr");
  }
  if (curNode->_pParent != nullptr) {
    if (curNode->_pParent->_pRight == curNode) {
      curNode->_pParent->_pRight = curNode->_pRight;
    } else {
      curNode->_pParent->_pLeft = curNode->_pRight;
    }
  }
  int flag = curNode->_pParent == nullptr ? 1 : 0;
  NodeT *bufR = curNode->_pRight;
  curNode->_pRight->_pParent = curNode->_pParent;
  curNode->_pParent = curNode->_pRight;
  NodeT *buffer = curNode->_pRight->_pLeft;
  curNode->_pRight->_pLeft = curNode;
  curNode->_pRight = buffer;
  if (buffer != nullptr) {
    buffer->_pParent = curNode;
    buffer = nullptr;
  }
  if (flag == 1) {
    _head = bufR;
  }
  changeBalance(curNode);
  changeBalance(bufR);
}

template <class K, class T>
void Map<K, T>::smallRightTurn(NodeT *curNode) {
  if (curNode == nullptr) {
    throw std::invalid_argument(
        "ERROR on function smallLeftTurn(), curNode = nullptr");
  }
  if (curNode->_pParent != nullptr) {
    if (curNode->_pParent->_pLeft == curNode) {
      curNode->_pParent->_pLeft = curNode->_pLeft;
    } else {
      curNode->_pParent->_pRight = curNode->_pLeft;
    }
  }
  int flag = curNode->_pParent == nullptr ? 1 : 0;
  NodeT *bufL = curNode->_pLeft;
  curNode->_pLeft->_pParent = curNode->_pParent;
  curNode->_pParent = curNode->_pLeft;
  NodeT *buffer = curNode->_pLeft->_pRight;

  curNode->_pLeft->_pRight = curNode;
  curNode->_pLeft = buffer;
  if (buffer != nullptr) {
    buffer->_pParent = curNode;
    buffer = nullptr;
  }
  if (flag == 1) {
    _head = bufL;
  }
  changeBalance(curNode);
  changeBalance(bufL);
}

template <class K, class T>
void Map<K, T>::bigRightTurn(NodeT *curNode) {
  smallLeftTurn(curNode->_pLeft);
  smallRightTurn(curNode);
}

template <class K, class T>
void Map<K, T>::bigLeftTurn(NodeT *curNode) {
  smallRightTurn(curNode->_pRight);
  smallLeftTurn(curNode);
}

template <class K, class T>
void Map<K, T>::balancing(NodeT *curNode) {
  while (curNode != nullptr) {
    if (curNode->_pLeft != nullptr) {
      curNode->_leftDeph =
          (curNode->_pLeft->_leftDeph * -1) >= curNode->_pLeft->_rightDeph
              ? curNode->_pLeft->_leftDeph - 1
              : (-1 * curNode->_pLeft->_rightDeph) - 1;
    } else {
      curNode->_leftDeph = 0;
    }
    if (curNode->_pRight != nullptr) {
      curNode->_rightDeph =
          (curNode->_pRight->_leftDeph * -1) >= curNode->_pRight->_rightDeph
              ? (curNode->_pRight->_leftDeph * -1) + 1
              : curNode->_pRight->_rightDeph + 1;
    } else {
      curNode->_rightDeph = 0;
    }
    curNode->_balance = curNode->_leftDeph + curNode->_rightDeph;
    if (curNode->_balance == -2 && curNode->_pLeft->_balance <= 0) {
      smallRightTurn(curNode);
    } else if (curNode->_balance == -2 && curNode->_pLeft->_balance > 0) {
      bigRightTurn(curNode);
    } else if (curNode->_balance == 2 && curNode->_pRight->_balance >= 0) {
      smallLeftTurn(curNode);
    } else if (curNode->_balance == 2 && curNode->_pRight->_balance < 0) {
      bigLeftTurn(curNode);
    } else {
      curNode = curNode->_pParent;
    }
  }
}

template <class K, class T>
int Map<K, T>::depthMax(NodeT *curNode) {
  if (curNode == nullptr) {
    return (-1);
  } else {
    int leftDepth = depthMax(curNode->_pLeft);
    int rightDepth = depthMax(curNode->_pRight);
    if (leftDepth > rightDepth)
      return (leftDepth + 1);
    else
      return (rightDepth + 1);
  }
}

template <class K, class T>
void Map<K, T>::changeBalance(NodeT *curNode) {
  if (curNode != nullptr) {
    if (curNode->_pLeft == nullptr) {
      curNode->_leftDeph = 0;
    } else {
      curNode->_leftDeph = (-1 * depthMax(curNode->_pLeft)) - 1;
    }
    if (curNode->_pRight == nullptr) {
      curNode->_rightDeph = 0;
    } else {
      curNode->_rightDeph = depthMax(curNode->_pRight) + 1;
    }
    curNode->_balance = curNode->_leftDeph + curNode->_rightDeph;
  }
}

template <class K, class T>
void Map<K, T>::printTree(NodeT *curNode) {
  if (curNode == nullptr) {
    curNode = _head;
  }
  if (curNode != nullptr) {
    std::cout << curNode->_kV.first
              // << "-" << curNode->_kV.second
              << "-" << curNode->_balance << "_balance"
              << "-" << curNode->_leftDeph << "_leftDeph"
              << "-" << curNode->_rightDeph << "_rightDeph ";
    if (curNode->_pParent == nullptr) {
      std::cout << " HEAD ";
    }
    std::cout << "" << std::endl;
    if (curNode->_pRight != nullptr) {
      printTree(curNode->_pRight);
    }
    if (curNode->_pLeft != nullptr) {
      printTree(curNode->_pLeft);
    }
  }
}

/*______________________Bonus____________________________________*/
template <class K, class T>
template <class... Args>
typename Map<K, T>::pair_type Map<K, T>::emplace(Args &&...args) {
  pair_type res;
  const size_type sizeArgs = sizeof...(Args);
  value_type masArgs[sizeArgs] = {args...};
  for (size_t i = 0; i < sizeArgs; i++) {
    res = insert(masArgs[i]);
  }
  return res;
}

/*______________________Map Iterator____________________________*/
template <class K, class T>
typename Map<K, T>::MapIterator &Map<K, T>::MapIterator::operator++() {
  if (_curParent == nullptr && _curIter == nullptr) {
    throw std::invalid_argument(
        "Operation ++ is not possible, iterator is nullptr");
  }
  if (_curParent != nullptr) {
    if (_curIter == nullptr) {
      _curIter = _curParent;
      _curParent = _curIter->_pParent;
    } else if (_curParent->_pLeft == _curIter && _curIter->_pRight == nullptr) {
      _curIter = _curParent;
      _curParent = _curParent->_pParent;
    } else if (_curIter->_pRight != nullptr) {
      _curIter = _curIter->_pRight;
      while (_curIter->_pLeft != nullptr) {
        _curIter = _curIter->_pLeft;
      }
      _curParent = _curIter->_pParent;
    } else {
      while (_curIter->_pParent != nullptr &&
             _curIter != _curIter->_pParent->_pLeft) {
        _curIter = _curIter->_pParent;
      }
      if (_curIter->_pParent == nullptr) {
        _curIter = nullptr;
        _curParent = _curParent->_pRight;
      } else {
        _curIter = _curIter->_pParent;
        _curParent = _curIter->_pParent;
      }
    }
  } else {
    _curParent = _curIter;
    _curIter = _curIter->_pRight;
    while (_curIter != nullptr && _curIter->_pLeft != nullptr) {
      _curIter = _curIter->_pLeft;
      _curParent = _curIter->_pParent;
    }
  }
  return *this;
}

template <class K, class T>
typename s21::Map<K, T>::MapIterator &
s21::Map<K, T>::MapIterator::operator--() {
  if (_curParent == nullptr && _curIter == nullptr) {
    throw std::invalid_argument(
        "Operation -- is not possible, iterator is nullptr");
  }
  if (_curParent != nullptr) {
    if (_curIter == nullptr) {
      _curIter = _curParent;
      _curParent = _curIter->_pParent;
    } else if (_curIter->_pLeft != nullptr) {
      _curIter = _curIter->_pLeft;
      while (_curIter->_pRight != nullptr) {
        _curIter = _curIter->_pRight;
      }
      _curParent = _curIter->_pParent;
    } else if (_curIter->_pLeft == nullptr &&
               _curIter->_pParent->_pLeft == _curIter) {
      _curIter = _curIter->_pParent;
      while (_curIter->_pParent != nullptr &&
             _curIter->_pParent->_pRight != _curIter) {
        _curIter = _curIter->_pParent;
      }
      if (_curIter->_pParent == nullptr) {
        _curParent = _curParent->_pLeft;
        _curIter = nullptr;
      } else {
        _curIter = _curIter->_pParent;
        _curParent = _curIter->_pParent;
      }
    } else if (_curIter->_pLeft == nullptr &&
               _curIter->_pParent->_pRight == _curIter) {
      _curIter = _curIter->_pParent;
      _curParent = _curIter->_pParent;
    }
  } else {
    if (_curIter->_pLeft == nullptr) {
      _curParent = _curIter;
      _curIter = nullptr;
    } else {
      _curIter = _curIter->_pLeft;
      while (_curIter->_pRight != nullptr) {
        _curIter = _curIter->_pRight;
      }
      _curParent = _curIter->_pParent;
    }
  }
  return *this;
}

}  // namespace s21
