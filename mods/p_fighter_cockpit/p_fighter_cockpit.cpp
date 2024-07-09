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

// The namespace "p_fighter_cockpit" is used to simplify the all.
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
        parent_object()->set_position_in_pixel(0, 0);
        parent_object()->set_height_in_scale(scls::Fraction(1));
        parent_object()->set_width_in_scale(scls::Fraction(1));

        // Create the title of the page
        scls::GUI_Text* title = (*parent_object()->new_object<scls::GUI_Text>("main_title", 0, 720)).get();
        title->set_border_color(scls::red);
        title->set_border_width_in_pixel(1);
        title->set_font_color(scls::white);
        title->set_font_size(150);
        title->set_height_in_scale(scls::Fraction(1, 5));
        title->set_width_in_scale(scls::Fraction(4, 5));
        title->set_width_in_scale(1);
        title->set_text("P-Fighter");
        title->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center);
        title->set_x_in_scale(scls::Fraction(0));
        title->set_y_in_scale(scls::Fraction(7, 10));
    }

    // P_Fighter_Cockpit destructor
    P_Fighter_Cockpit::~P_Fighter_Cockpit() {

    }

    // Use the P-Fighter cockpit mod
    void use_p_fighter_cockpit(std::string exec_path) {
        scls::Window* window = new scls::Window(900, 900, exec_path);

        std::shared_ptr<P_Fighter_Cockpit> page = *window->new_page_2d<P_Fighter_Cockpit>("p_fighter_cockpit");
        window->display_page_2d("p_fighter_cockpit");

        while(window->run()) {
            window->update_event();
            window->update();

            window->render();
        }

        delete window; window = 0;
    }
}
