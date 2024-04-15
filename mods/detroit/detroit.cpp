//******************
//
// detroit.cpp
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
// This file is the source code of the "Detroit" mod, made to test the library, with a school presentation about the city of Detroit.
//

#include "detroit.h"

// Use the "scls" namespace to simplify the using of the library
namespace detroit {
    // Simplest Detroit constructor
    Detroit::Detroit(std::string path) {
        // Create the window
        unsigned short height = 900;
        unsigned short width = 900;
        a_window = new scls::Window(width, height, path);
    }

    // Executes a frame in the simulation
    void Detroit::frame() {
        window()->update_event();
        window()->update();

        window()->render();
    }

    // Detroit destructor
    Detroit::~Detroit() {
        // Delete the window
        if(a_window != 0) {delete a_window; a_window = 0;}
    }

    // Load each pages in the object the object
    void Detroit::load_pages() {
        // Load the needed textures
        window()->new_texture("logo", "logo_matix.png", false);

        // Create and configure the welcome_page page
        scls::HUD_Page* welcome_page = window()->new_page<scls::HUD_Page>("welcome");
    }

    // Set a page in the object
    void Detroit::set_page(unsigned int page) {
        window()->set_is_resize_possible(false);
        if(page == 0)
        {
            // Set the current page to the welcome page
            window()->set_current_page("welcome");
        }
    }

    // Create a simple Detroit window
    void use_detroit(std::string path) {
        // Create and configure the window
        Detroit* window = new Detroit(path);
        window->load_pages();
        window->set_page(0);

        // Simulate the window
        while (window->run()) {
            window->frame();
        }

        // Finish the simulation.
    }
}
