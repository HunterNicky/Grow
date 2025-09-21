file(GLOB_RECURSE CLIENT_SOURCES *.cpp)

add_library(chroma_client STATIC
  ${CLIENT_SOURCES}
)
add_library(chroma::client ALIAS chroma_client)

target_link_libraries(chroma_client
    PRIVATE
        chroma::chroma_options
        chroma::chroma_warnings
        chroma::server
        chroma::shared
)

include_directories(chroma_client PRIVATE SYSTEM ${Boost_INCLUDE_DIRS})

target_link_system_libraries(
    chroma_client
    PRIVATE
        raylib
        raygui
)

target_include_directories(chroma_client
    PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_BINARY_DIR}/configured_files/include"
)

if(MSVC)
  target_compile_options(chroma_client PRIVATE /wd4242 /wd4701 /wd4703 /wd4996)
endif()
