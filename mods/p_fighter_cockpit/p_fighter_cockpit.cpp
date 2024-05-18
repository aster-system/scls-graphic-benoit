//******************
//
// p_figther_cockpit.cpp
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
// This file contains the source code of p_figther_cockpit.h.
//

#include "p_fighter_cockpit.h"

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace p_fighter_cockpit {
    //*********
    //
    // Main P_Fighter_Cockpit functions
    //
    //*********

    // Most basic P_Fighter_Cockpit constructor
    P_Fighter_Cockpit::P_Fighter_Cockpit(scls::_Window_Advanced_Struct* window, std::string name) : scls::GUI_Page(window, name) {
        set_scale(glm::vec3(2, 2, 1));
        parent_object()->set_background_color(scls::Color(0, 15, 0));
        parent_object()->set_size_in_pixel(glm::vec2(900, 450));

        /*// Create the title of the page
        scls::GUI_Text* title = parent_object()->new_object<scls::GUI_Text>("main_title", 0, 0);
        title->set_font_color(scls::white);
        title->set_font_size(150);
        title->set_position_in_object_size(glm::vec2(0.5, 0.1));
        title->set_size_in_object_size(glm::vec2(1.0, 0.2));
        title->set_text("P-Fighter");
        title->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center);

        // Test
        scls::GUI_Text_Input* test = parent_object()->new_object<scls::GUI_Text_Input>("test", 0, 0);
        test->set_font_color(scls::white);
        test->set_font_size(20);
        test->set_position_in_object_size(glm::vec2(0.5, 0.5));
        test->set_size_in_object_size(glm::vec2(0.9, 0.4));
        test->set_text("P-Fighter");
        test->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center); //*/
    }

    // P_Fighter_Cockpit destructor
    P_Fighter_Cockpit::~P_Fighter_Cockpit() {

    }

    // Use the P-Fighter cockpit mod
    void use_p_fighter_cockpit(std::string exec_path) {
        scls::Window* window = new scls::Window(900, 900, exec_path);

        P_Fighter_Cockpit* page = window->new_page<P_Fighter_Cockpit>("p_fighter_cockpit");
        window->display_page("p_fighter_cockpit");

        while(window->run()) {
            window->update_event();
            window->update();

            window->render();
        }

        delete window; window = 0;
    }
}
