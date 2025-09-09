include(cmake/CPM.cmake)

# Done as a function so that updates to variables like
# CMAKE_CXX_FLAGS don't propagate out to other
# targets
function(chroma_setup_dependencies)

  # For each dependency, see if it's
  # already been provided to us by a parent project
  
  set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

  if(NOT TARGET raylib)
    include(cmake/dependencies/raylib.cmake)
  endif()

  if(NOT TARGET raygui)
    include(cmake/dependencies/raygui.cmake)
  endif()

  include(cmake/dependencies/sqlite.cmake)

  include(cmake/dependencies/sqlite_orm.cmake)

endfunction()
