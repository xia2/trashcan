foreach file [glob ../../src/*.c] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

foreach file [glob ../../src/*.h] {
  puts "linking $file"
  set name [file tail $file]
  if ![file exists $name] {exec ln -s $file .}
}

exec ln -s ../../src/Makefile .
