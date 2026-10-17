#include "dots_native_2.h"

#include <algorithm>
#include <cmath>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

void DotsNative2::_bind_methods() {
	ClassDB::bind_method(D_METHOD("repel_dots_native", "delta", "repeller"), &DotsNative2::repel_dots_native);
}

// Example2: additional safety checks and cached named lookups
void DotsNative2::repel_dots_native(double delta, Object *repeller) {
	ERR_FAIL_NULL(repeller);

	static const StringName friction_name("FRICTION");
	static const StringName dots_name("dots");
	static const StringName position_name("position");
	static const StringName radius_name("radius");
	static const StringName strength_name("strength");
	static const StringName velocity_name("velocity");

	Variant self = this;
	Variant repeller_object = repeller;
	bool valid = false;

	const Variant friction_value = self.get_named(friction_name, valid);
	ERR_FAIL_COND(!valid);
	ERR_FAIL_COND(friction_value.get_type() != Variant::FLOAT);
	const double friction = friction_value;

	const double damping = std::exp(-friction * delta);

	const Variant dots_value = self.get_named(dots_name, valid);
	ERR_FAIL_COND(!valid);
	ERR_FAIL_COND(dots_value.get_type() != Variant::ARRAY);
	const Array dots = dots_value;

	const Variant position_value = repeller_object.get_named(position_name, valid);
	ERR_FAIL_COND(!valid);
	ERR_FAIL_COND(position_value.get_type() != Variant::VECTOR2);
	const Vector2 repeller_position = position_value;

	const Variant radius_value = repeller_object.get_named(radius_name, valid);
	ERR_FAIL_COND(!valid);
	ERR_FAIL_COND(radius_value.get_type() != Variant::FLOAT);
	const double repeller_radius = radius_value;

	const Variant strength_value = repeller_object.get_named(strength_name, valid);
	ERR_FAIL_COND(!valid);
	ERR_FAIL_COND(strength_value.get_type() != Variant::FLOAT);
	const double repeller_strength = strength_value;

	for (Variant dot : dots) {
		ERR_FAIL_COND(dot.get_type() != Variant::OBJECT);

		const Variant dot_position_value = dot.get_named(position_name, valid);
		ERR_FAIL_COND(!valid);
		ERR_FAIL_COND(dot_position_value.get_type() != Variant::VECTOR2);
		Vector2 dot_position = dot_position_value;

		const Vector2 offset = dot_position - repeller_position;
		const double strength = repeller_strength * std::max(1.0 - offset.length() / repeller_radius, 0.0);

		const Variant dot_velocity_value = dot.get_named(velocity_name, valid);
		ERR_FAIL_COND(!valid);
		ERR_FAIL_COND(dot_velocity_value.get_type() != Variant::VECTOR2);
		Vector2 dot_velocity = dot_velocity_value;

		dot_velocity += offset.normalized() * strength * delta;
		dot_velocity *= damping;

		// clamp velocity
		if (dot_velocity.length_squared() < 10.0) {
			dot_velocity = Vector2();
		}

		dot_position += dot_velocity * delta;

		// update / reflect values in gdscript
		dot.set_named(velocity_name, dot_velocity, valid);
		ERR_FAIL_COND(!valid);
		dot.set_named(position_name, dot_position, valid);
		ERR_FAIL_COND(!valid);
	}
}
