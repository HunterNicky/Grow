#include <string>

namespace chroma::client::core {
class Window
{
public:
  Window(int width, int height, const char *title);
  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  ~Window() = default;

  void static FullScreen();
  void Resize(int width, int height);

  [[nodiscard]] bool static ShouldClose();
  void static Close();
  void Init();

private:
  int width_{ 800 };
  int height_{ 600 };
  std::string title_{ "Chroma" };
};
}// namespace chroma::client::core
