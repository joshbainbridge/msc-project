#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_

#include <boost/random.hpp>
#include <tbb/enumerable_thread_specific.h>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      A uniform random generator
 * 
 * This random generator uses boost's mersenne twister algorithm and a uniform distribution to
 * produce float values between zero and one. Local copies are created for each thread and passed
 * to scene objects to prevent mutation of shared data.
 */
class RandomGenerator
{
public:
  /**
   * @brief      Creates a uniform random number between 0 and 1 
   *
   * @return     random number
   */
  float sample();

  /**
   * @brief      Overload so that class can be used with std::generate
   */
  inline float operator()(){return this->sample();}

private:
  boost::mt19937 m_generator;
  boost::uniform_real<float> m_uniform_dist;
};

typedef tbb::enumerable_thread_specific< RandomGenerator > LocalRandomGenerator;

MSC_NAMESPACE_END

#endif
