#include "dot_interface.h"
#include "interface_utils.h"

using namespace godot;

Vector2 DotInterface::get_position(const Variant &variant) {
	static const StringName name("position");
	return InterfaceUtils::get<Vector2>(variant, name);
}

void DotInterface::set_position(Variant &variant, const Vector2 &value) {
	static const StringName name("position");
	InterfaceUtils::set(variant, name, value);
}

Vector2 DotInterface::get_velocity(const Variant &variant) {
	static const StringName name("velocity");
	return InterfaceUtils::get<Vector2>(variant, name);
}

void DotInterface::set_velocity(Variant &variant, const Vector2 &value) {
	static const StringName name("velocity");
	InterfaceUtils::set(variant, name, value);
}
