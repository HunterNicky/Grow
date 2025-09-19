find_package(raygui QUIET CONFIG)
if(raygui_FOUND)
  if(TARGET raygui::raygui AND NOT TARGET raygui)
    add_library(raygui INTERFACE IMPORTED)
    target_link_libraries(raygui INTERFACE raygui::raygui)
  endif()
else()
  set(RAYGUI_DIR ${THIRD_PARTY_DIR}/raygui)

  add_library(raygui INTERFACE)
  target_include_directories(raygui
    SYSTEM
    INTERFACE
    ${RAYGUI_DIR}/src)
  target_link_libraries(raygui INTERFACE raylib)
endif()

