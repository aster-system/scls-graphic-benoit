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

    // Check if a project can be created
    void SCLS_Documentalist_GUI::check_project_creation() {
        std::string project_name = a_create_project_body_name->plain_text();

        if(project_name == "") {
            return;
        }
        create_project(a_create_project_body_name->plain_text());
    }

    // Check if a file pattern in a project can be created
    void SCLS_Documentalist_GUI::check_project_file_pattern_creation() {
        if(currently_displayed_project() == 0) {
            return;
        }
        std::string file_name = a_create_project_file_pattern_body_name->plain_text();

        if(file_name == "") {
            return;
        }
        create_project_file_pattern(file_name, currently_displayed_project());
    }

    // Create a new project
    void SCLS_Documentalist_GUI::create_project(std::string project_name) {
        scls::Project* created_project = new scls::Project(project_name);

        a_loaded_projects[project_name] = created_project;
        set_currently_dispalyed_project(created_project);
        save_loaded_project();
    }

    // Create a new file pattern in the currently displayed project
    void SCLS_Documentalist_GUI::create_project_file_pattern(std::string file_name, scls::Project* project) {
        project->new_pattern(file_name, "");

        load_navigation_project_buttons();
        save_loaded_project();
        set_project_file_pattern_body(file_name, project);
    }

    // Load a project in the GUI with its name
    void SCLS_Documentalist_GUI::load_project(scls::Project* project_to_load) {
        set_window_title("SCLS Documentalist \"Agatha\" - " + project_to_load->name());

        load_project_home(project_to_load);
    }

    // Load a file pattern of a project in the GUI
    void SCLS_Documentalist_GUI::load_project_file_pattern(std::string file_pattern) {
        unset_all();
        display_page("header");
        display_page("project_file_pattern_body");
        display_page("project_navigation");
        display_page("project_footer");

        std::cout << "T " << file_pattern << std::endl;
    }

    // Load the home page of a project in the GUI
    void SCLS_Documentalist_GUI::load_project_home(scls::Project* project_to_load) {
        unset_all();
        display_page("header");
        display_page("project_body");
        display_page("project_navigation");
        display_page("project_footer");
    }

    // Load the navigation buttons of the project
    void SCLS_Documentalist_GUI::load_navigation_project_buttons() {
        // Delete each buttons in the file
        for(int i = 0;i<static_cast<int>(a_project_navigation_file_pattern_button.size());i++) {
            a_project_navigation->delete_children(a_project_navigation_file_pattern_button[i]);
        }
        a_project_navigation_file_pattern_button.clear();

        // Create each buttons
        scls::Project* current_project = currently_displayed_project();
        scls::HUD_Object* last_object = a_project_navigation_home_button;
        for(int i = 0;i<static_cast<int>(current_project->patterns().size());i++) {
            std::string file_name = current_project->patterns()[i]->name();

            scls::HUD_Text* button = a_project_navigation->new_object<scls::HUD_Text>("help_navigation_home_button_" + file_name);
            button->set_font_size(100);
            button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            button->set_text(file_name);
            button->set_object_scale(0.1);
            button->move_bottom_of_object_in_parent(last_object);
            last_object = button;
            a_project_navigation_file_pattern_button.push_back(button);
        }
    }

    // Set a project file pattern body
    void SCLS_Documentalist_GUI::set_project_file_pattern_body(std::string file_name, scls::Project* project) {
        unset_all();
        display_page("header");
        display_page("project_file_pattern_body");
        display_page("project_navigation");
        display_page("project_footer");

        if(contains_loaded_file(file_name)) {
            a_project_file_pattern_content->set_text(loaded_files()[file_name].text);
        }
        else {
            scls::Text_Pattern* pattern = project->pattern_by_name(file_name);
            std::string file_content = pattern->base_text();
            _SCLS_Documentalist_GUI_File file; file.text = file_content; file.base_project = project;
            loaded_files()[file_name] = file;
            a_project_file_pattern_content->set_text(file_content);
        }
        a_currently_displayed_file_pattern = file_name;
    }

    //*********
    //
    // Annoying GUI stuff
    //
    //*********

    // Load the entire gui
    void SCLS_Documentalist_GUI::load() {
        load_language_fr();
        load_textures();

        load_help_navigation();
        load_main_header();
        load_project_navigation();

        // Load footers
        load_project_footer();
        load_welcome_page_footer();

        // Load bodies
        load_create_project_body();
        load_create_project_file_pattern_body();
        load_help_body();
        load_project_body();
        load_project_file_pattern();

        set_window_title("SCLS Documentalist \"Agatha\"");
    }

    // Load the page to create a project
    void SCLS_Documentalist_GUI::load_create_project_body() {
        // Body of the create a project
        double scale_multiplier = 0.99;
        a_create_project_body = new_page<scls::HUD_Page>("create_project_body");
        a_create_project_body->set_background_color(scls::white);
        a_create_project_body->set_position(glm::vec3(0.25, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_create_project_body->set_scale(glm::vec3(3.0/2.0 * scale_multiplier, (4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0) * scale_multiplier, 1));
        a_create_project_body->set_pixel_border_width(1);
        // Button of validation of creation of project in the page to create a project
        a_create_project_body_create = a_create_project_body->new_object<scls::HUD_Text>("create_project_body_create");
        a_create_project_body_create->set_font_size(50);
        a_create_project_body_create->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_project_body_create->set_text_offset(0.1);
        a_create_project_body_create->set_text(a_hud_text_content["create_a_project"]);
        a_create_project_body_create->set_pixel_border_width(1);
        a_create_project_body_create->set_object_scale_width(0.3);
        a_create_project_body_create->set_position(glm::vec2(-0.3, -0.4));
        // Title of the name of the page to create a project
        a_create_project_body_name_title = a_create_project_body->new_object<scls::HUD_Text>("create_project_body_name_title");
        a_create_project_body_name_title->set_font_size(50);
        a_create_project_body_name_title->set_text(a_hud_text_content["name_project"] + " :");
        a_create_project_body_name_title->set_object_scale_width(0.3);
        a_create_project_body_name_title->set_position(glm::vec2(-0.3, 0.4));
        // Input of the name of the page to create a project
        a_create_project_body_name = a_create_project_body->new_object<scls::HUD_Text_Input>("create_project_body_name");
        a_create_project_body_name->set_pixel_border_width(1);
        a_create_project_body_name->set_font_size(25);
        a_create_project_body_name->set_resize_texture_with_scale(false);
        a_create_project_body_name->set_texture_aligmnent_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_project_body_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        a_create_project_body_name->set_text("");
        a_create_project_body_name->set_scale(glm::vec2(0.4, 0.1));
        a_create_project_body_name->set_position(glm::vec2(0.15, 0.4));
    }

    // Load the page to create a file pattern in a project
    void SCLS_Documentalist_GUI::load_create_project_file_pattern_body() {
        // Body of the create a project
        double scale_multiplier = 0.99;
        a_create_project_file_pattern_body = new_page<scls::HUD_Page>("create_project_file_pattern_body");
        a_create_project_file_pattern_body->set_background_color(scls::white);
        a_create_project_file_pattern_body->set_position(glm::vec3(0.25, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_create_project_file_pattern_body->set_scale(glm::vec3(3.0/2.0 * scale_multiplier, (4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0) * scale_multiplier, 1));
        a_create_project_file_pattern_body->set_pixel_border_width(1);
        // Button of validation of creation of project in the page to create a project
        a_create_project_file_pattern_body_create = a_create_project_file_pattern_body->new_object<scls::HUD_Text>("create_project_file_pattern_body_create");
        a_create_project_file_pattern_body_create->set_font_size(50);
        a_create_project_file_pattern_body_create->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_project_file_pattern_body_create->set_text_offset(0.1);
        a_create_project_file_pattern_body_create->set_text(a_hud_text_content["create_a_file_pattern"]);
        a_create_project_file_pattern_body_create->set_pixel_border_width(1);
        a_create_project_file_pattern_body_create->set_object_scale_width(0.3);
        a_create_project_file_pattern_body_create->set_position(glm::vec2(-0.3, -0.4));
        // Title of the name of the page to create a project
        a_create_project_file_pattern_body_name_title = a_create_project_file_pattern_body->new_object<scls::HUD_Text>("create_project_file_pattern_body_name_title");
        a_create_project_file_pattern_body_name_title->set_font_size(50);
        a_create_project_file_pattern_body_name_title->set_text(a_hud_text_content["name_file_pattern"] + " :");
        a_create_project_file_pattern_body_name_title->set_object_scale_width(0.3);
        a_create_project_file_pattern_body_name_title->set_position(glm::vec2(-0.3, 0.4));
        // Input of the name of the page to create a project
        a_create_project_file_pattern_body_name = a_create_project_file_pattern_body->new_object<scls::HUD_Text_Input>("create_project_file_pattern_body_name");
        a_create_project_file_pattern_body_name->set_pixel_border_width(1);
        a_create_project_file_pattern_body_name->set_font_size(25);
        a_create_project_file_pattern_body_name->set_resize_texture_with_scale(false);
        a_create_project_file_pattern_body_name->set_texture_aligmnent_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_project_file_pattern_body_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        a_create_project_file_pattern_body_name->set_text("");
        a_create_project_file_pattern_body_name->set_scale(glm::vec2(0.4, 0.1));
        a_create_project_file_pattern_body_name->set_position(glm::vec2(0.15, 0.4));
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
        a_help_body_home->set_object_scale_width(0.75);
        a_help_body_home->set_position(glm::vec2(0, -0.15));
        // Create the the description text of the home part of the help page
        a_help_body_home_description = a_help_body->new_object<scls::HUD_Text>("help_body_welcome_description");
        a_help_body_home_description->set_font_size(30);
        a_help_body_home_description->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center);
        a_help_body_home_description->set_text(a_hud_text_content["help_body_home_description"]);
        a_help_body_home_description->set_object_scale_width(0.75);
        a_help_body_home_description->set_position(glm::vec2(0, -0.3));
        // Create the part text of the home part of the help page
        a_help_body_home_part = a_help_body->new_object<scls::HUD_Text>("help_body_welcome_part");
        a_help_body_home_part->set_font_size(50);
        a_help_body_home_part->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center);
        a_help_body_home_part->set_text(a_hud_text_content["help_body_home_part"]);
        a_help_body_home_part->set_object_scale_width(0.6);
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
        // Create the home button of the navigation
        a_help_navigation_home_button = a_help_navigation->new_object<scls::HUD_Text>("help_navigation_home_button");
        a_help_navigation_home_button->set_font_size(100);
        a_help_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_help_navigation_home_button->set_object_scale_width(0.3);
        a_help_navigation_home_button->move_top_of_parent();
    }

    // Load the project page navigation
    void SCLS_Documentalist_GUI::load_project_navigation() {
        // Navigation for the project
        a_project_navigation = new_page<scls::HUD_Page>("project_navigation");
        a_project_navigation->set_background_color(scls::white);
        a_project_navigation->set_position(glm::vec3(-0.75, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_project_navigation->set_scale(glm::vec3(1.0/2.0, 4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0, 1));
        a_project_navigation->set_pixel_border_width(1);
        // Load the project home button
        a_project_navigation_home_button = a_project_navigation->new_object<scls::HUD_Text>("project_navigation_home_button");
        a_project_navigation_home_button->set_font_size(100);
        a_project_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_navigation_home_button->set_text(a_hud_text_content["home_project"]);
        a_project_navigation_home_button->set_object_scale_width(0.6);
        a_project_navigation_home_button->move_top_of_parent();
    }

    // Load english
    void SCLS_Documentalist_GUI::load_language_en() {
        a_hud_text_content["create_a_file_pattern"] = scls::to_utf_8("Create a pattern file");
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Create a project");
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
    }

    // Load french
    void SCLS_Documentalist_GUI::load_language_fr() {
        a_hud_text_content["create_a_file_pattern"] = scls::to_utf_8("Créer un fichier modèle");
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Créer un projet");
        a_hud_text_content["help_body_home"] = "Bienvenue sur la GUI de SCLS Documentalist \"Agatha\"";
        std::string help_body_welcome_description = "Ce logiciel, fait par Aster Système, vous permet d'utiliser SCLS Documentalist \"Agatha\" simplement, sans avoir à</br>";
        help_body_welcome_description += "manipuler de lignes de code. Vous pouvez l'utiliser, que vous soyez un développeur qui veut documenter facilement</br>";
        help_body_welcome_description += "son code, soit que vous êtes quoi que ce soit d'autre. Il vous est distribué sous la license libre GPL V3.0.";
        a_hud_text_content["help_body_home_description"] = scls::to_utf_8(help_body_welcome_description);
        a_hud_text_content["help_body_home_part"] = scls::to_utf_8("Vous êtes actuellement sur la page d'aide. Naviguez</br>avec le sélécteur à gauche.");
        a_hud_text_content["home"] = "Accueil";
        a_hud_text_content["home_project"] = "Accueil du projet";
        a_hud_text_content["name_file_pattern"] = scls::to_utf_8("Nom du fichier modèle");
        a_hud_text_content["name_project"] = "Nom du project";
        a_hud_text_content["save"] = "Sauvegarder";
        a_hud_text_content["save_all"] = "Sauvegarder tout";
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

    // Load a file pattern of a project
    void SCLS_Documentalist_GUI::load_project_file_pattern() {
        // Body of a project file pattern page
        double scale_multiplier = 0.99;
        a_project_file_pattern_body = new_page<scls::HUD_Page>("project_file_pattern_body");
        a_project_file_pattern_body->set_background_color(scls::white);
        a_project_file_pattern_body->set_position(glm::vec3(0.25, 1.0/3.0 - MAIN_HEADER_HEIGHT, 1));
        a_project_file_pattern_body->set_scale(glm::vec3(3.0/2.0 * scale_multiplier, (4.0/3.0 - MAIN_HEADER_HEIGHT * 2.0) * scale_multiplier, 1));
        a_project_file_pattern_body->set_pixel_border_width(1);
        // Load the content of a project file pattern
        a_project_file_pattern_content = a_project_file_pattern_body->new_object<scls::HUD_Text_Input>("project_file_pattern_content");
        a_project_file_pattern_content->set_font_size(20);
        a_project_file_pattern_content->set_resize_texture_with_scale(false);
        a_project_file_pattern_content->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_project_file_pattern_content->set_text_offset(0);
        a_project_file_pattern_content->set_text(scls::to_utf_8("Le char Leclerc</br>Leclerc great because Leclerc good."));
        a_project_file_pattern_content->set_texture_aligmnent_horizontal(scls::Alignment_Horizontal::H_Left);
        a_project_file_pattern_content->set_position(glm::vec2(0, 0));
        a_project_file_pattern_content->set_scale(glm::vec2(0.98));
    }

    // Load the project page footer
    void SCLS_Documentalist_GUI::load_project_footer() {
        // Footer of the project page
        double scale_multiplier = 0.98;
        a_project_footer = new_page<scls::HUD_Page>("project_footer");
        a_project_footer->set_background_color(scls::white);
        a_project_footer->set_position(glm::vec3(0, -2.0/3.0, 1));
        a_project_footer->set_scale(glm::vec3(2.0 * scale_multiplier, (2.0/3.0) * scale_multiplier, 1));
        a_project_footer->set_pixel_border_width(1);
        // Create the button of creation of file in the project footer
        a_project_footer_create_file_pattern = a_project_footer->new_object<scls::HUD_Text>("project_footer_create_file_pattern");
        a_project_footer_create_file_pattern->set_font_size(100);
        a_project_footer_create_file_pattern->set_text_offset(0.05);
        a_project_footer_create_file_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_footer_create_file_pattern->set_text(a_hud_text_content["create_a_file_pattern"]);
        a_project_footer_create_file_pattern->set_pixel_border_width(1);
        a_project_footer_create_file_pattern->set_object_scale(0.125);
        a_project_footer_create_file_pattern->set_position(glm::vec2(-0.3, 0));
        // Create the button of saving all the project
        a_project_footer_save_all = a_project_footer->new_object<scls::HUD_Text>("project_footer_save_all");
        a_project_footer_save_all->set_font_size(100);
        a_project_footer_save_all->set_text_offset(0.05);
        a_project_footer_save_all->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_project_footer_save_all->set_pixel_border_width(1);
        a_project_footer_save_all->set_object_scale(0.2);
        a_project_footer_save_all->set_position(glm::vec2(0.3, -0.2));
        // Create the button of saving of a file in the project footer
        a_project_footer_save_file_pattern = a_project_footer->new_object<scls::HUD_Text>("project_footer_save_file_pattern");
        a_project_footer_save_file_pattern->set_font_size(100);
        a_project_footer_save_file_pattern->set_text_offset(0.05);
        a_project_footer_save_file_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_footer_save_file_pattern->set_text(a_hud_text_content["save"]);
        a_project_footer_save_file_pattern->set_pixel_border_width(1);
        a_project_footer_save_file_pattern->set_object_scale(0.2);
        a_project_footer_save_file_pattern->set_position(glm::vec2(0.3, 0.2));
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
        a_create_project_body_create->set_text(a_hud_text_content["create_a_project"]);
        a_create_project_body_name_title->set_text(a_hud_text_content["name_project"] + " :");
        a_create_project_file_pattern_body_create->set_text(a_hud_text_content["create_a_file_pattern"]);
        a_create_project_file_pattern_body_name_title->set_text(a_hud_text_content["name_file_pattern"] + " :");
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home_description->set_text(a_hud_text_content["help_body_home_description"]);
        a_help_body_home_part->set_text(a_hud_text_content["help_body_home_part"]);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
    }

    // Save the loaded file pattern
    void SCLS_Documentalist_GUI::save_loaded_file_pattern() {
        std::string to_save = a_project_file_pattern_content->text();
        scls::write_in_file("lesson.html", to_save);
    };

    // Save the entire loaded project
    void SCLS_Documentalist_GUI::save_loaded_project() {
        // Check the currently edited file
        if(currently_displayed_file_pattern() != "") {
            if(contains_loaded_file(currently_displayed_file_pattern())) {
                loaded_files()[currently_displayed_file_pattern()].text = a_project_file_pattern_content->text();
            }
            a_currently_displayed_file_pattern = "";
        }

        if(currently_displayed_project() == 0) return;
        std::string base_path = currently_displayed_project_name() + "/";
        std::string project_path = currently_displayed_project_name() + "/";

        if(!std::filesystem::exists(project_path)) {
            std::filesystem::create_directory(project_path);
        }

        // Save the main file


        // Save each files in the project
        for(std::map<std::string, _SCLS_Documentalist_GUI_File>::iterator it = loaded_files().begin();it!=loaded_files().end();it++) {
            if(it->second.base_project == currently_displayed_project()) {
                scls::Text_Pattern* current_pattern = currently_displayed_project()->pattern_by_name(it->first);
                if(current_pattern != 0) {
                    current_pattern->set_base_text(it->second.text);
                }
            }
        }

        // Save each files
        for(int i = 0;i<static_cast<int>(currently_displayed_project()->patterns().size());i++) {
            std::string file_name = base_path + currently_displayed_project()->patterns()[i]->name();
            scls::write_in_file(file_name, currently_displayed_project()->patterns()[i]->base_text());
        }
    }

    // Set the create project page
    void SCLS_Documentalist_GUI::set_create_project_body() {
        unset_all();
        display_page("header");
        display_page("create_project_body");
        display_page("help_navigation");
        display_page("welcome_footer");
    }

    // Set the create file pattern in project page
    void SCLS_Documentalist_GUI::set_create_project_file_pattern_body() {
        unset_all();
        display_page("header");
        display_page("create_project_file_pattern_body");
        display_page("project_navigation");
        display_page("project_footer");
    }

    // Run the GUI
    void SCLS_Documentalist_GUI::start() {
        unset_all();
        display_page("header");
        display_page("help_body");
        display_page("help_navigation");
        display_page("welcome_footer");

        while(run()) {
            update_event();
            update();

            // Check if a project creation is needed
            if(a_welcome_footer_create_project->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_create_project_body();
            }
            if(a_create_project_body_create->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                check_project_creation();
            }

            // Check if a pattern file creation is needed
            if(a_project_footer_create_file_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_create_project_file_pattern_body();
            }
            if(a_create_project_file_pattern_body_create->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                check_project_file_pattern_creation();
            }
            for(int i = 0;i<static_cast<int>(a_project_navigation_file_pattern_button.size());i++) {
                if(a_project_navigation_file_pattern_button[i]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                    set_project_file_pattern_body(a_project_navigation_file_pattern_button[i]->plain_text(), currently_displayed_project());
                }
            }

            // Save a project
            if(a_project_footer_save_all->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                save_loaded_project();
            }
            if(a_project_footer_save_file_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                save_loaded_file_pattern();
            }

            render();
        }
    }

    // Unset all the pages in the window
    void SCLS_Documentalist_GUI::unset_all() {
        hide_all_pages();

        // Check the currently edited file
        if(currently_displayed_file_pattern() != "") {
            if(contains_loaded_file(currently_displayed_file_pattern())) {
                loaded_files()[currently_displayed_file_pattern()].text = a_project_file_pattern_content->text();
            }
            a_currently_displayed_file_pattern = "";
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
