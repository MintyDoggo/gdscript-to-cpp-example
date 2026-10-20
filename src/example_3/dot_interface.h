#pragma once

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/vector2.hpp>

class DotInterface final {
public:
	DotInterface() = delete;
	static godot::Vector2 get_position(const godot::Variant &variant);
	static void set_position(godot::Variant &variant, const godot::Vector2 &value);

	static godot::Vector2 get_velocity(const godot::Variant &variant);
	static void set_velocity(godot::Variant &variant, const godot::Vector2 &value);
};
