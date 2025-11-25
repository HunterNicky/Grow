#pragma once

namespace chroma::app::states {
  enum class StateID {
    None = 0,
    MainMenuState = 1,
    OptionsMenuState = 2,
    GameState = 3,
    NetworkState = 4,
  };
}