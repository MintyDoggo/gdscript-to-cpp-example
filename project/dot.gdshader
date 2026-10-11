shader_type canvas_item;
render_mode unshaded;

void fragment() {
	if (distance(UV, vec2(0.5)) > 0.5) {
		discard;
	}
}
