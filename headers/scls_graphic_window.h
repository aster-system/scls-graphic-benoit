//******************
//
// scls_graphic_window.h
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
// This file contains the features allowing to display the window on the screen
//

#ifndef SCLS_WINDOW
#define SCLS_WINDOW

#include "../headers/advanced_struct.h"
#include "../headers/base_struct.h"
#include "../headers/model.h"
#include "scls_graphic_object.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
    //
    // Window input handling hiddens functions
    //
    //*********

    // Callback function for cursor enter in the window
    void _cursor_enter_callback(GLFWwindow* window, int entered);

    // Callback function for window resizing
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Callback function for mouse moving
    void _mouse_callback(GLFWwindow* window, double xpos, double ypos);

    class Window: public Advanced_Struct {
        // Class representing the game
    public:

        //*********
        //
        // Window main functions
        //
        //*********

        // Window base constructor
        Window(int window_width, int window_height, std::string exec_path);
        // Window base destructor
        ~Window();

        // Load the game from a config file
        void load_from_config_file(std::string path);
        // Return if the window is running or not
        bool run();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool contains_page(std::string name) { for(std::map<std::string, Object*>::iterator it = pages().begin();it!=pages().end();it++) if(it->first == name) return true; return false;};
        inline Object* current_page() {
            if(current_page_name() != "" && contains_page(current_page_name())) return pages()[current_page_name()];
            return 0;
        }
        inline Object* page(std::string name) {
            if(contains_page(name)) return pages()[name];
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to get does not exists.");
            return 0;
        };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Color background_color() { return a_background_color; };
        inline std::string current_page_name() {return a_current_page;};
        inline bool is_running() {return a_is_running;};
        inline std::map<std::string, Object*>& pages() {return a_pages;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_current_page(std::string new_current_page) {
            if(new_current_page != "" && !contains_page(new_current_page)) scls::print("Warning", "SCLS Window", "The \"" + new_current_page + "\" page you want to set as the current page does not exists.");
            else a_current_page = new_current_page;
        };
        inline void set_is_running(bool new_is_runnig) { a_is_running = new_is_runnig; };
        inline GLFWwindow* window() {return a_window;};

        //*********
        //
        // Window input handling
        //
        //*********

        // Load the keys of the window
        void _load_keys();
        // Disable the cursor from the game
        inline void hide_cursor() { glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); _cursor_state = GLFW_CURSOR_DISABLED; };
        // Properly resize the window
        void resize_window(unsigned int width, unsigned int height);
        // Enable the cursor from the game
        inline void show_cursor() { glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); _cursor_state = GLFW_CURSOR_NORMAL; };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned long current_cursor() const {return a_current_cursor;};
        inline bool is_cursor_on_window() { return a_cursor_on_window; };

        //*********
        //
        // Window operating
        //
        //*********

        // Render the scene
        virtual void render();
        // Update one frame of the game
        virtual void update();
        // Update the event of the game during this frame
        virtual void update_event();

    private:

        // Basics Window descriptors
        // Background color of the window
        Color a_background_color = white;
        // Pointer to the GLFW window
        GLFWwindow* a_window = 0;

        // Page handling
        // Name of the current page loaded
        std::string a_current_page = "";
        // If the window should continue to run or not
        bool a_is_running = true;
        // Map containing each pages in the window with their name as key
        std::map<std::string, Object*> a_pages = std::map<std::string, Object*>();

        //*********
        //
        // Window input handling
        //
        //*********

        // Current displayed cursor
        unsigned long a_current_cursor = GLFW_ARROW_CURSOR;
        // Pointer to the cursor
        GLFWcursor* a_cursor = 0;
        // Reference to the cursor_on_window bool
        bool &a_cursor_on_window;
        // Map of each keys in the window, with their character as the value
        std::map<std::string, unsigned int> a_keys = std::map<std::string, unsigned int>();

        //*********
        //
        // Window operating
        //
        //*********

        float a_last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
    };
}

#endif // SCLS_WINDOW
