#include "chroma/app/Application.h"
// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
  chroma::app::Application app;
  app.Run();
  app.Shutdown();

  return 0;
}
