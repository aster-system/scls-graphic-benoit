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
        // Change la valeur des balises de SCLS Image
        // Modification de la balise <h1>
        text_image_generator()->defined_balise("h1").style.font_size = 100;
        // Création de la balise <sep>
        scls::Text_Style style; style.alignment_horizontal = scls::Alignment_Horizontal::H_Center;
        scls::Text_Balise balise; balise.is_paragraph = true; balise.style = style; balise.style.font_size = 100;
        text_image_generator()->set_defined_balise("sep", balise);

        load_fr();
        load_explaination_page();
        load_navigation_page();
        load_welcome_page();
    }

    // Charger la langue anglaise
    void Nuclear_Energy_Usage::load_en() {
        a_text_content["explaination"] = "Explaination";
        a_text_content["explaination_energy_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/explaination_energy_text_en.html"));
        a_text_content["explaination_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/explaination_text_en.html"));
        a_text_content["politic"] = "Politic";
        a_text_content["the_atoms"] = "Atoms";
        a_text_content["the_energy"] = "Energy";
        a_text_content["welcome"] = "Welcome";
        a_text_content["welcome_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/welcome_text_en.html"));
    }

    // Charger la page d'explication
    void Nuclear_Energy_Usage::load_explaination_page() {
        // Page parent de la page d'explication
        a_explaination_page = new_page<scls::HUD_Page>("explaination");
        a_explaination_page->set_scale(glm::vec2(1.6, 2.0));
        a_explaination_page->move_right_of_parent();
        // Charge le texte d'explication de la page d'explication
        a_explaination_text = a_explaination_page->new_object<scls::HUD_Text>("explaination_text");
        a_explaination_text->set_background_color(scls::white);
        a_explaination_text->set_font_size(50);
        a_explaination_text->set_text(a_text_content["explaination_text"]);
        a_explaination_text->set_object_scale_width(0.9);
        a_explaination_text->set_position(glm::vec2(0));
        // Page parent de la page d'explication de l'énergie
        a_explaination_energy_page = new_page<scls::HUD_Page>("explaination_energy");
        a_explaination_energy_page->set_scale(glm::vec2(1.6, 2.0));
        a_explaination_energy_page->move_right_of_parent();
        // Charge le texte d'explication de la page d'explication de l'énergie
        a_explaination_energy_text = a_explaination_energy_page->new_object<scls::HUD_Text>("explaination_energy_text");
        a_explaination_energy_text->set_background_color(scls::white);
        a_explaination_energy_text->set_font_size(50);
        a_explaination_energy_text->set_text(a_text_content["explaination_energy_text"]);
        a_explaination_energy_text->set_object_scale_width(0.9);
        a_explaination_energy_text->set_position(glm::vec2(0));
    }

    // Charger la langue française
    void Nuclear_Energy_Usage::load_fr() {
        a_text_content["explaination"] = "Explication";
        a_text_content["explaination_energy_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/explaination_energy_text_fr.html"));
        a_text_content["explaination_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/explaination_text_fr.html"));
        a_text_content["politic"] = "Politique";
        a_text_content["the_atoms"] = "Les atomes";
        a_text_content["the_energy"] = scls::to_utf_8("L'énergie");
        a_text_content["welcome"] = "Accueil";
        a_text_content["welcome_text"] = scls::to_utf_8(scls::read_file("assets_nuclear_energy_usage/texts/welcome_text_fr.html"));
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
        a_navigation_welcome_button->set_font_size(100);
        a_navigation_welcome_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_welcome_button->set_text_offset(0.15);
        a_navigation_welcome_button->set_text(a_text_content["welcome"]);
        a_navigation_welcome_button->set_object_scale_width(1.0);
        a_navigation_welcome_button->move_top_of_parent();
        // Bouton d'explication de la page de navigation
        a_navigation_explaination_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_explaination_button");
        a_navigation_explaination_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_explaination_button->set_font_size(100);
        a_navigation_explaination_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_explaination_button->set_text_offset(0.15);
        a_navigation_explaination_button->set_text(a_text_content["explaination"]);
        a_navigation_explaination_button->set_object_scale_width(1.0);
        a_navigation_explaination_button->move_bottom_of_object_in_parent(a_navigation_welcome_button);
        // Bouton d'explication des atomes de la page de navigation
        a_navigation_explaination_atoms_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_explaination_atoms_button");
        a_navigation_explaination_atoms_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_explaination_atoms_button->set_font_size(75);
        a_navigation_explaination_atoms_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_explaination_atoms_button->set_text_offset(0.1);
        a_navigation_explaination_atoms_button->set_text(a_text_content["the_atoms"]);
        a_navigation_explaination_atoms_button->set_object_scale_width(0.75);
        a_navigation_explaination_atoms_button->move_bottom_of_object_in_parent(a_navigation_explaination_button);
        // Bouton d'explication de l'énergie nucléaire de la page de navigation
        a_navigation_explaination_energy_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_explaination_energy_button");
        a_navigation_explaination_energy_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_explaination_energy_button->set_font_size(75);
        a_navigation_explaination_energy_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_explaination_energy_button->set_text_offset(0.2);
        a_navigation_explaination_energy_button->set_text(a_text_content["the_energy"]);
        a_navigation_explaination_energy_button->set_object_scale_width(0.75);
        a_navigation_explaination_energy_button->move_bottom_of_object_in_parent(a_navigation_explaination_atoms_button);
        // Bouton de politique de la page de navigation
        a_navigation_politic_button = a_navigation_page->new_object<scls::HUD_Text>("navigation_politic_button");
        a_navigation_politic_button->set_background_color(scls::Color(102, 102, 255));
        a_navigation_politic_button->set_font_size(100);
        a_navigation_politic_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_navigation_politic_button->set_text_offset(0.15);
        a_navigation_politic_button->set_text(a_text_content["politic"]);
        a_navigation_politic_button->set_object_scale_width(1.0);
        a_navigation_politic_button->move_bottom_of_object_in_parent(a_navigation_explaination_button);
    }

    // Charger la page d'accueil
    void Nuclear_Energy_Usage::load_welcome_page() {
        // Page parent de la page d'accueil
        scls::HUD_Page* a_welcome_page = new_page<scls::HUD_Page>("welcome");
        a_welcome_page->set_scale(glm::vec2(1.6, 2.0));
        a_welcome_page->move_right_of_parent();
        // Créer le texte d'accueil
        scls::HUD_Text* a_welcome_text = a_welcome_page->new_object<scls::HUD_Text>("welcome_text");
        a_welcome_text->set_background_color(scls::white);
        a_welcome_text->set_font_size(50);
        a_welcome_text->set_text(a_text_content["welcome_text"]);
        a_welcome_text->set_object_scale_width(0.9);
        a_welcome_text->set_position(glm::vec2(0));
    }

    // Affiche la page d'explication de l'énergie
    void Nuclear_Energy_Usage::set_explaination_energy_page() {
        a_current_page = "explaination-energy";
        set_navigation_button();
        hide_all_pages();
        display_page("navigation");
        display_page("explaination_energy");
    }

    // Affiche la page d'explication
    void Nuclear_Energy_Usage::set_explaination_page() {
        a_current_page = "explaination-atoms";
        set_navigation_button();
        hide_all_pages();
        display_page("navigation");
        display_page("explaination");
    }

    // Affiche la page d'accueil
    void Nuclear_Energy_Usage::set_welcome_page() {
        a_current_page = "welcome";
        set_navigation_button();
        hide_all_pages();
        display_page("navigation");
        display_page("welcome");
    }

    // Met les boutons à leur endroit actuel
    void Nuclear_Energy_Usage::set_navigation_button() {
        // Cacher les boutons inutiles
        if(a_navigation_explaination_atoms_button != 0) a_navigation_explaination_atoms_button->set_visible(false);
        if(a_navigation_explaination_energy_button != 0) a_navigation_explaination_energy_button->set_visible(false);

        std::vector<std::string> cutted = scls::cut_string(a_current_page, "-");
        if(cutted.size() == 0 || cutted[0] == "welcome") {
            // Ajuster les boutons globaux
            if(a_navigation_welcome_button != 0) {
                a_navigation_welcome_button->move_top_of_parent();
                a_navigation_welcome_button->set_background_color(scls::Color(202, 202, 255));
            }
            if(a_navigation_explaination_button != 0) {
                a_navigation_explaination_button->move_bottom_of_object_in_parent(a_navigation_welcome_button);
                a_navigation_explaination_button->set_background_color(scls::Color(102, 102, 255));
            }
            if(a_navigation_politic_button != 0) {
                a_navigation_politic_button->move_bottom_of_object_in_parent(a_navigation_explaination_button);
                a_navigation_politic_button->set_background_color(scls::Color(102, 102, 255));
            }
        }
        else if(cutted[0] == "explaination") {
            // Ajuster les boutons globaux
            if(a_navigation_welcome_button != 0) {
                a_navigation_welcome_button->move_top_of_parent();
                a_navigation_welcome_button->set_background_color(scls::Color(102, 102, 255));
            }
            if(a_navigation_explaination_button != 0) {
                a_navigation_explaination_button->move_bottom_of_object_in_parent(a_navigation_welcome_button);
                a_navigation_explaination_button->set_background_color(scls::Color(202, 202, 255));
            }
            if(a_navigation_explaination_atoms_button != 0) {
                a_navigation_explaination_atoms_button->move_bottom_of_object_in_parent(a_navigation_explaination_button);
                a_navigation_explaination_atoms_button->set_visible(true);
                if(cutted.size() > 1 && cutted[1] == "atoms") a_navigation_explaination_atoms_button->set_background_color(scls::Color(202, 202, 255));
                else a_navigation_explaination_atoms_button->set_background_color(scls::Color(102, 102, 255));
            }
            if(a_navigation_explaination_energy_button != 0) {
                a_navigation_explaination_energy_button->move_bottom_of_object_in_parent(a_navigation_explaination_atoms_button);
                a_navigation_explaination_energy_button->set_visible(true);
                if(cutted.size() > 1 && cutted[1] == "energy") a_navigation_explaination_energy_button->set_background_color(scls::Color(202, 202, 255));
                else a_navigation_explaination_energy_button->set_background_color(scls::Color(102, 102, 255));
            }
            if(a_navigation_politic_button != 0) {
                a_navigation_politic_button->move_bottom_of_object_in_parent(a_navigation_explaination_energy_button);
                a_navigation_politic_button->set_background_color(scls::Color(102, 102, 255));
            }
        }
    }

    // Commencer le programme
    void Nuclear_Energy_Usage::start() {
        // Affiche la page d'accueil
        set_welcome_page();

        // Boucle infini de programme
        while(run()) {
            update_event();
            update();

            // Changer de page grâce à la barre de navigation
            if(a_navigation_explaination_energy_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_explaination_energy_page();
            }
            if(a_navigation_explaination_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT) || a_navigation_explaination_atoms_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_explaination_page();
            }
            if(a_navigation_welcome_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_welcome_page();
            }

            render();
        }
    }

    // Destructeur de base de Nuclear_Energy_Usage
    Nuclear_Energy_Usage::~Nuclear_Energy_Usage() {

    }

    // Utiliser facilement le projet
    void use_nuclear_energy_usage(std::string exec_path) {
        Nuclear_Energy_Usage *neu = new Nuclear_Energy_Usage(960, 540, exec_path);
        neu->set_minimum_window_height(540);
        neu->set_minimum_window_width(960);
        neu->load();
        neu->start();

        delete neu; neu = 0;
    }
}
