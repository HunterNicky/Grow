add_library(stb INTERFACE)

target_include_directories(stb
    INTERFACE
    SYSTEM ${THIRD_PARTY_DIR}/stb
)
