find_package(sqlite_orm QUIET CONFIG)
if(sqlite_orm_FOUND)
  if(TARGET sqlite_orm::sqlite_orm AND NOT TARGET sqlite_orm)
    add_library(sqlite_orm INTERFACE IMPORTED)
    target_link_libraries(sqlite_orm INTERFACE sqlite_orm::sqlite_orm)
  endif()
else()
  add_library(sqlite_orm INTERFACE)
  target_include_directories(sqlite_orm
    SYSTEM
    INTERFACE
    ${THIRD_PARTY_DIR}/sqlite_orm/include)
endif()

