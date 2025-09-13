set(BoostRoot ${THIRD_PARTY_DIR}/boost)

set(BUILD_TESTING OFF CACHE BOOL "Disable Boost tests" FORCE)

add_compile_definitions(
  _CRT_SECURE_NO_WARNINGS
  _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
)

if(WIN32)
  add_compile_options(/EHsc /wd4996 /wd4530 /wd4267 /wd4244)
  add_compile_definitions(
    WINVER=0x0A00
    _WIN32_WINNT=0x0A00
    NTDDI_VERSION=0x0A000000
  )
endif()

add_subdirectory(${THIRD_PARTY_DIR}/boost)
