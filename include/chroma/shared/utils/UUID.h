#pragma once

#include <uuid_v4.h>

namespace chroma::shared::utils {
class UUID
{
public:
  static UUIDv4::UUID Generate() { return GetGenerator().getUUID(); }

private:
  static UUIDv4::UUIDGenerator<std::mt19937_64> &GetGenerator()
  {
    static UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator;
    return uuid_generator;
  };
};

}// namespace chroma::shared::utils
