//******************
//
// efa.cpp
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
// This file is the source code of the "EFA" mod, made to test the library.
// It's a simple file analyzer.
//

#include "efa.h"

// Use the "efa" namespace to simplify the using of the library
namespace efa {
    // Simplest EFA constructor
    EFA::EFA(std::string path) {
        // Create the window
        unsigned short height = 900;
        unsigned short width = 900;
        a_window = new scls::Window(width, height, path);
    }

    // Executes a frame in the simulation
    void EFA::frame() {
        window()->update_event();
        window()->update();

        window()->render();
    }

    // EFA destructor
    EFA::~EFA() {
        // Delete the window
        if(a_window != 0) {delete a_window; a_window = 0;}
    }

    // Create a simple EFA window
    void use_efa(std::string path) {
        // Create and configure the window
        EFA* window = new EFA(path);

        // Simulate the window
        while (window->run()) {
            window->frame();
        }

        // Finish the simulation.
        delete window; window = 0;
    }
}
