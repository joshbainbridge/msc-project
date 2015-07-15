#include <core/RaySort.h>

MSC_NAMESPACE_BEGIN

class CompareOrg
{
public:
  CompareOrg(int _axis = 0) : m_axis(_axis) {;}

  bool operator()(RayUncompressed const &lhs, RayUncompressed const &rhs)
  {
    return lhs.org[m_axis] < rhs.org[m_axis];
  }

private:
  size_t m_axis;
};

class CompareDir
{
public:
  CompareDir(int _axis = 0) : m_axis(_axis) {;}

  bool operator()(RayUncompressed const &lhs, RayUncompressed const &rhs)
  {
    return lhs.dir[m_axis] < rhs.dir[m_axis];
  }

private:
  size_t m_axis;
};

tbb::task* RaySort::execute()
{
  size_t size = (m_end - m_begin);

  if(size > 4096)
  {
    int temp = ((m_limits.max[0] - m_limits.min[0]) < (m_limits.max[1] - m_limits.min[1])) ? 1 : 0;
    int axis = (temp < (m_limits.max[2] - m_limits.min[2])) ? 2 : temp;

    std::sort(&m_output[m_begin], &m_output[m_end], CompareOrg(axis));

    size_t middle = m_begin + (size / 2);
    size_t lsize = (middle - m_begin);
    size_t rsize = (m_end - middle);

    BoundingBox3f llimits;
    BoundingBox3f rlimits;

    if(lsize > 4096)
    {
      llimits = m_limits;
      llimits.max[axis] = m_output[middle - 1].org[axis];
    }
    else
    {
      llimits.min[0] = m_output[m_begin].dir[0];
      llimits.min[1] = m_output[m_begin].dir[1];
      llimits.min[2] = m_output[m_begin].dir[2];
      llimits.max[0] = m_output[m_begin].dir[0];
      llimits.max[1] = m_output[m_begin].dir[1];
      llimits.max[2] = m_output[m_begin].dir[2];

      for(size_t iterator = m_begin; iterator < middle; ++iterator)
      {
        if(m_output[iterator].dir[0] < llimits.min[0])
          llimits.min[0] = m_output[iterator].dir[0];
        if(m_output[iterator].dir[1] < llimits.min[1])
          llimits.min[1] = m_output[iterator].dir[1];
        if(m_output[iterator].dir[2] < llimits.min[2])
          llimits.min[2] = m_output[iterator].dir[2];

        if(m_output[iterator].dir[0] > llimits.max[0])
          llimits.max[0] = m_output[iterator].dir[0];
        if(m_output[iterator].dir[1] > llimits.max[1])
          llimits.max[1] = m_output[iterator].dir[1];
        if(m_output[iterator].dir[2] > llimits.max[2])
          llimits.max[2] = m_output[iterator].dir[2];
      }
    }

    if(rsize > 4096)
    {
      rlimits = m_limits;
      rlimits.min[axis] = m_output[middle].org[axis];
    }
    else
    {
      rlimits.min[0] = m_output[middle].dir[0];
      rlimits.min[1] = m_output[middle].dir[1];
      rlimits.min[2] = m_output[middle].dir[2];
      rlimits.max[0] = m_output[middle].dir[0];
      rlimits.max[1] = m_output[middle].dir[1];
      rlimits.max[2] = m_output[middle].dir[2];

      for(size_t iterator = middle; iterator < m_end; ++iterator)
      {
        if(m_output[iterator].dir[0] < rlimits.min[0])
          rlimits.min[0] = m_output[iterator].dir[0];
        if(m_output[iterator].dir[1] < rlimits.min[1])
          rlimits.min[1] = m_output[iterator].dir[1];
        if(m_output[iterator].dir[2] < rlimits.min[2])
          rlimits.min[2] = m_output[iterator].dir[2];

        if(m_output[iterator].dir[0] > rlimits.max[0])
          rlimits.max[0] = m_output[iterator].dir[0];
        if(m_output[iterator].dir[1] > rlimits.max[1])
          rlimits.max[1] = m_output[iterator].dir[1];
        if(m_output[iterator].dir[2] > rlimits.max[2])
          rlimits.max[2] = m_output[iterator].dir[2];
      }
    }

    RaySort &lrange = *new( tbb::task::allocate_child() ) RaySort(m_begin, middle, llimits, m_output);
    RaySort &rrange = *new( tbb::task::allocate_child() ) RaySort(middle, m_end, rlimits, m_output);

    tbb::task::set_ref_count(3);

    tbb::task::spawn(lrange);
    tbb::task::spawn(rrange);
    tbb::task::wait_for_all();
  }
  else if(size > 64)
  {
    int temp = ((m_limits.max[0] - m_limits.min[0]) < (m_limits.max[1] - m_limits.min[1])) ? 1 : 0;
    int axis = (temp < (m_limits.max[2] - m_limits.min[2])) ? 2 : temp;

    std::sort(&m_output[m_begin], &m_output[m_end], CompareDir(axis));

    size_t middle = m_begin + (size / 2);
    size_t lsize = (middle - m_begin);
    size_t rsize = (m_end - middle);

    BoundingBox3f llimits;
    BoundingBox3f rlimits;

    llimits = m_limits;
    llimits.max[axis] = m_output[middle - 1].dir[axis];
    rlimits = m_limits;
    llimits.min[axis] = m_output[middle].dir[axis];

    RaySort &lrange = *new( tbb::task::allocate_child() ) RaySort(m_begin, middle, llimits, m_output);
    RaySort &rrange = *new( tbb::task::allocate_child() ) RaySort(middle, m_end, rlimits, m_output);

    tbb::task::set_ref_count(3);

    tbb::task::spawn(lrange);
    tbb::task::spawn(rrange);
    tbb::task::wait_for_all();
  }

  return NULL;
}

MSC_NAMESPACE_END