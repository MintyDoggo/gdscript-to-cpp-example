class_name Player
extends RefCounted

const MAX_SPEED := 600.0
const ACCELERATION := 2000.0
const RADIUS := 12.0

var velocity := Vector2.ZERO
var position := Vector2.ZERO
var repeller := Repeller.new()
var visual := MeshInstance2D.new()


func initialize() -> void:
	var mesh := QuadMesh.new()
	mesh.size = Vector2.ONE * RADIUS * 2.0
	var material := ShaderMaterial.new()
	material.shader = preload("res://dot.gdshader")
	visual.mesh = mesh
	visual.material = material
	visual.modulate = Color.CORAL
	visual.position = position

func update_movement(delta: float) -> void:
	velocity = velocity.move_toward(Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down") * MAX_SPEED, ACCELERATION * delta)
	position += velocity * delta
	visual.position = position
	repeller.position = position
