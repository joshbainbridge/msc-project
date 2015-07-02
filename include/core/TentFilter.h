#ifndef _TENTFILTER_H_
#define _TENTFILTER_H_

#include <core/Common.h>
#include <core/FilterInterface.h>

MSC_NAMESPACE_BEGIN

class TentFilter : public FilterInterface
{
public:
  TentFilter();

  void virtualFunc(const float value_one, const float value_two) const;

private:
  float m_other_variable;
};

MSC_NAMESPACE_END

#endif
