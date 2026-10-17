extends Node2D

const USE_NATIVE := true

var player := Player.new()
var dots := Dots.new()


func _ready() -> void:
	var size := get_viewport_rect().size
	player.position = size * 0.5
	dots.initialize(size)
	add_child(dots.visual)
	player.initialize()
	add_child(player.visual)


func _physics_process(delta: float) -> void:
	player.update_movement(delta)
	player.repeller.position = player.position
	if USE_NATIVE:
		dots.repel_dots_native(delta, player.repeller)
	else:
		dots.repel_dots(delta, player.repeller)
	dots.update_visuals()
