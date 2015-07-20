#include <core/Batch.h>

MSC_NAMESPACE_BEGIN

Batch::~Batch()
{
}

void Batch::construct(const int _exponent)
{
  m_size = pow(2, _exponent);

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

void Batch::add(const int _size, const int _index, RayCompressed* _data)
{
   m_mutexes[_index].lock();

  if(m_pointers[_index] == m_ends[_index])
  {
    m_outfile[_index].close();
    m_batch_queue.push(m_paths[_index]);

    std::string file_location = boost::filesystem::temp_directory_path().string();
    std::string file_name = boost::filesystem::unique_path().string();
    m_paths[_index] = file_location + file_name;

    boost::iostreams::mapped_file_params params;
    params.path = m_paths[_index];
    params.new_file_size = m_size * sizeof(RayCompressed);
    params.mode = std::ios_base::out;

    m_outfile[_index].open(params);
    m_pointers[_index] = (RayCompressed*)(m_outfile[_index].data());
    m_ends[_index] = m_pointers[_index] + m_size;
  }

  m_pointers[_index] = std::copy(_data, _data + _size, m_pointers[_index]);
  
  m_mutexes[_index].unlock();
}

bool Batch::pop(std::string* batch)
{
  return m_batch_queue.try_pop(*batch);
}

void Batch::clear()
{
  for(size_t iterator = 0; iterator < 6; ++iterator)
  {
    m_outfile[iterator].close();
    boost::filesystem::remove(m_paths[iterator]);
  }

  std::string path;
  while(m_batch_queue.try_pop(path))
  {
    boost::filesystem::remove(path);
    std::cout << "deleted file " << path << std::endl;
  }
}

MSC_NAMESPACE_END