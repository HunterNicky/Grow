set(RAYGUI_DIR ${THIRD_PARTY_DIR}/raygui)

add_library(raygui INTERFACE)
target_include_directories(raygui
  SYSTEM
  INTERFACE
  ${RAYGUI_DIR}/src)
target_link_libraries(raygui INTERFACE raylib)

