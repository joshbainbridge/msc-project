#
# Find Embree
#
# Try to find Embree.
# This module defines the following variables:
# - EMBREE_INCLUDE_DIRS
# - EMBREE_LIBRARIES
# - EMBREE_FOUND
#
# The following variables can be set as arguments for the module.
# - EMBREE_ROOT_DIR : Root library directory of Embree
#

# Additional modules
include(FindPackageHandleStandardArgs)

# Find include files
find_path(
  EMBREE_INCLUDE_DIR
  NAMES embree2/rtcore.h
  PATHS
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
  DOC "The directory where embree2/rtcore.h resides")

# Find library files
# Try to use static libraries
find_library(
  EMBREE_LIBRARY
  NAMES embree.2
  PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    ${EMBREE_ROOT_DIR}/lib
  DOC "The Embree library")

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(EMBREE DEFAULT_MSG EMBREE_INCLUDE_DIR EMBREE_LIBRARY)

# Define EMBREE_LIBRARIES and EMBREE_INCLUDE_DIRS
if (EMBREE_FOUND)
  set(EMBREE_LIBRARIES ${EMBREE_LIBRARY})
  set(EMBREE_INCLUDE_DIRS ${EMBREE_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(EMBREE_INCLUDE_DIR EMBREE_LIBRARY)