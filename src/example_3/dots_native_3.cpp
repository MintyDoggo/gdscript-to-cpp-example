#include "dots_native_3.h"
#include "dot_interface.h"
#include "dots_interface.h"
#include "repeller_interface.h"

#include <algorithm>
#include <cmath>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>

using namespace godot;

void DotsNative3::_bind_methods() {
	ClassDB::bind_method(D_METHOD("repel_dots_native", "delta", "repeller"), &DotsNative3::repel_dots_native);
}

// Example3: typed and stateless interfaces around accessors
void DotsNative3::repel_dots_native(double delta, Object *repeller) {
	ERR_FAIL_NULL(repeller);

	Variant variant = this;
	Variant repeller_variant = repeller;

	const double friction = DotsInterface::get_friction(variant);
	const double damping = std::exp(-friction * delta);

	const Array dots = DotsInterface::get_dots(variant);
	const Vector2 repeller_position = RepellerInterface::get_position(repeller_variant);
	const double repeller_radius = RepellerInterface::get_radius(repeller_variant);
	const double repeller_strength = RepellerInterface::get_strength(repeller_variant);

	for (Variant dot : dots) {
		ERR_FAIL_COND(dot.get_type() != Variant::OBJECT);

		Vector2 dot_position = DotInterface::get_position(dot);
		const Vector2 offset = dot_position - repeller_position;
		const double strength = repeller_strength * std::max(1.0 - offset.length() / repeller_radius, 0.0);

		Vector2 dot_velocity = DotInterface::get_velocity(dot);
		dot_velocity += offset.normalized() * strength * delta;
		dot_velocity *= damping;

		// clamp velocity
		if (dot_velocity.length_squared() < 10.0) {
			dot_velocity = Vector2();
		}
		dot_position += dot_velocity * delta;

		// update / reflect values in gdscript
		DotInterface::set_velocity(dot, dot_velocity);
		DotInterface::set_position(dot, dot_position);
	}
}
