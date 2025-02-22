find_package(nlohmann_json 3.0.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.68 REQUIRED COMPONENTS filesystem)

# some source files are shared with tests and GUI/CLI executables
# compile these as shared library so that it can be reused by both
# without the need to list and compile the same sources twice
add_library(filter_spirit SHARED
	fs/parser/parser.cpp
	fs/parser/print_error.cpp
	fs/parser/detail/grammar.cpp
	fs/compiler/build_filter_blocks.cpp
	fs/compiler/resolve_symbols.cpp
	fs/compiler/print_error.cpp
	fs/compiler/detail/add_action.cpp
	fs/compiler/detail/add_conditions.cpp
	fs/compiler/detail/evaluate.cpp
	fs/compiler/detail/queries.cpp
	fs/compiler/detail/determine_types_of.cpp
	fs/generator/generate_filter.cpp
	fs/generator/generator.cpp
	fs/lang/action_set.cpp
	fs/lang/condition_set.cpp
	fs/lang/filter_block.cpp
	fs/lang/object.cpp
	fs/lang/item_price_data.cpp
	fs/lang/item_price_metadata.cpp
	fs/lang/data_source_type.cpp
	fs/lang/item_price_metadata.cpp
	fs/log/buffered_logger.cpp
	fs/log/console_logger.cpp
	fs/log/logger.cpp
	fs/log/utility.cpp
	fs/utility/file.cpp
	fs/utility/dump_json.cpp
	fs/network/http.cpp
	fs/network/url_encode.cpp
	fs/network/poe_watch/download_data.cpp
	fs/network/poe_watch/parse_data.cpp
	fs/network/poe_watch/api_data.cpp
	fs/network/poe_ninja/download_data.cpp
	fs/network/poe_ninja/parse_data.cpp
	fs/network/poe_ninja/api_data.cpp
)

target_include_directories(filter_spirit
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

# Filter Spirit requires C++17
target_compile_features(filter_spirit
	PRIVATE
		cxx_std_17
)

# we add -Wno-deprecated-copy for GCC due to a false positive
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92145
target_compile_options(filter_spirit
	PRIVATE
		$<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Wno-deprecated-copy -ffast-math>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -Wpedantic -ffast-math>
		$<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(filter_spirit
	PRIVATE
		nlohmann_json::nlohmann_json
		OpenSSL::SSL
		Boost::filesystem
)

# Windows requires additional networking libraries for Boost ASIO
if(WIN32)
	target_link_libraries(filter_spirit PRIVATE wsock32 ws2_32)
endif()
