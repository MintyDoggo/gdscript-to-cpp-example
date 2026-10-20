#include "repeller_interface.h"
#include "interface_utils.h"

using namespace godot;

Vector2 RepellerInterface::get_position(const Variant &variant) {
	static const StringName name("position");
	return InterfaceUtils::get<Vector2>(variant, name);
}

double RepellerInterface::get_radius(const Variant &variant) {
	static const StringName name("radius");
	return InterfaceUtils::get<double>(variant, name);
}

double RepellerInterface::get_strength(const Variant &variant) {
	static const StringName name("strength");
	return InterfaceUtils::get<double>(variant, name);
}
