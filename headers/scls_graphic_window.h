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

#include "../headers/scls_graphic_window_advanced_struct.h"
#include "../headers/scls_graphic_window_base_struct.h"
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

    //*********
    //
    // Window class
    //
    //*********

    class Window: public _Window_Advanced_Struct {
        // Class representing the window
    public:

        //*********
        //
        // Window main functions
        //
        //*********

        // Window base constructor
        Window(int window_width, int window_height, std::string exec_path);
        // Window base destructor
        virtual ~Window();

        // Load the game from a config file
        void load_from_config_file(std::string path);
        // Return if the window is running or not
        bool run();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Color background_color() { return a_background_color; };
        inline bool is_running() {return a_is_running;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
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
        void set_maximum_window_height(unsigned short new_max_window_height) {
            _Window_Base_Struct::set_maximum_window_height(new_max_window_height);
            resize_window(window_width(), window_height());
        };
        void set_maximum_window_width(unsigned short new_max_window_width) {
            _Window_Base_Struct::set_maximum_window_width(new_max_window_width);
            resize_window(window_width(), window_height());
        };
        void set_minimum_window_height(unsigned short new_min_window_height) {
            _Window_Base_Struct::set_minimum_window_height(new_min_window_height);
            resize_window(window_width(), window_height());
        };
        void set_minimum_window_width(unsigned short new_min_window_width) {
            _Window_Base_Struct::set_minimum_window_width(new_min_window_width);
            resize_window(window_width(), window_height());
        };

        //*********
        //
        // Window page handling
        //
        //*********

        // Create a new page to the Window and return it
        template <typename _P>
        _P* new_page(std::string name);

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool contains_page(std::string name) { for(std::map<std::string, Object*>::iterator it = pages().begin();it!=pages().end();it++) if(it->first == name) return true; return false;};
        inline bool contains_displayed_page(std::string name) { for(int i = 0;i<static_cast<int>(displayed_pages_names().size());i++) { if(displayed_pages_names()[i] == name) return true;} return false;};
        inline std::vector<Object*> displayed_pages() {
            std::vector<Object*> to_return = std::vector<Object*>();
            if(displayed_pages_names().size() > 0) {
                for(int i = 0;i<static_cast<int>(displayed_pages_names().size());i++) {
                    to_return.push_back(pages()[displayed_pages_names()[i]]);
                }
            }
            return to_return;
        };
        inline void display_page(std::string new_page) {
            if(new_page != "" && !contains_page(new_page)) scls::print("Warning", "SCLS Window", "The \"" + new_page + "\" page you want to display does not exists.");
            else if(!contains_displayed_page(new_page)) displayed_pages_names().push_back(new_page);
        };
        inline void hide_all_pages() {
            for(int i = 0;i<static_cast<int>(displayed_pages_names().size());i++) {
                pages()[displayed_pages_names()[i]]->unload();
            }
            displayed_pages_names().clear();
        };
        inline Object* page(std::string name) {
            if(contains_page(name)) return pages()[name];
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to get does not exists.");
            return 0;
        };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline std::vector<std::string>& displayed_pages_names() {return a_displayed_pages;};
        inline bool is_resize_possible() {return a_is_resize_possible;};
        inline std::map<std::string, Object*>& pages() {return a_pages;};
        inline void set_is_resize_possible(bool new_is_resize_possible) {a_is_resize_possible = new_is_resize_possible;resize_window(window_width(), window_height());};

        //*********
        //
        // Window operating
        //
        //*********

        // Function called after that the window has been resized
        virtual void after_window_resizing(glm::vec2 last_size){apply_window_resizing(last_size);};
        // Hidden function to call the children that there has been a resizing
        inline void apply_window_resizing(glm::vec2 last_size){for(std::map<std::string, Object*>::iterator it = pages().begin(); it != pages().end(); it++) {it->second->after_window_resizing(last_size);}};
        // Render the scene
        virtual void render();
        // Update one frame of the game
        virtual void update();
        // Update the event of the game during this frame
        virtual void update_event();

    private:

        //*********
        //
        // Window main attributes
        //
        //*********

        // Basics Window descriptors
        // Background color of the window
        Color a_background_color = white;
        // Pointer to the GLFW window
        GLFWwindow* a_window = 0;
        // If the window can resize or not or not
        bool a_is_resize_possible = true;
        // If the window should continue to run or not
        bool a_is_running = true;

        //*********
        //
        // Window input handling
        //
        //*********

        // Map of each keys in the window, with their character as the value
        std::map<std::string, unsigned int> a_keys = std::map<std::string, unsigned int>();

        //*********
        //
        // Window page handling
        //
        //*********

        // Names of the displayed page
        std::vector<std::string> a_displayed_pages = std::vector<std::string>();
        // Map containing each pages in the window with their name as key
        std::map<std::string, Object*> a_pages = std::map<std::string, Object*>();

        //*********
        //
        // Window operating
        //
        //*********

        float a_last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
        // Last height of the window
        unsigned short a_last_window_height = 0;
        // Last width of the window
        unsigned short a_last_window_width = 0;
    };

    //*********
    //
    // Window template
    //
    //*********

    // Create a new page to the Window and return it
    template <typename O>
    O* Window::new_page(std::string name) {
        if(contains_page(name)) {
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to add in the window already exist.");
            return 0;
        }

        O* page = new O(this, name);
        pages()[name] = page;

        return page;
    }
}

#endif // SCLS_WINDOW
