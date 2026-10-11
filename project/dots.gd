class_name Dots
extends RefCounted

const DOT_COUNT := 20000
const FRICTION := 0.5

var dots: Array[Dot] = []
var visual := MultiMeshInstance2D.new()


func initialize(size: Vector2) -> void:
	for i in DOT_COUNT:
		var dot := Dot.new()
		dot.position = Vector2(randf() * size.x, randf() * size.y)
		dots.append(dot)
	var mesh := QuadMesh.new()
	mesh.size = Vector2(10, 10)
	var material := ShaderMaterial.new()
	material.shader = preload("res://dot.gdshader")
	visual.material = material
	visual.multimesh = MultiMesh.new()
	visual.multimesh.transform_format = MultiMesh.TRANSFORM_2D
	visual.multimesh.mesh = mesh
	visual.multimesh.instance_count = dots.size()
	update_visuals()


# TODO: port to c++
func repel_dots(delta: float, repeller: Repeller) -> void:
	var damping := exp(-FRICTION * delta)
	for dot in dots:
		var offset := dot.position - repeller.position
		var strength := repeller.strength * maxf(1.0 - offset.length() / repeller.radius, 0.0)
		dot.velocity += offset.normalized() * strength * delta
		dot.velocity *= damping

		# clamp velocity
		if dot.velocity.length_squared() < 10.0:
			dot.velocity = Vector2.ZERO
		dot.position += dot.velocity * delta


func update_visuals() -> void:
	for i in dots.size():
		visual.multimesh.set_instance_transform_2d(i, Transform2D(0.0, dots[i].position))
