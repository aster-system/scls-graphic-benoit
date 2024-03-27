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
#include "mods/wisim/wisim.h"

int main(int argc, char* argv[]) {
    srand(time(0));

    // Create a simple WiWim window
    wisim::use_wisim(argv[0]);

    return 0;
}
