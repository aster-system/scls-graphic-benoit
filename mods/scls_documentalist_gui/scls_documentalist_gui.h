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

        // Load the entire gui
        void load();
        // Load the welcome page body
        void load_help_body();
        // Load the help navigation
        void load_help_navigation();
        // Load the main header
        void load_main_header();
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

        // Run the gui
        void start();
    private:
        // Body of the help
        // Parent page of the help body
        scls::HUD_Page* a_help_body = 0;
        // Welcome text of the help body
        scls::HUD_Text* a_help_body_welcome = 0;
        // Description text of the welcome part of the help body
        scls::HUD_Text* a_help_body_welcome_description = 0;
        // Part text of the welcome part of the help body
        scls::HUD_Text* a_help_body_welcome_part = 0;

        // Navigation for the help
        // Parent page of the navigation
        scls::HUD_Page* a_help_navigation = 0;

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
    };

    // Use scls_documentalist_gui easily with a function
    void use_scls_documentalist_gui(std::string exec_path);
}

#endif // AGATHA_GUI
