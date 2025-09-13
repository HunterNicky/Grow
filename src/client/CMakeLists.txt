add_executable(intro client.cpp)

target_link_libraries(intro
    PRIVATE
        chroma::chroma_options
        chroma::chroma_warnings
        chroma::server
        chroma::shared
)

target_link_system_libraries(
    intro
    PRIVATE
        raylib
        raygui
        sqlite3
        sqlite_orm
        stb
        Boost::filesystem
)

target_include_directories(intro
    PRIVATE
		"${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_BINARY_DIR}/configured_files/include"
)

if(MSVC)
    target_compile_options(intro PRIVATE /wd4242 /wd4701 /wd4703)
endif()
