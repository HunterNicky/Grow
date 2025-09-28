#include <filesystem>
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <memory>
#include <raygui.h>
#include <sqlite_orm/sqlite_orm.h>
#include <sstream>
#include <string>
#include <utility>

#include "chroma/app/Application.h"
#include "chroma/app/events/Event.h"
#include "chroma/app/layers/Layer.h"
#include "chroma/client/client.h"

namespace chroma::app {
Application::Application() : delta_time_(0.F), window_(1280, 720, "Chroma") {};

void Application::Run()
{
  std::filesystem::path const dir = "data";
  if (!std::filesystem::exists(dir)) { std::filesystem::create_directory(dir); }

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

  const int virtualWidth = 1280;
  const int virtualHeight = 720;

  const RenderTexture2D target = LoadRenderTexture(virtualWidth, virtualHeight);

  while (!client::core::Window::ShouldClose()) {
    BeginTextureMode(target);
    ClearBackground(RAYWHITE);

    DrawText("User List:", 20, 20, 20, DARKGRAY);

    int y = 60;
    for (const auto &user : users) {
      std::stringstream string_stream;
      string_stream << user.id << ": " << user.name << " (" << user.age << " years old)";
      DrawText(string_stream.str().c_str(), 40, y, 18, BLACK);
      y += 30;
      delta_time_ = GetFrameTime();
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const std::string screenInfo = "Screen: " + std::to_string(screenWidth) + "x" + std::to_string(screenHeight);
    DrawText(screenInfo.c_str(), 20, 450, 20, DARKGRAY);

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

    if (GuiButton({ 460, 500, 200, 40 }, "Toggle Fullscreen") != 0) { window_.FullScreen(); }

    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(target.texture,
      { 0, 0, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) },
      { 0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) },
      { 0, 0 },
      0.0F,
      WHITE);
    EndDrawing();
  }

  UnloadRenderTexture(target);
  client::core::Window::Close();
}

void Application::Initialize() {}

void Application::Shutdown() {}

void Application::PushLayer(std::unique_ptr<layer::Layer> layer) const { layer_stack_->PushLayer(std::move(layer)); }

void Application::PopLayer() const { layer_stack_->PopLayer(); }

void Application::PushOverlay(std::unique_ptr<layer::Layer> overlay) const
{
  layer_stack_->PushOverlay(std::move(overlay));
}

void Application::PopOverlay() const { layer_stack_->PopOverlay(); }

void Application::DispatchEvent(event::Event &event) const { event_dispatcher_->Dispatch(event); }
}// namespace chroma::app
