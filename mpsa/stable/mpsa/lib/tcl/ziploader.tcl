proc ::mpsa::zipload {SimName DirName} {
    set UniqueSimName1042 $SimName
    set UniqueLoadDir1042 $DirName
    exec gunzip $UniqueLoadDir1042/PclData.gz
    source $UniqueLoadDir1042/LoadScript.tcl
    exec gzip $UniqueLoadDir1042/PclData
}
