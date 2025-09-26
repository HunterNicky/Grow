file(GLOB_RECURSE APP_SOURCES *.cpp)

add_executable(intro ${APP_SOURCES})

target_link_libraries(intro
    PRIVATE
        chroma::chroma_options
        chroma::chroma_warnings
        chroma::client
)

target_link_system_libraries(
    intro
    PRIVATE
        Tracy::TracyClient
        raylib
        raygui
        sqlite3
        sqlite_orm
        Boost::filesystem
)

target_include_directories(intro
    PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_BINARY_DIR}/configured_files/include"
)

if(MSVC)
  target_compile_options(intro PRIVATE /wd4242 /wd4701 /wd4703 /wd4996)
endif()
