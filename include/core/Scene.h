#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include <boost/shared_ptr.hpp>

#include <core/Common.h>
#include <core/CameraInterface.h>
#include <core/LightInterface.h>

MSC_NAMESPACE_BEGIN

class Scene
{
public:
  inline void setCamera(boost::shared_ptr< CameraInterface > _camera) {m_camera = _camera;}
  inline void addLight(boost::shared_ptr< LightInterface > _light) {m_lights.push_back(_light);}
  // inline void addShape(boost::shared_ptr< ShapeInterface > _shape) {m_shapes.push_back(_shape);}
  // inline void addPrimitive(boost::shared_ptr< Primitive > _primitive) {m_primitives.push_back(_primitive);}
  
  inline boost::shared_ptr< CameraInterface > getCamera() const {return m_camera;}
  inline boost::shared_ptr< LightInterface > getLight(int _index) const {return m_lights[_index];}
  // inline boost::shared_ptr< ShapeInterface > getShape(int _index) const {return m_shapes[_index];}
  // inline boost::shared_ptr< Primitive > getPrimitive(int _index) const {return m_primitives[_index];}
  
  inline int countLight() const {return m_lights.size();}
  // inline int countShape() const {return m_shapes.size();}
  // inline int countPrimitive() const {return m_primitives.size();}
  
  bool intersectPrimitives(const Ray3f _ray) const;
  
private:
  boost::shared_ptr< CameraInterface > m_camera;
  std::vector< boost::shared_ptr< LightInterface > > m_lights;
  // std::vector< boost::shared_ptr< ShapeInterface > > m_shapes;
  // std::vector< boost::shared_ptr< Primitive > > m_primitives;
};

MSC_NAMESPACE_END

#endif
