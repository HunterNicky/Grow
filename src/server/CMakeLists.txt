file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(chroma_server STATIC
  ${SERVER_SOURCES}
)

add_library(chroma::server ALIAS chroma_server)

target_include_directories(chroma_server
    PUBLIC
      "${PROJECT_SOURCE_DIR}/include"
      "${CMAKE_BINARY_DIR}/configured_files/include"
)

target_link_libraries(chroma_server
  PUBLIC
    chroma::shared
    chroma::chroma_options
    chroma::chroma_warnings
)

if(MSVC)
  target_compile_options(chroma_server PRIVATE /wd4242 /wd4701 /wd4703 /wd4996)
endif()

