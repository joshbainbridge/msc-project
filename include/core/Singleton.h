#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <string>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      A templated class that is used to store one variable as singleton
 *
 * @tparam     type  data type to store
 * 
 * This is primarly used for storing the path to the current scene file to allow for relative
 * directories to be created when constructing the scene.
 */
template < typename type > class Singleton
{
public:
  /**
   * @brief      Retrieve reference to static data
   *
   * @return     singleton reference
   */
  static Singleton& instance()
  {
    static Singleton instance;
    return instance;
  }

  /**
   * @brief      Get singleton data
   *
   * @return     data
   */
  inline type getData() const {return m_data;}

  /**
   * @brief      Set singleton data
   *
   * @param[in]  _data  data
   */
  inline void setData(type _data) {m_data = _data;}

private:
  type m_data;

private:
  Singleton() {;}
};

typedef Singleton< std::string > SingletonString;

MSC_NAMESPACE_END

#endif
