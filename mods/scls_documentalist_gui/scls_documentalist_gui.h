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
    struct _SCLS_Documentalist_GUI_File {
        // Struct representing a loaded file in SCLS_Documentalist_GUI
        // Base project of the file
        scls::Project* base_project = 0;
        // Text in the file
        std::string text = "";
    };

    class SCLS_Documentalist_GUI : public scls::Window {
        // Class representing the page for an SCLS Documentalist "Agatha" GUI
    public:
        // SCLS_Documentalist_GUI base constructor
        SCLS_Documentalist_GUI(int window_width, int window_height, std::string exec_path);
        // SCLS_Documentalist_GUI base destructor
        virtual ~SCLS_Documentalist_GUI();

        // Check if a project can be created
        void check_project_creation();
        // Check if a file pattern in a project can be created
        void check_project_file_pattern_creation();
        // Clear the memory from the loaded projects
        inline void clear_memory_from_loaded_projects() {for(std::map<std::string, scls::Project*>::iterator it = loaded_projects().begin();it!=loaded_projects().end();it++){delete it->second;it->second=0;}loaded_projects().clear();};
        // Create a new project
        void create_project(std::string project_name);
        // Create a new file pattern in the currently displayed project
        void create_project_file_pattern(std::string file_name, scls::Project* project);
        // Load a project in the GUI
        void load_project(scls::Project* project_to_load);
        // Load a project in the GUI with its name
        inline void load_project(std::string name) {
            scls::Project* project_to_load = loaded_project(name);
            if(project_to_load != 0) load_project(project_to_load);
        };
        // Load a file pattern of a project in the GUI
        void load_project_file_pattern(std::string file_pattern);
        // Load the home page of a project in the GUI
        void load_project_home(scls::Project* project_to_load);
        // Load the home page of a project in the GUI with its name
        inline void load_project_home(std::string name) {
            scls::Project* project_to_load = loaded_project(name);
            if(project_to_load != 0) load_project_home(project_to_load);
        };
        // Load the navigation buttons of the project
        void load_navigation_project_buttons();
        // Save the loaded file pattern
        void save_loaded_file_pattern();
        // Save the entire loaded project
        void save_loaded_project();
        // Set a project file pattern body
        void set_project_file_pattern_body(std::string file_name, scls::Project* project);

        // Returns if the gui contains a loaded file in it
        inline bool contains_loaded_file(std::string file_name) {for(std::map<std::string, _SCLS_Documentalist_GUI_File>::iterator it = loaded_files().begin();it!=loaded_files().end();it++){if(it->first == file_name){return true;}}return false;};
        // Return if the gui contains a project by its name
        inline bool contains_loaded_project(std::string project_name) {for(std::map<std::string, scls::Project*>::iterator it = loaded_projects().begin();it!=loaded_projects().end();it++){if(it->first == project_name){return true;}}return false;};
        // Return a project by its name
        inline scls::Project* loaded_project(std::string project_name) {for(std::map<std::string, scls::Project*>::iterator it = loaded_projects().begin();it!=loaded_projects().end();it++){if(it->first == project_name){return it->second;}}return 0;};

        // Getters and setters
        inline std::string currently_displayed_file_pattern() const {return a_currently_displayed_file_pattern;};
        inline scls::Project* currently_displayed_project() {return a_currently_displayed_project;};
        inline std::string currently_displayed_project_name() {return currently_displayed_project()->name();};
        inline std::map<std::string, _SCLS_Documentalist_GUI_File>& loaded_files() {return a_loaded_files;};
        inline std::map<std::string, scls::Project*>& loaded_projects() {return a_loaded_projects;};
        inline void set_currently_dispalyed_project(scls::Project* project_to_display) {a_currently_displayed_project = project_to_display;load_project(project_to_display);};

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Load the entire gui
        void load();
        // Load the page to create a project
        void load_create_project_body();
        // Load the page to create a file pattern in a project
        void load_create_project_file_pattern_body();
        // Load the welcome page body
        void load_help_body();
        // Load the help navigation
        void load_help_navigation();
        // Load the main header
        void load_main_header();
        // Load the project page body
        void load_project_body();
        // Load a file pattern of a project
        void load_project_file_pattern();
        // Load the project page footer
        void load_project_footer();
        // Load the project page navigation
        void load_project_navigation();
        // Load the needed textures
        void load_textures();
        // Load the welcome page footer
        void load_welcome_page_footer();

        // Load each language
        // Load english
        void load_language_en();
        // Load french
        void load_language_fr();
        // Reload each HUD text
        void reload_text();

        // Set each pages
        // Set the create project page
        void set_create_project_body();
        // Set the create file pattern in project page
        void set_create_project_file_pattern_body();
        // Unset all the pages in the window
        void unset_all();

        // Run the gui
        void start();
    private:

        // Currently displayed file pattern
        std::string a_currently_displayed_file_pattern = "";
        // Currently displayed project
        scls::Project* a_currently_displayed_project = 0;
        // Loaded pattern files
        std::map<std::string, _SCLS_Documentalist_GUI_File> a_loaded_files = std::map<std::string, _SCLS_Documentalist_GUI_File>();
        // All the loaded project in the GUI
        std::map<std::string, scls::Project*> a_loaded_projects = std::map<std::string, scls::Project*>();
        // Project files pattern button
        std::vector<scls::HUD_Text*> a_project_navigation_file_pattern_button = std::vector<scls::HUD_Text*>();
        // Project main button
        scls::HUD_Text* a_project_navigation_home_button = 0;

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Body of the page to create a project
        // Parent page of the page to create a project
        scls::HUD_Page* a_create_project_body = 0;
        // Button of validation of creation of project in the page to create a project
        scls::HUD_Text* a_create_project_body_create = 0;
        // Title of the name of the page to create a project
        scls::HUD_Text* a_create_project_body_name_title = 0;
        // Input of the name of the page to create a project
        scls::HUD_Text_Input* a_create_project_body_name = 0;

        // Body of the page to create a file pattern in a project
        // Parent page of the page to create a file pattern in a project
        scls::HUD_Page* a_create_project_file_pattern_body = 0;
        // Button of validation of creation of project in the page to create a file pattern in a project
        scls::HUD_Text* a_create_project_file_pattern_body_create = 0;
        // Title of the name of the page to create a file pattern in a project
        scls::HUD_Text* a_create_project_file_pattern_body_name_title = 0;
        // Input of the name of the page to create a file pattern in a project
        scls::HUD_Text_Input* a_create_project_file_pattern_body_name = 0;

        // Body of the help
        // Parent page of the help body
        scls::HUD_Page* a_help_body = 0;
        // Home text of the help body
        scls::HUD_Text* a_help_body_home = 0;
        // Description text of the home part of the help body
        scls::HUD_Text* a_help_body_home_description = 0;
        // Part text of the home part of the help body
        scls::HUD_Text* a_help_body_home_part = 0;

        // Body of the project
        // Parent page of the project body
        scls::HUD_Page* a_project_body = 0;

        // Body of a project file pattern
        // Parent page of a project file pattern
        scls::HUD_Page* a_project_file_pattern_body = 0;
        // Content of a project file pattern
        scls::HUD_Text_Input* a_project_file_pattern_content = 0;

        // Footer of the project
        // Parent page of the project footer
        scls::HUD_Page* a_project_footer = 0;
        // Button of creation of file in the project footer
        scls::HUD_Text* a_project_footer_create_file_pattern = 0;
        // Button of saving all the project
        scls::HUD_Text* a_project_footer_save_all = 0;
        // Button of saving of a file in the project footer
        scls::HUD_Text* a_project_footer_save_file_pattern = 0;

        // Navigation of the project
        // Parent page of the project Navigation
        scls::HUD_Page* a_project_navigation = 0;

        // Navigation for the help
        // Parent page of the navigation
        scls::HUD_Page* a_help_navigation = 0;
        // Home text of the help body
        scls::HUD_Text* a_help_navigation_home_button = 0;

        // Content of each HUD text in a specified language
        std::map<std::string, std::string> a_hud_text_content = std::map<std::string, std::string>();

        // Main header for the mains buttons
        // Parent page of the header
        scls::HUD_Page* a_main_header = 0;
        // File button of the header
        scls::HUD_Text* a_main_header_file_button = 0;

        // Footer of the welcome
        // Parent page of the welcome footer
        scls::HUD_Page* a_welcome_footer = 0;
        // Button of creation of project in the welcome footer
        scls::HUD_Text* a_welcome_footer_create_project = 0;
    };

    // Use scls_documentalist_gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path);
}

#endif // AGATHA_GUI
