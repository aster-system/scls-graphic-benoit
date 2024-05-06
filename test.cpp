//******************
//
// test.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Graphic "Benoit" part allows the user to simply display a graphic window.
// It is named after the Mandelbrot fractal discoverer, Benoit Mandelbrot.
//
// This file contains allows to test the library.
//

#include "scls_graphic.h"
#include "mods/nuclear_energy_usage/nuclear_energy_usage.h"
// #include "mods/scls_documentalist_gui/scls_documentalist_gui.h"

int main(int argc, char* argv[]) {
    srand(time(0));

    // Create a simple Agatha GUI window
    // scls_documentalist_gui::use_scls_documentalist_gui(argv[0]);

    // Create a simple school project
    pleos::use_nuclear_energy_usage(argv[0]);

    return 0;
}
