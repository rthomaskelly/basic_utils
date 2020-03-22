#pragma once

#include <list>
#include <memory>
#include <type_traits>

#include "runtime.hpp"

namespace ryk {

//
// dlist implements a dynamicaly typed list
// by adapting std::list with a void* 
// 

//
// dlist_node wraps a dynamically typed element
// which will be used as a node in our dynamic list
class dlist_node
{
public:
  dlist_node() 
   : _data(nullptr) {}

  template<class T>
  void set(const T& t)
  {
    _data = std::make_shared<T>(t);
  }

  template<class T>
  dlist_node(const T& t)
   : dlist_node()
  {
    set(t);
  }

  dlist_node(const dlist_node& n)
   : _data(n._data)
  {
  } 
  dlist_node(dlist_node&& n)
   : _data(std::move(n._data))
  {
  }

  dlist_node& operator=(dlist_node n)
  {
    swap(n);
    return *this;
  }

  template<class T = void>
  const T& data() const
  {
    return *std::reinterpret_pointer_cast<T>(_data);
  }

  template<class T = void>
  T& data()
  {
    return const_cast<T&>(const_cast<const dlist_node*>(this)->data<T>());
  }
 
private:
  std::shared_ptr<void> _data;
  
  void swap(dlist_node& n)
  {
    std::swap(_data, n._data);
  }
};

class dlist : public RTTI
{
 public:
  using list_type = std::list<dlist_node>;
  using value_type = list_type::value_type;
  using size_type = list_type::size_type;
  using difference_type = list_type::difference_type; 
  using iterator = list_type::iterator;
  using const_iterator = list_type::const_iterator;

  dlist() : RTTI(ExpType::List) {}
  dlist(const dlist& dl) : RTTI(ExpType::List), _list(dl._list) {} 
  dlist(dlist&& dl) : RTTI(ExpType::List), _list(std::move(dl._list)) {} 
  dlist& operator=(dlist dl) { _type = ExpType::List; swap(dl); return *this; }

  template<class T>
  T& front()
  {
    return _list.front().data<T>();
  }
  template<class T>
  T& front() const
  {
    return _list.front().data<T>();
  }
  template<class T>
  T& back()
  {
    return _list.back().data<T>();
  }
  template<class T>
  T& back() const
  {
    return _list.back().data<T>();
  }
  void pop_front()
  {
    _list.pop_front(); 
  }
  void pop_back()
  {
    _list.pop_back();
  }
  template<class T>
  void push_front(const T& t)
  {
    _list.emplace_front(t);
  }
  template<class T>
  void push_back(const T& t)
  {
    _list.emplace_back(t);
  }
  iterator begin() noexcept
  {
    return _list.begin();
  }  
  iterator end() noexcept
  {
    return _list.end();
  }  
  const_iterator begin() const noexcept
  {
    return _list.begin();
  } 
  const_iterator end() const noexcept
  {
    return _list.end();
  } 
  ExpType getRTTI() const noexcept
  {
    // RTTI: Run time type info
    return ExpType::List;
  }
  
  friend std::ostream& operator<<(std::ostream&, const dlist&);
 protected:
  list_type _list;
  
  void swap(dlist& rhs)
  {
    std::swap(_list, rhs._list);
  }
};

} // namespace ryk
