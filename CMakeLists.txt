cmake_minimum_required (VERSION 2.8)
PROJECT (HDF5CHECK C CXX)

#-----------------------------------------------------------------------------
# Instructions for use : Normal Build
#
# For standard build of HDF5 examples.
#-----------------------------------------------------------------------------
IF (BUILD_SHARED_LIBS)
  SET (BUILD_NAME_EXT "SHARED")
ELSE (BUILD_SHARED_LIBS)
  SET (BUILD_NAME_EXT "STATIC")
ENDIF (BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Set the target names of all the libraries (from FindHDF5.cmake)
#-----------------------------------------------------------------------------
SET (HDF5_LIB_TARGET              "hdf5")

#-----------------------------------------------------------------------------
# Set the target names of the executable
#-----------------------------------------------------------------------------
SET (HDF5CHECK_LIB_NAME           "hdf5check")
SET (HDF5CHECK_UTILITY            "h5check")
SET (HDF5CHECK_LIB_TARGET         ${HDF5CHECK_LIB_NAME})

#-----------------------------------------------------------------------------
# Define some CMake variables for use later in the project
#-----------------------------------------------------------------------------
SET (HDF5CHECK_RESOURCES_DIR      ${HDF5CHECK_SOURCE_DIR}/config/cmake)
SET (HDF5CHECK_SRC_DIR            ${HDF5CHECK_SOURCE_DIR}/src)
SET (HDF5CHECK_TOOL_DIR           ${HDF5CHECK_SOURCE_DIR}/tool)

IF (NOT HDF5CHECK_INSTALL_BIN_DIR)
  SET (HDF5CHECK_INSTALL_BIN_DIR bin)
ENDIF (NOT HDF5CHECK_INSTALL_BIN_DIR)
IF (NOT HDF5CHECK_INSTALL_LIB_DIR)
  SET (HDF5CHECK_INSTALL_LIB_DIR lib)
ENDIF (NOT HDF5CHECK_INSTALL_LIB_DIR)
IF (NOT HDF5CHECK_INSTALL_INCLUDE_DIR)
  SET (HDF5CHECK_INSTALL_INCLUDE_DIR include)
ENDIF (NOT HDF5CHECK_INSTALL_INCLUDE_DIR)
IF (NOT HDF5CHECK_INSTALL_DATA_DIR)
  SET (HDF5CHECK_INSTALL_DATA_DIR share)
ENDIF (NOT HDF5CHECK_INSTALL_DATA_DIR)

#-----------------------------------------------------------------------------
# parse the full version number from h5_check.h and include in H5CHECK_VERS_INFO
#-----------------------------------------------------------------------------
FILE (READ ${HDF5CHECK_SRC_DIR}/h5_check_defines.h _h5_check_defines_h_contents)
STRING (REGEX REPLACE ".*#define[ \t]+H5Check_VERSION[ \t]+\"([0-9]*.[0-9]*).*$"
    "\\1" H5CHECK_FULL_VERSION ${_h5_check_defines_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+H5Check_VERSION[ \t]+\"([0-9]*).*$"
    "\\1" H5CHECK_VERS_MAJOR ${_h5_check_defines_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+H5Check_VERSION[ \t]+\"[0-9]*.([0-9]*).*$"
    "\\1" H5CHECK_VERS_MINOR ${_h5_check_defines_h_contents})
STRING (REGEX REPLACE ".*#define[ \t]+H5Check_VERSION[ \t]+\"[0-9]*.[0-9]*.snap([0-9]*).*$"
    "\\1" H5CHECK_VERS_RELEASE ${_h5_check_defines_h_contents})
#MESSAGE (STATUS "VERSION: ${H5CHECK_FULL_VERSION}")

#-----------------------------------------------------------------------------
# Basic HDF5CHECK stuff here
#-----------------------------------------------------------------------------
SET (HDF5CHECK_PACKAGE "HDF5CHECK")
SET (HDF5CHECK_PACKAGE_NAME "HDF5CHECK")
SET (HDF5CHECK_PACKAGE_VERSION "${H5CHECK_VERS_MAJOR}.${H5CHECK_VERS_MINOR}-${H5CHECK_VERS_RELEASE}")
SET (HDF5CHECK_PACKAGE_VERSION_MAJOR "${H5CHECK_VERS_MAJOR}.${H5CHECK_VERS_MINOR}")
SET (HDF5CHECK_PACKAGE_VERSION_MINOR "${H5CHECK_VERS_RELEASE}")
SET (HDF5CHECK_PACKAGE_STRING "${HDF5CHECK_PACKAGE_NAME} ${HDF5CHECK_PACKAGE_VERSION}")
SET (HDF5CHECK_PACKAGE_TARNAME "h5check")
SET (HDF5CHECK_PACKAGE_URL "http://www.hdfgroup.org/h5check")
SET (HDF5CHECK_PACKAGE_BUGREPORT "help@hdfgroup.org")

#-----------------------------------------------------------------------------
# Include some macros for reusable code
#-----------------------------------------------------------------------------
INCLUDE (${HDF5CHECK_RESOURCES_DIR}/HDFMacros.cmake)
INCLUDE (${HDF5CHECK_RESOURCES_DIR}/HDF5CHECKMacros.cmake)

#-----------------------------------------------------------------------------
# Setup output Directories
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
  )
  SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
  )
  SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
  )
