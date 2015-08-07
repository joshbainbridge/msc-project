#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <string>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

template < typename type > class Singleton
{
public:
  static Singleton& instance()
  {
    static Singleton instance;
    return instance;
  }

  inline type getData() const {return m_data;}
  inline void setData(type _data) {m_data = _data;}

private:
  type m_data;

private:
  Singleton() {;}
};

typedef Singleton< std::string > SingletonString;

MSC_NAMESPACE_END

#endif
