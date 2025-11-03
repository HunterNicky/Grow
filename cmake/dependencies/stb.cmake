find_package(stb QUIET CONFIG)
if (stb_FOUND)
  if (TARGET stb::stb AND NOT TARGET stb)
    add_library(stb INTERFACE IMPORTED)

    if (MSVC)
      target_compile_options(stb INTERFACE /w)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      target_compile_options(stb INTERFACE -w)
    endif ()

    target_link_libraries(stb INTERFACE stb::stb)
  endif ()
else ()
  add_library(stb INTERFACE)

  if (MSVC)
    target_compile_options(stb INTERFACE /w)
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(stb INTERFACE -w)
  endif ()

  target_include_directories(stb
    SYSTEM
    INTERFACE
    ${THIRD_PARTY_DIR}/stb
  )
endif ()
