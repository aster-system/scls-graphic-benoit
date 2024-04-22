//******************
//
// scls_documentalist_gui.cpp
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
// This file contains the source code of scls_documentalist_gui.h.
//

#include "scls_documentalist_gui.h"

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace scls_documentalist_gui {
    // SCLS_Documentalist_GUI base constructor
    SCLS_Documentalist_GUI::SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path) : Window(window_width, window_height, exec_path) {

    }

    // SCLS_Documentalist_GUI base destructor
    SCLS_Documentalist_GUI::~SCLS_Documentalist_GUI() {
    }

    // Run the GUI
    void SCLS_Documentalist_GUI::start() {
        while(run()) {
            update_event();
            update();



            render();
        }
    }

    // Use the agatha gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path) {
        SCLS_Documentalist_GUI* window = new SCLS_Documentalist_GUI(900, 900, exec_path);
        window->start();

        delete window; window = 0;
    }
}
