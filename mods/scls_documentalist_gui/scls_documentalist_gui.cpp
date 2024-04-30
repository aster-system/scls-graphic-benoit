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
#define MAIN_HEADER_HEIGHT ((MAIN_HEADER_PIXEL_HEIGHT/static_cast<double>(window_height()))/2.0)
#endif // MAIN_HEADER_HEIGHT

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace scls_documentalist_gui {
    // SCLS_Documentalist_GUI base constructor
    SCLS_Documentalist_GUI::SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path) : Window(window_width, window_height, exec_path) {
        set_background_color(scls::Color(200, 200, 200));
    }

    // SCLS_Documentalist_GUI base destructor
    SCLS_Documentalist_GUI::~SCLS_Documentalist_GUI() {
        clear_memory_from_loaded_projects();
    }

    // Create a new project
    void SCLS_Documentalist_GUI::create_project() {
        std::string project_name = "project";
        scls::Project* created_project = new scls::Project(project_name);

        a_loaded_projects[project_name] = created_project;
        load_project(created_project);
    }

    // Load a project in the GUI with its name
    void SCLS_Documentalist_GUI::load_project(scls::Project* project_to_load) {
        hide_all_pages();
        display_page("header");
        display_page("project_body");
        display_page("project_navigation");
        display_page("welcome_footer");

        set_window_title("SCLS Documentalist \"Agatha\" - " + project_to_load->name());

        // Load the project navigation
        a_project_navigation_home_button = a_project_navigation->new_object<scls::HUD_Text>("project_navigation_home_button");
        a_project_navigation_home_button->set_font_size(100);
        a_project_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_navigation_home_button->set_text(a_hud_text_content["home_project"]);
        a_project_navigation_home_button->set_object_scale_width(0.6);
        a_project_navigation_home_button->set_position(glm::vec2(0, 0));
    }

    // Load the entire gui
    void SCLS_Documentalist_GUI::load() {
        load_language_fr();
        load_textures();

        load_help_navigation();
        load_main_header();
        load_project_navigation();
        load_welcome_page_footer();

        // Load bodies
        load_help_body();
        load_project_body();

        set_window_title("SCLS Documentalist \"Agatha\"");
    }

    // Load the welcome page body
    void SCLS_Documentalist_GUI::load_help_body() {
        // Body of the welcome page
        double scale_multiplier = 0.99;
        a_help_body = new_page<scls::HUD_Page>("help_body");
        a_help_body->set_background_color(scls::white);
        a_help_body->set_position(glm::vec3(0.25, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_help_body->set_scale(glm::vec3(3.0/2.0 * scale_multiplier, (4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0) * scale_multiplier, 1));
        a_help_body->set_pixel_border_width(1);
        // Create the logo of the help page
        scls::HUD_Object* help_body_logo = a_help_body->new_object<scls::HUD_Object>("help_body_logo", "scls_documentalist_logo");
        help_body_logo->set_object_scale(0.6);
        help_body_logo->set_position(glm::vec2(0, 0.19));
        // Create the home text of the help page
        a_help_body_home = a_help_body->new_object<scls::HUD_Text>("help_body_welcome");
        a_help_body_home->set_font_size(50);
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home->set_object_scale_width(1.75);
        a_help_body_home->set_position(glm::vec2(0, -0.15));
        // Create the the description text of the home part of the help page
        a_help_body_home_description = a_help_body->new_object<scls::HUD_Text>("help_body_welcome_description");
        a_help_body_home_description->set_font_size(30);
        a_help_body_home_description->set_text_alignment(scls::Text_Alignment_Horizontal::Center);
        a_help_body_home_description->set_text(a_hud_text_content["help_body_home_description"]);
        a_help_body_home_description->set_object_scale_width(1.75);
        a_help_body_home_description->set_position(glm::vec2(0, -0.3));
        // Create the part text of the home part of the help page
        a_help_body_home_part = a_help_body->new_object<scls::HUD_Text>("help_body_welcome_part");
        a_help_body_home_part->set_font_size(50);
        a_help_body_home_part->set_text_alignment(scls::Text_Alignment_Horizontal::Center);
        a_help_body_home_part->set_text(a_hud_text_content["help_body_home_part"]);
        a_help_body_home_part->set_object_scale_width(1.15);
        a_help_body_home_part->set_position(glm::vec2(0, -0.425));
    }

    // Load the help navigation
    void SCLS_Documentalist_GUI::load_help_navigation() {
        // Navigation for the help
        a_help_navigation = new_page<scls::HUD_Page>("help_navigation");
        a_help_navigation->set_background_color(scls::white);
        a_help_navigation->set_position(glm::vec3(-0.75, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_help_navigation->set_scale(glm::vec3(1.0/2.0, 4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0, 1));
        a_help_navigation->set_pixel_border_width(1);
        // Y position of the buttons
        double buttons_y = 0;
        // Create the home button of the navigation
        a_help_navigation_home_button = a_help_navigation->new_object<scls::HUD_Text>("help_navigation_home_button");
        a_help_navigation_home_button->set_font_size(100);
        a_help_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_help_navigation_home_button->set_object_scale_width(0.3);
        a_help_navigation_home_button->set_position(glm::vec2(0, buttons_y));
        buttons_y += a_help_navigation_home_button->scale()[0];
    }

    // Load the project page navigation
    void SCLS_Documentalist_GUI::load_project_navigation() {
        // Navigation for the project
        a_project_navigation = new_page<scls::HUD_Page>("project_navigation");
        a_project_navigation->set_background_color(scls::white);
        a_project_navigation->set_position(glm::vec3(-0.75, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_project_navigation->set_scale(glm::vec3(1.0/2.0, 4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0, 1));
        a_project_navigation->set_pixel_border_width(1);
    }

    // Load english
    void SCLS_Documentalist_GUI::load_language_en() {
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Create a project");
        a_hud_text_content["help_body_home"] = "Welcome to SCLS Documentalist \"Agatha\" GUI";
        std::string help_body_welcome_description = "This software, made by Aster System, provides a simply use to SCLS Documentalist \"Agatha\", whitout having to</br>";
        help_body_welcome_description += "handle a lot of lines of code. You can use it, as as a developer who wants to easily documentate its code, or as</br>";
        help_body_welcome_description += "anybody else. The software is published under the terms of the license GPL V3.0.";
        a_hud_text_content["help_body_home_description"] = scls::to_utf_8(help_body_welcome_description);
        a_hud_text_content["help_body_home_part"] = scls::to_utf_8("You currently are in the help page. Browse with the</br>selector at your left.");
        a_hud_text_content["home"] = "Home";
        a_hud_text_content["home_project"] = "Project home";
    }

    // Load french
    void SCLS_Documentalist_GUI::load_language_fr() {
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Créer un projet");
        a_hud_text_content["help_body_home"] = "Bienvenue sur la GUI de SCLS Documentalist \"Agatha\"";
        std::string help_body_welcome_description = "Ce logiciel, fait par Aster Système, vous permet d'utiliser SCLS Documentalist \"Agatha\" simplement, sans avoir à</br>";
        help_body_welcome_description += "manipuler de lignes de code. Vous pouvez l'utiliser, que vous soyez un développeur qui veut documenter facilement</br>";
        help_body_welcome_description += "son code, soit que vous êtes quoi que ce soit d'autre. Il vous est distribué sous la license libre GPL V3.0.";
        a_hud_text_content["help_body_home_description"] = scls::to_utf_8(help_body_welcome_description);
        a_hud_text_content["help_body_home_part"] = scls::to_utf_8("Vous êtes actuellement sur la page d'aide. Naviguez</br>avec le sélécteur à gauche.");
        a_hud_text_content["home"] = "Accueil";
        a_hud_text_content["home_project"] = "Accueil du projet";
    }

    // Load the main header
    void SCLS_Documentalist_GUI::load_main_header() {
        // Main header for the mains buttons
        double main_header_buttons_x = -0.5;
        a_main_header = new_page<scls::HUD_Page>("header");
        a_main_header->set_position(glm::vec3(0, 1.0 - MAIN_HEADER_HEIGHT, 1));
        a_main_header->set_background_color(scls::white);
        a_main_header->set_scale(glm::vec3(2.0, MAIN_HEADER_HEIGHT * 2.0, 1));
        // Create the file button
        a_main_header_file_button = a_main_header->new_object<scls::HUD_Text>("main_header_file_button");
        a_main_header_file_button->set_font_size(MAIN_HEADER_PIXEL_HEIGHT);
        a_main_header_file_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_main_header_file_button->set_text("File");
        a_main_header_file_button->set_object_scale(1); main_header_buttons_x += a_main_header_file_button->scale()[0];
        a_main_header_file_button->set_position(glm::vec2(main_header_buttons_x, 0));
    }

    // Load the project page body
    void SCLS_Documentalist_GUI::load_project_body() {
        // Body of the welcome page
        double scale_multiplier = 0.99;
        a_project_body = new_page<scls::HUD_Page>("project_body");
        a_project_body->set_background_color(scls::white);
        a_project_body->set_position(glm::vec3(0.25, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_project_body->set_scale(glm::vec3(3.0/2.0 * scale_multiplier, (4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0) * scale_multiplier, 1));
        a_project_body->set_pixel_border_width(1);
    }

    // Load the needed textures
    void SCLS_Documentalist_GUI::load_textures() {
        // Load the SCLS Documentalist logo
        new_texture("scls_documentalist_logo", "assets/textures/scls_documentalist_logo.png", false);
    }

    // Load the welcome page footer
    void SCLS_Documentalist_GUI::load_welcome_page_footer() {
        // Footer of the welcome page
        double scale_multiplier = 0.98;
        a_welcome_footer = new_page<scls::HUD_Page>("welcome_footer");
        a_welcome_footer->set_background_color(scls::white);
        a_welcome_footer->set_position(glm::vec3(0, -2.0/3.0, 1));
        a_welcome_footer->set_scale(glm::vec3(2.0 * scale_multiplier, (2.0/3.0) * scale_multiplier, 1));
        a_welcome_footer->set_pixel_border_width(1);
        // Create the button of creation of project in the welcome footer
        a_welcome_footer_create_project = a_welcome_footer->new_object<scls::HUD_Text>("welcome_footer_create_project");
        a_welcome_footer_create_project->set_font_size(100);
        a_welcome_footer_create_project->set_text_offset(0.05);
        a_welcome_footer_create_project->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
        a_welcome_footer_create_project->set_pixel_border_width(1);
        a_welcome_footer_create_project->set_object_scale(0.2);
        a_welcome_footer_create_project->set_position(glm::vec2(-0.3, 0));
    }

    // Reload each HUD text
    void SCLS_Documentalist_GUI::reload_text() {
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home_description->set_text(a_hud_text_content["help_body_home_description"]);
        a_help_body_home_part->set_text(a_hud_text_content["help_body_home_part"]);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
    }

    // Run the GUI
    void SCLS_Documentalist_GUI::start() {
        hide_all_pages();
        display_page("header");
        display_page("help_body");
        display_page("help_navigation");
        display_page("welcome_footer");

        while(run()) {
            update_event();
            update();

            if(a_welcome_footer_create_project->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_project();
            }

            render();
        }
    }

    // Use the agatha gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path) {
        SCLS_Documentalist_GUI* window = new SCLS_Documentalist_GUI(960, 540, exec_path);
        window->load();
        window->set_minimum_window_height(540);
        window->set_minimum_window_width(960);
        window->start();

        delete window; window = 0;
    }
}
