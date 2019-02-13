# Circuit Simulator - release notes

## Version 0.4.1 alpha - 7 Dec 2018

### Features:

  *	Added version history notes.
  * Tweaked, improved and implemented various editor routines (e.g. selection-related ones)
  * Press Q/E while creating component to rotate it before placing
  * correctly shows simulation results (current values) for voltage source/resistor
  * cleaning log button
  * option to show voltages for individual nodes in simulation results window
  * dangling nodes (with no outbound links, and non-coupled) left after link deletion are now deleted as well

### Code:

  * Moved actions/slots related to schematic editing to CircuitEditor class, introduced files CircuitEditor_slots.cpp and \_actions.cpp
  * applyComponentStamp() now supports more than two nodes

## Version 0.5 alpha 

### Features:

 DONE
  * some interface tweaks
  * detailed feedback for exprTk parser errors in the log window
  * node voltage values in component properties window showing properly
  * short component names (e.g. R1 for Resistor 1) displayed next to components

TODO:
  * non-linear circuit analysis (one available component - Ideal Diode with Schottky model) 
  
### Code:  

 DONE
  * switched completely to Eigen for matrix manipulations

TODO:

* CRITICAL fix memory leaks (clean up GeometryObjects etc.)