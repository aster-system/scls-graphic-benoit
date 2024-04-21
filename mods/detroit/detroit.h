//******************
//
// detroit.h
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
// This file is the header of the "Detroit" mod, made to test the library, with a school presentation about the city of Detroit.
//

#ifndef EFA_MOD
#define EFA_MOD

#include "../../scls_graphic.h"

// Use the "efa" namespace to simplify the using of the library
namespace detroit {
    class Detroit {
        // Class representing the main window used in the mod.
    public:
        // Simplest Detroit constructor
        Detroit(std::string path);
        // Detroit destructor
        ~Detroit();

        // Executes a frame in the simulation
        void frame();

        // Load each pages in the object the object
        void load_pages();
        // Set the big map page in the object
        void set_big_map_page();
        // Set the detroit farm page in the object
        void set_detroit_farm_page();
        // Set the detroit history page with the numbers part in the object
        void set_detroit_history_numbers();
        // Set the detroit history page in the object
        void set_detroit_history_page();
        // Set the detroit history page with the statistic part in the object
        void set_detroit_history_statistic();
        // Set the detroit map page in the object
        void set_detroit_map_page();
        // Set the video page in the object
        void set_video_page();
        // Set the welcome page in the object
        void set_welcome_page();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool run() {return window()->run();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned int current_presentation_state() {return a_current_presentation_state;};
        inline scls::Window* window() {return a_window;};
    private:
        // Current state of the presentation
        unsigned int a_current_presentation_state = 0;

        // Window attributes
        // Pointer to the main Window
        scls::Window* a_window = 0;

        // Create and configure the welcome_page page
        scls::HUD_Page* welcome_page = 0;
        scls::HUD_Object* welcome_page_background = 0;
        scls::HUD_Text* welcome_page_start = 0;
        scls::HUD_Object* welcome_page_uk_flag = 0;
        scls::HUD_Object* welcome_page_usa_flag = 0;
        scls::HUD_Text* welcome_page_title = 0;
        // Create and configure the navigation map
        scls::HUD_Object* navigation_page_background = 0;
        scls::HUD_Text* navigation_page_big_map_button = 0;
        scls::HUD_Text* navigation_page_detroit_farm_button = 0;
        scls::HUD_Text* navigation_page_detroit_history_button = 0;
        scls::HUD_Text* navigation_page_detroit_map_button = 0;
        scls::HUD_Text* navigation_page_video_button = 0;
        // Create and configure the detroit_history page
        scls::HUD_Object* detroit_history_page_background = 0;
        scls::HUD_Object* detroit_history_page_1950 = 0;
        scls::HUD_Text* detroit_history_page_1950_african_american = 0;
        scls::HUD_Text* detroit_history_page_1950_obesity = 0;
        scls::HUD_Text* detroit_history_page_1950_population = 0;
        scls::HUD_Text* detroit_history_page_1950_title = 0;
        scls::HUD_Object* detroit_history_page_2020 = 0;
        scls::HUD_Text* detroit_history_page_2020_african_american = 0;
        scls::HUD_Text* detroit_history_page_2020_farm = 0;
        scls::HUD_Text* detroit_history_page_2020_obesity = 0;
        scls::HUD_Text* detroit_history_page_2020_population = 0;
        scls::HUD_Text* detroit_history_page_2020_production = 0;
        scls::HUD_Text* detroit_history_page_2020_title = 0;
        scls::HUD_Text* detroit_history_page_2020_size = 0;
        scls::HUD_Text* detroit_history_page_2020_vacant = 0;
        scls::HUD_Text* detroit_history_settings_page_number = 0;
        scls::HUD_Text* detroit_history_settings_page_statistic = 0;
    };

    // Create a simple Detroit window
    void use_detroit(std::string path);
}

#endif // WISIM
