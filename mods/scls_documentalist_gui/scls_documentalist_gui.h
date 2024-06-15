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

        // Returns if the program contains a loaded pattern by its name
        bool contains_loaded_pattern_by_name(std::string project_name);
        // Returns if the program contains an opened file pattern
        bool contains_opened_file_pattern(scls::Text_Pattern* pattern_to_test);
        // Create a file pattern with the GUI datas
        void create_file_pattern();
        // Create a file pattern
        void create_file_pattern(std::string name);
        // Create a pattern with the GUI datas
        void create_pattern();
        // Create a pattern
        void create_pattern(std::string name, std::string path);
        // Returns a loaded pattern by its name, or 0 if it does not exists
        scls::Project* loaded_pattern_by_name(std::string project_name);
        // Open an existing pattern from a path
        void open_pattern(std::string path);
        // Save all the project
        void save_all();
        // Unload every patterns
        void unload_patterns();

        // Getters and setters
        inline scls::Project* currently_displayed_pattern() const { return a_currently_displayed_pattern; };

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Function called after that the window has been resized
        virtual void after_window_resizing(glm::vec2 last_size){scls::Window::after_window_resizing(last_size);place_all();};
        // Load the entire gui
        void load();
        // Load the main header
        void load_main_header();
        // Load bodies
        // Load the create file pattern body
        void load_create_file_pattern_body();
        // Load the create pattern body
        void load_create_pattern_body();
        // Load the file explorer
        void load_file_explorer();
        // Load the file pattern
        void load_file_pattern_body();
        // Load the help body
        void load_help_body();
        // Load the pattern main body
        void load_pattern_main_body();
        // Load navigations
        // Load the help navigation
        void load_help_navigation();
        // Load the pattern navigation
        void load_pattern_navigation();
        // Load the buttons in the pattern navigation
        void load_pattern_navigation_buttons();
        // Load the footers
        // Load the pattern footer
        void load_pattern_footer();
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
        // Display the create file pattern page of the software
        void display_create_file_pattern();
        // Display the create pattern of the software
        void display_create_pattern();
        // Display the file explorer of the software
        void display_file_explorer();
        // Display a file pattern of a project
        void display_file_pattern(scls::Text_Pattern* pattern_to_display);
        // Display the help part of the software
        void display_help();
        // Display the main page of a pattern
        void display_pattern_main(scls::Project* project_to_display);

        // Reset a page
        // Place all the objects in the program
        void place_all();
        // Reset the create pattern page
        void reset_create_pattern_page();
        // Unload the buttons in the pattern navigation
        void unload_pattern_navigation_buttons();
        // Unset all the pattern
        void unset_all();

        // Returns the parent object of the GUI
        inline scls::GUI_Object* parent_object() {return a_page->parent_object();};
    private:

        // Current path of the program
        std::string a_current_path = "";
        // Currently displayed file_pattern
        scls::Text_Pattern* a_currently_displayed_file_pattern = 0;
        // Currently displayed pattern
        scls::Project* a_currently_displayed_pattern = 0;
        // Every loaded projects
        std::vector<scls::Project*> a_loaded_patterns = std::vector<scls::Project*>();
        // Every opened patterns files
        std::map<scls::Text_Pattern*, std::string> a_opened_files_pattern = std::map<scls::Text_Pattern*, std::string>();
        // Buttons in the pattern navigation
        std::vector<scls::GUI_Object*> a_pattern_navigation_buttons = std::vector<scls::GUI_Object*>();

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Current state to know how to use the chosen file with the file explorer
        unsigned char a_current_file_to_be_chosen = 0;

        // Main page of the GUI
        scls::GUI_Page* a_page = 0;

        // Body of the create file pattern page
        // Parent page of the create file pattern body
        scls::GUI_Object* a_create_file_pattern_body = 0;
        // Input of the name of the file pattern
        scls::GUI_Text_Input* a_create_file_pattern_name = 0;
        // Title of the input of the name of the file pattern
        scls::GUI_Text* a_create_file_pattern_name_title = 0;
        // Validate the creation of the file pattern
        scls::GUI_Text* a_create_file_pattern_validation = 0;

        // Body of the create pattern page
        // Parent page of the create pattern body
        scls::GUI_Object* a_create_pattern_body = 0;
        // Input of the name of the pattern
        scls::GUI_Text_Input* a_create_pattern_name = 0;
        // Title of the input of the name of the pattern
        scls::GUI_Text* a_create_pattern_name_title = 0;
        // Input of the path of the pattern
        scls::GUI_Text* a_create_pattern_path = 0;
        // Button to change the path of the pattern
        scls::GUI_Text* a_create_pattern_path_change = 0;
        // Title of the input of the path of the pattern
        scls::GUI_Text* a_create_pattern_path_title = 0;
        // Validation button of the create pattern body
        scls::GUI_Text* a_create_pattern_validation = 0;

        // Body of the file explorer
        // Parent page of the file explorer
        scls::GUI_File_Explorer* a_file_explorer = 0;

        // Body of the file pattern page
        // Parent page of the file pattern body
        scls::GUI_Object* a_file_pattern_body = 0;
        // Text in the file pattern
        scls::GUI_Text_Input* a_file_pattern_text = 0;

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

        // Footer of the pattern
        // Parent page of the pattern footer
        scls::GUI_Object* a_pattern_footer = 0;
        // Button to create a pattern file in the pattern footer
        scls::GUI_Text* a_pattern_footer_create_file_pattern = 0;
        // Button to save all in the pattern footer
        scls::GUI_Text* a_pattern_footer_save_all = 0;

        // Navigation of the pattern
        // Parent page of the pattern navigation
        scls::GUI_Object* a_pattern_navigation = 0;

        // Body of the pattern main
        // Parent page of the pattern main body
        scls::GUI_Object* a_pattern_main_body = 0;
        // Home text of the pattern main body
        scls::GUI_Text* a_pattern_main_body_title = 0;

        // Footer of the welcome
        // Parent page of the welcome footer
        scls::GUI_Object* a_welcome_footer = 0;
        // Button of creation of pattern in the welcome footer
        scls::GUI_Text* a_welcome_footer_create_pattern = 0;
        // Button to open a pattern in the welcome footer
        scls::GUI_Text* a_welcome_footer_open_pattern = 0;
    };

    // Use scls_documentalist_gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path);
}

#endif // AGATHA_GUI
