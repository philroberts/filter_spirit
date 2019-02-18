#pragma once
#include "compiler/compiler.hpp"
#include "compiler/error.hpp"
#include "lang/types.hpp"
#include "lang/constants_map.hpp"
#include "lang/condition_set.hpp"
#include "lang/action_set.hpp"
#include "parser/ast.hpp"
#include "parser/parser.hpp"
#include <string_view>
#include <optional>

namespace fs::compiler
{

/**
 * @brief attempt to convert sequence of characters (eg RRGBW) to a group
 * @return empty if string was not valid
 */
[[nodiscard]]
std::optional<lang::group> identifier_to_group(std::string_view identifier);

[[nodiscard]]
std::variant<lang::object, error::error_variant> identifier_to_object(
	const parser::ast::identifier& identifier,
	parser::range_type position_of_identifier,
	const lang::constants_map& map);

[[nodiscard]]
std::variant<lang::object, error::error_variant> expression_to_object(
	const parser::ast::value_expression& value_expression,
	const parser::lookup_data& lookup_data,
	const lang::constants_map& map);

[[nodiscard]]
std::variant<lang::single_object, error::error_variant> construct_single_object_of_type(
	lang::single_object_type wanted_type,
	lang::single_object object,
	parser::range_type object_value_origin,
	parser::range_type object_type_origin);

[[nodiscard]]
std::variant<lang::single_object, error::error_variant> construct_single_object_of_type(
	lang::single_object_type wanted_type,
	lang::object object);

[[nodiscard]]
std::variant<lang::array_object, error::error_variant> construct_array_object_of_type(
	lang::single_object_type inner_array_type,
	lang::object object);

[[nodiscard]] // FIXME: does not set returned object's origins
std::variant<lang::object, error::error_variant> construct_object_of_type(
	lang::object_type wanted_type,
	lang::object&& object);

[[nodiscard]]
std::variant<lang::action_set, error::error_variant> construct_action_set(
	const std::vector<parser::ast::action>& actions,
	const lang::constants_map& map,
	const parser::lookup_data& lookup_data);

[[nodiscard]]
std::variant<lang::condition_set, error::error_variant> construct_condition_set(
	const std::vector<parser::ast::condition>& conditions,
	const lang::constants_map& map,
	const parser::lookup_data& lookup_data);


// FIXME: investigate code duplication and potential missing promotion bugs with:
// - identifier_to_object
// - construct_{single|array|}object_of_type
template <typename T> [[nodiscard]]
std::variant<T, error::error_variant> identifier_to_type(
	const parser::ast::identifier& identifier,
	const lang::constants_map& map,
	const parser::lookup_data& lookup_data)
{
	const auto it = map.find(identifier.value);
	if (it == map.end())
	{
		return error::no_such_name{
			lookup_data.position_of(identifier)
		};
	}

	const lang::object& object = it->second;

	if (std::holds_alternative<lang::array_object>(object.value))
	{
		return error::type_mismatch_in_expression{
			lang::type_to_enum<T>(),
			lang::type_of_object(object),
			object.type_origin
		};
	}

	const auto& single_object = std::get<lang::single_object>(object.value);

	if (!std::holds_alternative<T>(single_object))
	{
		return error::type_mismatch_in_expression{
			lang::type_to_enum<T>(),
			lang::type_of_object(object),
			object.type_origin
		};
	}

	return std::get<T>(single_object);
}

}
