# CMakeLists.txt
cmake_minimum_required(VERSION 3.30)
project(grow)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT APPLE)
  set(CMAKE_CXX_CLANG_TIDY
      clang-tidy)
endif()

# Find dependencies provided by vcpkg (via vcpkg.cmake)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)

set(
    GROW
    src/main.cpp
)

# main target
add_executable(main ${GROW})
target_link_libraries(main PRIVATE
                                  raylib
                                  glfw
                                  sqlite_orm::sqlite_orm
                                  )
set_property(TARGET main PROPERTY CXX_STANDARD 23)

add_custom_target(
    check-format
    COMMAND clang-format --dry-run ${GROW}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "checking format in src..."
)

add_custom_target(
    format
    COMMAND clang-format -i ${GROW}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "checking format in src..."
)
