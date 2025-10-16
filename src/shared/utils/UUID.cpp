#include "chroma/shared/utils/UUID.h"
#include <uuid_v4.h>
#include <random>

namespace chroma::shared::utils {

UUIDv4::UUIDGenerator<std::mt19937_64>& UUID::uuidGenerator() {
	static UUIDv4::UUIDGenerator<std::mt19937_64> generator;
	return generator;
}

} // namespace chroma::shared::utils