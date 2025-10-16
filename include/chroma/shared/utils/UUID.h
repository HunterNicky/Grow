#pragma once

#include <uuid_v4.h>

namespace chroma::shared::utils {
class UUID
{
public:
 static UUIDv4::UUID Generate() {static UUIDv4::UUIDGenerator<std::mt19937_64> generator; return generator.getUUID();}
private:
  static UUIDv4::UUIDGenerator<std::mt19937_64>& uuidGenerator();
};

}// namespace chroma::shared::utils
