//******************
//
// wisim.cpp
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
// This file is the source code of the "WiSim" mod, made to test the library.
// It's a simple 2D physic simulator.
//

#include "wisim.h"

// Use the "wisim" namespace to simplify the using of the library
namespace wisim {
    // Simplest WiSim constructor
    WiSim::WiSim(std::string path) {
        // Create the window
        unsigned short height = 900;
        unsigned short width = 900;
        a_window = new Window(width, height, path, true);
    }

    // Executes a frame in the simulation
    void WiSim::frame() {
        window()->update_event();
        window()->update();

        window()->render();
    }

    // WiSim destructor
    WiSim::~WiSim() {
        // Delete the window
        if(a_window != 0) {delete a_window; a_window = 0;}
    }

    // Create a simple WiSim window
    void use_wisim(std::string path) {
        // Create and configure the window
        WiSim* window = new WiSim(path);

        // Simulate the window
        while (window->run()) {
            window->frame();
        }

        // Finish the simulation.
        delete window; window = 0;
    }
}
