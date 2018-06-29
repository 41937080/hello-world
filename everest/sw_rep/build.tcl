#!/usr/bin/tclsh

# Before 2015.4 we had to add "-tclargs" to get arguments
# from xsdk into this script, now that string is one of the arguments
# change to always use the last argument as the one we parse to 
# detect what we're building 
if {$argc > 0} {
  set arg [lindex $argv [expr {$argc - 1}]]
} else {
  set arg ""
}

set hdf_path $arg


proc get_processor_name {hw_project_name} {
  set periphs [getperipherals $hw_project_name]
  # For each line of the peripherals table
  foreach line [split $periphs "\n"] {
    set values [regexp -all -inline {\S+} $line]
    # If the last column is "PROCESSOR", then get the "IP INSTANCE" name (1st col)
    if {[lindex $values end] == "PROCESSOR"} {
      return [lindex $values 0]
    }
  }
  return ""
}


# Workspace in root
setws .

# Create a HW-project from a HDF-file
createhw -name app_hw -hwspec $hdf_path
openhw app_hw/system.hdf

# Generate BSP
#createbsp -name app_bsp -hwproject app_hw -proc psu_cortexa53_0 -os standalone -arch 64
#createbsp -name app_bsp -hwproject app_hw -proc [get_processor_name app_hw] -os standalone -arch 64
createbsp -name app_bsp -hwproject app_hw -proc [get_processor_name app_hw] -os freertos901_xilinx -arch 64

# Add extra compiler flag for FREERTOS_BSP
configbsp -bsp app_bsp -append extra_compiler_flags "-DFREERTOS_BSP"

# Add libraries
setlib -bsp app_bsp -lib xilffs

# Remove terminal on UART
#configbsp -bsp app_bsp stdin none
#configbsp -bsp app_bsp stdout none 

# Update BSP and regen
# Changes are stored in RAM until this is done
updatemss -mss app_bsp/system.mss
regenbsp -bsp app_bsp


projects -build

closehw app_hw/system.hdf

# Exit SDK-batch-mode
exit
