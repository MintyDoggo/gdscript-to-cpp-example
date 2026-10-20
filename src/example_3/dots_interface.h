#pragma once

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/variant.hpp>

class DotsInterface final {
public:
	DotsInterface() = delete;
	static double get_friction(const godot::Variant &variant);
	static godot::Array get_dots(const godot::Variant &variant);
};
