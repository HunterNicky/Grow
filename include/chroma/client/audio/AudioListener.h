#pragma once

#include <raylib.h>

namespace chroma::client::audio {

class AudioListener
{
public:
  AudioListener();
  void SetPosition(Vector2 position);
  [[nodiscard]] Vector2 GetPosition() const;

private:
  Vector2 position_;
};

}// namespace chroma::client::audio
