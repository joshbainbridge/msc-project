#ifndef _OPENIMAGEWRAPPER_H_
#define _OPENIMAGEWRAPPER_H_

#include <OpenImageIO/texture.h>
#include <OpenImageIO/imagecache.h>
#include <tbb/enumerable_thread_specific.h>

#include <core/Constants.h>

MSC_NAMESPACE_BEGIN

typedef OpenImageIO::ImageCache* ImageCache;
typedef OpenImageIO::TextureSystem* TextureSystem;
typedef tbb::enumerable_thread_specific< TextureSystem > LocalTextureSystem;

inline TextureSystem nullTextureSystem()
{
  return NULL;
}

MSC_NAMESPACE_END

#endif