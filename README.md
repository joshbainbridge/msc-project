Sorted Shading for Uni-Directional Pathtracing:
===============================================

This repository is the the implementation of a uni-directional pathtracer based upon the design described by Eisenacher et al. (2013), as well as other well established optimisations for production rendering. Compilation should have optimisations enabled and the Embree library will need to be compiled with RTCORE_ENABLE_RAY_MASK turned on.

Required dependencies are:

* CMake 2.8
* Boost 1.55
* TBB 4.2
* Embree 2.6.1
* OpenEXR 2.2
* OpenColorIO 1.0.9
* OpenImageIO 1.5.13
* yaml-cpp 0.5.1
* Eigen 3.2.4
* Python 2.7
* GLFW 3.0
* OpenGL 3.3
* GLEW 1.11