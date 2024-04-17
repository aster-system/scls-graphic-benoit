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
        window()->new_texture("welcome_page_background", "assets/textures/detroit/welcome_page_background.png", false);
        window()->new_texture("uk_flag", "assets/textures/detroit/uk_flag.png", false);
        window()->new_texture("usa_flag", "assets/textures/detroit/usa_flag.png", false);

        // Create and configure the welcome_page page
        welcome_page = window()->new_page<scls::HUD_Page>("welcome");
        // Create each objects
        welcome_page_background = welcome_page->new_object<scls::HUD_Object>("welcome_page_background", 0, "welcome_page_background");
        welcome_page_start = welcome_page->new_object<scls::HUD_Text>("welcome_page_start");
        welcome_page_uk_flag = welcome_page->new_object<scls::HUD_Object>("welcome_page_uk_flag", 0, "uk_flag");
        welcome_page_usa_flag = welcome_page->new_object<scls::HUD_Object>("welcome_page_usa_flag", 0, "usa_flag");
        welcome_page_title = welcome_page->new_object<scls::HUD_Text>("welcome_page_title");
        // Configure each objects
        // Configure the back ground
        welcome_page_background->transform()->set_scale(glm::vec3(2, 2, 2));
        // Configure the start button
        welcome_page_start->set_background_color(scls::Color(255, 255, 255, 185));
        welcome_page_start->set_border_width(0.1);
        welcome_page_start->set_font_color(scls::black);
        welcome_page_start->set_font_family("consolab");
        welcome_page_start->set_font_size(100);
        welcome_page_start->set_overflighted_cursor(GLFW_HAND_CURSOR);
        welcome_page_start->set_scale(0.2);
        welcome_page_start->set_text("Start");
        welcome_page_start->set_text_offset(0.2);
        welcome_page_start->transform()->set_position(glm::vec3(0, -0.2, 1.0));
        // Configure the flags
        welcome_page_uk_flag->set_scale(0.35);
        welcome_page_uk_flag->transform()->set_position(glm::vec3(-0.5, 0.25, 1.0));
        welcome_page_usa_flag->set_scale(0.35);
        welcome_page_usa_flag->transform()->set_position(glm::vec3(0.5, 0.25, 1.0));
        // Configure the title
        welcome_page_title->set_background_color(scls::Color(255, 255, 255, 185));
        welcome_page_title->set_border_width(0.1);
        welcome_page_title->set_font_color(scls::black);
        welcome_page_title->set_font_family("consolab");
        welcome_page_title->set_font_size(150);
        welcome_page_title->set_scale(0.3);
        welcome_page_title->set_text("Detroit");
        welcome_page_title->set_text_offset(0.2);
        welcome_page_title->transform()->set_position(glm::vec3(0, 0.65, 1.0));
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
