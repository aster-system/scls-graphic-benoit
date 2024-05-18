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

#include "mods/p_fighter_cockpit/p_fighter_cockpit.h"
// #include "mods/scls_documentalist_gui/scls_documentalist_gui.h"

int main(int argc, char* argv[]) {
    srand(time(0));

    // Create a simple Agatha GUI window
    // scls_documentalist_gui::use_scls_documentalist_gui(argv[0]);

    // Create a labyrinth creator
    p_fighter_cockpit::use_p_fighter_cockpit(argv[0]);

    return 0;
}
