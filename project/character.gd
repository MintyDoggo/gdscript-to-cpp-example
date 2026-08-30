class_name Character
extends CharacterBody2D


var controller := Controller.new()
var movement := Movement.new()


func _physics_process(delta: float) -> void:
	var direction := controller.get_direction()
	velocity = movement.calculate_velocity(velocity, direction, delta)
	move_and_slide()
