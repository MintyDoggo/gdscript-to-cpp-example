#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <array>
#include <type_traits>

class InterfaceUtils final {
private:
	static godot::Variant call_packed(
			godot::Variant &variant,
			const godot::StringName &method_name,
			godot::Variant::Type expected_type,
			const godot::Variant **arguments,
			int argument_count);
	static void report_call_error(const godot::StringName &method_name, const GDExtensionCallError &error);
	static godot::Variant validate_call_result(const godot::StringName &method_name, const godot::Variant &result, godot::Variant::Type expected_type);

public:
	InterfaceUtils() = delete;
	static void report_error(const godot::String &message);

	template <typename Return>
	static Return get(const godot::Variant &variant, const godot::StringName &property_name) {
		constexpr godot::Variant::Type expected_type = static_cast<godot::Variant::Type>(godot::GetTypeInfo<Return>::VARIANT_TYPE);
		bool valid = false;
		godot::Variant value = variant.get_named(property_name, valid);
		if (!valid) {
			report_error("Cannot read required property `" + godot::String(property_name) + "`");
			return Return{};
		}
		if (value.get_type() != expected_type) {
			report_error("Required property has the wrong type `" + godot::String(property_name) + "`");
			return Return{};
		}
		return static_cast<Return>(value);
	}

	template <typename ExpectedType>
	static void set(godot::Variant &variant, const godot::StringName &property_name, const ExpectedType &value) {
		constexpr godot::Variant::Type expected_type = static_cast<godot::Variant::Type>(godot::GetTypeInfo<ExpectedType>::VARIANT_TYPE);
		godot::Variant variant_value = value;

		bool valid = false;
		if (variant_value.get_type() != expected_type) {
			report_error("Value has the wrong type for required property `" + godot::String(property_name) + "`");
			return;
		}
		variant.set_named(property_name, variant_value, valid);
		if (!valid) {
			report_error("Cannot write required property `" + godot::String(property_name) + "`");
		}
	}

	template <typename Return = void, typename... Args>
	static Return call(godot::Variant &variant, const godot::StringName &method_name, const Args &...call_arguments) {
		// convert call_arguments to godot variants and create pointers for `callp` method
		std::array<godot::Variant, sizeof...(Args)> arguments{
			godot::Variant(call_arguments)...
		};
		std::array<const godot::Variant *, sizeof...(Args)> argument_pointers;

		// populate the `argument_pointers` array with the values from `arguments`
		for (size_t i = 0; i < arguments.size(); i++) {
			argument_pointers[i] = &arguments[i];
		}

		if constexpr (std::is_void_v<Return>) {
			call_packed(variant, method_name, godot::Variant::NIL, argument_pointers.data(), static_cast<int>(argument_pointers.size()));
			return;
		} else {
			constexpr godot::Variant::Type expected_type = static_cast<godot::Variant::Type>(godot::GetTypeInfo<Return>::VARIANT_TYPE);
			return static_cast<Return>(call_packed(variant, method_name, expected_type, argument_pointers.data(), static_cast<int>(argument_pointers.size())));
		}
	}
};
