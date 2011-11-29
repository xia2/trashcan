foreach file [glob ../../stable/mpsa/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/mpsa/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/tree/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/tree/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/cloud+/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/cloud+/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/pairlist/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/pairlist/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/sf/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/sf/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/sn/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../stable/sn/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/tpp/src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../development/tpp/src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../makefiles/development/*] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

exec rm mpsa_Init.c
exec ln -s ../init/development/mpsa_Init.c .