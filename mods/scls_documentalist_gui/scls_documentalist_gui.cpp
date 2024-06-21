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
        unload_patterns();
        unload_replicas();
    }

    //*********
    //
    // Annoying GUI stuff
    //
    //*********

    // Display the create file pattern page of the software
    void SCLS_Documentalist_GUI::display_create_file_pattern() {
        unset_all();

        a_create_file_pattern_body->set_visible(true);
        a_main_header->set_visible(true);
        a_pattern_footer->set_visible(true);
        a_pattern_navigation->set_visible(true);
    }

    // Display the create pattern of the software
    void SCLS_Documentalist_GUI::display_create_pattern() {
        unset_all();

        a_create_pattern_body->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display the create replica page of the software
    void SCLS_Documentalist_GUI::display_create_replica() {
        unset_all();

        a_create_replica_body->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display the create replica file page of the software
    void SCLS_Documentalist_GUI::display_create_replica_file() {
        unset_all();

        load_create_replica_file_pattern_navigation_buttons();
        a_create_replica_file_body->set_visible(true);
        a_create_replica_pattern_file->set_visible(true);
        a_main_header->set_visible(true);
        a_replica_footer->set_visible(true);
    }

    // Display the file explorer of the software
    void SCLS_Documentalist_GUI::display_file_explorer() {
        unset_all();

        a_file_explorer->set_current_user_document_directory();
        a_file_explorer->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display a file pattern of a pattern
    void SCLS_Documentalist_GUI::display_file_pattern(scls::Text_Pattern* pattern_to_display) {
        unset_all();

        a_file_pattern_body->set_visible(true);
        a_main_header->set_visible(true);
        a_pattern_footer->set_visible(true);
        a_pattern_navigation->set_visible(true);

        if(contains_opened_file_pattern(pattern_to_display)) {
            a_file_pattern_text->set_text(a_opened_files_pattern[pattern_to_display]);
        }
        else {
            if(a_file_pattern_text->attached_text_image() != 0) a_file_pattern_text->reset();
            a_opened_files_pattern[pattern_to_display] = pattern_to_display->base_text().to_std_string();
            a_file_pattern_text->set_text(pattern_to_display->base_text().to_std_string());
        }
        a_currently_displayed_file_pattern = pattern_to_display;
    }

    // Display the help page
    void SCLS_Documentalist_GUI::display_help() {
        unset_all();

        a_help_body->set_visible(true);
        a_help_navigation->set_visible(true);
        a_main_header->set_visible(true);
        a_welcome_footer->set_visible(true);
    }

    // Display the main page of a pattern
    void SCLS_Documentalist_GUI::display_pattern_main(scls::Pattern_Project* project_to_display) {
        a_currently_displayed_pattern.reset(project_to_display);
        unset_all();

        a_main_header->set_visible(true);
        a_pattern_footer->set_visible(true);
        a_pattern_main_body->set_visible(true);
        a_pattern_navigation->set_visible(true);

        load_pattern_navigation_buttons();
        a_pattern_main_body_title->set_text(a_hud_text_content["pattern"] + " : " + project_to_display->name());
        a_pattern_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
    }

    // Display the main page of a replica
    void SCLS_Documentalist_GUI::display_replica_main(const std::shared_ptr<scls::Replica_Project>& replica_to_display) {
        a_currently_displayed_replica_project = replica_to_display;
        unset_all();

        a_main_header->set_visible(true);
        a_replica_footer->set_visible(true);
        a_replica_main_body->set_visible(true);
        a_replica_navigation->set_visible(true);

        load_replica_navigation_buttons();
        a_replica_main_body_title->set_text(a_hud_text_content["replica"] + " : " + replica_to_display->name());
        a_replica_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
    }

    // Display the export page of a replica
    void SCLS_Documentalist_GUI::display_replica_export(const std::shared_ptr<scls::Replica_Project>& replica_to_export) {
        if(replica_to_export.get() == 0) return;
        unset_all();

        a_main_header->set_visible(true);
        a_replica_footer->set_visible(true);
        a_replica_export_body->set_visible(true);
        a_replica_navigation->set_visible(true);

        load_replica_navigation_buttons();
        a_replica_export_body_title->set_text(a_hud_text_content["export"] + " " + replica_to_export->name());
        a_replica_export_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
    }

    // Load the entire gui
    void SCLS_Documentalist_GUI::load() {
        load_language_fr();

        // Page of the GUI
        a_page = new_page<scls::GUI_Page>("scls_documentalist_agatha");
        a_page->set_scale(glm::vec3(2, 2, 1));
        a_page->parent_object()->set_background_color(scls::Color(255, 255, 255));
        a_page->parent_object()->set_position_in_pixel(0, 0);
        a_page->parent_object()->set_height_in_scale(scls::Fraction(1));
        a_page->parent_object()->set_width_in_scale(scls::Fraction(1));
        display_page("scls_documentalist_agatha");

        load_main_header();

        // Load navigations
        load_help_navigation();
        load_pattern_navigation();
        load_replica_navigation();

        // Load footers
        load_pattern_footer();
        load_replica_footer();
        load_welcome_footer();

        // Load bodies
        load_create_file_pattern_body();
        load_create_pattern_body();
        load_create_replica_body();
        load_create_replica_file_body();
        load_file_explorer();
        load_file_pattern_body();
        load_help_body();
        load_pattern_main_body();
        load_replica_export_body();
        load_replica_main_body();

        reset_create_pattern_page();
        reset_create_replica_page();
        set_window_title("SCLS Documentalist \"Agatha\"");

        place_all();
    }

    // Load the create file pattern body
    void SCLS_Documentalist_GUI::load_create_file_pattern_body() {
        // Body of the create file pattern page
        a_create_file_pattern_body = parent_object()->new_object<scls::GUI_Object>("create_file_pattern_body");
        a_create_file_pattern_body->set_background_color(scls::white);
        a_create_file_pattern_body->set_height_in_scale(scls::Fraction(3, 5));
        a_create_file_pattern_body->set_width_in_scale(scls::Fraction(7, 10));
        a_create_file_pattern_body->set_border_width_in_pixel(1);
        a_create_file_pattern_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_file_pattern_body->move_right_in_parent(1);
        // Input of the name of the file pattern
        a_create_file_pattern_name = a_create_file_pattern_body->new_object<scls::GUI_Text_Input>("create_file_pattern_name");
        a_create_file_pattern_name->set_border_width_in_pixel(1);
        a_create_file_pattern_name->set_font_size(20);
        a_create_file_pattern_name->set_x_in_scale(scls::Fraction(2, 5));
        a_create_file_pattern_name->set_y_in_scale(scls::Fraction(4, 5));
        a_create_file_pattern_name->set_height_in_scale(scls::Fraction(7, 20));
        a_create_file_pattern_name->set_width_in_scale(scls::Fraction(1, 10));
        a_create_file_pattern_name->set_text("");
        a_create_file_pattern_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_file_pattern_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the file pattern
        a_create_file_pattern_name_title = a_create_file_pattern_body->new_object<scls::GUI_Text>("create_file_pattern_name_title");
        a_create_file_pattern_name_title->set_font_size(100);
        a_create_file_pattern_name_title->set_x_in_scale(scls::Fraction(-2, 5));
        a_create_file_pattern_name_title->set_y_in_scale(scls::Fraction(4, 5));
        a_create_file_pattern_name_title->set_height_in_scale(scls::Fraction(7, 20));
        a_create_file_pattern_name_title->set_width_in_scale(scls::Fraction(3, 20));
        a_create_file_pattern_name_title->set_text(a_hud_text_content["file_pattern_name"] + " :");
        a_create_file_pattern_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validate the creation of the file pattern
        a_create_file_pattern_validation = a_create_file_pattern_body->new_object<scls::GUI_Text>("create_file_pattern_validation");
        a_create_file_pattern_validation->set_border_width_in_pixel(1);
        a_create_file_pattern_validation->set_font_size(100);
        a_create_file_pattern_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_file_pattern_validation->set_x_in_scale(scls::Fraction(-2, 5));
        a_create_file_pattern_validation->set_y_in_scale(scls::Fraction(-4, 5));
        a_create_file_pattern_validation->set_height_in_scale(scls::Fraction(1, 4));
        a_create_file_pattern_validation->set_width_in_scale(scls::Fraction(1, 10));
        a_create_file_pattern_validation->set_text(a_hud_text_content["file_pattern_create"]);
        a_create_file_pattern_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Load the create pattern body
    void SCLS_Documentalist_GUI::load_create_pattern_body() {
        // Body of the create pattern page
        a_create_pattern_body = parent_object()->new_object<scls::GUI_Object>("create_pattern_body");
        a_create_pattern_body->set_background_color(scls::white);
        a_create_pattern_body->set_height_in_scale(scls::Fraction(3, 5));
        a_create_pattern_body->set_width_in_scale(scls::Fraction(7, 10));
        a_create_pattern_body->set_border_width_in_pixel(1);
        a_create_pattern_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_pattern_body->move_right_in_parent(1);
        // Input of the name of the pattern
        a_create_pattern_name = a_create_pattern_body->new_object<scls::GUI_Text_Input>("create_pattern_name");
        a_create_pattern_name->set_border_width_in_pixel(1);
        a_create_pattern_name->set_font_size(20);
        a_create_pattern_name->set_height_in_scale(scls::Fraction(1, 10));
        a_create_pattern_name->set_width_in_scale(scls::Fraction(7, 20));
        a_create_pattern_name->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_pattern_name->set_y_in_scale(scls::Fraction(4, 5));
        a_create_pattern_name->set_text("");
        a_create_pattern_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_pattern_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the pattern
        a_create_pattern_name_title = a_create_pattern_body->new_object<scls::GUI_Text>("create_pattern_name_title");
        a_create_pattern_name_title->set_font_size(100);
        a_create_pattern_name_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_pattern_name_title->set_y_in_scale(scls::Fraction(4, 5));
        a_create_pattern_name_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_pattern_name_title->set_width_in_scale(scls::Fraction(7, 20));
        a_create_pattern_name_title->set_text(a_hud_text_content["name_pattern"] + " :");
        a_create_pattern_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Input of the path of the pattern
        a_create_pattern_path = a_create_pattern_body->new_object<scls::GUI_Text>("create_pattern_path");
        a_create_pattern_path->set_font_size(100);
        a_create_pattern_path->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_pattern_path->set_y_in_scale(scls::Fraction(11, 20));
        a_create_pattern_path->set_height_in_scale(scls::Fraction(3, 20));
        a_create_pattern_path->set_width_in_scale(scls::Fraction(7, 20));
        a_create_pattern_path->set_text("C:/");
        a_create_pattern_path->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        // Button to change the path of the pattern
        a_create_pattern_path_change = a_create_pattern_body->new_object<scls::GUI_Text>("create_pattern_path_change");
        a_create_pattern_path_change->set_border_width_in_pixel(1);
        a_create_pattern_path_change->set_font_size(50);
        a_create_pattern_path_change->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_pattern_path_change->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_pattern_path_change->set_y_in_scale(scls::Fraction(2, 5));
        a_create_pattern_path_change->set_height_in_scale(scls::Fraction(1, 10));
        a_create_pattern_path_change->set_width_in_scale(scls::Fraction(7, 20));
        a_create_pattern_path_change->set_text(a_hud_text_content["change_path"]);
        a_create_pattern_path_change->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Title of the input of the path of the pattern
        a_create_pattern_path_title = a_create_pattern_body->new_object<scls::GUI_Text>("create_pattern_path_title");
        a_create_pattern_path_title->set_font_size(100);
        a_create_pattern_path_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_pattern_path_title->set_y_in_scale(scls::Fraction(11, 20));
        a_create_pattern_path_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_pattern_path_title->set_width_in_scale(scls::Fraction(7, 20));
        a_create_pattern_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_create_pattern_path_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validation button of the create pattern body
        a_create_pattern_validation = a_create_pattern_body->new_object<scls::GUI_Text>("create_pattern_validation");
        a_create_pattern_validation->set_border_width_in_pixel(1);
        a_create_pattern_validation->set_font_size(100);
        a_create_pattern_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_pattern_validation->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_pattern_validation->set_y_in_scale(scls::Fraction(1, 20));
        a_create_pattern_validation->set_height_in_scale(scls::Fraction(1, 10));
        a_create_pattern_validation->set_width_in_scale(scls::Fraction(1, 4));
        a_create_pattern_validation->set_text(a_hud_text_content["create_pattern"]);
        a_create_pattern_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Load the create replica body
    void SCLS_Documentalist_GUI::load_create_replica_body() {
        // Body of the create replica page
        a_create_replica_body = parent_object()->new_object<scls::GUI_Object>("create_replica_body");
        a_create_replica_body->set_background_color(scls::white);
        a_create_replica_body->set_height_in_scale(scls::Fraction(3, 5));
        a_create_replica_body->set_width_in_scale(scls::Fraction(7, 10));
        a_create_replica_body->set_border_width_in_pixel(1);
        a_create_replica_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_replica_body->move_right_in_parent(1);
        // Input of the name of the replica
        a_create_replica_name = a_create_replica_body->new_object<scls::GUI_Text_Input>("create_replica_name");
        a_create_replica_name->set_border_width_in_pixel(1);
        a_create_replica_name->set_font_size(20);
        a_create_replica_name->set_height_in_scale(scls::Fraction(1, 10));
        a_create_replica_name->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_name->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_replica_name->set_y_in_scale(scls::Fraction(4, 5));
        a_create_replica_name->set_text("");
        a_create_replica_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_replica_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the replica
        a_create_replica_name_title = a_create_replica_body->new_object<scls::GUI_Text>("create_replica_name_title");
        a_create_replica_name_title->set_font_size(100);
        a_create_replica_name_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_replica_name_title->set_y_in_scale(scls::Fraction(4, 5));
        a_create_replica_name_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_replica_name_title->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_name_title->set_text(a_hud_text_content["replica_name"] + " :");
        a_create_replica_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Input of the path of the replica
        a_create_replica_path = a_create_replica_body->new_object<scls::GUI_Text>("create_replica_path");
        a_create_replica_path->set_font_size(100);
        a_create_replica_path->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_replica_path->set_y_in_scale(scls::Fraction(11, 20));
        a_create_replica_path->set_height_in_scale(scls::Fraction(3, 20));
        a_create_replica_path->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_path->set_text("C:/");
        a_create_replica_path->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        // Button to change the path of the replica
        a_create_replica_path_change = a_create_replica_body->new_object<scls::GUI_Text>("create_replica_path_change");
        a_create_replica_path_change->set_border_width_in_pixel(1);
        a_create_replica_path_change->set_font_size(50);
        a_create_replica_path_change->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_replica_path_change->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_replica_path_change->set_y_in_scale(scls::Fraction(2, 5));
        a_create_replica_path_change->set_height_in_scale(scls::Fraction(1, 10));
        a_create_replica_path_change->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_path_change->set_text(a_hud_text_content["change_path"]);
        a_create_replica_path_change->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Title of the input of the path of the replica
        a_create_replica_path_title = a_create_replica_body->new_object<scls::GUI_Text>("create_replica_path_title");
        a_create_replica_path_title->set_font_size(100);
        a_create_replica_path_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_replica_path_title->set_y_in_scale(scls::Fraction(11, 20));
        a_create_replica_path_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_replica_path_title->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_create_replica_path_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validation button of the create replica body
        a_create_replica_validation = a_create_replica_body->new_object<scls::GUI_Text>("create_replica_validation");
        a_create_replica_validation->set_border_width_in_pixel(1);
        a_create_replica_validation->set_font_size(100);
        a_create_replica_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_replica_validation->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_replica_validation->set_y_in_scale(scls::Fraction(1, 20));
        a_create_replica_validation->set_height_in_scale(scls::Fraction(1, 10));
        a_create_replica_validation->set_width_in_scale(scls::Fraction(1, 4));
        a_create_replica_validation->set_text(a_hud_text_content["create_replica"]);
        a_create_replica_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
    }

    // Load the create replica file body
    void SCLS_Documentalist_GUI::load_create_replica_file_body() {
        // Body of the create file pattern page
        a_create_replica_file_body = parent_object()->new_object<scls::GUI_Object>("create_replica_file_body");
        a_create_replica_file_body->set_background_color(scls::white);
        a_create_replica_file_body->set_height_in_scale(scls::Fraction(3, 5));
        a_create_replica_file_body->set_width_in_scale(scls::Fraction(7, 10));
        a_create_replica_file_body->set_border_width_in_pixel(1);
        a_create_replica_file_body->move_bottom_of_object_in_parent(a_main_header);
        a_create_replica_file_body->move_right_in_parent(1);
        // Input of the name of the file pattern
        a_create_replica_file_name = a_create_replica_file_body->new_object<scls::GUI_Text_Input>("create_replica_file_name");
        a_create_replica_file_name->set_border_width_in_pixel(1);
        a_create_replica_file_name->set_font_size(20);
        a_create_replica_file_name->set_height_in_scale(scls::Fraction(1, 10));
        a_create_replica_file_name->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_file_name->set_x_in_object_scale(scls::Fraction(7, 10));
        a_create_replica_file_name->set_y_in_scale(scls::Fraction(4, 5));
        a_create_replica_file_name->set_text("");
        a_create_replica_file_name->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_create_replica_file_name->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
        // Title of the input of the name of the file pattern
        a_create_replica_file_name_title = a_create_replica_file_body->new_object<scls::GUI_Text>("create_replica_file_name_title");
        a_create_replica_file_name_title->set_font_size(100);
        a_create_replica_file_name_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_replica_file_name_title->set_y_in_scale(scls::Fraction(4, 5));
        a_create_replica_file_name_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_replica_file_name_title->set_width_in_scale(scls::Fraction(7, 20));
        a_create_replica_file_name_title->set_text(a_hud_text_content["replica_file_name"] + " :");
        a_create_replica_file_name_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Input of the pattern file of the replica
        a_create_replica_pattern_file_title = a_create_replica_file_body->new_object<scls::GUI_Text>("create_replica_pattern_file_title");
        a_create_replica_pattern_file_title->set_font_size(100);
        a_create_replica_pattern_file_title->set_x_in_object_scale(scls::Fraction(1, 2));
        a_create_replica_pattern_file_title->set_y_in_scale(scls::Fraction(3, 5));
        a_create_replica_pattern_file_title->set_height_in_scale(scls::Fraction(3, 20));
        a_create_replica_pattern_file_title->set_width_in_scale(scls::Fraction(17, 20));
        a_create_replica_pattern_file_title->set_text(a_hud_text_content["replica_file_choose_pattern"]);
        a_create_replica_pattern_file_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        // Validate the creation of the file pattern
        a_create_replica_file_validation = a_create_replica_file_body->new_object<scls::GUI_Text>("create_replica_file_validation");
        a_create_replica_file_validation->set_border_width_in_pixel(1);
        a_create_replica_file_validation->set_font_size(100);
        a_create_replica_file_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_create_replica_file_validation->set_x_in_object_scale(scls::Fraction(3, 10));
        a_create_replica_file_validation->set_y_in_scale(scls::Fraction(1, 20));
        a_create_replica_file_validation->set_height_in_scale(scls::Fraction(1, 10));
        a_create_replica_file_validation->set_width_in_scale(scls::Fraction(1, 4));
        a_create_replica_file_validation->set_text(a_hud_text_content["replica_file_create"]);
        a_create_replica_file_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);

        // Input of the pattern file of the replica
        a_create_replica_pattern_file = parent_object()->new_object<scls::GUI_Scroller>("create_replica_pattern_file");
        a_create_replica_pattern_file->set_height_in_scale(scls::Fraction(3, 5));
        a_create_replica_pattern_file->set_width_in_scale(scls::Fraction(1, 4));
        a_create_replica_pattern_file->set_border_width_in_pixel(1);
        a_create_replica_pattern_file->move_bottom_of_object_in_parent(a_main_header);
        a_create_replica_pattern_file->move_left_in_parent();
    }

    // Load the file explorer
    void SCLS_Documentalist_GUI::load_file_explorer() {
        a_file_explorer = parent_object()->new_object<scls::GUI_File_Explorer>("file_explorer");
        a_file_explorer->set_height_in_scale(scls::Fraction(3, 5));
        a_file_explorer->set_width_in_scale(scls::Fraction(7, 10));
        a_file_explorer->move_bottom_of_object_in_parent(a_main_header);
        a_file_explorer->move_right_in_parent(1);
    }

    // Load the file pattern
    void SCLS_Documentalist_GUI::load_file_pattern_body() {
        // Parent page of the file pattern body
        a_file_pattern_body = parent_object()->new_object<scls::GUI_Object>("file_pattern_body");
        a_file_pattern_body->set_background_color(scls::white);
        a_file_pattern_body->set_height_in_scale(scls::Fraction(3, 5));
        a_file_pattern_body->set_width_in_scale(scls::Fraction(7, 10));
        a_file_pattern_body->set_border_width_in_pixel(1);
        a_file_pattern_body->move_bottom_of_object_in_parent(a_main_header);
        a_file_pattern_body->move_right_in_parent(1);
        // Text in the file pattern
        a_file_pattern_text = a_file_pattern_body->new_object<scls::GUI_Text_Input>("file_pattern_text");
        a_file_pattern_text->set_border_width_in_pixel(1);
        a_file_pattern_text->set_font_family("CascadiaMono");
        a_file_pattern_text->set_font_size(14);
        a_file_pattern_text->set_height_in_scale(scls::Fraction(1, 10));
        a_file_pattern_text->set_width_in_scale(scls::Fraction(1));
        a_file_pattern_text->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
        a_file_pattern_text->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Top);
        a_file_pattern_text->move_left_in_parent();
        a_file_pattern_text->move_top_in_parent();
    }

    // Load the welcome page body
    void SCLS_Documentalist_GUI::load_help_body() {
        // Body of the welcome page
        a_help_body = parent_object()->new_object<scls::GUI_Object>("help_body");
        a_help_body->set_background_color(scls::white);
        a_help_body->set_height_in_scale(scls::Fraction(3, 5));
        a_help_body->set_width_in_scale(scls::Fraction(7, 10));
        a_help_body->set_border_width_in_pixel(1);
        // Create the logo of the help page
        a_help_body_logo = a_help_body->new_object<scls::GUI_Object>("help_body_logo");
        a_help_body_logo->set_height_in_scale(scls::Fraction(13, 20));
        a_help_body_logo->set_width_in_scale(scls::Fraction(1, 2));
        a_help_body_logo->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
        a_help_body_logo->texture()->set_image_by_path("assets/textures/scls_documentalist_logo.png");
        a_help_body_logo->set_x_in_object_scale(scls::Fraction(1, 2));
        a_help_body_logo->move_top_in_parent(1);
        // Create the home text of the help page
        a_help_body_home = a_help_body->new_object<scls::GUI_Text>("help_body_welcome");
        a_help_body_home->set_font_size(20);
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_body_home->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_help_body_home->set_height_in_scale(scls::Fraction(3, 10));
        a_help_body_home->set_width_in_scale(scls::Fraction(9, 10));
        a_help_body_home->set_x_in_object_scale(scls::Fraction(1, 2));
        a_help_body_home->move_bottom_in_parent();
    }

    // Load the help navigation
    void SCLS_Documentalist_GUI::load_help_navigation() {
        // Navigation for the help
        a_help_navigation = parent_object()->new_object<scls::GUI_Scroller>("help_navigation");
        a_help_navigation->set_height_in_scale(scls::Fraction(3, 5));
        a_help_navigation->set_width_in_scale(scls::Fraction(1, 4));
        a_help_navigation->set_border_width_in_pixel(1);
        a_help_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_help_navigation->move_left_in_parent();
        // Create the home button of the navigation
        a_help_navigation_home_button = a_help_navigation->new_object_in_scroller<scls::GUI_Text>("help_navigation_home_button");
        a_help_navigation_home_button->set_font_size(75);
        a_help_navigation_home_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_help_navigation_home_button->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
        a_help_navigation_home_button->set_height_in_scale(scls::Fraction(1, 10));
        a_help_navigation_home_button->set_width_in_scale(scls::Fraction(1));
        a_help_navigation_home_button->move_top_in_parent();
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
        a_hud_text_content["create_pattern"] = scls::to_utf_8("Créer un modèle");
        a_hud_text_content["create_replica"] = scls::to_utf_8("Créer une réplique");
        a_hud_text_content["export"] = scls::to_utf_8("Exporter");
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
        a_hud_text_content["name_pattern"] = scls::to_utf_8("Nom du modèle");
        a_hud_text_content["open_pattern"] = scls::to_utf_8("Ouvrir un modèle");
        a_hud_text_content["path_project"] = scls::to_utf_8("Chemin d'accès du projet");
        a_hud_text_content["pattern"] = scls::to_utf_8("Modèle");
        a_hud_text_content["project_home"] = "Accueil du projet";
        a_hud_text_content["replica"] = scls::to_utf_8("Réplique");
        a_hud_text_content["replica_create"] = scls::to_utf_8("Créer une réplique");
        a_hud_text_content["replica_file_choose_pattern"] = scls::to_utf_8("Choisissez le fichier modèle de ce fichier réplique à gauche");
        a_hud_text_content["replica_file_create"] = scls::to_utf_8("Créer un fichier réplique");
        a_hud_text_content["replica_file_name"] = scls::to_utf_8("Nom du fichier réplique");
        a_hud_text_content["replica_home"] = scls::to_utf_8("Accueil de la réplique");
        a_hud_text_content["replica_open"] = scls::to_utf_8("Ouvrir une réplique");
        a_hud_text_content["replica_name"] = scls::to_utf_8("Nom de la réplique");
        a_hud_text_content["save"] = "Sauvegarder";
        a_hud_text_content["save_all"] = "Sauvegarder tout";
        a_hud_text_content["switch_to_text_creation"] = scls::to_utf_8("Passer à la création de texte");
    }

    // Load the main header
    void SCLS_Documentalist_GUI::load_main_header() {
        // Main header for the mains buttons
        a_main_header = parent_object()->new_object<scls::GUI_Object>("header");
        a_main_header->set_background_color(scls::white);
        a_main_header->set_height_in_scale(scls::Fraction(1, 20));
        a_main_header->set_width_in_scale(scls::Fraction(1));
        a_main_header->move_top_in_parent();
        // Create the file button
        a_main_header_file_button = a_main_header->new_object<scls::GUI_Text>("main_header_file_button");
        a_main_header_file_button->set_font_size(MAIN_HEADER_PIXEL_HEIGHT);
        a_main_header_file_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_main_header_file_button->set_text("File");
        a_main_header_file_button->set_height_in_scale(scls::Fraction(1));
        a_main_header_file_button->set_width_in_scale(scls::Fraction(1, 10));
        a_main_header_file_button->set_y_in_scale(0);
        a_main_header_file_button->move_left_in_parent();
    }

    // Load the pattern footer
    void SCLS_Documentalist_GUI::load_pattern_footer() {
        // Footer of the pattern page
        a_pattern_footer = parent_object()->new_object<scls::GUI_Object>("pattern_footer");
        a_pattern_footer->set_background_color(scls::white);
        a_pattern_footer->set_x_in_object_scale(scls::Fraction(1, 2));
        a_pattern_footer->set_height_in_scale(scls::Fraction(7, 20));
        a_pattern_footer->set_width_in_scale(scls::Fraction(1));
        a_pattern_footer->set_border_width_in_pixel(1);
        a_pattern_footer->move_bottom_in_parent();
        a_pattern_footer->set_height_in_scale(scls::Fraction(67, 200));
        a_pattern_footer->set_width_in_scale(scls::Fraction(39, 40));
        // Button to create a pattern file in the pattern footer
        a_pattern_footer_create_file_pattern = a_pattern_footer->new_object<scls::GUI_Text>("pattern_footer_create_file_pattern");
        a_pattern_footer_create_file_pattern->set_font_size(75);
        a_pattern_footer_create_file_pattern->set_text_offset(0.05);
        a_pattern_footer_create_file_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_pattern_footer_create_file_pattern->set_text(a_hud_text_content["file_pattern_create"]);
        a_pattern_footer_create_file_pattern->set_border_width_in_pixel(1);
        a_pattern_footer_create_file_pattern->set_height_in_scale(scls::Fraction(1, 5));
        a_pattern_footer_create_file_pattern->set_width_in_scale(scls::Fraction(3, 10));
        a_pattern_footer_create_file_pattern->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_pattern_footer_create_file_pattern->move_left_in_parent();
        a_pattern_footer_create_file_pattern->move_top_in_parent();
        // Button to create a replica file in the pattern footer
        a_pattern_footer_create_replica = a_pattern_footer->new_object<scls::GUI_Text>("pattern_footer_create_file_replica");
        a_pattern_footer_create_replica->set_font_size(75);
        a_pattern_footer_create_replica->set_text_offset(0.05);
        a_pattern_footer_create_replica->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_pattern_footer_create_replica->set_text(a_hud_text_content["replica_create"]);
        a_pattern_footer_create_replica->set_border_width_in_pixel(1);
        a_pattern_footer_create_replica->set_height_in_scale(scls::Fraction(1, 5));
        a_pattern_footer_create_replica->set_width_in_scale(scls::Fraction(3, 10));
        a_pattern_footer_create_replica->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_pattern_footer_create_replica->move_right_of_object_in_parent(a_pattern_footer_create_file_pattern);
        a_pattern_footer_create_replica->move_top_in_parent();
        // Button to save all in the pattern footer
        a_pattern_footer_save_all = a_pattern_footer->new_object<scls::GUI_Text>("pattern_footer_save_all");
        a_pattern_footer_save_all->set_font_size(75);
        a_pattern_footer_save_all->set_text_offset(0.05);
        a_pattern_footer_save_all->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_pattern_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_pattern_footer_save_all->set_border_width_in_pixel(1);
        a_pattern_footer_save_all->set_height_in_scale(scls::Fraction(1, 5));
        a_pattern_footer_save_all->set_width_in_scale(scls::Fraction(3, 10));
        a_pattern_footer_save_all->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_pattern_footer_save_all->move_left_in_parent();
        a_pattern_footer_save_all->move_bottom_in_parent();
    }

    // Load the pattern main body
    void SCLS_Documentalist_GUI::load_pattern_main_body() {
        // Parent page of the pattern main body
        a_pattern_main_body = parent_object()->new_object<scls::GUI_Object>("pattern_main_body");
        a_pattern_main_body->set_background_color(scls::white);
        a_pattern_main_body->set_height_in_scale(scls::Fraction(3, 5));
        a_pattern_main_body->set_width_in_scale(scls::Fraction(7, 10));
        a_pattern_main_body->set_border_width_in_pixel(1);
        a_pattern_main_body->move_bottom_of_object_in_parent(a_main_header);
        a_pattern_main_body->move_right_in_parent(1);
        // Home text of the pattern main body
        a_pattern_main_body_title = a_pattern_main_body->new_object<scls::GUI_Text>("pattern_main_body_title");
        a_pattern_main_body_title->set_font_size(100);
        a_pattern_main_body_title->set_text("");
        a_pattern_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_pattern_main_body_title->set_height_in_scale(scls::Fraction(3, 20));
        a_pattern_main_body_title->set_width_in_scale(scls::Fraction(9, 10));
        a_pattern_main_body_title->set_x_in_scale(scls::Fraction(0));
        a_pattern_main_body_title->move_top_in_parent();
    }

    // Load the pattern navigation
    void SCLS_Documentalist_GUI::load_pattern_navigation() {
        // Parent page of the pattern navigation
        a_pattern_navigation = parent_object()->new_object<scls::GUI_Object>("pattern_navigation");
        a_pattern_navigation->set_height_in_scale(scls::Fraction(3, 5));
        a_pattern_navigation->set_width_in_scale(scls::Fraction(1, 4));
        a_pattern_navigation->set_border_width_in_pixel(1);
        a_pattern_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_pattern_navigation->move_left_in_parent();
    }

    // Load the buttons in the pattern navigation
    void SCLS_Documentalist_GUI::load_pattern_navigation_buttons() {
        unload_pattern_navigation_buttons();
        scls::Pattern_Project* pattern_to_display = currently_displayed_pattern();

        // Add the "project_home" button
        scls::GUI_Text* current_button = a_pattern_navigation->new_object<scls::GUI_Text>("project_navigation_home_button");
        current_button->set_font_size(75);
        current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        current_button->set_text(a_hud_text_content["project_home"]);
        current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        current_button->set_height_in_scale(scls::Fraction(1, 10));
        current_button->set_width_in_scale(scls::Fraction(1));
        current_button->move_left_in_parent();
        current_button->move_top_in_parent(1); current_button->calculate_adapted_scale(true);
        a_pattern_navigation_buttons.push_back(current_button);

        // Load each buttons
        scls::GUI_Text* last_current_button = current_button;
        std::vector<scls::Text_Pattern*>& patterns = a_currently_displayed_pattern->patterns();
        for(int i = 0;i<static_cast<int>(patterns.size());i++) {
            current_button = a_pattern_navigation->new_object<scls::GUI_Text>("project_navigation_button_" + std::to_string(i));
            current_button->set_font_size(75);
            current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            current_button->set_text(patterns[i]->name().to_std_string());
            current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
            current_button->set_height_in_scale(scls::Fraction(1, 10));
            current_button->set_width_in_scale(scls::Fraction(1));
            current_button->move_left_in_parent();
            current_button->move_bottom_of_object_in_parent(last_current_button);
            a_pattern_navigation_buttons.push_back(current_button); current_button->calculate_adapted_scale(true);
            last_current_button = current_button;
        }
    }

    // Load the replica export body
    void SCLS_Documentalist_GUI::load_replica_export_body() {
        // Parent page of the replica main body
        a_replica_export_body = parent_object()->new_object<scls::GUI_Object>("replica_export_body");
        a_replica_export_body->set_background_color(scls::white);
        a_replica_export_body->set_height_in_scale(scls::Fraction(3, 5));
        a_replica_export_body->set_width_in_scale(scls::Fraction(7, 10));
        a_replica_export_body->set_border_width_in_pixel(1);
        a_replica_export_body->move_bottom_of_object_in_parent(a_main_header);
        a_replica_export_body->move_right_in_parent(1);
        // Home text of the replica main body
        a_replica_export_body_title = a_replica_export_body->new_object<scls::GUI_Text>("replica_export_body_titlee_body_title");
        a_replica_export_body_title->set_font_size(100);
        a_replica_export_body_title->set_text("");
        a_replica_export_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_replica_export_body_title->set_height_in_scale(scls::Fraction(3, 20));
        a_replica_export_body_title->set_width_in_scale(scls::Fraction(9, 10));
        a_replica_export_body_title->set_x_in_scale(scls::Fraction(0));
        a_replica_export_body_title->move_top_in_parent();
        // Input of the path of the replica
        a_replica_export_body_path = a_replica_export_body->new_object<scls::GUI_Text>("replica_export_body_path");
        a_replica_export_body_path->set_font_size(100);
        a_replica_export_body_path->set_x_in_object_scale(scls::Fraction(7, 10));
        a_replica_export_body_path->set_y_in_scale(scls::Fraction(11, 20));
        a_replica_export_body_path->set_height_in_scale(scls::Fraction(3, 20));
        a_replica_export_body_path->set_width_in_scale(scls::Fraction(7, 20));
        a_replica_export_body_path->set_text("C:/");
        a_replica_export_body_path->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        // Button to change the path of the replica
        a_replica_export_body_path_change = a_replica_export_body->new_object<scls::GUI_Text>("replica_export_body_path_change");
        a_replica_export_body_path_change->set_border_width_in_pixel(1);
        a_replica_export_body_path_change->set_font_size(50);
        a_replica_export_body_path_change->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_replica_export_body_path_change->set_x_in_object_scale(scls::Fraction(7, 10));
        a_replica_export_body_path_change->set_y_in_scale(scls::Fraction(2, 5));
        a_replica_export_body_path_change->set_height_in_scale(scls::Fraction(1, 10));
        a_replica_export_body_path_change->set_width_in_scale(scls::Fraction(7, 20));
        a_replica_export_body_path_change->set_text(a_hud_text_content["change_path"]);
        a_replica_export_body_path_change->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Title of the input of the path of the replica
        a_replica_export_body_path_title = a_replica_export_body->new_object<scls::GUI_Text>("replica_export_body_path_title");
        a_replica_export_body_path_title->set_font_size(100);
        a_replica_export_body_path_title->set_x_in_object_scale(scls::Fraction(3, 10));
        a_replica_export_body_path_title->set_y_in_scale(scls::Fraction(11, 20));
        a_replica_export_body_path_title->set_height_in_scale(scls::Fraction(3, 20));
        a_replica_export_body_path_title->set_width_in_scale(scls::Fraction(7, 20));
        a_replica_export_body_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_replica_export_body_path_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        // Validate the export of the replica in the replica export body
        a_replica_export_body_validation = a_replica_export_body->new_object<scls::GUI_Text>("replica_export_body_validation");
        a_replica_export_body_validation->set_border_width_in_pixel(1);
        a_replica_export_body_validation->set_font_size(100);
        a_replica_export_body_validation->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_replica_export_body_validation->set_x_in_object_scale(scls::Fraction(3, 10));
        a_replica_export_body_validation->set_y_in_scale(scls::Fraction(1, 20));
        a_replica_export_body_validation->set_height_in_scale(scls::Fraction(1, 10));
        a_replica_export_body_validation->set_width_in_scale(scls::Fraction(1, 4));
        a_replica_export_body_validation->set_text(a_hud_text_content["export"]);
        a_replica_export_body_validation->set_texture_alignment(scls::Alignment_Texture::T_Fit);
    }

    // Load the replica footer
    void SCLS_Documentalist_GUI::load_replica_footer() {
        // Footer of the pattern page
        a_replica_footer = parent_object()->new_object<scls::GUI_Object>("replica_footer");
        a_replica_footer->set_background_color(scls::white);
        a_replica_footer->set_x_in_object_scale(scls::Fraction(1, 2));
        a_replica_footer->set_height_in_scale(scls::Fraction(7, 20));
        a_replica_footer->set_width_in_scale(scls::Fraction(1));
        a_replica_footer->set_border_width_in_pixel(1);
        a_replica_footer->move_bottom_in_parent();
        a_replica_footer->set_height_in_scale(scls::Fraction(67, 200));
        a_replica_footer->set_width_in_scale(scls::Fraction(39, 40));
        // Button to create a pattern file in the pattern footer
        a_replica_footer_create_file_replica = a_replica_footer->new_object<scls::GUI_Text>("replica_footer_create_file_replica");
        a_replica_footer_create_file_replica->set_font_size(75);
        a_replica_footer_create_file_replica->set_text_offset(0.05);
        a_replica_footer_create_file_replica->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_replica_footer_create_file_replica->set_text(a_hud_text_content["replica_file_create"]);
        a_replica_footer_create_file_replica->set_border_width_in_pixel(1);
        a_replica_footer_create_file_replica->set_height_in_scale(scls::Fraction(1, 5));
        a_replica_footer_create_file_replica->set_width_in_scale(scls::Fraction(3, 10));
        a_replica_footer_create_file_replica->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_replica_footer_create_file_replica->move_left_in_parent();
        a_replica_footer_create_file_replica->move_top_in_parent();
        // Button to export in the replica footer
        a_replica_footer_export = a_replica_footer->new_object<scls::GUI_Text>("replica_footer_export");
        a_replica_footer_export->set_font_size(75);
        a_replica_footer_export->set_text_offset(0.05);
        a_replica_footer_export->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_replica_footer_export->set_text(a_hud_text_content["export"]);
        a_replica_footer_export->set_border_width_in_pixel(1);
        a_replica_footer_export->set_height_in_scale(scls::Fraction(1, 5));
        a_replica_footer_export->set_width_in_scale(scls::Fraction(3, 10));
        a_replica_footer_export->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_replica_footer_export->move_right_in_parent();
        a_replica_footer_export->move_bottom_in_parent();
        // Button to save all in the pattern footer
        a_replica_footer_save_all = a_replica_footer->new_object<scls::GUI_Text>("replica_footer_save_all");
        a_replica_footer_save_all->set_font_size(75);
        a_replica_footer_save_all->set_text_offset(0.05);
        a_replica_footer_save_all->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_replica_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_replica_footer_save_all->set_border_width_in_pixel(1);
        a_replica_footer_save_all->set_height_in_scale(scls::Fraction(1, 5));
        a_replica_footer_save_all->set_width_in_scale(scls::Fraction(3, 10));
        a_replica_footer_save_all->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_replica_footer_save_all->move_left_in_parent();
        a_replica_footer_save_all->move_bottom_in_parent();
    }

    // Load the replica main body
    void SCLS_Documentalist_GUI::load_replica_main_body() {
        // Parent page of the replica main body
        a_replica_main_body = parent_object()->new_object<scls::GUI_Object>("replica_main_body");
        a_replica_main_body->set_background_color(scls::white);
        a_replica_main_body->set_height_in_scale(scls::Fraction(3, 5));
        a_replica_main_body->set_width_in_scale(scls::Fraction(7, 10));
        a_replica_main_body->set_border_width_in_pixel(1);
        a_replica_main_body->move_bottom_of_object_in_parent(a_main_header);
        a_replica_main_body->move_right_in_parent(1);
        // Home text of the replica main body
        a_replica_main_body_title = a_replica_main_body->new_object<scls::GUI_Text>("replica_main_body_title");
        a_replica_main_body_title->set_font_size(100);
        a_replica_main_body_title->set_text("");
        a_replica_main_body_title->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_replica_main_body_title->set_height_in_scale(scls::Fraction(3, 20));
        a_replica_main_body_title->set_width_in_scale(scls::Fraction(9, 10));
        a_replica_main_body_title->set_x_in_scale(scls::Fraction(0));
        a_replica_main_body_title->move_top_in_parent();
    }

    // Load the replica navigation
    void SCLS_Documentalist_GUI::load_replica_navigation() {
        // Parent page of the replica navigation
        a_replica_navigation = parent_object()->new_object<scls::GUI_Scroller>("replica_navigation");
        a_replica_navigation->set_height_in_scale(scls::Fraction(3, 5));
        a_replica_navigation->set_width_in_scale(scls::Fraction(1, 4));
        a_replica_navigation->set_border_width_in_pixel(1);
        a_replica_navigation->move_bottom_of_object_in_parent(a_main_header);
        a_replica_navigation->move_left_in_parent();
    }

    // Load the welcome page footer
    void SCLS_Documentalist_GUI::load_welcome_footer() {
        // Footer of the welcome page
        a_welcome_footer = parent_object()->new_object<scls::GUI_Object>("welcome_footer");
        a_welcome_footer->set_background_color(scls::white);
        a_welcome_footer->set_x_in_object_scale(scls::Fraction(1, 2));
        a_welcome_footer->set_height_in_scale(scls::Fraction(7, 20));
        a_welcome_footer->set_width_in_scale(scls::Fraction(1));
        a_welcome_footer->set_border_width_in_pixel(1);
        a_welcome_footer->move_bottom_in_parent();
        a_welcome_footer->set_height_in_scale(scls::Fraction(67, 200));
        a_welcome_footer->set_width_in_scale(scls::Fraction(39, 40));
        // Create the button of creation of pattern in the welcome footer
        a_welcome_footer_create_pattern = a_welcome_footer->new_object<scls::GUI_Text>("welcome_footer_create_pattern");
        a_welcome_footer_create_pattern->set_font_size(75);
        a_welcome_footer_create_pattern->set_text_offset(0.05);
        a_welcome_footer_create_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_create_pattern->set_text(a_hud_text_content["create_pattern"]);
        a_welcome_footer_create_pattern->set_border_width_in_pixel(1);
        a_welcome_footer_create_pattern->set_height_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_create_pattern->set_width_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_create_pattern->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_welcome_footer_create_pattern->move_left_in_parent();
        a_welcome_footer_create_pattern->move_top_in_parent();
        // Button to open a replica in the welcome footer
        a_welcome_footer_open_replica = a_welcome_footer->new_object<scls::GUI_Text>("welcome_footer_create_replica");
        a_welcome_footer_open_replica->set_font_size(75);
        a_welcome_footer_open_replica->set_text_offset(0.05);
        a_welcome_footer_open_replica->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_open_replica->set_text(a_hud_text_content["replica_open"]);
        a_welcome_footer_open_replica->set_border_width_in_pixel(1);
        a_welcome_footer_open_replica->set_height_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_open_replica->set_width_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_open_replica->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_welcome_footer_open_replica->move_right_of_object_in_parent(a_welcome_footer_create_pattern);
        a_welcome_footer_open_replica->move_top_in_parent();
        // Create the button to open a pattern in the welcome footer
        a_welcome_footer_open_pattern = a_welcome_footer->new_object<scls::GUI_Text>("welcome_footer_open_pattern");
        a_welcome_footer_open_pattern->set_font_size(75);
        a_welcome_footer_open_pattern->set_text_offset(0.05);
        a_welcome_footer_open_pattern->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_welcome_footer_open_pattern->set_text(a_hud_text_content["open_pattern"]);
        a_welcome_footer_open_pattern->set_border_width_in_pixel(1);
        a_welcome_footer_open_pattern->set_x_in_scale(scls::Fraction(1, 2));
        a_welcome_footer_open_pattern->set_height_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_open_pattern->set_width_in_scale(scls::Fraction(1, 4));
        a_welcome_footer_open_pattern->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
        a_welcome_footer_open_pattern->move_left_in_parent();
        a_welcome_footer_open_pattern->move_bottom_of_object_in_parent(a_welcome_footer_create_pattern);
    }

    // Place all the objects in the program
    void SCLS_Documentalist_GUI::place_all() {
        // Header
        if(a_main_header != 0) {
            a_main_header->set_height_in_scale(scls::Fraction(1, 20));
            a_main_header->set_width_in_scale(scls::Fraction(1));
            a_main_header->move_top_in_parent();
        }
        if(a_main_header_file_button != 0) {
            a_main_header_file_button->set_height_in_scale(scls::Fraction(1));
            a_main_header_file_button->set_width_in_scale(scls::Fraction(1, 10));
            a_main_header_file_button->set_y_in_scale(0);
            a_main_header_file_button->move_left_in_parent();
        }

        // Create pattern body
        if(a_create_pattern_body != 0) {
            a_create_pattern_body->move_bottom_of_object_in_parent(a_main_header);
            a_create_pattern_body->move_right_in_parent(1);
        }
        // Create replica body
        if(a_create_replica_body != 0) {
            a_create_replica_body->move_bottom_of_object_in_parent(a_main_header);
            a_create_replica_body->move_right_in_parent(1);
        }
        // Create replica file body
        if(a_create_replica_file_body != 0) {
            a_create_replica_file_body->move_bottom_of_object_in_parent(a_main_header);
            a_create_replica_file_body->move_right_in_parent(1);
        }
        if(a_create_replica_pattern_file != 0) {
            a_create_replica_pattern_file->move_bottom_of_object_in_parent(a_main_header);
            a_create_replica_pattern_file->move_left_in_parent();
        }
        // File explorer
        if(a_file_explorer != 0) {
            a_file_explorer->move_bottom_of_object_in_parent(a_main_header);
            a_file_explorer->move_right_in_parent(1);
        }
        // File pattern body
        if(a_file_pattern_body != 0) {
            a_file_pattern_body->move_bottom_of_object_in_parent(a_main_header);
            a_file_pattern_body->move_right_in_parent(1);
        }
        // File pattern text
        if(a_file_pattern_text != 0) {
            a_file_pattern_text->set_height_in_scale(scls::Fraction(1));
            a_file_pattern_text->set_width_in_scale(scls::Fraction(1));
            a_file_pattern_text->move_left_in_parent();
            a_file_pattern_text->move_top_in_parent();
        }
        // Help body
        if(a_help_body != 0) {
            a_help_body->move_bottom_of_object_in_parent(a_main_header);
            a_help_body->move_right_in_parent(1);
        }
        if(a_help_body_logo != 0) {
            a_help_body_logo->set_x_in_object_scale(scls::Fraction(1, 2));
            a_help_body_logo->move_top_in_parent(1);
        }
        // Help navigation
        if(a_help_navigation != 0) {
            a_help_navigation->move_bottom_of_object_in_parent(a_main_header);
            a_help_navigation->move_left_in_parent();
        }
        if(a_help_navigation_home_button != 0) {
            a_help_navigation_home_button->move_top_in_parent(1);
        }
        // Pattern main body
        if(a_pattern_main_body != 0) {
            a_pattern_main_body->move_bottom_of_object_in_parent(a_main_header);
            a_pattern_main_body->move_right_in_parent(1);
        }
        if(a_pattern_main_body_title != 0) {
            a_pattern_main_body_title->set_x_in_scale(scls::Fraction(0));
            a_pattern_main_body_title->move_top_in_parent(1);
        }
        // Pattern main footer
        if(a_pattern_footer != 0) {
            a_pattern_footer->move_bottom_in_parent();
            a_pattern_main_body_title->set_height_in_scale(scls::Fraction(3, 20));
            a_pattern_main_body_title->set_width_in_scale(scls::Fraction(9, 10));
        }
        if(a_pattern_footer_create_file_pattern != 0) {
            a_pattern_footer_create_file_pattern->move_left_in_parent();
            a_pattern_footer_create_file_pattern->move_top_in_parent();
        }
        if(a_pattern_footer_create_replica != 0) {
            a_pattern_footer_create_replica->move_right_of_object_in_parent(a_pattern_footer_create_file_pattern);
            a_pattern_footer_create_replica->move_top_in_parent();
        }
        if(a_pattern_footer_save_all) {
            a_pattern_footer_save_all->move_left_in_parent();
            a_pattern_footer_save_all->move_bottom_in_parent();
        }
        // Pattern main body
        if(a_pattern_main_body != 0) {
            a_pattern_main_body->move_bottom_of_object_in_parent(a_main_header);
            a_pattern_main_body->move_right_in_parent(1);
        }
        // Pattern navigation
        if(a_pattern_navigation != 0) {
            a_pattern_navigation->move_bottom_of_object_in_parent(a_main_header);
            a_pattern_navigation->move_left_in_parent();
        }
        // Replica export body
        if(a_replica_export_body != 0) {
            a_replica_export_body->move_bottom_of_object_in_parent(a_main_header);
            a_replica_export_body->move_right_in_parent(1);
        }
        if (a_replica_main_body_title != 0) {
            a_replica_main_body_title->set_x_in_scale(scls::Fraction(0));
            a_replica_main_body_title->move_top_in_parent();
        }
        // Replica footer
        if(a_replica_footer != 0) {
            a_replica_footer->set_height_in_scale(scls::Fraction(67, 200));
            a_replica_footer->set_width_in_scale(scls::Fraction(39, 40));
        }
        if(a_replica_footer_create_file_replica != 0) {
            a_replica_footer_create_file_replica->move_left_in_parent();
            a_replica_footer_create_file_replica->move_top_in_parent();
        }
        if(a_replica_footer_export != 0) {
            a_replica_footer_export->move_right_in_parent();
            a_replica_footer_export->move_bottom_in_parent();
        }
        if(a_replica_footer_save_all) {
            a_replica_footer_save_all->move_left_in_parent();
            a_replica_footer_save_all->move_bottom_in_parent();
        }
        // Replica main body
        if(a_replica_main_body != 0) {
            a_replica_main_body->move_bottom_of_object_in_parent(a_main_header);
            a_replica_main_body->move_right_in_parent(1);
        }
        if(a_replica_export_body_title != 0) {
            a_replica_export_body_title->move_top_in_parent();
        }
        // Replica navigation
        if(a_replica_navigation != 0) {
            a_replica_navigation->move_bottom_of_object_in_parent(a_main_header);
            a_replica_navigation->move_left_in_parent();
        }
        // Welcome footer
        if(a_welcome_footer_create_pattern != 0) {
            a_welcome_footer_create_pattern->move_left_in_parent();
            a_welcome_footer_create_pattern->move_top_in_parent();
        }
        if(a_welcome_footer_open_pattern != 0 && a_welcome_footer_create_pattern != 0) {
            a_welcome_footer_open_pattern->move_left_in_parent();
            a_welcome_footer_open_pattern->move_bottom_of_object_in_parent(a_welcome_footer_create_pattern);
        }
        if(a_welcome_footer_open_replica != 0) {
            a_welcome_footer_open_replica->move_right_of_object_in_parent(a_welcome_footer_create_pattern);
            a_welcome_footer_open_replica->move_top_in_parent();
        }
    }

    // Reload each HUD text
    void SCLS_Documentalist_GUI::reload_text() {
        a_create_pattern_name_title->set_text(a_hud_text_content["name_pattern"] + " :");
        a_create_pattern_path_title->set_text(a_hud_text_content["path_project"] + " :");
        a_help_body_home->set_text(a_hud_text_content["help_body_home"]);
        a_help_navigation_home_button->set_text(a_hud_text_content["home"]);
        a_pattern_footer_create_file_pattern->set_text(a_hud_text_content["file_pattern_create"]);
        a_pattern_footer_save_all->set_text(a_hud_text_content["save_all"]);
        a_welcome_footer_create_pattern->set_text(a_hud_text_content["create_pattern"]);
        a_welcome_footer_open_pattern->set_text(a_hud_text_content["open_pattern"]);
    }

    // Reset the create pattern page
    void SCLS_Documentalist_GUI::reset_create_pattern_page() { a_create_pattern_path->set_text(scls::current_user_document_directory()); }

    // Reset the create replica page
    void SCLS_Documentalist_GUI::reset_create_replica_page() { a_create_replica_path->set_text(scls::current_user_document_directory()); }

    // Save all the project
    void SCLS_Documentalist_GUI::save_all() {
        if(a_currently_displayed_file_pattern != 0) {
            a_opened_files_pattern[a_currently_displayed_file_pattern] = a_file_pattern_text->text();
        }

        for(std::map<scls::Text_Pattern*, std::string>::iterator it = a_opened_files_pattern.begin();it!=a_opened_files_pattern.end();it++) {
            it->first->set_base_text(scls::String(it->second));
        } a_opened_files_pattern.clear();

        currently_displayed_pattern()->save_sda_0_1(a_current_path);
    }

    // Unset all the project
    void SCLS_Documentalist_GUI::unset_all() {
        if(a_currently_displayed_file_pattern != 0) {
            a_opened_files_pattern[a_currently_displayed_file_pattern] = a_file_pattern_text->text();
        }

        unload_create_replica_file_pattern_navigation_buttons();

        a_currently_displayed_file_pattern = 0;
        a_current_file_to_be_chosen = 0;
        parent_object()->hide_children();
    }

    //*********
    //
    // Project handling function
    //
    //*********

    // Returns if the program contains a loaded pattern by its name
    bool SCLS_Documentalist_GUI::contains_loaded_pattern_by_name(std::string project_name) {return loaded_pattern_by_name(project_name) != 0;}

    // Returns if the program contains an opened file pattern
    bool SCLS_Documentalist_GUI::contains_opened_file_pattern(scls::Text_Pattern* pattern_to_test) { for(std::map<scls::Text_Pattern*, std::string>::iterator it = a_opened_files_pattern.begin();it!=a_opened_files_pattern.end();it++) { if(it->first == pattern_to_test) return true; } return false; }

    // Create a file pattern with the GUI datas
    void SCLS_Documentalist_GUI::create_file_pattern() {
        std::string name = a_create_file_pattern_name->plain_text();

        if(name != "" && !a_currently_displayed_pattern->contains_pattern_by_name(name)) {
            create_file_pattern(name);
        }
    }

    // Create a file pattern
    void SCLS_Documentalist_GUI::create_file_pattern(std::string name) {
        scls::Text_Pattern* new_pattern = a_currently_displayed_pattern->new_pattern(name, "");

        display_file_pattern(new_pattern);
        load_pattern_navigation_buttons();
    }

    // Create a pattern
    void SCLS_Documentalist_GUI::create_pattern(std::string name, std::string path) {
        path += "/" + name + "/";
        std::filesystem::create_directory(path);
        a_current_path = path;

        std::shared_ptr<scls::Pattern_Project> new_pattern = std::make_shared<scls::Pattern_Project>(name, path + "/" + name + ".sda");
        a_loaded_patterns.push_back(new_pattern);

        display_pattern_main(new_pattern.get());
        save_all();
    }

    // Create a pattern with the GUI datas
    void SCLS_Documentalist_GUI::create_pattern() {
        std::string name = a_create_pattern_name->plain_text();
        std::string path = a_create_pattern_path->plain_text();

        if(name != "" && !contains_loaded_pattern_by_name(name)) {
            create_pattern(name, path);
        }
    }

    // Create a replica
    void SCLS_Documentalist_GUI::create_replica(std::string name, std::string path) {
        path += "/" + name + "/";
        std::filesystem::create_directory(path);
        a_current_path = path;

        std::shared_ptr<scls::Replica_Project> new_project = std::make_shared<scls::Replica_Project>(name, a_currently_displayed_pattern);
        a_loaded_replicas.push_back(new_project);

        display_replica_main(new_project);
        save_replica_project(a_loaded_replicas[a_loaded_replicas.size() - 1]);
    }

    // Create a replica with the GUI datas
    void SCLS_Documentalist_GUI::create_replica() {
        std::string name = a_create_replica_name->plain_text();
        std::string path = a_create_replica_path->plain_text();

        if(name != "") {
            create_replica(name, path);
        }
    }

    // Load an unloaded pattern
    std::shared_ptr<scls::Pattern_Project>* SCLS_Documentalist_GUI::load_pattern(std::string path) {
        if(!std::filesystem::exists(path)) return 0;

        std::shared_ptr<scls::Pattern_Project> new_pattern; new_pattern.reset(scls::Pattern_Project::load_sda_0_1(path));
        path = scls::path_parent(path) + "/";
        a_current_path = path;

        a_loaded_patterns.push_back(new_pattern);
        return (&a_loaded_patterns[a_loaded_patterns.size() - 1]);
    }

    // Returns a loaded pattern by its name, or 0 if it does not exists
    scls::Pattern_Project* SCLS_Documentalist_GUI::loaded_pattern_by_name(std::string project_name) {
        for(int i = 0;i<static_cast<int>(a_loaded_patterns.size());i++) {
            if(a_loaded_patterns[i].get()->name() == project_name) return a_loaded_patterns[i].get();
        }
        return 0;
    }

    // Returns a loaded pattern by its path, or 0 if it does not exists
    scls::Pattern_Project* SCLS_Documentalist_GUI::loaded_pattern_by_path(std::string project_path) {
        for(int i = 0;i<static_cast<int>(a_loaded_patterns.size());i++) {
            if(a_loaded_patterns[i].get()->path() == project_path) return a_loaded_patterns[i].get();
        }
        return 0;
    }

    // Returns a loaded pattern shared point by its path, or 0 if it does not exists
    std::shared_ptr<scls::Pattern_Project>* SCLS_Documentalist_GUI::loaded_pattern_by_path_shared_point(std::string project_path) {
        for(int i = 0;i<static_cast<int>(a_loaded_patterns.size());i++) {
            if(a_loaded_patterns[i].get()->path() == project_path) return &(a_loaded_patterns[i]);
        }
        return 0;
    }

    // Open an existing pattern from a path
    void SCLS_Documentalist_GUI::open_pattern(std::string path) { if(!std::filesystem::exists(path)) return; display_pattern_main(load_pattern(path)->get()); }

    // Unload every projects
    void SCLS_Documentalist_GUI::unload_patterns() { a_loaded_patterns.clear(); }

    //*********
    //
    // Replica handling function
    //
    //*********

    // Create a replica file
    void SCLS_Documentalist_GUI::create_replica_file(std::string name, scls::Text_Pattern* pattern_file) {
        scls::Replica_Project* replica_project = a_currently_displayed_replica_project.get();
        scls::Replica_File* new_replica = replica_project->add_replica_file(name, pattern_file);

        if(new_replica != 0) {
            save_replica_project(a_currently_displayed_replica_project);
        }
    }

    // Create a replica file with the GUI datas
    void SCLS_Documentalist_GUI::create_replica_file() {
        std::string name = a_create_replica_file_name->plain_text();
        scls::Text_Pattern* pattern_file = a_currently_displayed_replica_project.get()->attached_pattern()->patterns()[0];

        if(name != "") {
            create_replica_file(name, pattern_file);
        }
    }

    // Export a project replica
    void SCLS_Documentalist_GUI::export_replica_project(const std::shared_ptr<scls::Replica_Project>& replica_project_to_export, std::string path) {
        if(replica_project_to_export.get() == 0) return;

        replica_project_to_export.get()->export_project(path);

        // GUI part
        display_replica_main(replica_project_to_export);
    }

    // Export a project replica with the GUI datas
    void SCLS_Documentalist_GUI::export_replica_project() {
        std::string path = a_replica_export_body_path->plain_text();

        export_replica_project(a_currently_displayed_replica_project, path);
    }

    // Load the buttons in the create replica file pattern navigation
    void SCLS_Documentalist_GUI::load_create_replica_file_pattern_navigation_buttons() {
        unload_create_replica_file_pattern_navigation_buttons();

        // Load each buttons
        scls::GUI_Text* last_current_button = 0;
        std::vector<scls::Text_Pattern*>& patterns = a_currently_displayed_replica_project.get()->attached_pattern()->patterns();
        for(int i = 0;i<static_cast<int>(patterns.size());i++) {
            scls::Text_Pattern* current_pattern = patterns[static_cast<int>(patterns.size()) - (i + 1)];
            scls::GUI_Text* current_button = a_create_replica_pattern_file->new_object_in_scroller<scls::GUI_Text>("create_replica_file_pattern_" + std::to_string(static_cast<int>(patterns.size()) - (i + 1)));
            current_button->set_font_size(75);
            current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            current_button->set_text(current_pattern->name().to_std_string());
            current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
            current_button->set_height_in_pixel(50);
            current_button->set_width_in_scale(scls::Fraction(1));
            current_button->move_left_in_parent();
            if(last_current_button == 0) current_button->move_bottom_in_parent();
            else  current_button->move_top_of_object_in_parent(last_current_button);
            current_button->calculate_adapted_scale(true);
            last_current_button = current_button;
        } //*/
        a_create_replica_pattern_file->check_scroller();
    }

    // Load the buttons in the replica navigation
    void SCLS_Documentalist_GUI::load_replica_navigation_buttons() {
        unload_replica_navigation_buttons();
        scls::Replica_Project* pattern_to_display = currently_displayed_replica();

        // Add the "project_home" button
        scls::GUI_Text* current_button = a_replica_navigation->new_object_in_scroller<scls::GUI_Text>("replica_navigation_home_button");
        current_button->set_font_size(75);
        current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        current_button->set_text(a_hud_text_content["replica_home"]);
        current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        current_button->set_height_in_pixel(75);
        current_button->set_width_in_scale(scls::Fraction(1));
        current_button->move_left_in_parent();
        current_button->move_bottom_in_parent(1); current_button->calculate_adapted_scale(true);
        a_replica_navigation_buttons.push_back(current_button);

        /*// Load each buttons
        scls::GUI_Text* last_current_button = current_button;
        std::vector<scls::Text_Pattern*>& patterns = a_currently_displayed_pattern->patterns();
        for(int i = 0;i<static_cast<int>(patterns.size());i++) {
            current_button = a_pattern_navigation->new_object<scls::GUI_Text>("project_navigation_button_" + std::to_string(i));
            current_button->set_font_size(75);
            current_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            current_button->set_text(patterns[i]->name().to_std_string());
            current_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
            current_button->set_height_in_scale(scls::Fraction(1, 10));
            current_button->set_width_in_scale(scls::Fraction(1));
            current_button->move_left_in_parent();
            current_button->move_bottom_of_object_in_parent(last_current_button);
            a_pattern_navigation_buttons.push_back(current_button); current_button->calculate_adapted_scale(true);
            last_current_button = current_button;
        } //*/

        a_replica_navigation->check_scroller();
    }

    // Open an existing replica from a path
    void SCLS_Documentalist_GUI::open_replica(std::string path) {
        if(!std::filesystem::exists(path)) return;

        // Get the informations about the pattern
        std::string pattern_path = scls::Replica_Project::replica_attached_pattern_from_path_sda_0_2(path);
        std::shared_ptr<scls::Pattern_Project>* needed_pattern = loaded_pattern_by_path_shared_point(pattern_path);
        if(needed_pattern == 0) { needed_pattern = load_pattern(pattern_path); }

        // Create the replica
        std::shared_ptr<scls::Replica_Project> new_replica; new_replica.reset(scls::Replica_Project::load_sda_0_2(path, *needed_pattern));
        path = scls::path_parent(path) + "/";
        a_current_path = path;

        a_loaded_replicas.push_back(new_replica);
        display_replica_main(new_replica);
    }

    // Save a project replica
    void SCLS_Documentalist_GUI::save_replica_project(const std::shared_ptr<scls::Replica_Project>& replica_project_to_save) {
        if(replica_project_to_save.get() == 0) return;

        replica_project_to_save.get()->save_sda_0_2(a_current_path);
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

            // Handle pattern creation
            if(a_welcome_footer_create_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_pattern();
            }
            else if(a_create_pattern_path_change->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
                a_current_file_to_be_chosen = _SCLS_CURRENT_FILE_TO_BE_CHOSEN_CREATE_PATTERN;
            }

            // Handle file explorer
            if(a_file_explorer->file_chosen()) {
                if(a_current_file_to_be_chosen == _SCLS_CURRENT_FILE_TO_BE_CHOSEN_CREATE_PATTERN) {
                    a_create_pattern_path->set_text(a_file_explorer->currently_selected_path());
                    display_create_pattern();
                }
                else if(a_current_file_to_be_chosen == _SCLS_CURRENT_FILE_TO_BE_CHOSEN_OPEN_PATTERN) {
                    open_pattern(a_file_explorer->currently_selected_path());
                }
                else if(a_current_file_to_be_chosen == _SCLS_CURRENT_FILE_TO_BE_CHOSEN_CREATE_REPLICA) {
                    a_create_replica_path->set_text(a_file_explorer->currently_selected_path());
                    display_create_replica();
                }
                else if(a_current_file_to_be_chosen == _SCLS_CURRENT_FILE_TO_BE_CHOSEN_OPEN_REPLICA) {
                    open_replica(a_file_explorer->currently_selected_path());
                }
                else if(a_current_file_to_be_chosen == _SCLS_CURRENT_FILE_TO_BE_CHOSEN_EXPORT_REPLICA) {
                    a_replica_export_body_path->set_text(a_file_explorer->currently_selected_path());
                    display_replica_export(a_currently_displayed_replica_project);
                }
            }

            // Handle pattern opening
            if(a_welcome_footer_open_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
                a_current_file_to_be_chosen = _SCLS_CURRENT_FILE_TO_BE_CHOSEN_OPEN_PATTERN;
            }

            if(a_create_pattern_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_pattern();
            }

            // Handle file pattern creation
            if(a_pattern_footer_create_file_pattern->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_file_pattern();
            }
            else if(a_create_file_pattern_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_file_pattern();
            }
            else if(a_pattern_footer_save_all->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                save_all();
            }

            // Handle replica creation
            if(a_pattern_footer_create_replica->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_replica();
            }
            if(a_create_replica_path_change->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
                a_current_file_to_be_chosen = _SCLS_CURRENT_FILE_TO_BE_CHOSEN_CREATE_REPLICA;
            }
            if(a_create_replica_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_replica();
            }
            if(a_replica_footer_create_file_replica->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_create_replica_file();
            }
            if(a_welcome_footer_open_replica->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
                a_current_file_to_be_chosen = _SCLS_CURRENT_FILE_TO_BE_CHOSEN_OPEN_REPLICA;
            }
            if(a_create_replica_file_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                create_replica_file();
            }

            // Handle replica export
            if(a_replica_footer_export->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                display_replica_export(a_currently_displayed_replica_project);
            }
            if(a_replica_export_body_validation->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                export_replica_project();
            }
            if(a_replica_export_body_path_change->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_file_explorer->set_current_user_document_directory();
                display_file_explorer();
                a_current_file_to_be_chosen = _SCLS_CURRENT_FILE_TO_BE_CHOSEN_EXPORT_REPLICA;
            }

            // Handle pattern navigation buttons
            if(a_pattern_navigation_buttons.size() > 0) {
                if(a_pattern_navigation_buttons[0]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                    display_pattern_main(a_currently_displayed_pattern.get());
                }
                else {
                    for(int i = 0;i<static_cast<int>(a_pattern_navigation_buttons.size());i++) {
                        if(a_pattern_navigation_buttons[i]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                            display_file_pattern(a_currently_displayed_pattern.get()->patterns()[i - 1]);
                        }
                    }
                }
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
