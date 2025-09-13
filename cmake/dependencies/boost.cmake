set(BoostRoot ${THIRD_PARTY_DIR}/boost)

set(BUILD_TESTING OFF CACHE BOOL "Disable Boost tests" FORCE)

add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS
  _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

add_subdirectory(${THIRD_PARTY_DIR}/boost)
if(MSVC)
  foreach(_cobalt_tgt IN ITEMS boost_cobalt boost_cobalt_io boost_cobalt_io_ssl)
    if (TARGET ${_cobalt_tgt})
      target_compile_definitions(${_cobalt_tgt} PRIVATE _WIN32_WINNT=0x0A00)
    endif()
  endforeach()
endif()