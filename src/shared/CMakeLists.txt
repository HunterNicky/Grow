file(GLOB_RECURSE SHARED_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(chroma_shared STATIC
  ${SHARED_SOURCES}
)
set(INTERFACE_FBS schemas/GameObject.fbs)

flatbuffers_generate_headers(
  TARGET incl
  SCHEMAS ${INTERFACE_FBS}
  FLAGS --scoped-enums --cpp-std c++17
)

target_link_libraries(incl INTERFACE flatbuffers)

add_library(chroma::shared ALIAS chroma_shared)

if(MSVC)
  target_compile_options(chroma_shared PRIVATE /arch:AVX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(chroma_shared PRIVATE -msse4.1)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MinGW")
  target_compile_options(chroma_shared PRIVATE -msse4.1)
endif()

target_include_directories(chroma_shared
  PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/configured_files/include"
)

target_link_system_libraries(chroma_shared
  PRIVATE
    raylib
    incl
    uuid_v4
)
target_link_libraries(chroma_shared
  PUBLIC
    chroma::chroma_options
    chroma::chroma_warnings
)

if(MSVC)
  target_compile_options(chroma_shared PRIVATE /wd4242 /wd4701 /wd4703 /wd4996 /wd4244)
endif()
