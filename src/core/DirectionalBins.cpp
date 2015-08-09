#include <core/DirectionalBins.h>

MSC_NAMESPACE_BEGIN

DirectionalBins::DirectionalBins(size_t _exponent) : m_exponent(_exponent)
{
  for(size_t index = 0; index < 6; ++index)
  {
    m_bin[index].size = 0;

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_bin[index].path = file_location + file_name;

    size_t file_size = pow(2, m_exponent);

    boost::iostreams::mapped_file_params params;
    params.path = m_bin[index].path;
    params.new_file_size = file_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_bin[index].outfile.open(params);
    m_bin[index].pointer = (RayCompressed*)(m_bin[index].outfile.data());
    m_bin[index].end = m_bin[index].pointer + file_size;
  }
}

DirectionalBins::~DirectionalBins()
{
  for(size_t index = 0; index < 6; ++index)
  {
    m_bin[index].outfile.close();
    boost::filesystem::remove(m_bin[index].path);
  }
}

void DirectionalBins::add(const int _size, const int _cardinal, RayCompressed* _data, tbb::concurrent_queue< BatchItem >* _batch_queue)
{
  m_bin[_cardinal].mutex.lock();

  size_t file_size = pow(2, m_exponent);

  if(m_bin[_cardinal].size + _size > file_size)
  {
    std::cout << pow(2, m_exponent) << ", " << m_bin[_cardinal].size << std::endl;
    m_bin[_cardinal].outfile.close();

    BatchItem batch;
    batch.filename = m_bin[_cardinal].path;
    batch.size = m_bin[_cardinal].size;
    _batch_queue->push(batch);

    m_bin[_cardinal].size = 0;

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_bin[_cardinal].path = file_location + file_name;

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
  size_t bin_size = 0;
  size_t bin_index = 0;
  for(size_t index = 0; index < 6; ++index)
  {
    if(m_bin[index].size > bin_size)
    {
      bin_size = m_bin[index].size;
      bin_index = index;
    }
  }

  if(m_bin[bin_index].size > 0)
  {
    std::cout << pow(2, m_exponent) << ", " << m_bin[bin_index].size << std::endl;
    m_bin[bin_index].outfile.close();
    
    BatchItem batch;
    batch.filename = m_bin[bin_index].path;
    batch.size = m_bin[bin_index].size;
    _batch_queue->push(batch);
    
    m_bin[bin_index].size = 0;
    
    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_bin[bin_index].path = file_location + file_name;
    
    size_t file_size = pow(2, m_exponent);
    
    boost::iostreams::mapped_file_params params;
    params.path = m_bin[bin_index].path;
    params.new_file_size = file_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;
    
    m_bin[bin_index].outfile.open(params);
    m_bin[bin_index].pointer = (RayCompressed*)(m_bin[bin_index].outfile.data());
    m_bin[bin_index].end = m_bin[bin_index].pointer + file_size;
  }
}

MSC_NAMESPACE_END