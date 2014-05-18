#! /bin/bash

KP_lateral=6500
KP_z=10000
KI_lateral=40000
KI_z=100000
KD_lateral=40000
KD_z=100000
STARTPOW=140
MINDEPTH=800

./bin/superquad_pid "$KP_lateral" "$KP_z" "$KI_lateral" "$KI_z" "$KD_lateral" "$KD_z" "$STARTPOW" "$MINDEPTH"

exit