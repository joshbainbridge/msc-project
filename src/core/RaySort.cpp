#include <core/RaySort.h>

MSC_NAMESPACE_BEGIN

tbb::task* RaySort::execute()
{
  if((m_end - m_begin) > 4096)
  {
    int temp = ((m_limits.max[0] - m_limits.min[0]) < (m_limits.max[1] - m_limits.min[1])) ? 1 : 0;
    int axis = (temp < (m_limits.max[2] - m_limits.min[2])) ? 2 : temp;

    std::sort(&m_array[m_begin], &m_array[m_end], RayCompare(axis));

    size_t middle = (m_end + m_begin) / 2;

    BoundingBox3f llimits = m_limits;
    llimits.max[axis] = m_array[middle].org[axis];
    BoundingBox3f rlimits = m_limits;
    llimits.min[axis] = m_array[middle].org[axis];

    RaySort &lrange = *new( tbb::task::allocate_child() ) RaySort(m_begin, middle, llimits, m_array);
    RaySort &rrange = *new( tbb::task::allocate_child() ) RaySort(middle, m_end, rlimits, m_array);

    tbb::task::set_ref_count(3);

    tbb::task::spawn(lrange);
    tbb::task::spawn(rrange);
    tbb::task::wait_for_all();

    return NULL;
  }

  // if(false)
  // {
  //   RaySort &lrange = *new( tbb::task::allocate_child() ) RaySort(m_begin, middle, llimits, m_array);
  //   RaySort &rrange = *new( tbb::task::allocate_child() ) RaySort(middle, m_end, rlimits, m_array);

  //   tbb::task::set_ref_count(3);

  //   tbb::task::spawn(lrange);
  //   tbb::task::spawn(rrange);
  //   tbb::task::wait_for_all();

  //   return NULL;
  // }

  // //else do this

  return NULL;
}

MSC_NAMESPACE_END