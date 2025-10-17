include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(chroma_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project

  set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

  include(cmake/dependencies/raylib.cmake)

  include(cmake/dependencies/raygui.cmake)

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
