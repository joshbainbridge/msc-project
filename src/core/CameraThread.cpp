#include <core/CameraThread.h>

MSC_NAMESPACE_BEGIN

void CameraThread::start()
{
  m_thread = boost::thread(&CameraThread::process, this);
}

void CameraThread::join()
{
  m_thread.join();
}

void CameraThread::process()
{

}

MSC_NAMESPACE_END