CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( "msc-project" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake $ENV{CMAKE_MODULE_PATH})

SET( CXX )
SET( FLAGS )

SET( SRC src)
SET( INC include)
SET( THR thirdparty )

SET( CORE_SOURCES
  ${SRC}/core/CameraInterface.cpp
  ${SRC}/core/GeometricCamera.cpp
  ${SRC}/core/Image.cpp
  ${SRC}/core/LightInterface.cpp
  ${SRC}/core/Pixel.cpp
  ${SRC}/core/QuadLight.cpp
  ${SRC}/core/RandomGenerator.cpp
  ${SRC}/core/Scene.cpp
  )

SET( CORE_HEADERS
  ${INC}/core/CameraInterface.h
  ${INC}/core/Common.h
  ${INC}/core/GeometricCamera.h
  ${INC}/core/Image.h
  ${INC}/core/LightInterface.h
  ${INC}/core/Pixel.h
  ${INC}/core/QuadLight.h
  ${INC}/core/RandomGenerator.h
  ${INC}/core/Scene.h
  ${INC}/core/Settings.h
  )

SET( FRB_SOURCES
  ${SRC}/framebuffer/Framebuffer.cpp
  )

SET( FRB_HEADERS
  ${INC}/framebuffer/PlatformSpecification.h
  ${INC}/framebuffer/Framebuffer.h
  )

SET( THIRD_PARTY
  ${THR}/tinyobjloader/tiny_obj_loader.cpp
  ${THR}/tinyobjloader/tiny_obj_loader.h
  )

ADD_EXECUTABLE( ${CMAKE_PROJECT_NAME}
  ${SRC}/main.cpp
  ${CORE_SOURCES}
  ${CORE_HEADERS}
  ${FRB_SOURCES}
  ${FRB_HEADERS}
  ${THIRD_PARTY}
  )

FIND_PACKAGE( Boost COMPONENTS system filesystem iostreams chrono python REQUIRED )
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
  TARGET_LINK_LIBRARIES( ${CMAKE_PROJECT_NAME} ${GLEW_LIBRARY} )

ENDIF()

SET(CMAKE_CXX_COMPILER ${CXX})
SET(CMAKE_CXX_FLAGS ${FLAGS})

INCLUDE_DIRECTORIES(
  include
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
  ${TBB_LIBRARIES}
  ${EMBREE_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${YAMLCPP_LIBRARY}
  ${OPENEXR_LIBRARIES}
  ${OCIO_LIBRARIES}
  ${OIIO_LIBRARIES}
  ${PYTHON_LIBRARIES}
  )

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/objects DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)