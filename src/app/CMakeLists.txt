file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(intro ${APP_SOURCES})

target_link_libraries(intro
    PRIVATE
        chroma::chroma_options
        chroma::chroma_warnings
        chroma::client
        chroma::server
)

target_link_system_libraries(
    intro
    PUBLIC
        Tracy::TracyClient
        raylib
        raygui
        sqlite3
        sqlite_orm
        uuid_v4_silenced
        enet::enet_static
)

target_include_directories(intro
    PRIVATE
      "${PROJECT_SOURCE_DIR}/include"
      "${CMAKE_BINARY_DIR}/configured_files/include"
)

if(MSVC)
  target_compile_options(intro PRIVATE /arch:AVX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(intro PRIVATE -mavx -mavx2 -msse4.1)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MinGW")
  target_compile_options(intro PRIVATE -msse4.1)
endif()

if(MSVC)
  target_compile_options(intro PRIVATE /wd4242 /wd4701 /wd4703 /wd4996)
endif()
