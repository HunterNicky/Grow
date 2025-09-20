#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "chroma/client/client.h"
#include <sqlite_orm/sqlite_orm.h>
#include <sstream>

#include "chroma/client/core/Application.h"
namespace fs = boost::filesystem;

namespace chroma::client::core {
Application::Application() : window_(800, 600, "Chroma") {};

void Application::run()
{
  fs::path const dir = "data";
  if (!fs::exists(dir)) { fs::create_directory(dir); }

  auto storage = sqlite_orm::make_storage("users.sqlite",
    sqlite_orm::make_table("users",
      sqlite_orm::make_column("id", &User::id, sqlite_orm::primary_key().autoincrement()),
      sqlite_orm::make_column("name", &User::name),
      sqlite_orm::make_column("age", &User::age)));

  storage.sync_schema();

  if (storage.count<User>() == 0) {
    storage.insert(User{ .name = "Alice", .age = 30 });
    storage.insert(User{ .name = "Bob", .age = 25 });
    storage.insert(User{ .name = "Charlie", .age = 35 });
  }

  auto users = storage.get_all<User>();
  window_.Init();
  SetTargetFPS(60);

  while (!Window::ShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("User List:", 20, 20, 20, DARKGRAY);

    int y = 60;
    for (const auto &user : users) {
      std::stringstream string_stream;
      string_stream << user.id << ": " << user.name << " (" << user.age << " years old)";
      DrawText(string_stream.str().c_str(), 40, y, 18, BLACK);
      y += 30;
    }

    if (GuiButton({ 20, 500, 200, 40 }, "Add User") != 0) {
      storage.insert(User{ .name = "New User", .age = 20 });
      users = storage.get_all<User>();
    }

    if (GuiButton({ 240, 500, 200, 40 }, "Remove Last User") != 0) {
      if (!users.empty()) {
        storage.remove<User>(users.back().id);
        users = storage.get_all<User>();
      }
    }

    // toggle fullscreen mode
    if (GuiButton({ 460, 500, 200, 40 }, "Toggle Fullscreen") != 0) { Window::FullScreen(); }

    EndDrawing();
  }

  Window::Close();
}
}// namespace chroma::client::core
