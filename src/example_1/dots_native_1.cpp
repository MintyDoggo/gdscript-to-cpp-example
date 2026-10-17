#include "dots_native_1.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include <algorithm>
#include <cmath>

using namespace godot;

void DotsNative1::_bind_methods() {
	ClassDB::bind_method(D_METHOD("repel_dots_native", "delta", "repeller"), &DotsNative1::repel_dots_native);
}

// Example1: naive solution
void DotsNative1::repel_dots_native(double delta, Object *repeller) {
	const double friction = get("FRICTION");
	const double damping = std::exp(-friction * delta);
	const Array dots = get("dots");
	const Vector2 repeller_position = repeller->get("position");
	const double repeller_radius = repeller->get("radius");
	const double repeller_strength = repeller->get("strength");

	for (int64_t i = 0; i < dots.size(); i++) {
		Object *dot = dots[i];
		Vector2 dot_position = dot->get("position");
		const Vector2 offset = dot_position - repeller_position;
		const double strength = repeller_strength * std::max(1.0 - offset.length() / repeller_radius, 0.0);

		Vector2 dot_velocity = dot->get("velocity");
		dot_velocity += offset.normalized() * strength * delta;
		dot_velocity *= damping;

		// clamp velocity
		if (dot_velocity.length_squared() < 10.0) {
			dot_velocity = Vector2();
		}
		dot_position += dot_velocity * delta;

		// update / reflect the values in gdscript
		dot->set("velocity", dot_velocity);
		dot->set("position", dot_position);
	}
}
