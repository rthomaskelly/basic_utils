#pragma once

#include <list>
#include <memory>
#include <type_traits>
#include <iostream>

#include "RTTI.hpp"

namespace ryk {

//
// runtime_list implements a runtime list capable of nesting
// in order to get it to nest we have to inherit from some type
// then use composition with std::list 
// 

class runtime_list : public RTTI
{
 public:
  using list_type = std::list<std::shared_ptr<RTTI>>;
  using value_type = list_type::value_type;
  using size_type = list_type::size_type;
  using difference_type = list_type::difference_type; 
  using iterator = list_type::iterator;
  using const_iterator = list_type::const_iterator;

  runtime_list();
  runtime_list(const runtime_list& rhs);
  runtime_list(runtime_list&& rhs);
  runtime_list& operator=(runtime_list rhs);

  const RTTI& front() const noexcept;
  RTTI& front() noexcept;
  
  const RTTI& back() const noexcept;
  RTTI& back() noexcept;

  //
  // these front() & back() variants perform a dynamic cast to T for you
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, const T&>
  front() const;
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, T&>
  front();
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, const T&>
  back() const;
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, T&>
  back();
  
  void pop_front();
  void pop_back();

  //
  // push_front() & push_back() take types inheriting from RTTI
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, void>
  push_front(const T& t);
  template<class T>
  std::enable_if_t<std::is_base_of_v<RTTI, T>, void>
  push_back(const T& t);

  std::size_t size() const noexcept;
  bool empty() const noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
 
  bool operator==(const runtime_list& rhs) const noexcept;
  bool operator!=(const runtime_list& rhs) const noexcept;
    
  friend std::ostream& operator<<(std::ostream&, const runtime_list&);
 protected:
  list_type _list;
  
  void swap(runtime_list& rhs);
  bool isEqual(const RTTI& rhs) const noexcept override;
};

inline runtime_list::runtime_list()
 : RTTI(ExpType::List)
{
}
inline runtime_list::runtime_list(const runtime_list& rhs)
 : RTTI(ExpType::List), _list(rhs._list)
{
} 
inline runtime_list::runtime_list(runtime_list&& rhs)
 : RTTI(ExpType::List), _list(std::move(rhs._list))
{
} 
inline runtime_list& runtime_list::operator=(runtime_list rhs)
{
  _type = ExpType::List;
  swap(rhs);
  return *this;
}

inline const RTTI& runtime_list::front() const noexcept
{
  return *_list.front();  
}
inline RTTI& runtime_list::front() noexcept
{
  return *_list.front();
}
inline const RTTI& runtime_list::back() const noexcept
{
  return *_list.back();
}
inline RTTI& runtime_list::back() noexcept
{
  return *_list.back();
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, const T&>
runtime_list::front() const
{
  return dynamic_cast<const T&>(front());
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, T&>
runtime_list::front()
{
  return dynamic_cast<T&>(front());
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, const T&>
runtime_list::back() const
{
  return dynamic_cast<const T&>(back());
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, T&>
runtime_list::back()
{
  return dynamic_cast<T&>(back());
}

inline void runtime_list::pop_front()
{
  _list.pop_front(); 
}
inline void runtime_list::pop_back()
{
  _list.pop_back();
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, void>
runtime_list::push_front(const T& t)
{
  _list.push_front(std::make_shared<T>(t));
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, void>
runtime_list::push_back(const T& t)
{
  _list.push_back(std::make_shared<T>(t));
}

inline std::size_t runtime_list::size() const noexcept
{
  return _list.size();
}
inline bool runtime_list::empty() const noexcept
{
  return _list.empty();
}

inline runtime_list::iterator runtime_list::begin() noexcept
{
  return _list.begin();
}  
inline runtime_list::iterator runtime_list::end() noexcept
{
  return _list.end();
}  
inline runtime_list::const_iterator runtime_list::begin() const noexcept
{
  return _list.begin();
} 
inline runtime_list::const_iterator runtime_list::end() const noexcept
{
  return _list.end();
} 

inline bool runtime_list::operator==(const runtime_list& rhs) const noexcept
{
  if (size() != rhs.size()) return false;
  auto rhs_iter = rhs.begin();
  for (auto iter = begin(); iter != end(); ++iter, ++rhs_iter)
  {
    if (*iter != *rhs_iter) return false;
  }
  return true;
}
inline bool runtime_list::operator!=(const runtime_list& rhs) const noexcept
{
  return !(*this == rhs);
}
inline bool runtime_list::isEqual(const RTTI& rhs) const noexcept
{
  return *this == dynamic_cast<const runtime_list&>(rhs);
}

inline void runtime_list::swap(runtime_list& rhs)
{
  std::swap(_list, rhs._list);
}

} // namespace ryk
