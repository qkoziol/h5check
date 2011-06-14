cmake_minimum_required (VERSION 2.8)
PROJECT (HDF5CHECK_LIB C CXX)

#-----------------------------------------------------------------------------
# Define Sources
#-----------------------------------------------------------------------------

SET (H5CHECK_LIB_SRCS
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5checker.c
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5checker_api.c
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5error.c
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5checkerV2_0.c
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5pline.c
)

SET (H5CHECK_LIB_HDRS
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5_check.h
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5_error.h
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5_pline.h
    ${HDF5CHECK_LIB_SOURCE_DIR}/h5check_public.h
)

ADD_LIBRARY (${HDF5CHECK_LIB_TARGET} ${LIB_TYPE} ${H5CHECK_LIB_SRCS} ${H5CHECK_LIB_HDRS} ${HDF5CHECK_BINARY_DIR}/h5chk_config.h)
TARGET_LINK_LIBRARIES (${HDF5CHECK_LIB_TARGET} ${LINK_LIBS})
SET_GLOBAL_VARIABLE (HDF5CHECK_LIBRARIES_TO_EXPORT "${HDF5CHECK_LIBRARIES_TO_EXPORT};${HDF5CHECK_LIB_TARGET}")
HDF_SET_LIB_OPTIONS (
    ${HDF5CHECK_LIB_TARGET} ${HDF5CHECK_LIB_NAME}
    ${LIB_TYPE}
    HDF5CHECK_LIB_NAME_RELEASE
    HDF5CHECK_LIB_NAME_DEBUG
)

##############################################################################
##############################################################################
###           I N S T A L L A T I O N                                      ###
##############################################################################
##############################################################################

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install 
#-----------------------------------------------------------------------------
INSTALL (
    FILES
        ${HDF5CHECK_LIB_SOURCE_DIR}/h5check_public.h
    DESTINATION
        ${HDF5CHECK_INSTALL_INCLUDE_DIR}
    COMPONENT
        headers
)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (HDF5CHECK_EXPORTED_TARGETS)
  INSTALL (
      TARGETS
          ${HDF5CHECK_LIB_TARGET}
      EXPORT
          ${HDF5CHECK_EXPORTED_TARGETS}
      LIBRARY DESTINATION ${HDF5CHECK_INSTALL_LIB_DIR} COMPONENT libraries
      ARCHIVE DESTINATION ${HDF5CHECK_INSTALL_LIB_DIR} COMPONENT libraries
      RUNTIME DESTINATION ${HDF5CHECK_INSTALL_BIN_DIR} COMPONENT libraries
  )
ENDIF (HDF5CHECK_EXPORTED_TARGETS)
 