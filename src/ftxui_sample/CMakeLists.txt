add_executable(intro main.cpp)

target_link_libraries(intro
    PRIVATE
        chroma::chroma_options
        chroma::chroma_warnings
        raylib
        raygui
        sqlite3
        sqlite_orm
)

target_include_directories(intro
    PRIVATE
        "${CMAKE_BINARY_DIR}/configured_files/include"
)