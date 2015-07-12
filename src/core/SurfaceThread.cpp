#include <core/SurfaceThread.h>

MSC_NAMESPACE_BEGIN

void SurfaceThread::start(tbb::concurrent_queue< SurfaceTask >* _queue)
{
  m_thread = boost::thread(&SurfaceThread::process, this, _queue);
}

void SurfaceThread::join()
{
  m_thread.join();
}

void SurfaceThread::process(tbb::concurrent_queue< SurfaceTask >* _queue)
{

}

MSC_NAMESPACE_END