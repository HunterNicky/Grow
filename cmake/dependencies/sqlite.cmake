find_package(SQLite3 QUIET)
if(SQLite3_FOUND AND TARGET SQLite::SQLite3)
  add_library(sqlite3 INTERFACE IMPORTED)
  target_link_libraries(sqlite3 INTERFACE SQLite::SQLite3)
else()
  add_library(sqlite3 ${THIRD_PARTY_DIR}/sqlite/sqlite3.c)
  target_include_directories(sqlite3
    SYSTEM
    PUBLIC
    ${THIRD_PARTY_DIR}/sqlite)
endif()

