#ifndef _OBJECTINTERFACE_H_
#define _OBJECTINTERFACE_H_

#include <core/Common.h>

MSC_NAMESPACE_BEGIN

/**
 * @brief      Abstract interface class for image samplers
 * 
 * This is a simple interface for using a object in a polymorphic sense. It only requires that
 * each inherited class be able to retrieve texture coordinates using a primitive identification
 * number and some barycentric coordinates.
 */
class ObjectInterface
{
public:
  /**
   * @brief      Virtual destructor required for interface 
   */
  virtual ~ObjectInterface() {}

  /**
   * @brief      Getter method for shader id
   *
   * @return     shader id
   */
  virtual int shader() const =0;

  /**
   * @brief      Gets u and v texture coordinates
   *
   * @param[in]  _primitive  primitive index value
   * @param[in]  _s          s coordinate
   * @param[in]  _t          t coordinate
   * @param      _output     output uv coordinates
   */
  virtual void texture(
    const size_t _primitive,
    const float _s,
    const float _t,
    Vector2f* _output
    ) const =0;
};

MSC_NAMESPACE_END

#endif
