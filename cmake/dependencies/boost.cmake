set(BoostRoot ${THIRD_PARTY_DIR}/boost)

set(BUILD_TESTING OFF CACHE BOOL "Disable Boost tests" FORCE)

add_compile_definitions(
	_CRT_SECURE_NO_WARNINGS
	_SILENCE_CXX23_DENORM_DEPRECATION_WARNING
)

add_subdirectory(${THIRD_PARTY_DIR}/boost)
