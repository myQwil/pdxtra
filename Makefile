# Makefile for myQwil

lib.name = myQwil

class.sources := $(patsubst %, %.c, \
muse chrd rand rind ntof fton sploat gloat slx sly same ceil radix delp \
0x21 0x21~ x is has pak unpak 0x40pak 0x40unpak chrono tabread2~ tabosc2~)

datafiles := $(patsubst %, %-help.pd, \
muse chrd rand rind ntof sploat slope same ceil radix delp \
0x21 0x21~ x is has pak rpak chrono ad adsr cupq cupqb linp linp~)

datafiles += $(patsubst %, %.pd, \
2^ ad adsr add~ all~ bt ct cupq cupqb linp linp~ dollar-slice fkick~ \
hms lead0 mancalc mantissa mantissal mantr phi fmosc~ fmore~ zp zp~)

suppress-wunused = yes
warn.flags = -Wall -Wshadow -Winline -Wstrict-aliasing

include pd-lib-builder/Makefile.pdlibbuilder
