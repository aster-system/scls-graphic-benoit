//******************
//
// wisim.h
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
// This file is the header of the "WiSim" mod, made to test the library.
//  It's a simple 2D physic simulator.
//

#ifndef WISIM
#define WISIM

#include "../../scls_graphic.h"

// Use the "wisim" namespace to simplify the using of the library
namespace wisim {
    class WiSim {
        // Class representing the main window used in the mod.
    public:
        // Simplest WiSim constructor
        WiSim(std::string path);
        // WiSim destructor
        ~WiSim();

        // Executes a frame in the simulation
        void frame();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool run() {return window()->run();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline scls::Window* window() {return a_window;};
    private:
        // Window attributes
        // Pointer to the main Window
        scls::Window* a_window = 0;
    };

    // Create a simple WiSim window
    void use_wisim(std::string path);
}

#endif // WISIM
