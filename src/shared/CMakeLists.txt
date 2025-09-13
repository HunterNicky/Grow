add_library(chroma_shared STATIC
	shared.cpp
)

add_library(chroma::shared ALIAS chroma_shared)

target_include_directories(chroma_shared
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
		"${CMAKE_BINARY_DIR}/configured_files/include"
)

target_link_libraries(chroma_shared
	PUBLIC
		chroma::chroma_options
		chroma::chroma_warnings
)

if(MSVC)
	target_compile_options(chroma_shared PRIVATE /wd4242 /wd4701 /wd4703)
endif()

