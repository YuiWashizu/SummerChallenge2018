#!/usr/bin/env zsh
#----------------------------------------------------------------------
# Start Easiroc controller with its stdin connected to this script
# using a pipe.
# ---------------------------------------------------------------------
# Use '>&2 echo ...' to output a text to the screen
# Use 'echo ...' to send a text to easiroc
#----------------------------------------------------------------------

# Number of data points
n=30
# Stage step size (2 um/step)
dx=100

(( i=0 ))

function daq1() {
    stage_pos=$1
    echo "Execute the command to move the stage to ${stage_pos}" 1>&2
    >&2 echo "Send commands to easiroc:"
    echo "Cmd1"
    echo "Cmd2"
    echo "Cmd3"
}

function daq_many() {
    >&2 echo 'Working with HV settings'
    sleep 5
    while [[ $i -lt $n ]]; do
	(( x=20000 + $i * $dx ))
	(( i = $i + 1 ))
	daq1 $x
	echo 'Sleep 3 seconds' 1>&2 
	sleep 3
    done
    echo 'quit'
}

daq_many | ./easiroc_dummy.py