ELSE (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  # if we are externally configured, but the project uses old cmake scripts
  # this may not be set
  IF (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
  ENDIF (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Targets built within this project are exported at Install time for use
# by other projects using FindH4H5.
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXPORTED_TARGETS)
  SET (HDF5CHECK_EXPORTED_TARGETS "hdf5check-targets")
ENDIF (NOT HDF5CHECK_EXPORTED_TARGETS)

#-----------------------------------------------------------------------------
# To include a library in the list exported by the project AT BUILD TIME,
# add it to this variable. This is NOT used by Make Install, but for projects
# which include hdf5check as a sub-project within their build tree
#-----------------------------------------------------------------------------
SET_GLOBAL_VARIABLE (HDF5CHECK_LIBRARIES_TO_EXPORT "")

#-----------------------------------------------------------------------------
# Run all the CMake configuration tests for our build environment
#-----------------------------------------------------------------------------
INCLUDE (${HDF5CHECK_RESOURCES_DIR}/ConfigureChecks.cmake)

#-----------------------------------------------------------------------------
# Option to use Shared/Static libs, default is static
#-----------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  SET (LIB_TYPE SHARED)
  SET (H5CHECK_BUILT_AS_DYNAMIC_LIB 1)
ELSE (BUILD_SHARED_LIBS)
  SET (H5CHECK_BUILT_AS_STATIC_LIB 1)
  IF (NOT WIN32)
    # should this be a user setting : Everyone uses it anyway ?
    ADD_DEFINITIONS (-DPIC)
  ENDIF (NOT WIN32)
ENDIF (BUILD_SHARED_LIBS)

#-----------------------------------------------------------------------------
# Option to Build Static executables
#-----------------------------------------------------------------------------
OPTION (BUILD_STATIC_EXECS "Build Static Executabless" OFF)
IF (BUILD_STATIC_EXECS)
  IF (NOT WIN32)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
  ENDIF (NOT WIN32)
ENDIF (BUILD_STATIC_EXECS)

#-----------------------------------------------------------------------------
# When building utility executables that generate other (source) files :
# we make use of the following variables defined in the root CMakeLists.
# Certain systems may add /Debug or /Release to output paths
# and we need to call the executable from inside the CMake configuration
#-----------------------------------------------------------------------------
SET (EXE_EXT "")
IF (WIN32)
  SET (EXE_EXT ".exe")
  IF (NOT CYGWIN)
    ADD_DEFINITIONS (-DBIND_TO_CURRENT_VCLIBS_VERSION=1)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS (-D_CONSOLE)
  ENDIF (NOT CYGWIN)
ENDIF (WIN32)

IF (MSVC)
  SET (CMAKE_MFC_FLAG 0)
ENDIF (MSVC)

SET (MAKE_SYSTEM)
IF (CMAKE_BUILD_TOOL MATCHES "make")
  SET (MAKE_SYSTEM 1)
ENDIF (CMAKE_BUILD_TOOL MATCHES "make")

SET (CFG_INIT "/${CMAKE_CFG_INTDIR}")
IF (MAKE_SYSTEM)
  SET (CFG_INIT "")
ENDIF (MAKE_SYSTEM)

#-----------------------------------------------------------------------------
# Compiler specific flags : Shouldn't there be compiler tests for these
#-----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS} -std=c99 -fomit-frame-pointer -finline-functions -fno-common")
ENDIF (CMAKE_COMPILER_IS_GNUCC)

#-----------------------------------------------------------------------------
# Option to allow the user to disable compiler warnings
#-----------------------------------------------------------------------------
OPTION (HDF_DISABLE_COMPILER_WARNINGS "Disable compiler warnings" OFF)
IF (HDF_DISABLE_COMPILER_WARNINGS)
  # MSVC uses /w to suppress warnings.  It also complains if another
  # warning level is given, so remove it.
  IF (MSVC)
    SET (HDF_WARNINGS_BLOCKED 1)
    STRING (REGEX REPLACE "(^| )([/-])W[0-9]( |$)" " "
        CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /w")
  ENDIF (MSVC)
  IF (WIN32 AND NOT CYGWIN)
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
  ENDIF (WIN32 AND NOT CYGWIN)
  # Borland uses -w- to suppress warnings.
  IF (BORLAND)
    SET (HDF_WARNINGS_BLOCKED 1)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w-")
  ENDIF (BORLAND)

  # Most compilers use -w to suppress warnings.
  IF (NOT HDF_WARNINGS_BLOCKED)
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
  ENDIF (NOT HDF_WARNINGS_BLOCKED)
ENDIF (HDF_DISABLE_COMPILER_WARNINGS)

#-----------------------------------------------------------------------------
# This is in here to help some of the GCC based IDES like Eclipse
# and code blocks parse the compiler errors and warnings better.
#-----------------------------------------------------------------------------
IF (CMAKE_COMPILER_IS_GNUCC)
  SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF (CMAKE_COMPILER_IS_GNUCC)
IF (CMAKE_COMPILER_IS_GNUCXX)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

#-----------------------------------------------------------------------------
# All examples need the main include directories
#-----------------------------------------------------------------------------
INCLUDE_DIRECTORIES (${HDF5CHECK_SRC_DIR} ${HDF5CHECK_BINARY_DIR})

#-----------------------------------------------------------------------------
# Option to Enable MPI Parallel
#-----------------------------------------------------------------------------
SET (CMAKE_MODULE_PATH ${HDF5CHECK_RESOURCES_DIR} ${CMAKE_MODULE_PATH})
OPTION (HDF_ENABLE_PARALLEL "Enable parallel build (requires MPI)" OFF)
IF (HDF_ENABLE_PARALLEL)
  INCLUDE (FindMPI)
  INCLUDE_DIRECTORIES (${MPI_INCLUDE_PATH})
  IF (MPI_FOUND)
    SET (H5_HAVE_PARALLEL 1)
    # MPI checks, only do these if MPI_FOUND is true, otherwise they always fail
    # and once set, they are cached as false and not regenerated
    SET (CMAKE_REQUIRED_LIBRARIES "${MPI_LIBRARY};${MPI_EXTRA_LIBRARY}" )
    CHECK_FUNCTION_EXISTS (MPI_File_get_size H5_HAVE_MPI_GET_SIZE) 
    # Used by Fortran + MPI
    CHECK_SYMBOL_EXISTS (MPI_Comm_c2f "${MPI_INCLUDE_PATH}/mpi.h"  HAVE_MPI_MULTI_LANG_Comm)
    CHECK_SYMBOL_EXISTS (MPI_Info_c2f "${MPI_INCLUDE_PATH}/mpi.h"  HAVE_MPI_MULTI_LANG_Info)
  ENDIF (MPI_FOUND)
ENDIF (HDF_ENABLE_PARALLEL)

# Parallel IO usage requires MPI to be Linked and Included
IF (HAVE_PARALLEL)
  SET (LINK_LIBS ${LINK_LIBS} ${MPI_LIBRARY})
  IF (MPI_LINK_FLAGS)
    SET (CMAKE_EXE_LINKER_FLAGS ${MPI_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  ENDIF (MPI_LINK_FLAGS)
  IF (MPI_EXTRA_LIBRARY)
    SET (LINK_LIBS ${LINK_LIBS} ${MPI_EXTRA_LIBRARY})
  ENDIF (MPI_EXTRA_LIBRARY)
ENDIF (HAVE_PARALLEL)

#-----------------------------------------------------------------------------
# Options for HDF5 Filters
#-----------------------------------------------------------------------------
MACRO (HDF_SETUP_FILTERS FILTER)
  OPTION (HDF_USE_FILTER_${FILTER} "Use the ${FILTER} Filter" ON)
  IF (HDF_USE_FILTER_${FILTER})
    SET (HAVE_FILTER_${FILTER} 1)
  ENDIF (HDF_USE_FILTER_${FILTER})
  # MESSAGE (STATUS "Filter ${FILTER} is ${HDF_USE_FILTER_${FILTER}}")
ENDMACRO (HDF_SETUP_FILTERS)

HDF_SETUP_FILTERS (SHUFFLE)
HDF_SETUP_FILTERS (FLETCHER32)
HDF_SETUP_FILTERS (NBIT)
HDF_SETUP_FILTERS (SCALEOFFSET)

INCLUDE (ExternalProject)
OPTION (HDF_ALLOW_EXTERNAL_SUPPORT "Allow External Library Building" "NO")
OPTION (ZLIB_USE_EXTERNAL "Use External Library Building for ZLIB" 0)
OPTION (SZIP_USE_EXTERNAL "Use External Library Building for SZIP" 0)
OPTION (HDF5_USE_EXTERNAL "Use External Library Building for HDF5" 0)
IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  SET (ZLIB_USE_EXTERNAL 1)
  SET (SZIP_USE_EXTERNAL 1)
  SET (HDF5_USE_EXTERNAL 1)
  IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN")
    SET (ZLIB_URL ${ZLIB_SVN_URL})
    SET (SZIP_URL ${SZIP_SVN_URL})
    SET (HDF5_URL ${HDF5_SVN_URL})
  ELSEIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    IF (NOT TGZ_PATH)
      SET (TGZ_PATH ${HDF5CHECK_SOURCE_DIR})
    ENDIF (NOT TGZ_PATH)
    SET (ZLIB_URL ${TGZ_PATH}/${ZLIB_TGZ_NAME})
    SET (SZIP_URL ${TGZ_PATH}/${SZIP_TGZ_NAME})
    SET (HDF5_URL ${TGZ_PATH}/${HDF5_TGZ_NAME})
  ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN")
ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")

#-----------------------------------------------------------------------------
# Option for ZLib support
#-----------------------------------------------------------------------------
OPTION (HDF_ENABLE_Z_LIB_SUPPORT "Enable Zlib Filters" OFF)
IF (HDF_ENABLE_Z_LIB_SUPPORT)
  IF (NOT ZLIB_HEADER)
    IF (NOT ZLIB_USE_EXTERNAL)
      FIND_PACKAGE (ZLIB)
    ENDIF (NOT ZLIB_USE_EXTERNAL)
    IF (ZLIB_FOUND)
      SET (HAVE_FILTER_DEFLATE 1)
      SET (HAVE_ZLIB_H 1)
      SET (HAVE_LIBZ 1)
      SET (ZLIB_HEADER "zlib.h")
      SET (ZLIB_INCLUDE_DIR_GEN ${ZLIB_INCLUDE_DIR})
    ELSE (ZLIB_FOUND)
      IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        EXTERNAL_ZLIB_LIBRARY (${HDF_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE})
        SET (HAVE_FILTER_DEFLATE 1)
        SET (HAVE_ZLIB_H 1)
        SET (HAVE_LIBZ 1)
        MESSAGE (STATUS "Filter ZLIB is built")
      ELSE (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
        MESSAGE (FATAL " ZLib is Required for ZLib support in HDF5CHECK")
      ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
    ENDIF (ZLIB_FOUND)
  ELSE (NOT ZLIB_HEADER)
    # This project is being called from within another and ZLib is already configured
    SET (HAVE_FILTER_DEFLATE 1)
    SET (HAVE_ZLIB_H 1)
    SET (HAVE_LIBZ 1)
  ENDIF (NOT ZLIB_HEADER)
  SET (LINK_LIBS ${LINK_LIBS} ${ZLIB_LIBRARIES})
  INCLUDE_DIRECTORIES (${ZLIB_INCLUDE_DIRS})
  MESSAGE (STATUS "Filter ZLIB is ON")
ENDIF (HDF_ENABLE_Z_LIB_SUPPORT)

#-----------------------------------------------------------------------------
# Option for SzLib support
#-----------------------------------------------------------------------------
OPTION (HDF_ENABLE_SZIP_SUPPORT "Use SZip Filter" OFF)
IF (HDF_ENABLE_SZIP_SUPPORT)
  OPTION (HDF_ENABLE_SZIP_ENCODING "Use SZip Encoding" OFF)
  IF (NOT SZIP_USE_EXTERNAL)
    FIND_PACKAGE (SZIP)
  ENDIF (NOT SZIP_USE_EXTERNAL)
  IF (SZIP_FOUND)
    SET (HAVE_FILTER_SZIP 1)
    SET (HAVE_SZLIB_H 1)
    SET (HAVE_LIBSZ 1)
    SET (SZIP_INCLUDE_DIR_GEN ${SZIP_INCLUDE_DIR})
  ELSE (SZIP_FOUND)
    IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      EXTERNAL_SZIP_LIBRARY (${HDF_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE} ${HDF_ENABLE_SZIP_ENCODING})
      SET (HAVE_FILTER_SZIP 1)
      SET (HAVE_SZLIB_H 1)
      SET (HAVE_LIBSZ 1)
      MESSAGE (STATUS "Filter SZIP is built")
    ELSE (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      MESSAGE (FATAL_ERROR "SZIP is Required for SZIP support in HDF5CHECK")
    ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  ENDIF (SZIP_FOUND)
  SET (LINK_LIBS ${LINK_LIBS} ${SZIP_LIBRARIES})
  INCLUDE_DIRECTORIES (${SZIP_INCLUDE_DIRS})
  MESSAGE (STATUS "Filter SZIP is ON")
  IF (HDF_ENABLE_SZIP_ENCODING)
    SET (HAVE_SZIP_ENCODER 1)
  ENDIF (HDF_ENABLE_SZIP_ENCODING)
ENDIF (HDF_ENABLE_SZIP_SUPPORT)

#-----------------------------------------------------------------------------
# HDF5 support
#-----------------------------------------------------------------------------
SET (CMAKE_MODULE_PATH ${HDF5CHECK_RESOURCES_DIR} ${CMAKE_MODULE_PATH})

IF (NOT USE_SHARED_LIBS)
  SET (HDF5_USE_STATIC_LIBRARIES 1)
ENDIF (NOT USE_SHARED_LIBS)

IF (NOT HDF5_HEADER)
  IF (NOT HDF5_USE_EXTERNAL)
    FIND_PACKAGE (HDF5)
  ENDIF (NOT HDF5_USE_EXTERNAL)
  IF (HDF5_FOUND)
    SET (HAVE_H5PUBCONF_H 1)
    SET (HAVE_HDF5 1)
    SET (HDF5_HEADER "h5pubconf.h")
    SET (HDF5_INCLUDE_DIR_GEN ${HDF5_INCLUDE_DIR})
    MESSAGE (STATUS "HDF5 found")
  ELSE (HDF5_FOUND)
    IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      EXTERNAL_HDF5_LIBRARY (${HDF_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE})
      SET (HAVE_H5PUBCONF_H 1)
      SET (HAVE_HDF5 1)
      SET (HDF5_HEADER "h5pubconf.h")
      MESSAGE (STATUS "HDF5 is built")
    ELSE (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      MESSAGE (STATUS " HDF5 is Required for HDF5CHECK tests")
    ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  ENDIF (HDF5_FOUND)
ELSE (NOT HDF5_HEADER)
  # This project is being called from within another and HDF5 is already configured
  SET (HAVE_H5PUBCONF_H 1)
  SET (HAVE_HDF5 1)
ENDIF (NOT HDF5_HEADER)
IF (NOT HDF5_LIBRARY)
  SET (HDF5_LIBRARY ${HDF5_LIBRARIES})
ENDIF (NOT HDF5_LIBRARY)

#-----------------------------------------------------------------------------
# Option for external libraries on windows
#-----------------------------------------------------------------------------
OPTION (HDF_PACKAGE_EXTLIBS "CPACK - include external libraries" OFF)
  IF (HDF_PACKAGE_EXTLIBS)
    IF (HDF_ENABLE_Z_LIB_SUPPORT AND ZLIB_FOUND)
      PACKAGE_ZLIB_LIBRARY (${HDF_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE})
    ENDIF (HDF_ENABLE_Z_LIB_SUPPORT AND ZLIB_FOUND)
    
    IF (HDF_ENABLE_SZIP_SUPPORT AND SZIP_FOUND)
      PACKAGE_SZIP_LIBRARY (${HDF_ALLOW_EXTERNAL_SUPPORT} ${LIB_TYPE})
    ENDIF (HDF_ENABLE_SZIP_SUPPORT AND SZIP_FOUND)
  ENDIF (HDF_PACKAGE_EXTLIBS)

#-----------------------------------------------------------------------------
# Option to use threadsafe
# Note: Currently CMake only allows configuring of threadsafe on WINDOWS.
#-----------------------------------------------------------------------------
IF (WIN32 AND NOT CYGWIN)
  OPTION (HDF_ENABLE_THREADSAFE "Enable Threadsafety" OFF)
  IF (HDF_ENABLE_THREADSAFE)
    # check for unsupported options
    IF (HDF_ENABLE_PARALLEL)
      MESSAGE (FATAL " **** Parallel and Threadsafe options are mutually exclusive **** ")
    ENDIF (HDF_ENABLE_PARALLEL)
    SET (HAVE_WIN_THREADS 1)
    IF (HAVE_IOEO)
      MESSAGE (STATUS " **** Windows Threads only available in WINVER>=0x600 (Vista or Windows 7)**** ")
      SET (HAVE_WIN_THREADS 1)
    ENDIF (HAVE_IOEO)
    SET (HAVE_THREADSAFE 1)
  ENDIF (HDF_ENABLE_THREADSAFE)
ENDIF (WIN32 AND NOT CYGWIN)

#-----------------------------------------------------------------------------
# Build Library
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY (${HDF5CHECK_SOURCE_DIR}/src ${PROJECT_BINARY_DIR}/src)

IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  IF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
    ADD_DEPENDENCIES (${HDF5CHECK_LIB_TARGET} ZLIB)
  ENDIF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
  IF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
    ADD_DEPENDENCIES (${HDF5CHECK_LIB_TARGET} SZIP)
  ENDIF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")

#-----------------------------------------------------------------------------
# Build utility to copy and strip X lines of file
#-----------------------------------------------------------------------------
IF (BUILD_TESTING)
  SET (XLATE_UTILITY "xlatefile")
  ADD_EXECUTABLE(${XLATE_UTILITY} ${HDF5CHECK_RESOURCES_DIR}/xlatefile.c)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# Dashboard and Testing Settings
#-----------------------------------------------------------------------------
OPTION (BUILD_TESTING "Build HDF5CHECK Testing" OFF)
IF (BUILD_TESTING AND HDF5_FOUND)
  SET (DART_TESTING_TIMEOUT 1200
      CACHE INTEGER
      "Timeout in seconds for each test (default 1200=20minutes)"
  )
  ENABLE_TESTING ()
  INCLUDE (CTest)
    IF (EXISTS "${HDF5CHECK_SOURCE_DIR}/test" AND IS_DIRECTORY "${HDF5CHECK_SOURCE_DIR}/test")
      ADD_SUBDIRECTORY (${HDF5CHECK_SOURCE_DIR}/test      ${PROJECT_BINARY_DIR}/test)
    ENDIF (EXISTS "${HDF5CHECK_SOURCE_DIR}/test" AND IS_DIRECTORY "${HDF5CHECK_SOURCE_DIR}/test")
  INCLUDE (${HDF5CHECK_SOURCE_DIR}/CTestConfig.cmake)
  CONFIGURE_FILE (${HDF5CHECK_RESOURCES_DIR}/CTestCustom.cmake ${HDF5CHECK_BINARY_DIR}/CTestCustom.ctest @ONLY)
ENDIF (BUILD_TESTING AND HDF5_FOUND)

#-----------------------------------------------------------------------------
# Build tool
#-----------------------------------------------------------------------------
ADD_SUBDIRECTORY (${HDF5CHECK_SOURCE_DIR}/tool ${PROJECT_BINARY_DIR}/tool)

#-----------------------------------------------------------------------------
# Generate the h5chk_config.h file containing user settings needed by compilation
#-----------------------------------------------------------------------------
CONFIGURE_FILE (${HDF5CHECK_RESOURCES_DIR}/h5chk_config.h.in ${HDF5CHECK_BINARY_DIR}/h5chk_config.h @ONLY)

#-----------------------------------------------------------------------------
# Add file(s) to CMake Install
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_INSTALL_NO_DEVELOPMENT)
  INSTALL (
      FILES ${PROJECT_BINARY_DIR}/h5chk_config.h
      DESTINATION ${HDF5CHECK_INSTALL_INCLUDE_DIR}
      COMPONENT headers
  )
ENDIF (NOT HDF5CHECK_INSTALL_NO_DEVELOPMENT)

#-----------------------------------------------------------------------------
# Add Target(s) to CMake Install for import into other projects
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  INSTALL (
      EXPORT ${HDF5CHECK_EXPORTED_TARGETS}
      DESTINATION ${HDF5CHECK_INSTALL_DATA_DIR}/cmake/hdf5check
      FILE hdf5check-targets.cmake
  )
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Export all exported targets to the build tree for use by parent project
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  EXPORT (
      TARGETS  ${HDF5CHECK_LIB_DEPENDENCIES}
      FILE hdf5check-targets.cmake
  )
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf5-configcheck.cmake file for the build directory
#-----------------------------------------------------------------------------
SET (HDF5CHECK_INCLUDES_BUILD_TIME
    ${HDF5CHECK_SRC_DIR} ${HDF5CHECK_TOOL_DIR} ${HDF5CHECK_BINARY_DIR}
)
SET (HDF5CHECK_VERSION_STRING @HDF5CHECK_PACKAGE_VERSION@)
SET (HDF5CHECK_VERSION_MAJOR  @HDF5CHECK_PACKAGE_VERSION_MAJOR@)
SET (HDF5CHECK_VERSION_MINOR  @HDF5CHECK_PACKAGE_VERSION_MINOR@)

CONFIGURE_FILE (
    ${HDF5CHECK_RESOURCES_DIR}/hdf5check-config.cmake.build.in 
    ${HDF5CHECK_BINARY_DIR}/hdf5check-config.cmake @ONLY
)

#-----------------------------------------------------------------------------
# Configure the hdf5-config.cmake file for the install directory
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  CONFIGURE_FILE (
      ${HDF5CHECK_RESOURCES_DIR}/hdf5check-config.cmake.install.in
      ${HDF5CHECK_BINARY_DIR}/CMakeFiles/hdf5check-config.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF5CHECK_BINARY_DIR}/CMakeFiles/hdf5check-config.cmake
      DESTINATION ${HDF5CHECK_INSTALL_DATA_DIR}/cmake/hdf5check
  )
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Configure the hdf5check-config-version .cmake file for the install directory
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  CONFIGURE_FILE (
      ${HDF5CHECK_RESOURCES_DIR}/hdf5check-config-version.cmake.in
      ${HDF5CHECK_BINARY_DIR}/CMakeFiles/hdf5check-config-version.cmake @ONLY
  )
  INSTALL (
      FILES ${HDF5CHECK_BINARY_DIR}/CMakeFiles/hdf5check-config-version.cmake
      DESTINATION ${HDF5CHECK_INSTALL_DATA_DIR}/cmake/hdf5check
  )
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Add Document File(s) to CMake Install
#-----------------------------------------------------------------------------
IF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)
  INSTALL (
      FILES
          ${HDF5CHECK_SOURCE_DIR}/RELEASE.txt
          ${HDF5CHECK_SOURCE_DIR}/COPYING
          ${HDF5CHECK_SOURCE_DIR}/README
      DESTINATION ${HDF5CHECK_INSTALL_DATA_DIR}
      COMPONENT hdfdocuments
  )
  IF (EXISTS "${HDF5CHECK_SOURCE_DIR}/doc" AND IS_DIRECTORY "${HDF5CHECK_SOURCE_DIR}/doc")
    SET (release_files
        ${HDF5CHECK_SOURCE_DIR}/doc/h5check.doc
        ${HDF5CHECK_SOURCE_DIR}/doc/h5check.pdf
        ${HDF5CHECK_SOURCE_DIR}/doc/h5checker_obj.htm
    )
    INSTALL (
        FILES ${release_files}
        DESTINATION ${HDF5CHECK_INSTALL_DATA_DIR}/doc
        COMPONENT hdfdocuments
    )
  ENDIF (EXISTS "${HDF5CHECK_SOURCE_DIR}/doc" AND IS_DIRECTORY "${HDF5CHECK_SOURCE_DIR}/doc")
ENDIF (NOT HDF5CHECK_EXTERNALLY_CONFIGURED)

#-----------------------------------------------------------------------------
# Set the cpack variables
#-----------------------------------------------------------------------------
  SET (CPACK_PACKAGE_VENDOR "HDF Group")
  SET (CPACK_PACKAGE_NAME "${HDF5CHECK_PACKAGE_NAME}")
  SET (CPACK_PACKAGE_INSTALL_DIRECTORY "${HDF5CHECK_PACKAGE_NAME}")
  SET (CPACK_PACKAGE_VERSION "${HDF5CHECK_PACKAGE_VERSION}")
  SET (CPACK_PACKAGE_VERSION_MAJOR "${HDF5CHECK_PACKAGE_VERSION_MAJOR}")
  SET (CPACK_PACKAGE_VERSION_MINOR "${HDF5CHECK_PACKAGE_VERSION_MINOR}")
  SET (CPACK_PACKAGE_VERSION_PATCH "")
  SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/RELEASE.txt")
  SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
  SET (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
  SET (CPACK_PACKAGE_RELOCATABLE TRUE)
  IF (WIN32)
    SET (CPACK_NSIS_CONTACT "help@hdfgroup.org")
    SET (CPACK_NSIS_MODIFY_PATH ON)
    SET (CPACK_NSIS_PACKAGE_NAME "HDF5CHECK ${HDF5CHECK_PACKAGE_VERSION}")
  ELSE (WIN32)
    SET (CPACK_RPM_COMPONENT_INSTALL ON)
  ENDIF (WIN32)
  SET (CPACK_MONOLITHIC_INSTALL ON)
  
  INCLUDE(InstallRequiredSystemLibraries)

  SET (CPACK_INSTALL_CMAKE_PROJECTS "${HDF5CHECK_BINARY_DIR};HDF5CHECK;ALL;/")
  
  IF (HDF_PACKAGE_EXTLIBS)
    IF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
      IF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${ZLIB_INCLUDE_DIR_GEN};ZLIB;ALL;/")
      ENDIF (ZLIB_FOUND AND ZLIB_USE_EXTERNAL)
      IF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${SZIP_INCLUDE_DIR_GEN};SZIP;ALL;/")
      ENDIF (SZIP_FOUND AND SZIP_USE_EXTERNAL)
      IF (HDF5_FOUND AND HDF5_USE_EXTERNAL)
        SET (CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${HDF5_INCLUDE_DIR_GEN};HDF5;ALL;/")
      ENDIF (HDF5_FOUND AND HDF5_USE_EXTERNAL)
    ENDIF (HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "SVN" OR HDF_ALLOW_EXTERNAL_SUPPORT MATCHES "TGZ")
  ENDIF (HDF_PACKAGE_EXTLIBS)
  
  INCLUDE (CPack)

  #---------------------------------------------------------------------------
  # Now list the cpack commands
  #---------------------------------------------------------------------------
  CPACK_ADD_COMPONENT (hdfapplications 
      DISPLAY_NAME "HDF5CHECK Applications" 
      DEPENDS libraries
      GROUP Applications
  )
  CPACK_ADD_COMPONENT (libraries 
      DISPLAY_NAME "HDF5CHECK Libraries"
      GROUP Runtime
  )
  CPACK_ADD_COMPONENT (headers 
      DISPLAY_NAME "HDF5CHECK Headers" 
      DEPENDS libraries
      GROUP Development
  )
  CPACK_ADD_COMPONENT (hdfdocuments 
      DISPLAY_NAME "HDF5CHECK Documents"
      GROUP Documents
  )
