add_library(sqlite_orm INTERFACE)
target_include_directories(sqlite_orm
  SYSTEM
  INTERFACE
  ${THIRD_PARTY_DIR}/sqlite_orm/include)

