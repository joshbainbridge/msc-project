#ifndef _COMPUTEPOINTS_H_
#define _COMPUTEPOINTS_H_

#include <stddef.h>

#include <tbb/tbb.h>

MSC_NAMESPACE_BEGIN

struct Item
{
  int value_one;
  int value_two;
  int sort_id;
};

inline bool operator==(const Item &lhs, const Item &rhs){return (lhs.sort_id == rhs.sort_id);}
inline bool operator!=(const Item &lhs, const Item &rhs){return (lhs.sort_id != rhs.sort_id);}
inline bool operator<(const Item &lhs, const Item &rhs){return (lhs.sort_id < rhs.sort_id);}

template < typename iterator_type, typename pointer_type > class ConstantRange {
public:
  ConstantRange(iterator_type _begin, iterator_type _end, pointer_type _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_array(_array)
  {;}

  ConstantRange(ConstantRange<iterator_type, pointer_type> &r, tbb::split)
  {
    m_array = r.m_array;
    m_end = r.m_end;

    iterator_type s = r.m_begin;
    for(iterator_type i = r.m_begin; i < (r.m_end - 1); ++i)
    {
      if(m_array[i] != m_array[i + 1])
      {
        s = i + 1;
        break;
      }
    }

    m_begin = s;
    r.m_end = s;
  }

  bool empty() const {return m_begin == m_end;}
  bool is_divisible() const {return m_array[m_begin] != m_array[m_end - 1];}

  iterator_type begin() const {return m_begin;}
  iterator_type end() const {return m_end;}

  static const bool is_splittable_in_proportion = false;

public:
  iterator_type m_begin;
  iterator_type m_end;
  pointer_type m_array;
};

class ItemFunctor
{
public:
  ItemFunctor(Item* _array) : m_array(_array) {;}

  inline void operator()(const ConstantRange<size_t, Item*> &r) const
  {
    for(size_t i = r.begin(); i < r.end(); ++i)
    {
      // Compute shading here
    }
  }

private:
  Item* m_array;
};

void computePoints(Item* array, size_t n)
{
  tbb::parallel_sort(&array[0], &array[n]);
  tbb::parallel_for(ConstantRange<size_t, Item*>(0, n, array), ItemFunctor(array), tbb::simple_partitioner());
}

MSC_NAMESPACE_END

#endif