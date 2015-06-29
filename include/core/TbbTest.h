#ifndef _TBBTEST_H_
#define _TBBTEST_H_

#include <Common.h>

#include <tbb/tbb.h>

MSC_NAMESPACE_BEGIN

class ShadingTask : public tbb::task
{
public:
  ShadingTask(size_t _begin, size_t _end, msc::Item* _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_array(_array)
  {;}

  tbb::task* execute()
  {
    std::cout << "Shading" << std::endl;

    return NULL;
  }

public:
  size_t m_begin;
  size_t m_end;
  msc::Item* m_array;
};

template < typename iterator_type, typename pointer_type > class NewTask : public tbb::task
{
public:
  NewTask(iterator_type _begin, iterator_type _end, pointer_type _array)
   : m_begin(_begin)
   , m_end(_end)
   , m_array(_array)
  {;}

  tbb::task* execute()
  {
    if(m_array[m_begin] != m_array[m_end - 1])
    {
      iterator_type s = m_begin;
      for(iterator_type i = m_begin; i < (m_end - 1); ++i)
      {
        if(m_array[i] != m_array[i + 1])
        {
          s = i + 1;
          break;
        }
      }

      NewTask &lrange = *new( tbb::task::allocate_child() ) NewTask(m_begin, s, m_array);
      NewTask &rrange = *new( tbb::task::allocate_child() ) NewTask(s, m_end, m_array);

      tbb::task::set_ref_count(3);
      
      tbb::task::spawn(lrange);
      tbb::task::spawn(rrange);
      tbb::task::wait_for_all();
    }
    else if((m_end - m_begin) > 4096)
    {
      iterator_type s = (m_end - m_begin) / 2;

      NewTask &lrange = *new( tbb::task::allocate_child() ) NewTask(m_begin, s, m_array);
      NewTask &rrange = *new( tbb::task::allocate_child() ) NewTask(s, m_end, m_array);

      tbb::task::set_ref_count(3);
      
      tbb::task::spawn(lrange);
      tbb::task::spawn(rrange);
      tbb::task::wait_for_all();
    }
    else
    {
      std::cout << m_begin << ", " << m_end << std::endl;
      ShadingTask &shader = *new( tbb::task::allocate_child() ) ShadingTask(m_begin, m_end, m_array);
      
      tbb::task::set_ref_count(2);

      tbb::task::spawn_and_wait_for_all(shader);
    }

    return NULL;
  }

public:
  iterator_type m_begin;
  iterator_type m_end;
  pointer_type m_array;
};

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
      std::cout << m_array[i].sort_id << std::endl;
    }
  }

private:
  Item* m_array;
};

void computePoints(Item* array, size_t n)
{
  tbb::parallel_sort(&array[0], &array[n]);

  NewTask<size_t, msc::Item*> &a = *new(tbb::task::allocate_root()) NewTask<size_t, msc::Item*>(0, size, array);
  tbb::task::spawn_root_and_wait(a);
  
  // tbb::parallel_for(ConstantRange<size_t, Item*>(0, n, array), ItemFunctor(array), tbb::simple_partitioner());
}

MSC_NAMESPACE_END

#endif