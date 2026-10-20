#include "dots_interface.h"
#include "interface_utils.h"

using namespace godot;

double DotsInterface::get_friction(const Variant &variant) {
	static const StringName name("FRICTION");
	return InterfaceUtils::get<double>(variant, name);
}

Array DotsInterface::get_dots(const Variant &variant) {
	static const StringName name("dots");
	return InterfaceUtils::get<Array>(variant, name);
}
