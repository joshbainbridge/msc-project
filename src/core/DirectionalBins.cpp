#include <core/DirectionalBins.h>

MSC_NAMESPACE_BEGIN

void DirectionalBins::construct(const int _exponent, tbb::concurrent_queue< BatchItem >* _batch_queue)
{
  m_size = pow(2, _exponent);
  m_batch_queue = _batch_queue;

  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_paths[iterator] = file_location + file_name;

    boost::iostreams::mapped_file_params params;
    params.path = m_paths[iterator];
    params.new_file_size = m_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_outfile[iterator].open(params);
    m_pointers[iterator] = (RayCompressed*)(m_outfile[iterator].data());
    m_ends[iterator] = m_pointers[iterator] + m_size;
  }
}

void DirectionalBins::add(const int _size, const int _cardinal, RayCompressed* _data)
{
   m_mutexes[_cardinal].lock();

  if(m_pointers[_cardinal] == m_ends[_cardinal])
  {
    m_outfile[_cardinal].close();

    BatchItem batch;
    batch.filename = m_paths[_cardinal];
    batch.size = m_size;
    m_batch_queue->push(batch);

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_paths[_cardinal] = file_location + file_name;

    boost::iostreams::mapped_file_params params;
    params.path = m_paths[_cardinal];
    params.new_file_size = m_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_outfile[_cardinal].open(params);
    m_pointers[_cardinal] = (RayCompressed*)(m_outfile[_cardinal].data());
    m_ends[_cardinal] = m_pointers[_cardinal] + m_size;
  }

  m_pointers[_cardinal] = std::copy(_data, _data + _size, m_pointers[_cardinal]);
  
  m_mutexes[_cardinal].unlock();
}

bool DirectionalBins::pop(BatchItem* batch)
{
  return m_batch_queue->try_pop(*batch);
}

void DirectionalBins::flush()
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_outfile[iterator].close();

    BatchItem batch;
    batch.filename = m_paths[iterator];
    batch.size = m_size;
    m_batch_queue->push(batch);
  }
}

void DirectionalBins::clear()
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_outfile[iterator].close();
    boost::filesystem::remove(m_paths[iterator]);
  }

  BatchItem batch;
  while(m_batch_queue->try_pop(batch))
  {
    boost::filesystem::remove(batch.filename);
    std::cout << "deleted file " << batch.filename << std::endl;
  }
}

MSC_NAMESPACE_END