find_package(stb QUIET CONFIG)
if(stb_FOUND)
  if(TARGET stb::stb AND NOT TARGET stb)
    add_library(stb INTERFACE IMPORTED)
    target_link_libraries(stb INTERFACE stb::stb)
  endif()
else()
  add_library(stb INTERFACE)

  target_include_directories(stb
      INTERFACE
      SYSTEM ${THIRD_PARTY_DIR}/stb
  )
endif()
