Description :
=============
by simulation, single phase motor indicated as lamp and R supply(ie as single phase supply).
VAR_POWER_POT is adjusted to vary current across VAR_POWER_POT to simulate R supply current. When current across VAR_POWER_POT is within range of >= MIN_R_RMS_CURRENT
and <= MAX_R_RMS_CURRENT, then trigger TRIAC gate ON to start or continue run motor and MOTOR_ON LED is used as an indicator. When current across VAR_POWER_POT is either < MIN_R_RMS_CURRENT, indicated by R_CURRENT_MIN LED or > MAX_R_RMS_CURRENT, indicated by R_CURRENT_MAX LED, then stop the motor by trigger
TRIAC gate OFF, as R supply current is abnormal, which will damage the motor, others machines and circuits connected to R supply.

CAUTION:
========
Schematics and simulation is done by using Proteus CAD. NOT EXPERIMENTED IN REAL TIME ENVIRONMENT.

Purpose :
=========
In all my respective repositories, I just shared my works that I worked as the learning path and practiced, with designed, developed, implemented, simulated and tested, including some projects, assignments, documentations and all other related files and some programming that might not being implement, not being completed, lacks some features or have some bugs. Purpose of all my repositories, if used, can be used for LEARNING AND EDUCATIONAL PURPOSE ONLY. It can be used as the open source and freeware. Kindly read the LICENSE.txt for license, terms and conditions about the use of source codes, binaries, documentation and all other files, located in all my repositories. 

