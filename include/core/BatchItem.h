#ifndef _BATCHITEM_H_
#define _BATCHITEM_H_

#include <string>

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

struct BatchItem
{
  std::string filename;
  size_t size;
};

MSC_NAMESPACE_END

#endif
