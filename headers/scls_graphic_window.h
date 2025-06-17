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

#include "scls_graphic_gui_object.h"
#include <stack>

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
    //
    // Hiddens callback functions
    //
    //*********

    // Callback function for cursor enter in the window
    void _cursor_enter_callback(GLFWwindow* window, int entered);

    // Callback function for window resizing
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // Callback function for mouse moving
    void _mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // Callback function for mouse wheel scrolling
    void _mouse_wheel_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    //*********
    //
    // Window class
    //
    //*********

    // Balises for a Window loading
    extern std::shared_ptr<__Balise_Container> window_loading_balises;

    struct Window_Loader : public __XML_Loader {
        // Class containing datas about a loaded window

        // Window_Loader constructor
        Window_Loader(const std::string& file_content) : __XML_Loader(file_content) {};

        // Created objects by name
        std::map<std::string, std::shared_ptr<Object>> created_objects = std::map<std::string, std::shared_ptr<Object>>();
        // Created objects in a stack
        std::stack<std::shared_ptr<Object>> created_objects_stack = std::stack<std::shared_ptr<Object>>();
    };

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
        inline void set_window_title(std::string new_window_title) {a_window_title = new_window_title;glfwSetWindowTitle(window(), new_window_title.c_str());};
        inline GLFWwindow* window() {return a_window;};
        inline std::string window_title() {return a_window_title;};

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

        // Create a new 2D page to the Window and return it
        template <typename _P>
        std::shared_ptr<_P>* new_page_2d(std::string page_name);
        // Create a new 3D page to the Window and return it
        template <typename _P>
        std::shared_ptr<_P>* new_page_3d(std::string page_name);

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool contains_page_2d(std::string name) { for(std::map<std::string, std::shared_ptr<Object>>::iterator it = pages_2d().begin();it!=pages_2d().end();it++) if(it->first == name) return true; return false;};
        inline bool contains_page_3d(std::string name) { for(std::map<std::string, std::shared_ptr<Object>>::iterator it = pages_3d().begin();it!=pages_3d().end();it++) if(it->first == name) return true; return false;};
        inline bool contains_displayed_page_2d(std::string name) { for(int i = 0;i<static_cast<int>(displayed_pages_2d_names().size());i++) { if(displayed_pages_2d_names()[i] == name) return true;} return false;};
        inline bool contains_displayed_page_3d(std::string name) { for(int i = 0;i<static_cast<int>(displayed_pages_3d_names().size());i++) { if(displayed_pages_3d_names()[i] == name) return true;} return false;};
        inline std::vector<std::shared_ptr<Object>> displayed_pages_2d() {
            std::vector<std::shared_ptr<Object>> to_return = std::vector<std::shared_ptr<Object>>();
            if(displayed_pages_2d_names().size() > 0) {
                for(int i = 0;i<static_cast<int>(displayed_pages_2d_names().size());i++) {
                    to_return.push_back(pages_2d()[displayed_pages_2d_names()[i]]);
                }
            }
            return to_return;
        };
        inline std::vector<std::shared_ptr<Object>> displayed_pages_3d() {std::vector<std::shared_ptr<Object>> to_return = std::vector<std::shared_ptr<Object>>();if(displayed_pages_3d_names().size() > 0) {for(int i = 0;i<static_cast<int>(displayed_pages_3d_names().size());i++) {to_return.push_back(pages_3d()[displayed_pages_3d_names()[i]]);}}return to_return;};
        virtual void display_page_2d(std::string new_page_2d) {if(new_page_2d != "" && !contains_page_2d(new_page_2d)){scls::print("Warning", "SCLS Window", "The \"" + new_page_2d + "\" 2D page you want to display does not exists.");}else if(!contains_displayed_page_2d(new_page_2d)){displayed_pages_2d_names().push_back(new_page_2d);page_2d(new_page_2d)->get()->after_displaying();a_displayed_pages_2d_modified_during_this_frame=true;}};
        inline void display_page_2d(std::shared_ptr<Object> new_page_2d){display_page_2d(new_page_2d.get()->name());};
        inline void display_page_3d(std::string new_page_3d) {if(new_page_3d != "" && !contains_page_3d(new_page_3d)){scls::print("Warning", "SCLS Window", "The \"" + new_page_3d + "\" 3D page you want to display does not exists.");}else if(!contains_displayed_page_3d(new_page_3d)){displayed_pages_3d_names().push_back(new_page_3d);}};
        virtual void hide_all_pages_2d() {for(int i = 0;i<static_cast<int>(displayed_pages_2d_names().size());i++) {pages_2d()[displayed_pages_2d_names()[i]].get()->after_hiding();} displayed_pages_2d_names().clear();a_displayed_pages_2d_modified_during_this_frame=true;};
        inline std::shared_ptr<Object>* page_2d(std::string page_name) {if(contains_page_2d(page_name)){return &pages_2d()[page_name];}scls::print("Warning", "SCLS Window", "The \"" + page_name + "\" page you want to get does not exists.");return 0;};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline bool displayed_pages_2d_modified_during_this_frame() const {return a_displayed_pages_2d_modified_during_this_frame;};
        inline std::vector<std::string>& displayed_pages_2d_names() {return a_displayed_pages_2d;};
        inline std::vector<std::string>& displayed_pages_3d_names() {return a_displayed_pages_3d;};
        inline bool is_resize_possible() {return a_is_resize_possible;};
        inline std::map<std::string, std::shared_ptr<Object>>& pages_2d() {return a_pages_2d;};
        inline std::map<std::string, std::shared_ptr<Object>>& pages_3d() {return a_pages_3d;};
        inline void set_is_resize_possible(bool new_is_resize_possible) {a_is_resize_possible = new_is_resize_possible;resize_window(window_width(), window_height());};

        //*********
        //
        // Window operating
        //
        //*********

        // Function called after that the window has been resized
        virtual void after_window_resizing(glm::vec2 last_size){apply_window_resizing(last_size);};
        // Hidden function to call the children that there has been a resizing
        inline void apply_window_resizing(glm::vec2 last_size){for(std::map<std::string, std::shared_ptr<Object>>::iterator it = pages_2d().begin(); it != pages_2d().end(); it++) {it->second->after_window_resizing(last_size);}};
        // Render the scene
        void render_always();
        virtual void render();
        // Update one frame of the game
        virtual void update();
        // Update the event of the game during this frame
        virtual void update_event();

        //*********
        //
        // Load the window
        //
        //*********

        // Create an object from a type
        virtual std::shared_ptr<Object> __create_loaded_object_from_type(std::string object_name, std::string object_type);
        // Create an page 3D from a type
        virtual std::shared_ptr<Object> __create_loaded_page_3d_from_type(std::string object_name, std::string object_type);
        // Load the page from XML
        std::shared_ptr<Window_Loader> load_from_xml(std::string window_path);
        // Load an object in a page from XML
        void __load_object_from_xml(std::string object_name, std::string object_type, std::shared_ptr<__XML_Text_Base> object_content, std::shared_ptr<Window_Loader> loader);
        // Load a 3D page from XML
        void __load_page_3d_from_xml(std::string object_name, std::string object_type, std::shared_ptr<__XML_Text_Base> object_content, std::shared_ptr<Window_Loader> loader);

    private:

        //*********
        //
        // Window main attributes
        //
        //*********

        // Basics Window descriptors
        // Background color of the window
        Color a_background_color = Color(255, 255, 255);
        // Pointer to the GLFW window
        GLFWwindow* a_window = 0;
        // Title of the window
        std::string a_window_title = "";
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

        // Names of the displayed 2D page
        std::vector<std::string> a_displayed_pages_2d = std::vector<std::string>();
        bool a_displayed_pages_2d_modified_during_this_frame = false;
        // Map containing each pages 2D in the window with their name as key
        std::map<std::string, std::shared_ptr<Object>> a_pages_2d = std::map<std::string, std::shared_ptr<Object>>();
        // Names of the displayed 3D page
        std::vector<std::string> a_displayed_pages_3d = std::vector<std::string>();
        // Map containing each pages 3D in the window with their name as key
        std::map<std::string, std::shared_ptr<Object>> a_pages_3d = std::map<std::string, std::shared_ptr<Object>>();

        //*********
        //
        // Window operating
        //
        //*********

        double a_last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS
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
    template <typename _P>
    std::shared_ptr<_P>* Window::new_page_2d(std::string page_name) {
        if(contains_page_2d(page_name)) {
            scls::print("Warning", "SCLS Window", "The \"" + page_name + "\" 2D page you want to add in the window already exist.");
            return 0;
        }

        std::shared_ptr<_P> page = std::make_shared<_P>(this, page_name);
        pages_2d()[page_name] = page;

        // Return the static pointer
        std::shared_ptr<_P>* to_return = reinterpret_cast<std::shared_ptr<_P>*>(&pages_2d()[page_name]);
        return to_return;
    }

    // Create a new 3D page to the Window and return it
    template <typename _P>
    std::shared_ptr<_P>* Window::new_page_3d(std::string page_name) {
        if(contains_page_3d(page_name)) {
            scls::print("Warning", "SCLS Window", "The \"" + page_name + "\" 3D page you want to add in the window already exist.");
            return 0;
        }

        std::shared_ptr<_P> page = std::make_shared<_P>(this, page_name);
        pages_3d()[page_name] = page;

        // Return the static pointer
        std::shared_ptr<_P>* to_return = reinterpret_cast<std::shared_ptr<_P>*>(&pages_3d()[page_name]);
        return to_return;
    };
}

#endif // SCLS_WINDOW
