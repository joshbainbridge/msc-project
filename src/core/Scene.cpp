#include <core/Scene.h>

MSC_NAMESPACE_BEGIN

//Intersect all primitves in scene
bool Scene::intersectPrimitives(const Ray3f _ray) const
{
  // bool intersection_found = false;
  // Transmission closest_intersection;

  // //Loop through all the primitves
  // int primitve_count = m_primitives.size();
  // for (int i = 0; i < primitve_count; ++i)
  // {
  //   Transmission primitive_intersection;
  //   if(m_primitives[i]->intersect(_ray, &primitive_intersection) && (primitive_intersection.distance < closest_intersection.distance))
  //   {
  //     closest_intersection = primitive_intersection;
  //     intersection_found = true;
  //   }
  // }

  // //If intersection found, set the material and the transmission, then return true
  // if (intersection_found)
  // {
  //   closest_intersection.position += closest_intersection.normal * M_EPSILON;
  //   *_output = closest_intersection;
  //   return true;
  // }

  // //Intersection was not found
  return false;
}

MSC_NAMESPACE_END