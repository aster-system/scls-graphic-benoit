//******************
//
// nuclear_energy_usage.cpp
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
// This file contains the source code of nuclear_energy_usage.h.
// The documentations / comments will be made in french for clarity purpose.
//

#include "nuclear_energy_usage.h"

// Utilisation de l'espace de nom "pleos" pour simplifier le code
namespace pleos {

    // Constructeur de base de Nuclear_Energy_Usage
    Nuclear_Energy_Usage::Nuclear_Energy_Usage(int window_width, int window_height, std::string exec_path) : scls::Window(window_width, window_height, exec_path) {

    }

    // Charger toute la fenêtre
    void Nuclear_Energy_Usage::load() {
        load_navigation_page();
        load_welcome_page();
    }

    // Charger la page de navigation
    void Nuclear_Energy_Usage::load_navigation_page() {
        // Page parent de la page de navigation
        scls::HUD_Page* a_navigation_page = new_page<scls::HUD_Page>("navigation");
        a_navigation_page->set_background_color(scls::Color(102, 102, 255));
        a_navigation_page->set_scale(glm::vec2(0.4, 2.0));
        a_navigation_page->move_left_of_parent();
        // Bouton d'accueil de la page de navigation
        a_navigation_welcome_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_welcome_button");
        a_navigation_welcome_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_welcome_button->set_font_size(75);
        a_navigation_welcome_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_welcome_button->set_text("Accueil");
        a_navigation_welcome_button->set_object_scale(0.075);
        a_navigation_welcome_button->move_top_of_parent();
        // Bouton d'explication de la page de navigation
        a_navigation_explaination_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_explaination_button");
        a_navigation_explaination_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_explaination_button->set_font_size(75);
        a_navigation_explaination_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_explaination_button->set_text("Explication");
        a_navigation_explaination_button->set_object_scale(0.075);
        a_navigation_explaination_button->move_bottom_of_object_in_parent(a_navigation_welcome_button, 0.02);
    }

    // Charger la page d'accueil
    void Nuclear_Energy_Usage::load_welcome_page() {

    }

    // Commencer le programme
    void Nuclear_Energy_Usage::start() {
        // Met la partie accueil
        display_page("navigation");

        // Boucle infini de programme
        while(run()) {
            update_event();
            update();



            render();
        }
    }

    // Destructeur de base de Nuclear_Energy_Usage
    Nuclear_Energy_Usage::~Nuclear_Energy_Usage() {

    }

    // Utiliser facilement le projet
    void use_nuclear_energy_usage(std::string exec_path) {
        Nuclear_Energy_Usage *neu = new Nuclear_Energy_Usage(960, 540, exec_path);
        neu->load();
        neu->set_minimum_window_height(540);
        neu->set_minimum_window_width(960);
        neu->start();

        delete neu; neu = 0;
    }
}
