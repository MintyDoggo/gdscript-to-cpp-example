#pragma once

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector2.hpp>

class RepellerInterface final {
public:
	RepellerInterface() = delete;
	static godot::Vector2 get_position(const godot::Variant &variant);
	static double get_radius(const godot::Variant &variant);
	static double get_strength(const godot::Variant &variant);
};
