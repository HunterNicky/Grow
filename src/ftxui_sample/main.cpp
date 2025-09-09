#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <sstream>

struct User
{
  int id{};
  std::string name;
  int age{};
};

int main()
{
  auto storage = sqlite_orm::make_storage("users.sqlite",
    sqlite_orm::make_table("users",
      sqlite_orm::make_column("id", &User::id, sqlite_orm::primary_key().autoincrement()),
      sqlite_orm::make_column("name", &User::name),
      sqlite_orm::make_column("age", &User::age)));

  storage.sync_schema();

  if (storage.count<User>() == 0) {
    storage.insert(User{ .id = 1, .name = "Alice", .age = 30 });
    storage.insert(User{ .id = 1, .name = "Bob", .age = 25 });
    storage.insert(User{ .id = 1, .name = "Charlie", .age = 35 });
  }

  auto users = storage.get_all<User>();

  InitWindow(800, 600, "raylib");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("User List:", 20, 20, 20, DARKGRAY);

    int y = 60;
    for (auto &user : users) {
      std::stringstream ss;
      ss << user.id << ": " << user.name << " (" << user.age << " years old)";
      DrawText(ss.str().c_str(), 40, y, 18, BLACK);
      y += 30;
    }

    if (GuiButton({ 20, 500, 200, 40 }, "Add User")) {
      int newId = static_cast<int>(storage.count<User>()) + 1;
      storage.insert(User{ .id = newId, .name = "New User", .age = 20 });
      users = storage.get_all<User>();
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
