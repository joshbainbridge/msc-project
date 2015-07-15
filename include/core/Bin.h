#ifndef _BIN_H_
#define _BIN_H_

#include <boost/shared_ptr.hpp>

#include <core/Common.h>
#include <core/RayCompressed.h>

MSC_NAMESPACE_BEGIN

struct Bin
{
  size_t size;
  size_t index[6];
  boost::shared_ptr< RayCompressed[] > bin[6];
};

MSC_NAMESPACE_END

#endif