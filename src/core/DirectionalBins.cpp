#include <core/DirectionalBins.h>

MSC_NAMESPACE_BEGIN

DirectionalBins::DirectionalBins(size_t _exponent) : m_exponent(_exponent)
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_bin[iterator].size = 0;

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_bin[iterator].path = file_location + file_name;

    size_t file_size = pow(2, m_exponent);

    boost::iostreams::mapped_file_params params;
    params.path = m_bin[iterator].path;
    params.new_file_size = file_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_bin[iterator].outfile.open(params);
    m_bin[iterator].pointer = (RayCompressed*)(m_bin[iterator].outfile.data());
    m_bin[iterator].end = m_bin[iterator].pointer + file_size;
  }
}

DirectionalBins::~DirectionalBins()
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_bin[iterator].outfile.close();
    boost::filesystem::remove(m_bin[iterator].path);
  }
}

void DirectionalBins::add(const int _size, const int _cardinal, RayCompressed* _data, tbb::concurrent_queue< BatchItem >* _batch_queue)
{
  m_bin[_cardinal].mutex.lock();

  if(m_bin[_cardinal].pointer == m_bin[_cardinal].end)
  {
    m_bin[_cardinal].outfile.close();

    BatchItem batch;
    batch.filename = m_bin[_cardinal].path;
    batch.size = m_bin[_cardinal].size;
    _batch_queue->push(batch);

    m_bin[_cardinal].size = 0;

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_bin[_cardinal].path = file_location + file_name;

    size_t file_size = pow(2, m_exponent);

    boost::iostreams::mapped_file_params params;
    params.path = m_bin[_cardinal].path;
    params.new_file_size = file_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_bin[_cardinal].outfile.open(params);
    m_bin[_cardinal].pointer = (RayCompressed*)(m_bin[_cardinal].outfile.data());
    m_bin[_cardinal].end = m_bin[_cardinal].pointer + file_size;
  }

  m_bin[_cardinal].pointer = std::copy(_data, _data + _size, m_bin[_cardinal].pointer);
  m_bin[_cardinal].size = m_bin[_cardinal].size + _size;
  
  m_bin[_cardinal].mutex.unlock();
}

void DirectionalBins::flush(tbb::concurrent_queue< BatchItem >* _batch_queue)
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_bin[iterator].outfile.close();
    
    if(m_bin[iterator].size != 0)
    {
      BatchItem batch;
      batch.filename = m_bin[iterator].path;
      batch.size = m_bin[iterator].size;
      _batch_queue->push(batch);
    }
  }
}

MSC_NAMESPACE_END