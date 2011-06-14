cmake_minimum_required (VERSION 2.8)
PROJECT (HDF5CHECK_TOOL C CXX)

#-----------------------------------------------------------------------------
# Setup include Directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${HDF5CHECK_DIR}/src)
INCLUDE_DIRECTORIES (${HDF5CHECK_PROJECT_DIR})

# --------------------------------------------------------------------
# Add the h5check executable
# --------------------------------------------------------------------
ADD_EXECUTABLE (${HDF5CHECK_UTILITY} ${HDF5CHECK_TOOL_SOURCE_DIR}/h5checker_main.c)
TARGET_NAMING (${HDF5CHECK_UTILITY} ${LIB_TYPE})
TARGET_LINK_LIBRARIES (${HDF5CHECK_UTILITY} ${HDF5CHECK_LIB_TARGET} ${HDF5_LIB_TARGET})

SET (HDF5CHECK_DEP_EXECUTABLES ${HDF5CHECK_UTILITY})
  