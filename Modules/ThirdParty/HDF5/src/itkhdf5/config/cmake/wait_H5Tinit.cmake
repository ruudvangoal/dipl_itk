#[[ ITK --start
cmake_minimum_required (VERSION 3.12)
# ITK --stop ]]

message(STATUS "Check for existence of ${HDF5_GENERATED_SOURCE_DIR}/H5Tinit.c")
execute_process(COMMAND ls ${HDF5_GENERATED_SOURCE_DIR}/H5Tinit.c RESULT_VARIABLE H5TI_result OUTPUT_QUIET ERROR_QUIET)
while(H5TI_result)
  ctest_sleep(30)
  message(STATUS "Checking again for existence of ${HDF5_GENERATED_SOURCE_DIR}/H5Tinit.c")
  execute_process(COMMAND ls ${HDF5_GENERATED_SOURCE_DIR}/H5Tinit.c RESULT_VARIABLE H5TI_result OUTPUT_QUIET ERROR_QUIET)
endwhile(H5TI_result)
file (TOUCH "${HDF5_BINARY_DIR}/H5Tinit_created")
message(STATUS "Found ${HDF5_GENERATED_SOURCE_DIR}/H5Tinit.c")
