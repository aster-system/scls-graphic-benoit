//******************
//
// detroit.h
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
// This file is the header of the "Detroit" mod, made to test the library, with a school presentation about the city of Detroit.
//

#ifndef EFA_MOD
#define EFA_MOD

#include "../../scls_graphic.h"

// Use the "efa" namespace to simplify the using of the library
namespace detroit {
    class Detroit {
        // Class representing the main window used in the mod.
    public:
        // Simplest Detroit constructor
        Detroit(std::string path);
        // Detroit destructor
        ~Detroit();

        // Executes a frame in the simulation
        void frame();

        // Load each pages in the object the object
        void load_pages();
        // Set a page in the object
        void set_page(unsigned int page);

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool run() {return window()->run();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline scls::Window* window() {return a_window;};
    private:
        // Window attributes
        // Pointer to the main Window
        scls::Window* a_window = 0;

        // Create and configure the welcome_page page
        scls::HUD_Page* welcome_page = 0;
        scls::HUD_Object* welcome_page_background = 0;
        scls::HUD_Text* welcome_page_start = 0;
        scls::HUD_Object* welcome_page_uk_flag = 0;
        scls::HUD_Object* welcome_page_usa_flag = 0;
        scls::HUD_Text* welcome_page_title = 0;
    };

    // Create a simple Detroit window
    void use_detroit(std::string path);
}

#endif // WISIM
