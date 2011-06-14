cmake_minimum_required (VERSION 2.8)
PROJECT (HDF5CHECK_TEST C CXX)

INCLUDE_DIRECTORIES (${HDF5_INCLUDE_DIRS})

ADD_EXECUTABLE (testgen ${HDF5CHECK_TEST_SOURCE_DIR}/testgen.c ${HDF5CHECK_TEST_SOURCE_DIR}/testgen.h)
TARGET_NAMING (testgen ${LIB_TYPE})
TARGET_LINK_LIBRARIES (testgen ${HDF5CHECK_LIB_TARGET} ${HDF5_LIB_TARGET})

# --------------------------------------------------------------------
# Copy all the HDF5 files from the test directory into the source directory
# --------------------------------------------------------------------
SET (HDF5CHECK_TEST_FILES
    vms_data.h5
    base_addr.h5
    leaf_internal_k.h5
    offsets_lengths.h5
    sb_version.h5
    signature.h5
)

FOREACH (h5_tfile ${HDF5CHECK_TEST_FILES})
  SET (dest "${PROJECT_BINARY_DIR}/${h5_tfile}")
#  MESSAGE (STATUS " Copying ${PROJECT_SOURCE_DIR}/invalidfiles/${h5_tfile} ${dest}")
  ADD_CUSTOM_COMMAND (
      TARGET     ${HDF5CHECK_UTILITY}
      POST_BUILD
      COMMAND    ${CMAKE_COMMAND}
      ARGS       -E copy_if_different ${PROJECT_SOURCE_DIR}/invalidfiles/${h5_tfile} ${dest}
  )
ENDFOREACH (h5_tfile ${HDF5CHECK_TEST_FILES})
 
##############################################################################
##############################################################################
###           T H E   T E S T S  M A C R O S                               ###
##############################################################################
##############################################################################

MACRO (ADD_H5_TEST file expectcode)
  ADD_TEST (NAME H5CHECK-${file} COMMAND "${CMAKE_COMMAND}"
      -D "TEST_PROGRAM=$<TARGET_FILE:${HDF5CHECK_UTILITY}>"
      -D "TEST_ARGS:STRING=${ARGN}"
      -D "TEST_FILE=${file}"
      -D "TEST_EXPECT=${expectcode}"
      -D "TEST_MASK_ERROR=true"
      -D "TEST_OUTPUT=${file}.txt"
      -D "TEST_REFERENCE="
      -D "TEST_FOLDER=${PROJECT_BINARY_DIR}"
      -P "${HDF5CHECK_RESOURCES_DIR}/runTest.cmake"
  )
  IF (NOT "${last_test}" STREQUAL "")
    SET_TESTS_PROPERTIES (H5CHECK-${file} PROPERTIES DEPENDS ${last_test})
  ENDIF (NOT "${last_test}" STREQUAL "")
  SET (last_test "H5CHECK-${file}")
ENDMACRO (ADD_H5_TEST file expectcode)

# Remove any output file left over from previous test run
ADD_TEST (
    NAME h5checktest-clear-objects
    COMMAND    ${CMAKE_COMMAND}
        -E remove 
#        *.bin full*.data
         basic_types.h5
         alternate_sb.h5
         array.h5
         attr.h5
         basic_types.h5
         compound.h5
         cyclical.h5
         enum.h5
         external_empty.h5
         external_full.h5
         filters.h5
         group_dsets.h5
         hierarchical.h5
         linear.h5
         log.h5
         multipath.h5
         rank_dsets_empty.h5
         rank_dsets_full.h5
         refer.h5
         root.h5
         stdio.h5
         time.h5
         vl.h5
         new_grat.h5
         sohm.h5
         ext_dangle1.h5
         ext_dangle2.h5
         ext_self1.h5
         ext_self2.h5
         ext_self3.h5
         ext_mult1.h5
         ext_mult2.h5
         ext_mult3.h5
         ext_mult3.h5
         ext_mult4.h5
         ext_pingpong1.h5
         ext_pingpong2.h5
         ext_pingpong2.h5
         ext_toomany1.h5
         ext_toomany2.h5
         ext_toomany2.h5
         ext_links.h5
)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (h5checktest-clear-objects PROPERTIES DEPENDS ${last_test})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "h5checktest-clear-objects")
    
