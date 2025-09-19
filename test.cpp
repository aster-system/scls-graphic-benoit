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
    // Exemple
    std::string redaction = std::string();
    scls::Formula test = scls::string_to_formula("sqrt(3x*x + 2x)");
    test.set_redaction(&redaction);
    test *= scls::string_to_formula("sqrt(3x*x + x)");
    test *= scls::string_to_formula("sqrt(9x*x*x*x + 6*x*x*x + 3x*x*x + 2x*x)");
    std::cout << redaction << std::endl << test.to_std_string(0) << std::endl;

    /*// Create a test window
    std::shared_ptr<scls::Window> window = std::make_shared<scls::Window>(800, 800, argv[0]);
    std::shared_ptr<scls::GUI_Page> page = *window.get()->new_page_2d<scls::GUI_Page>("gui");
    page.get()->load_from_xml(std::string("assets/choice.txt"), false); page.get()->after_xml_loading();
    window.get()->display_page_2d("gui");

    // Main loop
    while(window.get()->run()) {
        window.get()->update_event();
        window.get()->update();

        window.get()->render();
    }//*/

    return 0;
}
