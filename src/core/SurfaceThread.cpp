#include <core/SurfaceThread.h>

MSC_NAMESPACE_BEGIN

void SurfaceThread::start()
{
  m_thread = boost::thread(&SurfaceThread::process, this);
}

void SurfaceThread::join()
{
  m_thread.join();
}

void SurfaceThread::process()
{

}

MSC_NAMESPACE_END