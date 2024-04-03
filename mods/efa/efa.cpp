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

    //*********
    //
    // BPS 2D
    //
    //*********

    // Load the BPS 2D
    void EFA::load_bps2d() {
        // Create and configure the page
        scls::HUD_Page* page = window()->new_page<scls::HUD_Page>("bps2d");

        window()->new_texture("logo", "logo_matix.png");

        scls::Object* object = page->new_object<scls::HUD_Object>("Henry");
    }

    // Set the BPS 2D page
    void EFA::set_bps2d() {
        window()->set_current_page("bps2d");
    }

    // Create a simple EFA window
    void use_efa(std::string path) {
        // Create and configure the window
        EFA* window = new EFA(path);
        window->load_bps2d();
        window->set_bps2d();

        // Simulate the window
        while (window->run()) {
            window->frame();
        }

        // Finish the simulation.
    }
}
