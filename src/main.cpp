#include <raylib.h>
#include <sqlite_orm/sqlite_orm.h>

#include <sstream>

struct User {
  int id;
  std::string name;
  int age;
};

int main() {
  using namespace sqlite_orm;

  auto storage = make_storage(
      "users.sqlite",
      make_table(
          "users", make_column("id", &User::id, primary_key().autoincrement()),
          make_column("name", &User::name), make_column("age", &User::age)));

  storage.sync_schema();

  if (storage.count<User>() == 0) {
    storage.insert(User{.id = 1, .name = "Alice", .age = 30});
    storage.insert(User{.id = 1, .name = "Bob", .age = 25});
    storage.insert(User{.id = 1, .name = "Charlie", .age = 35});
  }

  auto users = storage.get_all<User>();

  InitWindow(800, 600, "raylib");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("User List:", 20, 20, 20, DARKGRAY);

    int y = 60;
    for (auto& user : users) {
      std::stringstream ss;
      ss << user.id << ": " << user.name << " (" << user.age << " years old)";
      DrawText(ss.str().c_str(), 40, y, 18, BLACK);
      y += 30;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
