#include <chroma/server/server.h>
#include <chroma/shared/shared.h>

namespace chroma::server {
int start()
{
  (void)shared::placeholder();
  return 0;
}
}// namespace chroma::server
