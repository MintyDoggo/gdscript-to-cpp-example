class_name Movement
extends RefCounted


var speed: float = 450.0
var acceleration: float = 500.0


func calculate_velocity(current_velocity: Vector2, direction: Vector2, delta: float) -> Vector2:
	var target_velocity := direction * speed
	return current_velocity.move_toward(target_velocity, acceleration * delta)
