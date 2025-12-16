#pragma once

namespace chroma::client::ui::panel {
  enum class PanelID {
    None = 0,
    MainMenuPanel,
    OptionsMenuPanel,
    AudioOptionsPanel,
    VideoOptionsPanel,
    LevelSelectPanel,
    MainBackgroundPanel,
    GameHUDPanel,
    PausePanel,
    PauseBackgroundPanel,
    DebugShaderPanel,
    BorderPassDebugPanel,
  };
}