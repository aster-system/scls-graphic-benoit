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
    // Page class
    //
    //*********

    class _Page {
        // Class representing a page in the window
    public:

        //*********
        //
        // _Page hiddens methods
        //
        //*********

        // _Page base constructor
        _Page(_Window_Advanced_Struct* window_struct, std::string name);
        // _Page base destructor
        virtual ~_Page();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline GLFWcursor* cursor() {return a_cursor;};
        inline std::string name() const {return a_name;};
        inline _Window_Advanced_Struct* window_struct() {return a_window_struct;};

        //*********
        //
        // _Page operating
        //
        //*********

        // Render the page
        virtual void render();
        // Soft reset the page
        virtual void soft_reset();
        // Update the page
        virtual void update();
        // Update the event of the page
        virtual void update_event(){soft_reset();};

        //*********
        //
        // Window page handling
        //
        //*********

        // Returns if the page contains an object or not
        inline bool contains_object(std::string object_name) {for(std::map<std::string, Object*>::iterator it = objects().begin();it!=objects().end();it++) if(it->first == object_name) return true; return false;};
        // Creates an object into the page and returns it
        template <typename O = Object>
        O* new_object(std::string object_name, Object* parent = 0, std::string object_texture = "");

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline std::vector<Object*>& children(){return a_children;};
        std::map<std::string, Object*>& objects() {return a_objects;};

    protected:

        //*********
        //
        // _Page hiddens attributes
        //
        //*********

        // Pointer to the cursor
        GLFWcursor* a_cursor = 0;

    private:

        //*********
        //
        // _Page hiddens attributes
        //
        //*********

        // Direct children of the page
        std::vector<Object*> a_children = std::vector<Object*>();
        // Objects in the page
        std::map<std::string, Object*> a_objects = std::map<std::string, Object*>();
        // Name of the page
        std::string a_name = "";
        // Pointer to the window struct
        _Window_Advanced_Struct* a_window_struct = 0;
    };

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
        void set_maximum_window_height(unsigned short new_max_window_height) { _Window_Base_Struct::set_maximum_window_height(new_max_window_height); resize_window(window_width(), window_height()); };
        void set_maximum_window_width(unsigned short new_max_window_width) { _Window_Base_Struct::set_maximum_window_width(new_max_window_width); resize_window(window_width(), window_height()); };
        void set_minimum_window_height(unsigned short new_min_window_height) { _Window_Base_Struct::set_minimum_window_height(new_min_window_height); resize_window(window_width(), window_height()); };
        void set_minimum_window_width(unsigned short new_min_window_width) { _Window_Base_Struct::set_minimum_window_width(new_min_window_width); resize_window(window_width(), window_height()); };

        //*********
        //
        // Window page handling
        //
        //*********

        // Create a new page to the Window and return it
        template <typename _P>
        _P* new_page(std::string name);

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool contains_page(std::string name) { for(std::map<std::string, _Page*>::iterator it = pages().begin();it!=pages().end();it++) if(it->first == name) return true; return false;};
        inline _Page* current_page() {
            if(current_page_name() != "" && contains_page(current_page_name())) return pages()[current_page_name()];
            return 0;
        }
        inline _Page* page(std::string name) {
            if(contains_page(name)) return pages()[name];
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to get does not exists.");
            return 0;
        };

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline std::string current_page_name() {return a_current_page;};
        inline bool is_resize_possible() {return a_is_resize_possible;};
        inline std::map<std::string, _Page*>& pages() {return a_pages;};
        inline void set_current_page(std::string new_current_page) {
            if(new_current_page != "" && !contains_page(new_current_page)) scls::print("Warning", "SCLS Window", "The \"" + new_current_page + "\" page you want to set as the current page does not exists.");
            else a_current_page = new_current_page;
        };
        inline void set_is_resize_possible(bool new_is_resize_possible) {a_is_resize_possible = new_is_resize_possible;resize_window(window_width(), window_height());};

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
        bool a_is_resize_possible = false;
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

        // Name of the current page loaded
        std::string a_current_page = "";
        // Map containing each pages in the window with their name as key
        std::map<std::string, _Page*> a_pages = std::map<std::string, _Page*>();

        //*********
        //
        // Window operating
        //
        //*********

        float a_last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
    };

    //*********
    //
    // _Page template
    //
    //*********

    // Creates an object into the page and returns it
    template <typename O>
    O* _Page::new_object(std::string object_name, Object* parent, std::string object_texture) {
        if(contains_object(object_name)) {
            scls::print("Warning", "SCLS Page", "The \"" + object_name + "\" object you want to add in the page \"" + name() + "\" already exist.");
            return 0;
        }

        if(object_texture == "") {
            object_texture = object_name + "_texture";
            window_struct()->new_texture(object_texture);
        }

        O* object = new O(window_struct(), object_name, object_texture);
        objects()[object_name] = object;

        if(parent == 0) {
            children().push_back(object);
        }

        return object;
    }

    //*********
    //
    // Window template
    //
    //*********

    // Create a new page to the Window and return it
    template <typename _P>
    _P* Window::new_page(std::string name) {
        if(contains_page(name)) {
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to add in the window already exist.");
            return 0;
        }

        _P* page = new _P(this, name);
        pages()[name] = page;

        return page;
    }
}

#endif // SCLS_WINDOW
