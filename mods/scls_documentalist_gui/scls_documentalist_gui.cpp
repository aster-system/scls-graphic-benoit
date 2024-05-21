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

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace scls_documentalist_gui {
    // SCLS_Documentalist_GUI base constructor
    SCLS_Documentalist_GUI::SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path) : Window(window_width, window_height, exec_path) {
        set_background_color(scls::Color(200, 200, 200));
    }

    // SCLS_Documentalist_GUI base destructor
    SCLS_Documentalist_GUI::~SCLS_Documentalist_GUI() {

    }

    //*********
    //
    // Annoying GUI stuff
    //
    //*********

    // Display the help page
    void SCLS_Documentalist_GUI::display_help() {

    }

    // Load the entire gui
    void SCLS_Documentalist_GUI::load() {
        load_language_fr();

        // Page of the GUI
        a_page = new_page<scls::GUI_Page>("scls_documentalist_agatha");
        a_page->set_scale(glm::vec3(2, 2, 1));
        a_page->parent_object()->set_background_color(scls::Color(255, 255, 255));
        a_page->parent_object()->set_position_in_pixel(glm::vec2(0, 0));
        a_page->parent_object()->set_size_in_scale(glm::vec2(1, 1));
        display_page("scls_documentalist_agatha");

        load_main_header();

        // Load navigations
        // load_help_navigation();

        // Load footers
        // load_welcome_footer();

        // Load bodies
        load_help_body();

        set_window_title("SCLS Documentalist \"Agatha\"");
    }

    // Load the welcome page body
    void SCLS_Documentalist_GUI::load_help_body() {
        // Body of the welcome page
        a_help_body = parent_object()->new_object<scls::GUI_Object>("help_body");
        a_help_body->set_background_color(scls::white);
        a_help_body->set_size_in_scale(glm::vec2(0.8, 0.65));
        a_help_body->set_border_width_in_pixel(1);
        a_help_body->move_bottom_of_object_in_parent(a_main_header);
        a_help_body->move_right_of_parent();
        // Create the logo of the help page
        // scls::GUI_Object* help_body_logo = a_help_body->new_object<scls::GUI_Object>("help_body_logo");
        // help_body_logo->set_size_in_scale(glm::vec2(1.0, 0.5));
        // help_body_logo->set_position_in_scale(glm::vec2(0, 0.19));
        // Create the home text of the help page
        a_help_body_home = a_help_body->new_object<scls::GUI_Text>("help_body_welcome");
        a_help_body_home->set_border_color(scls::red);
        a_help_body_home->set_font_size(20);
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_help_body_home->set_size_in_scale(glm::vec2(0.75, 1.0));
        a_help_body_home->set_position_in_scale(glm::vec2(0, -0.15));
    }

    // Load the help navigation
    void SCLS_Documentalist_GUI::load_help_navigation() {
        // Navigation for the help
        a_help_navigation = parent_object()->new_object<scls::GUI_Object>("help_navigation");
        a_help_navigation->set_background_color(scls::red);
        a_help_navigation->set_size_in_scale(glm::vec2(0.25, 0.6));
        a_help_navigation->set_border_width_in_pixel(1);
        a_help_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_help_navigation->move_left_of_parent();
        // Create the home button of the navigation
        a_help_navigation_home_button = a_help_navigation->new_object<scls::GUI_Text>("help_navigation_home_button");
        a_help_navigation_home_button->set_font_size(100);
        a_help_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_help_navigation_home_button->set_size_in_scale(glm::vec2(0.3, 1.0));
        a_help_navigation_home_button->move_top_of_parent();
    }

    // Load english
    void SCLS_Documentalist_GUI::load_language_en() {
        a_hud_text_content["create_a_file_pattern"] = scls::to_utf_8("Create a pattern file");
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Create a project");
        a_hud_text_content["final_path"] = "Final path";
        a_hud_text_content["help_body_home"] = "Welcome to SCLS Documentalist \"Agatha\" GUI";
        std::string help_body_welcome_description = "This software, made by Aster System, provides a simply use to SCLS Documentalist \"Agatha\", whitout having to</br>";
        help_body_welcome_description += "handle a lot of lines of code. You can use it, as as a developer who wants to easily documentate its code, or as</br>";
        help_body_welcome_description += "anybody else. The software is published under the terms of the license GPL V3.0.";
        a_hud_text_content["help_body_home_description"] = scls::to_utf_8(help_body_welcome_description);
        a_hud_text_content["help_body_home_part"] = scls::to_utf_8("You currently are in the help page. Browse with the</br>selector at your left.");
        a_hud_text_content["home"] = "Home";
        a_hud_text_content["home_project"] = "Project home";
        a_hud_text_content["name_file_pattern"] = "Name of the file pattern";
        a_hud_text_content["name_project"] = "Name of the project";
        a_hud_text_content["save"] = "Save";
        a_hud_text_content["save_all"] = "Save all";
        a_hud_text_content["switch_to_text_creation"] = "Switch to text creation";
    }

    // Load french
    void SCLS_Documentalist_GUI::load_language_fr() {
        a_hud_text_content["create_a_file_pattern"] = scls::to_utf_8("Créer un fichier modèle");
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Créer un projet");
        a_hud_text_content["final_path"] = "Fichier final";
        std::string help_body_home = "<h1>Bienvenue sur la GUI de SCLS Documentalist \"Agatha\"</h1>";
        help_body_home += "Ce logiciel, fait par Aster Système, vous permet d'utiliser SCLS Documentalist \"Agatha\" simplement, sans avoir à</br>";
        help_body_home += "manipuler de lignes de code. Vous pouvez l'utiliser, que vous soyez un développeur qui veut documenter facilement</br>";
        help_body_home += "son code, soit que vous êtes quoi que ce soit d'autre. Il vous est distribué sous la license libre GPL V3.0.</br>";
        help_body_home += "Vous êtes actuellement sur la page d'aide. Naviguez avec le sélécteur à gauche.";
        a_hud_text_content["help_body_home"] = scls::to_utf_8(help_body_home);
        a_hud_text_content["home"] = "Accueil";
        a_hud_text_content["home_project"] = "Accueil du projet";
        a_hud_text_content["name_file_pattern"] = scls::to_utf_8("Nom du fichier modèle");
        a_hud_text_content["name_project"] = "Nom du project";
        a_hud_text_content["save"] = "Sauvegarder";
        a_hud_text_content["save_all"] = "Sauvegarder tout";
        a_hud_text_content["switch_to_text_creation"] = scls::to_utf_8("Passer à la création de texte");
    }

    // Load the main header
    void SCLS_Documentalist_GUI::load_main_header() {
        // Main header for the mains buttons
        a_main_header = parent_object()->new_object<scls::GUI_Object>("header");
        a_main_header->set_background_color(scls::white);
        a_main_header->set_height_in_scale(0.05);
        a_main_header->set_width_in_scale(1.0);
        a_main_header->move_top_of_parent();
        // Create the file button
        a_main_header_file_button = a_main_header->new_object<scls::GUI_Text>("main_header_file_button");
        a_main_header_file_button->set_font_size(MAIN_HEADER_PIXEL_HEIGHT);
        a_main_header_file_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_main_header_file_button->set_text("File");
        a_main_header_file_button->set_size_in_scale(glm::vec2(0.1, 1.0));
        a_main_header_file_button->set_y_in_scale(0);
        a_main_header_file_button->move_left_of_parent();
    }

    // Load the welcome page footer
    void SCLS_Documentalist_GUI::load_welcome_footer() {
        // Footer of the welcome page
        double scale_multiplier = 0.98;
        a_welcome_footer = parent_object()->new_object<scls::GUI_Object>("welcome_footer");
        a_welcome_footer->set_background_color(scls::white);
        a_welcome_footer->set_position_in_scale(glm::vec3(0, -2.0/3.0, 1));
        a_welcome_footer->set_size_in_scale(glm::vec3(2.0 * scale_multiplier, (2.0/3.0) * scale_multiplier, 1));
        a_welcome_footer->set_border_width_in_pixel(1);
        // Create the button of creation of project in the welcome footer
        a_welcome_footer_create_project = a_welcome_footer->new_object<scls::GUI_Text>("welcome_footer_create_project");
        a_welcome_footer_create_project->set_font_size(100);
        a_welcome_footer_create_project->set_text_offset(0.05);
        a_welcome_footer_create_project->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
        a_welcome_footer_create_project->set_border_width_in_pixel(1);
        a_welcome_footer_create_project->set_size_in_scale(glm::vec2(1.0, 0.2));
        a_welcome_footer_create_project->set_position_in_scale(glm::vec2(-0.3, 0));
    }

    // Reload each HUD text
    void SCLS_Documentalist_GUI::reload_text() {
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
    }

    //*********
    //
    // Processing functions
    //
    //*********

    // Run the GUI
    void SCLS_Documentalist_GUI::start() {
        display_help();

        while(run()) {
            update_event();
            update();

            render();
        }
    }

    // Use the agatha gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path) {
        SCLS_Documentalist_GUI* window = new SCLS_Documentalist_GUI(960, 540, exec_path);
        window->set_minimum_window_height(540);
        window->set_minimum_window_width(960);
        window->load();
        window->start();

        delete window; window = 0;
    }
}
