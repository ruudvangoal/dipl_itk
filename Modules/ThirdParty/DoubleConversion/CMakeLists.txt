project(ITKDoubleConversion)
set(ITKDoubleConversion_THIRD_PARTY 1)

option(ITK_USE_SYSTEM_DOUBLECONVERSION
  "Use an outside build of double-conversion" OFF)

mark_as_advanced(ITK_USE_SYSTEM_DOUBLECONVERSION)

if(ITK_USE_SYSTEM_DOUBLECONVERSION)
  find_package(double-conversion 3.1.6 REQUIRED)
  get_target_property(ITKDoubleConversion_INCLUDE_DIRS double-conversion::double-conversion INTERFACE_INCLUDE_DIRECTORIES)
  get_target_property(ITKDoubleConversion_LIBRARIES double-conversion::double-conversion LOCATION)
else()
  set(ITKDoubleConversion_INCLUDE_DIRS
    ${ITKDoubleConversion_SOURCE_DIR}/src
    ${ITKDoubleConversion_BINARY_DIR}/src/double-conversion)
  set(ITKDoubleConversion_LIBRARIES itkdouble-conversion)
endif()

itk_module_impl()
