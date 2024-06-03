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

// #include "mods/p_fighter_cockpit/p_fighter_cockpit.h"
#include "mods/scls_documentalist_gui/scls_documentalist_gui.h"

// Main function in the program
int main(int argc, char* argv[]) {
    srand(time(0));

    /*scls::Text_Image_Generator generator = scls::Text_Image_Generator();
    for(int i = 0;i<1000;i++) {
        scls::Image* img = generator.image("<h1>Test</h1>Salut les gens c'est Rebecca je suis un vampire XD LOLOLOLOL<div>Paragraphe paragra para prarparpapraprapap</div>");
        // delete img;
    }

    std::cout << "Bon" << std::endl;
    std::string test = "";
    std::cin >> test; //*/

    // Create a simple Agatha GUI window
    scls_documentalist_gui::use_scls_documentalist_gui(argv[0]);

    // Create a labyrinth creator
    // p_fighter_cockpit::use_p_fighter_cockpit(argv[0]);

    return 0;
}
