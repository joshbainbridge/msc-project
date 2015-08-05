#ifndef _RAYUNCOMPRESSED_H_
#define _RAYUNCOMPRESSED_H_

#include <core/EmbreeWrapper.h>
#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Structure to contain a uncompressed ray
 */
struct RayUncompressed
{
  union
  {
    RTCRay rtc_ray;
    struct RTCORE_ALIGN(16)
    {
      //Ray data
      float org[3];
      float align0;
      
      float dir[3];
      float align1;
      
      float tnear;
      float tfar;

      float time;
      int mask;
      
      // Hit data
      float Ng[3];
      float align2;
      
      float u;
      float v;

      int geomID;
      int primID;
      int instID;
    };
  };

  //Added data
  float weight[3];
  float lastPdf;
  int rayDepth;
  int sampleID;
};

inline bool operator==(const RayUncompressed &lhs, const RayUncompressed &rhs)
{return (lhs.geomID == rhs.geomID);}
inline bool operator!=(const RayUncompressed &lhs, const RayUncompressed &rhs)
{return (lhs.geomID != rhs.geomID);}

/**
 * @brief      Comparison functor for uncompressed rays based on hit geometry and primative id
 */
struct CompareHit
{
  bool operator()(const RayUncompressed &lhs, const RayUncompressed &rhs) const
  {
    return (lhs.geomID < rhs.geomID) || (lhs.geomID == rhs.geomID && lhs.primID < rhs.primID);
  }
};

/**
 * @brief      Comparison functor for uncompressed rays based on position
 */
class CompareOrg
{
public:
  CompareOrg(int _axis = 0) : m_axis(_axis) {;}

  bool operator()(const RayUncompressed &lhs, const RayUncompressed &rhs) const
  {
    return lhs.org[m_axis] < rhs.org[m_axis];
  }

private:
  size_t m_axis;
};

/**
 * @brief      Comparison functor for uncompressed rays based on direction
 */
class CompareDir
{
public:
  CompareDir(int _axis = 0) : m_axis(_axis) {;}

  bool operator()(const RayUncompressed &lhs, const RayUncompressed &rhs) const
  {
    return lhs.dir[m_axis] < rhs.dir[m_axis];
  }

private:
  size_t m_axis;
};

MSC_NAMESPACE_END

#endif
