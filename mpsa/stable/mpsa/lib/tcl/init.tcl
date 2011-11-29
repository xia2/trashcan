proc unknown {name args} {
    if {[auto_load $name] == 0} then {
	puts "command $name not recognised"
	return -code error
    } else {
	uplevel $name $args
    }
}