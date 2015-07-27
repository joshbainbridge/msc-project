CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( "msc-project" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake $ENV{CMAKE_MODULE_PATH})

SET( COMPILER clang )
SET( FLAGS )
SET( TYPE RelWithDebInfo )

SET( SRC src )
SET( INC include )
SET( THR thirdparty )

SET( CORE_SOURCES
  ${SRC}/core/Pathtracer.cpp
  ${SRC}/core/RandomGenerator.cpp
  ${SRC}/core/DirectionalBins.cpp
  ${SRC}/core/ThinLensCamera.cpp
  ${SRC}/core/PinHoleCamera.cpp
  ${SRC}/core/TentFilter.cpp
  ${SRC}/core/BoxFilter.cpp
  ${SRC}/core/StratifiedSampler.cpp
  ${SRC}/core/IndependentSampler.cpp
  ${SRC}/core/GridSampler.cpp
  ${SRC}/core/Camera.cpp
  ${SRC}/core/Shading.cpp
  ${SRC}/core/RaySort.cpp
  ${SRC}/core/RayIntersect.cpp
  ${SRC}/core/RayDecompress.cpp
  ${SRC}/core/RayBoundingbox.cpp
  ${SRC}/core/LambertShader.cpp
  ${SRC}/core/NullShader.cpp
  ${SRC}/core/Convolve.cpp
  )

SET( CORE_HEADERS
  ${INC}/core/Pathtracer.h
  ${INC}/core/Constants.h
  ${INC}/core/Common.h
  ${INC}/core/EmbreeWrapper.h
  ${INC}/core/OpenImageWrapper.h
  ${INC}/core/Settings.h
  ${INC}/core/Image.h
  ${INC}/core/Scene.h
  ${INC}/core/RandomGenerator.h
  ${INC}/core/Buffer.h
  ${INC}/core/DirectionalBins.h
  ${INC}/core/CameraInterface.h
  ${INC}/core/ThinLensCamera.h
  ${INC}/core/PinHoleCamera.h
  ${INC}/core/FilterInterface.h
  ${INC}/core/TentFilter.h
  ${INC}/core/BoxFilter.h
  ${INC}/core/SamplerInterface.h
  ${INC}/core/StratifiedSampler.h
  ${INC}/core/IndependentSampler.h
  ${INC}/core/GridSampler.h
  ${INC}/core/Camera.h
  ${INC}/core/Shading.h
  ${INC}/core/RayCompressed.h
  ${INC}/core/RayUncompressed.h
  ${INC}/core/RaySort.h
  ${INC}/core/RayIntersect.h
  ${INC}/core/RayDecompress.h
  ${INC}/core/RayBoundingbox.h
  ${INC}/core/PolygonObject.h
  ${INC}/core/ShaderInterface.h
  ${INC}/core/NullShader.h
  ${INC}/core/LambertShader.h
  ${INC}/core/BatchItem.h
  ${INC}/core/Convolve.h
  )

SET( FRM_SOURCES
  ${SRC}/framebuffer/Framebuffer.cpp
  )

SET( FRM_HEADERS
  ${INC}/framebuffer/PlatformSpecification.h
  ${INC}/framebuffer/Framebuffer.h
  )

SET( THIRD_PARTY
  ${THR}/tinyobjloader/tiny_obj_loader.cpp
  ${THR}/tinyobjloader/tiny_obj_loader.h
  )

ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME}
  ${SRC}/main.cpp
  )

ADD_LIBRARY( pathtracer
  ${CORE_SOURCES}
  ${CORE_HEADERS}
  ${THIRD_PARTY}
  )

ADD_LIBRARY( framebuffer
  ${FRM_SOURCES}
  ${FRM_HEADERS}
  )

FIND_PACKAGE( Boost COMPONENTS
  system filesystem program_options unit_test_framework iostreams thread chrono python REQUIRED )
FIND_PACKAGE( TBB REQUIRED )
FIND_PACKAGE( Embree REQUIRED )
FIND_PACKAGE( Eigen3 REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( GLFW REQUIRED )
FIND_PACKAGE( YamlCpp REQUIRED )
FIND_PACKAGE( OpenEXR REQUIRED )
FIND_PACKAGE( OpenColorIO REQUIRED )
FIND_PACKAGE( OpenImageIO REQUIRED )
FIND_PACKAGE( PythonLibs REQUIRED )

IF( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" OR "Linux" )
  
  FIND_PACKAGE( GLEW REQUIRED )

  INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_DIR} )
  TARGET_LINK_LIBRARIES( framebuffer ${GLEW_LIBRARY} )

ENDIF()

SET(CMAKE_CXX_COMPILER ${CXX} )
SET(CMAKE_CXX_FLAGS ${FLAGS} )
SET(CMAKE_BUILD_TYPE ${TYPE} )

INCLUDE_DIRECTORIES(
  ${INC}
  ${THR}
  ${Boost_INCLUDE_DIRS}
  ${TBB_INCLUDE_DIRS}
  ${EMBREE_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${YAMLCPP_INCLUDE_DIR}
  ${OPENEXR_INCLUDE_PATHS}
  ${OCIO_INCLUDES}
  ${OIIO_INCLUDES}
  ${PYTHON_INCLUDE_DIRS}
  )

TARGET_LINK_LIBRARIES( ${CMAKE_PROJECT_NAME}
  ${Boost_LIBRARIES}
  pathtracer
  framebuffer
  )

TARGET_LINK_LIBRARIES( pathtracer
  ${Boost_LIBRARIES}
  ${TBB_LIBRARIES}
  ${EMBREE_LIBRARIES}
  ${YAMLCPP_LIBRARY}
  ${OPENEXR_LIBRARIES}
  ${OCIO_LIBRARIES}
  ${OIIO_LIBRARIES}
  ${PYTHON_LIBRARIES}
  )

TARGET_LINK_LIBRARIES( framebuffer
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

INSTALL(
  TARGETS ${CMAKE_PROJECT_NAME} pathtracer
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  )

INSTALL(
  FILES ${CORE_HEADERS}
  PUBLIC_HEADER DESTINATION include/pathtracer
  )

ENABLE_TESTING()

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)