include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(chroma_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

  find_package(raylib QUIET)
  if (NOT raylib_FOUND)
     CPMAddPackage("gh:raysan5/raylib#5.5")
  endif()
  
  find_package(raygui QUIET CONFIG)
  if (NOT raygui_FOUND)
     CPMAddPackage("gh:raysan5/raygui#4.0")
  if (NOT TARGET raygui)
     add_library(raygui INTERFACE)
     if (MSVC)
         target_compile_options(raygui INTERFACE /w)
     elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
         target_compile_options(raygui INTERFACE -w)
     endif()

     target_include_directories(raygui SYSTEM INTERFACE "${raygui_SOURCE_DIR}/src")
  endif()

  endif()

  include(cmake/dependencies/sqlite.cmake)

  include(cmake/dependencies/sqlite_orm.cmake)

  include(cmake/dependencies/stb.cmake)

  # Me parece que as boost não é necessária, tem outras libs que não são tão pesadas que suprem
  # find_package(Boost CONFIG QUIET COMPONENTS uuid)
  # if(NOT Boost_FOUND OR NOT TARGET Boost::uuid AND NOT TARGET Boost::headers)
  #   include(cmake/CPM.cmake)
  #
  #   set(BOOST_INCLUDE_LIBRARIES "uuid")
  #
  #   CPMAddPackage(
  #     NAME Boost
  #     URL "https://github.com/boostorg/boost/releases/download/boost-1.87.0/boost-1.87.0-cmake.tar.xz"
  #     OPTIONS
  #       "BOOST_ENABLE_CMAKE ON"
  #       "BOOST_INCLUDE_LIBRARIES ${BOOST_INCLUDE_LIBRARIES}"
  #       "BUILD_TESTING OFF"
  #   )
  # endif()

CPMAddPackage(
    NAME uuid_v4
    GITHUB_REPOSITORY crashoz/uuid_v4
    GIT_TAG master
)

  if (NOT TARGET uuid_v4_silenced)
    add_library(uuid_v4_silenced INTERFACE)

    if (MSVC)
        target_compile_options(uuid_v4_silenced INTERFACE /w)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(uuid_v4_silenced INTERFACE -w)
    endif()

    target_link_libraries(uuid_v4_silenced INTERFACE uuid_v4)
    target_include_directories(uuid_v4_silenced SYSTEM INTERFACE
        "${uuid_v4_SOURCE_DIR}"
    )
  endif()


  find_package(Flatbuffers CONFIG QUIET)
  if(Flatbuffers_FOUND)
    if(TARGET flatbuffers::flatbuffers AND NOT TARGET flatbuffers)
      add_library(flatbuffers INTERFACE IMPORTED)
      target_link_libraries(flatbuffers INTERFACE flatbuffers::flatbuffers)
    endif()
  else()
    CPMAddPackage(
      NAME flatbuffers
      GITHUB_REPOSITORY google/flatbuffers
      GIT_TAG v25.9.23
      OPTIONS
        "FLATBUFFERS_BUILD_TESTS OFF"
    )
  endif()

  CPMAddPackage(
    NAME Tracy
    GITHUB_REPOSITORY wolfpld/tracy
    GIT_TAG v0.12.2
    OPTIONS
        "TRACY_ENABLE ON"
  )

  CPMAddPackage(
          NAME Enet
          GITHUB_REPOSITORY zpl-c/enet
          GIT_TAG v2.6.5
          OPTIONS
            "ENET_STATIC ON"
            "ENET_TEST OFF"
            "ENET_SHARED OFF"
  )

endfunction()