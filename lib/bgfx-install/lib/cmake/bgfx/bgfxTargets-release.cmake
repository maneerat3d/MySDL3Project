#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "bgfx::bx" for configuration "Release"
set_property(TARGET bgfx::bx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bgfx::bx PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/bx.lib"
  )

list(APPEND _cmake_import_check_targets bgfx::bx )
list(APPEND _cmake_import_check_files_for_bgfx::bx "${_IMPORT_PREFIX}/lib/bx.lib" )

# Import target "bgfx::bimg" for configuration "Release"
set_property(TARGET bgfx::bimg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bgfx::bimg PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/bimg.lib"
  )

list(APPEND _cmake_import_check_targets bgfx::bimg )
list(APPEND _cmake_import_check_files_for_bgfx::bimg "${_IMPORT_PREFIX}/lib/bimg.lib" )

# Import target "bgfx::bgfx" for configuration "Release"
set_property(TARGET bgfx::bgfx APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bgfx::bgfx PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/bgfx.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bgfx.dll"
  )

list(APPEND _cmake_import_check_targets bgfx::bgfx )
list(APPEND _cmake_import_check_files_for_bgfx::bgfx "${_IMPORT_PREFIX}/lib/bgfx.lib" "${_IMPORT_PREFIX}/bin/bgfx.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
