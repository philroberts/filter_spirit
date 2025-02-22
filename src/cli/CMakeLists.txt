find_package(Boost 1.68 REQUIRED
	COMPONENTS
		program_options
		filesystem
)

add_executable(filter_spirit_cli
	parse_args.cpp
	main.cpp
	core.cpp
)

target_include_directories(filter_spirit_cli
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(filter_spirit_cli
	PRIVATE
		cxx_std_17
)

target_compile_options(filter_spirit_cli
	PRIVATE
		$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -ffast-math>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -Wpedantic -ffast-math>
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(filter_spirit_cli
	PRIVATE
		filter_spirit
		Boost::program_options
		Boost::filesystem
)
