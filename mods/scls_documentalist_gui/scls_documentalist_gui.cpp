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

#ifndef MAIN_HEADER_PIXEL_HEIGHT
#define MAIN_HEADER_PIXEL_HEIGHT 20.0
#endif // MAIN_HEADER_PIXEL_HEIGHT

#ifndef MAIN_HEADER_HEIGHT
#define MAIN_HEADER_HEIGHT (MAIN_HEADER_PIXEL_HEIGHT/static_cast<double>(window_height()))
#endif // MAIN_HEADER_HEIGHT

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace scls_documentalist_gui {
    // SCLS_Documentalist_GUI base constructor
    SCLS_Documentalist_GUI::SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path) : Window(window_width, window_height, exec_path) {

    }

    // SCLS_Documentalist_GUI base destructor
    SCLS_Documentalist_GUI::~SCLS_Documentalist_GUI() {
    }

    // Load the entire gui
    void SCLS_Documentalist_GUI::load() {
        load_help_navigation();
        load_main_header();
        load_welcome_page_footer();
    }

    // Load the help navigation
    void SCLS_Documentalist_GUI::load_help_navigation() {
        // Navigation for the help
        a_help_navigation = new_page<scls::HUD_Page>("help_navigation");
        a_help_navigation->set_position(glm::vec3(-0.75, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_help_navigation->set_scale(glm::vec3(1.0/2.0, 4.0/3.0 - MAIN_HEADER_HEIGHT, 1));
    }

    // Load the main header
    void SCLS_Documentalist_GUI::load_main_header() {
        // Main header for the mains buttons
        double main_header_buttons_x = -1.0;
        a_main_header = new_page<scls::HUD_Page>("header");
        a_main_header->set_position(glm::vec3(0, 1.0 - MAIN_HEADER_HEIGHT, 1));
        a_main_header->set_scale(glm::vec3(1.0, MAIN_HEADER_HEIGHT, 1));
        // Create the file button
        a_main_header_file_button = a_main_header->new_object<scls::HUD_Text>("main_header_file_button");
        a_main_header_file_button->set_font_size(MAIN_HEADER_PIXEL_HEIGHT);
        a_main_header_file_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_main_header_file_button->set_text("File");
        a_main_header_file_button->set_object_scale(1); main_header_buttons_x += a_main_header_file_button->scale()[0];
        a_main_header_file_button->set_position(glm::vec2(main_header_buttons_x, 0));
    }

    // Load the welcome page footer
    void SCLS_Documentalist_GUI::load_welcome_page_footer() {
        // Footer of the welcome page
        a_welcome_footer = new_page<scls::HUD_Page>("welcome_footer");
        a_welcome_footer->set_background_color(scls::red);
        a_welcome_footer->set_position(glm::vec3(0, -2.0/3.0 - MAIN_HEADER_HEIGHT / 4.0, 1));
        a_welcome_footer->set_scale(glm::vec3(2.0, 2.0/3.0 - MAIN_HEADER_HEIGHT / 2.0, 1));
    }

    // Run the GUI
    void SCLS_Documentalist_GUI::start() {
        hide_all_pages();
        display_page("header");
        display_page("help_navigation");
        display_page("welcome_footer");

        while(run()) {
            update_event();
            update();



            render();
        }
    }

    // Use the agatha gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path) {
        SCLS_Documentalist_GUI* window = new SCLS_Documentalist_GUI(900, 900, exec_path);
        window->load();
        window->start();

        delete window; window = 0;
    }
}
