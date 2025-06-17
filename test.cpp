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

SCLS_INIT

// Main function in the program
int main(int argc, char* argv[]) {
    // Create a test window
    std::shared_ptr<scls::Window> window = std::make_shared<scls::Window>(800, 800, argv[0]);
    std::shared_ptr<scls::GUI_Page> page = *window.get()->new_page_2d<scls::GUI_Page>("gui");
    page.get()->load_from_xml(std::string("assets/choice.txt"), false); page.get()->after_xml_loading();
    window.get()->display_page_2d("gui");

    // Main loop
    while(window.get()->run()) {
        window.get()->update_event();
        window.get()->update();

        window.get()->render();
    }

    return 0;
}
