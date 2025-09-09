add_library(sqlite3 ${THIRD_PARTY_DIR}/sqlite/sqlite3.c)
target_include_directories(sqlite3
  SYSTEM
  PUBLIC
  ${THIRD_PARTY_DIR}/sqlite)

