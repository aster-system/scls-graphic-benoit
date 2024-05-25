//******************
//
// scls_documentalist_gui.h
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
// This file contains the features for the mod "SCLS Documentalist GUI", adding a GUI to SCLS Documentalist "Agatha".
//
//******************
// Dev. part :
//
// This mod should allows the user to fully handle an SCLS Documentalist "Agatha" project.
//
// The welcome page allows the user to open or create a project.
//

#ifndef AGATHA_GUI
#define AGATHA_GUI

#include "../../scls_graphic.h"
#include "../../mods_hidden/scls_documentalist.h"

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace scls_documentalist_gui {
    class SCLS_Documentalist_GUI : public scls::Window {
        // Class representing the page for an SCLS Documentalist "Agatha" GUI
    public:
        // SCLS_Documentalist_GUI base constructor
        SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path);
        // SCLS_Documentalist_GUI base destructor
        virtual ~SCLS_Documentalist_GUI();

        //*********
        //
        // Processing functions
        //
        //*********

        // Run the gui
        void start();

        //*********
        //
        // Project handling function
        //
        //*********

        // Create a project with the GUI datas
        void create_project();
        // Create a project
        void create_project(std::string name, std::string path);

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Load the entire gui
        void load();
        // Load the create project body
        void load_create_project_body();
        // Load the file explorer
        void load_file_explorer();
        // Load the help body
        void load_help_body();
        // Load the help navigation
        void load_help_navigation();
        // Load the main header
        void load_main_header();
        // Load the welcome page footer
        void load_welcome_footer();

        // Load each language
        // Load english
        void load_language_en();
        // Load french
        void load_language_fr();
        // Reload each HUD text
        void reload_text();

        // Display each pages
        // Display the create project of the software
        void display_create_project();
        // Display the file explorer of the software
        void display_file_explorer();
        // Display the help part of the software
        void display_help();

        // Reset a page
        // Reset the create project page
        void reset_create_project_page();

        // Returns the parent object of the GUI
        inline scls::GUI_Object* parent_object() {return a_page->parent_object();};
    private:

        unsigned char a_current_file_to_be_chosen = 0;

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Main page of the GUI
        scls::GUI_Page* a_page = 0;

        // Body of the create project page
        // Parent page of the create project body
        scls::GUI_Object* a_create_project_body = 0;
        // Input of the name of the project
        scls::GUI_Text_Input* a_create_project_name = 0;
        // Title of the input of the name of the project
        scls::GUI_Text* a_create_project_name_title = 0;
        // Input of the path of the project
        scls::GUI_Text* a_create_project_path = 0;
        // Button to change the path of the project
        scls::GUI_Text* a_create_project_path_change = 0;
        // Title of the input of the path of the project
        scls::GUI_Text* a_create_project_path_title = 0;
        // Validation button of the create project body
        scls::GUI_Text* a_create_project_validation = 0;

        // Body of the file explorer
        // Parent page of the file explorer
        scls::GUI_File_Explorer* a_file_explorer = 0;

        // Body of the help
        // Parent page of the help body
        scls::GUI_Object* a_help_body = 0;
        // Home text of the help body
        scls::GUI_Text* a_help_body_home = 0;

        // Navigation for the help
        // Parent page of the navigation
        scls::GUI_Object* a_help_navigation = 0;
        // Home text of the help body
        scls::GUI_Text* a_help_navigation_home_button = 0;

        // Content of each HUD text in a specified language
        std::map<std::string, std::string> a_hud_text_content = std::map<std::string, std::string>();

        // Main header for the mains buttons
        // Parent page of the header
        scls::GUI_Object* a_main_header = 0;
        // File button of the header
        scls::GUI_Text* a_main_header_file_button = 0;

        // Footer of the welcome
        // Parent page of the welcome footer
        scls::GUI_Object* a_welcome_footer = 0;
        // Button of creation of project in the welcome footer
        scls::GUI_Text* a_welcome_footer_create_project = 0;
    };

    // Use scls_documentalist_gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path);
}

#endif // AGATHA_GUI
