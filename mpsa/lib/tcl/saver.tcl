proc ::mpsa::saver {SimName DirName} {
    set fout [open $DirName/LoadScript.tcl w]
    puts $fout "foreach pip {[::mpsa::PipNameWrite]} {"
    puts -nonewline $fout {    if { [::mpsa::PipCheck $pip] == "no" } }
    puts $fout " then {"
    puts $fout {        puts "pip $pip not defined"}
    puts $fout "        return -code error"
    puts $fout "    }"
    puts $fout "}"
    
    foreach particledata [::mpsa::ParticleNameWrite] {
	set name [lindex $particledata 1]
	puts $fout "if { \[::mpsa::ParticleDefnCheck $name\] == \"no\" } then {"
	puts $fout "    ::mpsa::ParticleRegister [::mpsa::ParticleDefnWrite $name]"
	puts $fout "}"
    }

    set Types [::mpsa::ParticleNameWrite]
    set NTypes [llength $Types]

    puts $fout "IDConversionCreate $NTypes"

    foreach particledata $Types {
	puts $fout "IDConversionWrite $particledata"
    }

    puts $fout {::mpsa::SimCreate $UniqueSimName1042}
    puts $fout {set fin [open $UniqueLoadDir1042/PclData r]}
    puts $fout {fconfigure $fin -translation binary}
    puts $fout {::mpsa::ParticleRead $UniqueSimName1042 $fin}
    puts $fout {close $fin}
    puts $fout "IDConversionDelete"

    flush $fout
    close $fout

    set fout [open $DirName/PclData w]
    fconfigure $fout -translation binary
    ::mpsa::ListCreate $SimName UniqueList1042
    ::mpsa::ListAppendBasic UniqueList1042 all
    ::mpsa::ListWrite UniqueList1042 $fout
    close $fout
    ::mpsa::ListClear UniqueList1042
    ::mpsa::ListDelete UniqueList1042
}