ADD_TEST (NAME h5checktest-gen-objects COMMAND $<TARGET_FILE:testgen>)
IF (NOT "${last_test}" STREQUAL "")
  SET_TESTS_PROPERTIES (h5checktest-gen-objects PROPERTIES DEPENDS ${last_test})
ENDIF (NOT "${last_test}" STREQUAL "")
SET (last_test "h5checktest-gen-objects")

################################################################
################################################################
###           T H E   T E S T S                              ###
################################################################
################################################################

# Toss in a bunch of tests.  Not sure if they are the right kinds.
# test the help syntax

# ========================================
# The following tests are expected to pass.
# ========================================
ADD_H5_TEST(basic_types.h5 0)
ADD_H5_TEST(alternate_sb.h5 0)
ADD_H5_TEST(array.h5 0)
ADD_H5_TEST(attr.h5 0)
ADD_H5_TEST(compound.h5 0)
ADD_H5_TEST(cyclical.h5 0)
ADD_H5_TEST(enum.h5 0)
ADD_H5_TEST(external_empty.h5 0)
ADD_H5_TEST(external_full.h5 0)
ADD_H5_TEST(filters.h5 0)
ADD_H5_TEST(group_dsets.h5 0)
ADD_H5_TEST(hierarchical.h5 0)
ADD_H5_TEST(linear.h5 0)
ADD_H5_TEST(log.h5 0)
ADD_H5_TEST(multipath.h5 0)
ADD_H5_TEST(rank_dsets_empty.h5 0)
ADD_H5_TEST(rank_dsets_full.h5 0)
ADD_H5_TEST(refer.h5 0)
ADD_H5_TEST(root.h5 0)
ADD_H5_TEST(stdio.h5 0)
ADD_H5_TEST(time.h5 0)
ADD_H5_TEST(vl.h5 0)
# these 2 files are generated only when 1.8 library is used
ADD_H5_TEST(new_grat.h5 0)
ADD_H5_TEST(sohm.h5 0)
# files for external links are generated only when 1.8 library is used
ADD_H5_TEST(ext_dangle1.h5 0 --exte)
ADD_H5_TEST(ext_dangle2.h5 0 -e)
ADD_H5_TEST(ext_self1.h5 0 --exter)
ADD_H5_TEST(ext_self2.h5 0 -e)
ADD_H5_TEST(ext_self3.h5 0 --ex)
ADD_H5_TEST(ext_mult1.h5 0 -e)
ADD_H5_TEST(ext_mult2.h5 0 -e)
ADD_H5_TEST(ext_mult3.h5 0 -e)
ADD_H5_TEST(ext_mult4.h5 0 -e)
ADD_H5_TEST(ext_pingpong1.h5 0 -e)
ADD_H5_TEST(ext_pingpong2.h5 0 -e)
ADD_H5_TEST(ext_toomany1.h5 0 --external)
ADD_H5_TEST(ext_toomany2.h5 0 --external)
ADD_H5_TEST(ext_links.h5 0 -e)

# future tests for non-default VFD files
#ADD_H5_TEST(family00000.h5 0)
#ADD_H5_TEST(family00001.h5 0)
#ADD_H5_TEST(family00002.h5 0)
#ADD_H5_TEST(split-m.h5 0)
#ADD_H5_TEST(split-r.h5 0)
#ADD_H5_TEST(multi-b.h5 0)
#ADD_H5_TEST(multi-g.h5 0)
#ADD_H5_TEST(multi-l.h5 0)
#ADD_H5_TEST(multi-o.h5 0)
#ADD_H5_TEST(multi-r.h5 0)
#ADD_H5_TEST(multi-s.h5 0)

# ========================================
# The following tests are expected to fail.
# ========================================
ADD_H5_TEST(base_addr.h5 2)
# Temporary block out since this file is not really invalid.
#ADD_H5_TEST(leaf_internal_k.h5 2)
ADD_H5_TEST(offsets_lengths.h5 2)
ADD_H5_TEST(sb_version.h5 2)
ADD_H5_TEST(signature.h5 2)
# this is a valid 1.8 file
# this should fail when checked against 1.6 format
ADD_H5_TEST(vms_data.h5 2 --format=16)

