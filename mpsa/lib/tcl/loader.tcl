proc ::mpsa::load {SimName DirName} {
    set UniqueSimName1042 $SimName
    set UniqueLoadDir1042 $DirName
    source $UniqueLoadDir1042/LoadScript.tcl
}
