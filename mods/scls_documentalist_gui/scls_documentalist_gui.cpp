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
        unload_projects();
    }

    //*********
    //
    // Annoying GUI stuff
    //
    //*********

    // Display the create file pattern page of the software
    void SCLS_Documentalist_GUI::display_create_file_pattern() {
        parent_object()->hide_children();

        a_create_file_pattern_body->set_visible(true);
        a_main_header->set_visible(true);
        a_project_footer->set_visible(true);
        a_project_navigation->set_visible(true);
    }

    // Display the create project of the software
    void SCLS_Documentalist_GUI::display_create_project() {
        parent_object()->hide_children();

        a_create_project_body->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display the file explorer of the software
    void SCLS_Documentalist_GUI::display_file_explorer() {
        parent_object()->hide_children();

        a_file_explorer->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display a file pattern of a project
    void SCLS_Documentalist_GUI::display_file_pattern(scls::Text_Pattern* pattern_to_display) {
        parent_object()->hide_children();

        a_file_pattern_body->set_visible(true);
        a_main_header->set_visible(true);
        a_project_footer->set_visible(true);
        a_project_navigation->set_visible(true);
    }

    // Display the help page
    void SCLS_Documentalist_GUI::display_help() {
        parent_object()->hide_children();

        a_help_body->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display the main page of a project
    void SCLS_Documentalist_GUI::display_project_main(scls::Project* project_to_display) {
        a_currently_displayed_project = project_to_display;
        parent_object()->hide_children();

        a_main_header->set_visible(true);
        a_project_footer->set_visible(true);
        a_project_main_body->set_visible(true);
        a_project_navigation->set_visible(true);

        load_project_navigation_buttons();
        a_project_main_body_title->set_text(a_hud_text_content["project"] + " : " + project_to_display->name());
        a_project_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
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
        load_help_navigation();
        load_project_navigation();

        // Load footers
        load_project_footer();
        load_welcome_footer();

        // Load bodies
        load_create_file_pattern_body();
        load_create_project_body();
        load_file_explorer();
        load_file_pattern_body();
        load_help_body();
        load_project_main_body();

        reset_create_project_page();
        set_window_title("SCLS Documentalist \"Agatha\"");
    }

    // Load the create file pattern body
    void SCLS_Documentalist_GUI::load_create_file_pattern_body() {
        // Body of the create file pattern page
        a_create_file_pattern_body = parent_object()->new_object<scls::GUI_Object>("create_file_pattern_body");
        a_create_file_pattern_body->set_background_color(scls::white);
        a_create_file_pattern_body->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_create_file_pattern_body->set_border_width_in_pixel(1);
        a_create_file_pattern_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_file_pattern_body->move_right_of_parent(0.025);
        // Input of the name of the file pattern
        a_create_file_pattern_name = a_create_file_pattern_body->new_object<scls::GUI_Text_Input>("create_file_pattern_name");
        a_create_file_pattern_name->set_border_width_in_pixel(1);
        a_create_file_pattern_name->set_font_size(20);
        a_create_file_pattern_name->set_position_in_scale(glm::vec2(0.4, 0.8));
        a_create_file_pattern_name->set_size_in_scale(glm::vec2(0.35, 0.1));
        a_create_file_pattern_name->set_text("");
        a_create_file_pattern_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_file_pattern_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the file pattern
        a_create_file_pattern_name_title = a_create_file_pattern_body->new_object<scls::GUI_Text>("create_file_pattern_name_title");
        a_create_file_pattern_name_title->set_font_size(100);
        a_create_file_pattern_name_title->set_position_in_scale(glm::vec2(-0.4, 0.8));
        a_create_file_pattern_name_title->set_size_in_scale(glm::vec2(0.35, 0.15));
        a_create_file_pattern_name_title->set_text(a_hud_text_content["file_pattern_name"] + " :");
        a_create_file_pattern_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validate the creation of the file pattern
        a_create_file_pattern_validation = a_create_file_pattern_body->new_object<scls::GUI_Text>("create_file_pattern_validation");
        a_create_file_pattern_validation->set_border_width_in_pixel(1);
        a_create_file_pattern_validation->set_font_size(100);
        a_create_file_pattern_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_file_pattern_validation->set_position_in_scale(glm::vec2(-0.4, -0.8));
        a_create_file_pattern_validation->set_size_in_scale(glm::vec2(0.25, 0.1));
        a_create_file_pattern_validation->set_text(a_hud_text_content["file_pattern_create"]);
        a_create_file_pattern_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Load the create project body
    void SCLS_Documentalist_GUI::load_create_project_body() {
        // Body of the create project page
        a_create_project_body = parent_object()->new_object<scls::GUI_Object>("create_project_body");
        a_create_project_body->set_background_color(scls::white);
        a_create_project_body->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_create_project_body->set_border_width_in_pixel(1);
        a_create_project_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_project_body->move_right_of_parent(0.025);
        // Input of the name of the project
        a_create_project_name = a_create_project_body->new_object<scls::GUI_Text_Input>("create_project_name");
        a_create_project_name->set_border_width_in_pixel(1);
        a_create_project_name->set_font_size(20);
        a_create_project_name->set_position_in_scale(glm::vec2(0.4, 0.8));
        a_create_project_name->set_size_in_scale(glm::vec2(0.35, 0.1));
        a_create_project_name->set_text("");
        a_create_project_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_project_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the project
        a_create_project_name_title = a_create_project_body->new_object<scls::GUI_Text>("create_project_name_title");
        a_create_project_name_title->set_font_size(100);
        a_create_project_name_title->set_position_in_scale(glm::vec2(-0.4, 0.8));
        a_create_project_name_title->set_size_in_scale(glm::vec2(0.35, 0.15));
        a_create_project_name_title->set_text(a_hud_text_content["name_project"] + " :");
        a_create_project_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Input of the path of the project
        a_create_project_path = a_create_project_body->new_object<scls::GUI_Text>("create_project_path");
        a_create_project_path->set_font_size(100);
        a_create_project_path->set_position_in_scale(glm::vec2(0.4, 0.55));
        a_create_project_path->set_size_in_scale(glm::vec2(0.35, 0.1));
        a_create_project_path->set_text("C:/");
        a_create_project_path->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        // Button to change the path of the project
        a_create_project_path_change = a_create_project_body->new_object<scls::GUI_Text>("create_project_path_change");
        a_create_project_path_change->set_border_width_in_pixel(1);
        a_create_project_path_change->set_font_size(50);
        a_create_project_path_change->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_project_path_change->set_position_in_scale(glm::vec2(0.4, 0.3));
        a_create_project_path_change->set_size_in_scale(glm::vec2(0.35, 0.1));
        a_create_project_path_change->set_text(a_hud_text_content["change_path"]);
        a_create_project_path_change->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Title of the input of the path of the project
        a_create_project_path_title = a_create_project_body->new_object<scls::GUI_Text>("create_project_path_title");
        a_create_project_path_title->set_font_size(100);
        a_create_project_path_title->set_position_in_scale(glm::vec2(-0.4, 0.55));
        a_create_project_path_title->set_size_in_scale(glm::vec2(0.35, 0.15));
        a_create_project_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_create_project_path_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validation button of the create project body
        a_create_project_validation = a_create_project_body->new_object<scls::GUI_Text>("create_project_validation");
        a_create_project_validation->set_border_width_in_pixel(1);
        a_create_project_validation->set_font_size(100);
        a_create_project_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_project_validation->set_position_in_scale(glm::vec2(-0.4, -0.8));
        a_create_project_validation->set_size_in_scale(glm::vec2(0.25, 0.1));
        a_create_project_validation->set_text(a_hud_text_content["create_a_project"]);
        a_create_project_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Load the file explorer
    void SCLS_Documentalist_GUI::load_file_explorer() {
        a_file_explorer = parent_object()->new_object<scls::GUI_File_Explorer>("file_explorer");
        a_file_explorer->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_file_explorer->move_bottom_of_object_in_parent(a_main_header);
        a_file_explorer->move_right_of_parent(0.025);
    }

    // Load the file pattern
    void SCLS_Documentalist_GUI::load_file_pattern_body() {
        // Parent page of the file pattern body
        a_file_pattern_body = parent_object()->new_object<scls::GUI_Object>("file_pattern_body");
        a_file_pattern_body->set_background_color(scls::white);
        a_file_pattern_body->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_file_pattern_body->set_border_width_in_pixel(1);
        a_file_pattern_body->move_bottom_of_object_in_parent(a_main_header);
        a_file_pattern_body->move_right_of_parent(0.025);
        // Text in the file pattern
        a_file_pattern_text = a_file_pattern_body->new_object<scls::GUI_Text_Input>("file_pattern_text");
        a_file_pattern_text->set_font_size(20);
        a_file_pattern_text->set_size_in_scale(glm::vec2(0.95, 0.95));
        a_file_pattern_text->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_file_pattern_text->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Top);
        a_file_pattern_text->move_left_of_parent();
        a_file_pattern_text->move_top_of_parent();
    }

    // Load the welcome page body
    void SCLS_Documentalist_GUI::load_help_body() {
        // Body of the welcome page
        a_help_body = parent_object()->new_object<scls::GUI_Object>("help_body");
        a_help_body->set_background_color(scls::white);
        a_help_body->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_help_body->set_border_width_in_pixel(1);
        a_help_body->move_bottom_of_object_in_parent(a_main_header);
        a_help_body->move_right_of_parent(0.025);
        // Create the logo of the help page
        scls::GUI_Object* help_body_logo = a_help_body->new_object<scls::GUI_Object>("help_body_logo");
        help_body_logo->set_size_in_scale(glm::vec2(0.5, 0.65));
        help_body_logo->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
        help_body_logo->texture()->set_image_by_path("assets/textures/scls_documentalist_logo.png");
        help_body_logo->set_x_in_scale(0.0);
        help_body_logo->move_top_of_parent(0.05);
        // Create the home text of the help page
        a_help_body_home = a_help_body->new_object<scls::GUI_Text>("help_body_welcome");
        a_help_body_home->set_font_size(20);
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_help_body_home->set_size_in_scale(glm::vec2(0.9, 0.3));
        a_help_body_home->set_x_in_scale(0.0);
        a_help_body_home->move_bottom_of_parent();
    }

    // Load the help navigation
    void SCLS_Documentalist_GUI::load_help_navigation() {
        // Navigation for the help
        a_help_navigation = parent_object()->new_object<scls::GUI_Object>("help_navigation");
        a_help_navigation->set_size_in_scale(glm::vec2(0.25, 0.6));
        a_help_navigation->set_border_width_in_pixel(1);
        a_help_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_help_navigation->move_left_of_parent();
        // Create the home button of the navigation
        a_help_navigation_home_button = a_help_navigation->new_object<scls::GUI_Text>("help_navigation_home_button");
        a_help_navigation_home_button->set_font_size(75);
        a_help_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_help_navigation_home_button->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
        a_help_navigation_home_button->set_size_in_scale(glm::vec2(1.0, 0.1));
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
        a_hud_text_content["project"] = "Project";
        a_hud_text_content["save"] = "Save";
        a_hud_text_content["save_all"] = "Save all";
        a_hud_text_content["switch_to_text_creation"] = "Switch to text creation";
    }

    // Load french
    void SCLS_Documentalist_GUI::load_language_fr() {
        a_hud_text_content["change_path"] = scls::to_utf_8("Changer le chemin d'accès");
        a_hud_text_content["create_a_project"] = scls::to_utf_8("Créer un projet");
        a_hud_text_content["file_pattern_create"] = scls::to_utf_8("Créer un fichier modèle");
        a_hud_text_content["file_pattern_name"] = scls::to_utf_8("Nom du fichier modèle");
        a_hud_text_content["final_path"] = "Fichier final";
        std::string help_body_home = "<h1>Bienvenue sur la GUI de SCLS Documentalist \"Agatha\"</h1>";
        help_body_home += "Ce logiciel, fait par Aster Système, vous permet d'utiliser SCLS Documentalist \"Agatha\" simplement, sans avoir à</br>";
        help_body_home += "manipuler de lignes de code. Vous pouvez l'utiliser, que vous soyez un développeur qui veut documenter facilement</br>";
        help_body_home += "son code, soit que vous êtes quoi que ce soit d'autre. Il vous est distribué sous la license libre GPL V3.0.</br>";
        help_body_home += "Vous êtes actuellement sur la page d'aide. Naviguez avec le sélécteur à gauche.";
        a_hud_text_content["help_body_home"] = scls::to_utf_8(help_body_home);
        a_hud_text_content["home"] = "Accueil";
        a_hud_text_content["name_project"] = "Nom du projet";
        a_hud_text_content["path_project"] = scls::to_utf_8("Chemin d'accès du projet");
        a_hud_text_content["project"] = "Projet";
        a_hud_text_content["project_home"] = "Accueil du projet";
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

    // Load the project footer
    void SCLS_Documentalist_GUI::load_project_footer() {
        // Footer of the project page
        a_project_footer = parent_object()->new_object<scls::GUI_Object>("project_footer");
        a_project_footer->set_background_color(scls::white);
        a_project_footer->set_x_in_scale(0.0);
        a_project_footer->set_size_in_scale(glm::vec2(1.0, 0.35));
        a_project_footer->set_border_width_in_pixel(1);
        a_project_footer->move_bottom_of_parent();
        a_project_footer->set_size_in_scale(glm::vec2(0.975, 0.335));
        // Button to create a pattern file in the project footer
        a_project_footer_create_file_pattern = a_project_footer->new_object<scls::GUI_Text>("project_footer_create_file_pattern");
        a_project_footer_create_file_pattern->set_font_size(75);
        a_project_footer_create_file_pattern->set_text_offset(0.05);
        a_project_footer_create_file_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_footer_create_file_pattern->set_text(a_hud_text_content["file_pattern_create"]);
        a_project_footer_create_file_pattern->set_border_width_in_pixel(1);
        a_project_footer_create_file_pattern->set_position_in_scale(glm::vec2(0, 0));
        a_project_footer_create_file_pattern->set_size_in_scale(glm::vec2(0.3, 0.2));
        a_project_footer_create_file_pattern->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_project_footer_create_file_pattern->move_left_of_parent();
        a_project_footer_create_file_pattern->move_top_of_parent();
        // Button to save all in the project footer
        a_project_footer_save_all = a_project_footer->new_object<scls::GUI_Text>("project_footer_save_all");
        a_project_footer_save_all->set_font_size(75);
        a_project_footer_save_all->set_text_offset(0.05);
        a_project_footer_save_all->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_project_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_project_footer_save_all->set_border_width_in_pixel(1);
        a_project_footer_save_all->set_position_in_scale(glm::vec2(0, 0));
        a_project_footer_save_all->set_size_in_scale(glm::vec2(0.3, 0.2));
        a_project_footer_save_all->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_project_footer_save_all->move_left_of_parent();
        a_project_footer_save_all->move_bottom_of_parent();
    }

    // Load the project main body
    void SCLS_Documentalist_GUI::load_project_main_body() {
        // Parent page of the project main body
        a_project_main_body = parent_object()->new_object<scls::GUI_Object>("project_main_body");
        a_project_main_body->set_background_color(scls::white);
        a_project_main_body->set_size_in_scale(glm::vec2(0.7, 0.6));
        a_project_main_body->set_border_width_in_pixel(1);
        a_project_main_body->move_bottom_of_object_in_parent(a_main_header);
        a_project_main_body->move_right_of_parent(0.025);
        // Home text of the project main body
        a_project_main_body_title = a_project_main_body->new_object<scls::GUI_Text>("project_main_body_title");
        a_project_main_body_title->set_font_size(100);
        a_project_main_body_title->set_text("");
        a_project_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_project_main_body_title->set_size_in_scale(glm::vec2(0.9, 0.15));
        a_project_main_body_title->set_x_in_scale(0.0);
        a_project_main_body_title->move_top_of_parent();
    }

    // Load the project navigation
    void SCLS_Documentalist_GUI::load_project_navigation() {
        // Parent page of the project navigation
        a_project_navigation = parent_object()->new_object<scls::GUI_Object>("project_navigation");
        a_project_navigation->set_size_in_scale(glm::vec2(0.25, 0.6));
        a_project_navigation->set_border_width_in_pixel(1);
        a_project_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_project_navigation->move_left_of_parent();
    }

    // Load the buttons in the project navigation
    void SCLS_Documentalist_GUI::load_project_navigation_buttons() {
        unload_project_navigation_buttons();
        scls::Project* project_to_display = currently_displayed_project();

        // Add the "project_home" button
        scls::GUI_Text* current_button = a_project_navigation->new_object<scls::GUI_Text>("project_navigation_home_button");
        current_button->set_font_size(75);
        current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        current_button->set_text(a_hud_text_content["project_home"]);
        current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        current_button->set_size_in_scale(glm::vec2(1.0, 0.1));
        current_button->move_left_of_parent();
        current_button->move_top_of_parent();
        a_project_navigation_buttons.push_back(current_button);

        // Load each buttons
        scls::GUI_Text* last_current_button = current_button;
        std::vector<scls::Text_Pattern*>& patterns = a_currently_displayed_project->patterns();
        for(int i = 0;i<static_cast<int>(patterns.size());i++) {
            current_button = a_project_navigation->new_object<scls::GUI_Text>("project_navigation_button_" + std::to_string(i));
            current_button->set_font_size(75);
            current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            current_button->set_text(patterns[i]->name().to_std_string());
            current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
            current_button->set_size_in_scale(glm::vec2(1.0, 0.1));
            current_button->move_left_of_parent();
            current_button->move_bottom_of_object_in_parent(last_current_button);
            a_project_navigation_buttons.push_back(current_button);
            last_current_button = current_button;
        }
    }

    // Load the welcome page footer
    void SCLS_Documentalist_GUI::load_welcome_footer() {
        // Footer of the welcome page
        a_welcome_footer = parent_object()->new_object<scls::GUI_Object>("welcome_footer");
        a_welcome_footer->set_background_color(scls::white);
        a_welcome_footer->set_x_in_scale(0.0);
        a_welcome_footer->set_size_in_scale(glm::vec2(1.0, 0.35));
        a_welcome_footer->set_border_width_in_pixel(1);
        a_welcome_footer->move_bottom_of_parent();
        a_welcome_footer->set_size_in_scale(glm::vec2(0.975, 0.335));
        // Create the button of creation of project in the welcome footer
        a_welcome_footer_create_project = a_welcome_footer->new_object<scls::GUI_Text>("welcome_footer_create_project");
        a_welcome_footer_create_project->set_font_size(75);
        a_welcome_footer_create_project->set_text_offset(0.05);
        a_welcome_footer_create_project->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
        a_welcome_footer_create_project->set_border_width_in_pixel(1);
        a_welcome_footer_create_project->set_position_in_scale(glm::vec2(0, 0));
        a_welcome_footer_create_project->set_size_in_scale(glm::vec2(0.25, 0.24));
        a_welcome_footer_create_project->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Reload each HUD text
    void SCLS_Documentalist_GUI::reload_text() {
        a_create_project_name_title->set_text(a_hud_text_content["name_project"] + " :");
        a_create_project_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_project_footer_create_file_pattern->set_text(a_hud_text_content["file_pattern_create"]);
        a_project_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_welcome_footer_create_project->set_text(a_hud_text_content["create_a_project"]);
    }

    // Reset the create project page
    void SCLS_Documentalist_GUI::reset_create_project_page() { a_create_project_path->set_text(scls::current_user_document_directory()); }

    // Save all the project
    void SCLS_Documentalist_GUI::save_all() {
        currently_displayed_project()->save_sda_0_1(a_current_path);
    }

    // Unload the buttons in the project navigation
    void SCLS_Documentalist_GUI::unload_project_navigation_buttons() { a_project_navigation->delete_children(); a_project_navigation_buttons.clear(); }

    //*********
    //
    // Project handling function
    //
    //*********

    // Returns if the program contains a loaded project by its name
    bool SCLS_Documentalist_GUI::contains_loaded_project_by_name(std::string project_name) {return loaded_project_by_name(project_name) != 0;}

    // Create a file pattern with the GUI datas
    void SCLS_Documentalist_GUI::create_file_pattern() {
        std::string name = a_create_file_pattern_name->plain_text();

        if(name != "" && !a_currently_displayed_project->contains_pattern_by_name(name)) {
            create_file_pattern(name);
        }
    }

    // Create a file pattern
    void SCLS_Documentalist_GUI::create_file_pattern(std::string name) {
        scls::Text_Pattern* new_pattern = a_currently_displayed_project->new_pattern(name, "");

        display_file_pattern(new_pattern);
        load_project_navigation_buttons();
    }

    // Create a project with the GUI datas
    void SCLS_Documentalist_GUI::create_project() {
        std::string name = a_create_project_name->plain_text();
        std::string path = a_create_project_path->plain_text();

        if(name != "" && !contains_loaded_project_by_name(name)) {
            create_project(name, path);
        }
    }

    // Create a project
    void SCLS_Documentalist_GUI::create_project(std::string name, std::string path) {
        path += "/" + name + "/";
        std::filesystem::create_directory(path);
        a_current_path = path;

        scls::Project* new_project = new scls::Project(name);
        a_loaded_projects.push_back(new_project);

        display_project_main(new_project);
        save_all();
    }

    // Returns a loaded project by its name, or 0 if it does not exists
    scls::Project* SCLS_Documentalist_GUI::loaded_project_by_name(std::string project_name) { for(int i = 0;i<static_cast<int>(a_loaded_projects.size());i++) { if(a_loaded_projects[i]->name() == project_name) return a_loaded_projects[i]; } return 0; }

    // Unload every projects
    void SCLS_Documentalist_GUI::unload_projects() { for(int i = 0;i<static_cast<int>(a_loaded_projects.size());i++) { delete a_loaded_projects[i]; } a_loaded_projects.clear(); }

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

            a_project_footer_create_file_pattern->move_left_of_parent();
            a_project_footer_create_file_pattern->move_top_of_parent();

            if(a_welcome_footer_create_project->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_project();
            }
            else if(a_create_project_path_change->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_current_file_to_be_chosen = 0;
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
            }

            if(a_file_explorer->file_chosen()) {
                if(a_current_file_to_be_chosen == 0) {
                    a_create_project_path->set_text(a_file_explorer->currently_selected_path());
                    display_create_project();
                }
            }

            if(a_create_project_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_project();
            }

            if(a_project_footer_create_file_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_file_pattern();
            }
            else if(a_create_file_pattern_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_file_pattern();
            }
            else if(a_project_footer_save_all->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                save_all();
            }

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
