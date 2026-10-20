#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

class DotsNative3 : public godot::RefCounted {
	GDCLASS(DotsNative3, godot::RefCounted)

protected:
	static void _bind_methods();

public:
	void repel_dots_native(double delta, godot::Object *repeller);
};
