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
        window()->set_is_resize_possible(false);
    }

    // Executes a frame in the simulation
    void Detroit::frame() {
        window()->update_event();
        window()->update();

        if(current_presentation_state() == 0) {
            if(welcome_page_start->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_current_presentation_state++;
                set_big_map_page();
            }
        }

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
        window()->new_texture("big_usa_map", "assets/textures/detroit/big_usa_map.png", false);
        window()->new_texture("blue", 5, 5, scls::Color(102, 102, 255));
        window()->new_texture("red", 5, 5, scls::red);

        // Create and configure the welcome_page page
        welcome_page = window()->new_page<scls::HUD_Page>("welcome");
        welcome_page->set_scale(2);
        // Create each objects
        welcome_page_background = welcome_page->new_object<scls::HUD_Object>("welcome_page_background", 0, "welcome_page_background");
        welcome_page_start = welcome_page->new_object<scls::HUD_Text>("welcome_page_start", welcome_page_background);
        welcome_page_uk_flag = welcome_page->new_object<scls::HUD_Object>("welcome_page_uk_flag", welcome_page_background, "uk_flag");
        welcome_page_usa_flag = welcome_page->new_object<scls::HUD_Object>("welcome_page_usa_flag", welcome_page_background, "usa_flag");
        welcome_page_title = welcome_page->new_object<scls::HUD_Text>("welcome_page_title", welcome_page_background);
        // Configure each objects
        // Configure the start button
        welcome_page_start->set_background_color(scls::Color(255, 255, 255, 185));
        welcome_page_start->set_border_width(0.1);
        welcome_page_start->set_font_color(scls::black);
        welcome_page_start->set_font_family("consolab");
        welcome_page_start->set_font_size(100);
        welcome_page_start->set_overflighted_cursor(GLFW_HAND_CURSOR);
        welcome_page_start->set_text("Start");
        welcome_page_start->set_text_offset(0.2);
        welcome_page_start->set_object_scale(0.1);
        welcome_page_start->transform()->set_position(glm::vec3(0, -0.1, 0.5));
        // Configure the flags
        welcome_page_uk_flag->set_object_scale(0.175);
        welcome_page_uk_flag->transform()->set_position(glm::vec3(-0.25, 0.125, 0.5));
        welcome_page_usa_flag->set_object_scale(0.175);
        welcome_page_usa_flag->transform()->set_position(glm::vec3(0.25, 0.125, 0.5));
        // Configure the title
        welcome_page_title->set_background_color(scls::Color(255, 255, 255, 185));
        welcome_page_title->set_border_width(0.1);
        welcome_page_title->set_font_color(scls::black);
        welcome_page_title->set_font_family("consolab");
        welcome_page_title->set_font_size(150);
        welcome_page_title->set_text("Detroit");
        welcome_page_title->set_text_offset(0.2);
        welcome_page_title->set_object_scale(0.15);
        welcome_page_title->transform()->set_position(glm::vec3(0, 0.325, 0.5));

        // Create and configure the navigation page
        scls::HUD_Page* navigation_page = window()->new_page<scls::HUD_Page>("navigation");
        navigation_page->set_position(glm::vec2(-7.0/9.0, 0));
        navigation_page->set_scale(glm::vec2(4.0/9.0, 2.0));
        // Create each objects
        scls::HUD_Object* navigation_page_background = navigation_page->new_object<scls::HUD_Object>("navigation_page_background", 0, "blue");
        scls::HUD_Text* navigation_page_big_map_button = navigation_page->new_object<scls::HUD_Text>("navigation_page_big_map_button", navigation_page_background);
        // Configure each objects
        navigation_page_background->set_scale(1);
        navigation_page_big_map_button->set_font_size(100);
        navigation_page_big_map_button->set_text("USA Map");
        navigation_page_big_map_button->set_object_scale(0.035);

        // Create and configure the big_map page
        scls::HUD_Page* big_map_page = window()->new_page<scls::HUD_Page>("big_map");
        big_map_page->set_position(glm::vec2(2.0/9.0, 2.0/9.0));
        big_map_page->set_scale(2.0 * (7.0 / 9.0));
        // Create each objects
        scls::HUD_Object* big_map_page_background = big_map_page->new_object<scls::HUD_Object>("big_map_page_background", 0, "big_usa_map");
        scls::HUD_Text* big_map_page_detroit_title = big_map_page->new_object<scls::HUD_Text>("big_map_page_detroit_title", big_map_page_background);
        scls::HUD_Object* big_map_page_point = big_map_page->new_object<scls::HUD_Object>("big_map_page_point", big_map_page_background, "red");
        // Configure each objects
        big_map_page_detroit_title->set_font_family("consolab");
        big_map_page_detroit_title->set_font_size(100);
        big_map_page_detroit_title->set_text("Detroit");
        big_map_page_detroit_title->set_object_scale(0.025);
        big_map_page_detroit_title->transform()->set_position(glm::vec3(0.1025, 0.0125, 0));
        big_map_page_point->transform()->set_position(glm::vec3(0.1025, -0.0175, 0));
        big_map_page_point->set_object_scale(0.0125);
    }

    // Set the big map page in the object
    void Detroit::set_big_map_page() {
        // Set the current page to the big map page
        window()->hide_all_pages();
        window()->display_page("big_map");
        window()->display_page("navigation");
    }

    // Set a page in the object
    void Detroit::set_welcome_page() {
        // Set the current page to the welcome page
        window()->hide_all_pages();
        window()->display_page("welcome");
    }

    // Create a simple Detroit window
    void use_detroit(std::string path) {
        // Create and configure the window
        Detroit* window = new Detroit(path);
        window->load_pages();
        window->set_welcome_page();

        // Simulate the window
        while (window->run()) {
            window->frame();
        }

        // Finish the simulation.
    }
}
