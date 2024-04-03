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
// This file is the header of the "EFA" mod, made to test the library.
//

#ifndef EFA_MOD
#define EFA_MOD

#include "../../scls_graphic.h"

// Use the "efa" namespace to simplify the using of the library
namespace efa {
    class EFA {
        // Class representing the main window used in the mod.
    public:
        // Simplest EFA constructor
        EFA(std::string path);
        // EFA destructor
        ~EFA();

        // Executes a frame in the simulation
        void frame();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool run() {return window()->run();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline scls::Window* window() {return a_window;};

        //*********
        //
        // BPS 2D
        //
        //*********
        //
        // BPS 2D is a part of the window, made to test the HUD, with a little cool 2D graphic simualtion.
        //
        //*********

        // Load the BPS 2D
        void load_bps2d();
        // Set the BPS 2D page
        void set_bps2d();
    private:
        // Window attributes
        // Pointer to the main Window
        scls::Window* a_window = 0;
    };

    // Create a simple EFA window
    void use_efa(std::string path);
}

#endif // WISIM
