#include "interface_utils.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void InterfaceUtils::report_error(const String &message) {
	// TODO: print a stack trace here so we can see which interface caused this
	ERR_PRINT(message);

#ifdef DEBUG_ENABLED
	Engine *engine = Engine::get_singleton();
	if (!engine) {
		return;
	}

	Object *debugger = engine->get_singleton("EngineDebugger");
	if (!debugger || !debugger->has_method("debug") || !static_cast<bool>(debugger->call("is_active"))) {
		return;
	}

	debugger->call("debug", true, true);
#endif
}

Variant InterfaceUtils::call_packed(
		Variant &variant,
		const StringName &method_name,
		Variant::Type expected_type,
		const Variant **arguments,
		int argument_count) {
	Variant result;
	GDExtensionCallError error{};
	variant.callp(method_name, arguments, argument_count, result, error);

	if (error.error != GDEXTENSION_CALL_OK) {
		report_call_error(method_name, error);

		// FIXME: this just returns the default value of the expected type which is probaly bad
		// we want to fail/panic here anyway so it shouldnt matter
		return UtilityFunctions::type_convert(Variant(), expected_type);
	}

	return validate_call_result(method_name, result, expected_type);
}

void InterfaceUtils::report_call_error(const StringName &method_name, const GDExtensionCallError &error) {
	switch (error.error) {
		case GDEXTENSION_CALL_OK:
			return;
		case GDEXTENSION_CALL_ERROR_INVALID_METHOD:
			report_error("Cannot call method: " + String(method_name) + ", ensure the method exists");
			break;
		case GDEXTENSION_CALL_ERROR_INVALID_ARGUMENT:
			report_error(
					"Method received the wrong type for argument index " + String::num_int64(error.argument) +
					": " + String(method_name) + " expected " +
					Variant::get_type_name(static_cast<Variant::Type>(error.expected)));
			break;
		case GDEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS:
			report_error(
					"Method received too many arguments: " + String(method_name) +
					" expected " + String::num_int64(error.expected));
			break;
		case GDEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS:
			report_error(
					"Method received too few arguments: " + String(method_name) +
					" expected " + String::num_int64(error.expected));
			break;
		case GDEXTENSION_CALL_ERROR_INSTANCE_IS_NULL:
			report_error("Method instance is null: " + String(method_name));
			break;
		case GDEXTENSION_CALL_ERROR_METHOD_NOT_CONST:
			report_error("Cannot call non const method in a const context: " + String(method_name));
			break;
		default:
			report_error("Unknown call error " + String::num_int64(error.error) + ": " + String(method_name));
			break;
	}
}

Variant InterfaceUtils::validate_call_result(const StringName &method_name, const Variant &result, Variant::Type expected_type) {
	if (result.get_type() != expected_type) {
		report_error("Method returned the wrong type `" + String(method_name) + "`");
		return UtilityFunctions::type_convert(Variant(), expected_type);
	}

	return result;
}
