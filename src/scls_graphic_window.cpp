//******************
//
// scls_graphic_window.cpp
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
// This file contains the source code allowing to display the window on the screen
//

#include "../headers/scls_graphic_window.h"
#include "../headers/model.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    bool cursor_on_window = false; // If the cursor is on the window or not
    double global_mouse_x = 100000; // Global variable representing the X pos of the mouse
    double global_mouse_y = 100000; // Global variable representing the Y pos of the mouse

    // Callback function for cursor enter in the window
    void _cursor_enter_callback(GLFWwindow* window, int entered) {
        if (entered)
        {
            // The cursor entered the content area of the window
            cursor_on_window = true;
        }
        else
        {
            // The cursor left the content area of the window
            cursor_on_window = false;
        }
    }

    // Callback function for mouse moving
    void _mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        global_mouse_x = xpos;
        global_mouse_y = ypos;
    }

    //*********
    //
    // Window input handling hiddens functions
    //
    //*********

    int _global_screen_width = 1280; // Global variable representing the width of the screen
    int _global_screen_height = 720; // Global variable representing the height of the screen

    // Callback function for window resizing
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        _global_screen_height = height;
        _global_screen_width = width;
    }

    //*********
    //
    // Window main functions
    //
    //*********

    // Window base constructor
    Window::Window(int a_window_width, int a_window_height, std::string a_exec_path): _Window_Advanced_Struct(global_mouse_x, global_mouse_y, _global_screen_width, _global_screen_height, a_exec_path), a_cursor_on_window(cursor_on_window) {

        // Load the keys
        _load_keys();

        // Configurate base_struct
        get_camera()->set_position(glm::vec3(0.0, 0.0, 0.0));
        get_camera()->set_rotation(glm::vec3(0.0, 0.0, 0.0));
        set_window_height(a_window_height);
        set_window_width(a_window_width);

        // GLFW Window generation
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        a_window = glfwCreateWindow(get_window_width(), get_window_height(), "Fenêtre OpenGL", NULL, NULL);
        if (a_window == NULL)
        {
            print("Error", "GLFW Loader", "Failed to create GLFW window");
            glfwTerminate();
        }
        glfwMakeContextCurrent(a_window);

        // Glad loading
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            print("Error", "GLAD Loader", "Failed to initialize GLAD");
            glfwTerminate();
        }

        // Texture setting
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthFunc(GL_LESS);

        // Callback functions
        glfwSetCursorPosCallback(a_window, _mouse_callback);
        glfwSetCursorEnterCallback(a_window, _cursor_enter_callback);
        glfwSetFramebufferSizeCallback(a_window, _framebuffer_size_callback);

        load_VAOs();
    }

    // Load the game from a config file
    void Window::load_from_config_file(std::string path) {
        std::string last_config_file_path = get_config_file_path();
        set_config_file_path(path);

        if (last_config_file_path != get_config_file_path()) // If the path is correct
        {
            std::vector<std::string> content = scls::cut_string(scls::read_file(get_config_file_path()), "\n");

            for (int i = 0; i < content.size(); i++) // Analyze each lines
            {
                std::vector<std::string> line = scls::cut_string(content[i], ":");
                std::string all_variables = "";
                for (int j = 1; j < static_cast<int>(line.size()); j++) { all_variables += line[j]; }

                if (line[0] == "screen_size") // If the line represents the size of the screen
                {
        std::vector<std::string> variables = scls::cut_string(all_variables, ";");
        resize_window(std::stoi(variables[0]), std::stoi(variables[1]));
    }
                else if (line[0] == "assets_path_directory") // If the line represents the assets path
                {
                    set_assets_directory_path(all_variables);
                }
            }
        }
    }

    // Return if the window is running or not
    bool Window::run() {
        bool to_return = glfwWindowShouldClose(a_window);
        if (!is_running()) { to_return = true; }

        if(to_return)
            glfwTerminate();
        return !to_return;
    }

    // Window destructor
    Window::~Window() {
        // Destroy the cursor
        if(a_cursor != 0) glfwDestroyCursor(a_cursor);

        std::map<std::string, Object*>& all_pages = pages();
        for (std::map<std::string, Object*>::iterator it = all_pages.begin(); it != all_pages.end(); it++)
        {
            delete it->second; // Destroy each pages
            it->second = 0;
        }
    }

    //*********
    //
    // Window input handling
    //
    //*********

    // Load the keys of the window
    void Window::_load_keys() {
        // Alphabet
        a_keys["a"] = GLFW_KEY_Q;
        a_keys["b"] = GLFW_KEY_B;
        a_keys["c"] = GLFW_KEY_C;
        a_keys["d"] = GLFW_KEY_D;
        a_keys["e"] = GLFW_KEY_E;
        a_keys["f"] = GLFW_KEY_F;
        a_keys["g"] = GLFW_KEY_G;
        a_keys["h"] = GLFW_KEY_H;
        a_keys["i"] = GLFW_KEY_I;
        a_keys["j"] = GLFW_KEY_J;
        a_keys["k"] = GLFW_KEY_K;
        a_keys["l"] = GLFW_KEY_L;
        a_keys["m"] = GLFW_KEY_SEMICOLON;
        a_keys["n"] = GLFW_KEY_N;
        a_keys["o"] = GLFW_KEY_O;
        a_keys["p"] = GLFW_KEY_P;
        a_keys["q"] = GLFW_KEY_A;
        a_keys["r"] = GLFW_KEY_R;
        a_keys["s"] = GLFW_KEY_S;
        a_keys["t"] = GLFW_KEY_T;
        a_keys["u"] = GLFW_KEY_U;
        a_keys["v"] = GLFW_KEY_V;
        a_keys["w"] = GLFW_KEY_Z;
        a_keys["x"] = GLFW_KEY_X;
        a_keys["y"] = GLFW_KEY_Y;
        a_keys["z"] = GLFW_KEY_W;

        // Numbers
        a_keys["0"] = GLFW_KEY_KP_0;
        a_keys["1"] = GLFW_KEY_KP_1;
        a_keys["2"] = GLFW_KEY_KP_2;
        a_keys["3"] = GLFW_KEY_KP_3;
        a_keys["4"] = GLFW_KEY_KP_4;
        a_keys["5"] = GLFW_KEY_KP_5;
        a_keys["6"] = GLFW_KEY_KP_6;
        a_keys["7"] = GLFW_KEY_KP_7;
        a_keys["8"] = GLFW_KEY_KP_8;
        a_keys["9"] = GLFW_KEY_KP_9;

        // Poncutation
        a_keys["_"] = GLFW_KEY_8;
        a_keys[":"] = GLFW_KEY_PERIOD;
        a_keys[";"] = GLFW_KEY_COMMA;

        // Other
        a_keys["backspace"] = GLFW_KEY_BACKSPACE;
        a_keys["down arrow"] = GLFW_KEY_DOWN;
        a_keys["enter"] = GLFW_KEY_ENTER;
        a_keys["left arrow"] = GLFW_KEY_LEFT;
        a_keys["left shift"] = GLFW_KEY_LEFT_SHIFT;
        a_keys["right arrow"] = GLFW_KEY_RIGHT;
        a_keys["right shift"] = GLFW_KEY_RIGHT_SHIFT;
        a_keys["space"] = GLFW_KEY_SPACE;
        a_keys["tab"] = GLFW_KEY_TAB;
        a_keys["up arrow"] = GLFW_KEY_UP;
    }

    // Properly resize the window
    void Window::resize_window(unsigned int width, unsigned int height) {
        glfwSetWindowSize(window(), width, height);
    }

    //*********
    //
    // Window page handling
    //
    //*********

    // Create a new page to the Window and return it
    Object* Window::new_page(std::string name) {
        if(contains_page(name)) {
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" page you want to add in the window already exist.");
            return 0;
        }


    }

    //*********
    //
    // Window operating
    //
    //*********

    // Render the scene
    void Window::render() {
        // Clear OpenGL window
        glClearColor(background_color().red(), background_color().green(), background_color().blue(), background_color().alpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthFunc(GL_ALWAYS);
        if (current_page_name() != "" && contains_page(current_page_name()))
        {
            Object* page = current_page();
            if (page != 0)
            {
                page->render();
            }
        }

        // Update OpenGL
        glfwSwapBuffers(window());
        glfwPollEvents();
    }

    // Update one frame of the game
    void Window::update() {
        Object* page = current_page();
        if (page != 0)
        {
            page->update();
        }
    }

    // Update the event of the game during this frame
    void Window::update_event() {
        // Calculate delta time
        set_delta_time(glfwGetTime() - a_last_frame_time);
        a_last_frame_time = glfwGetTime();

        if (get_delta_time() > 0.1) set_delta_time(0);

        // FPS calculation
        time_since_last_fps_calculation += get_delta_time();
        if (time_since_last_fps_calculation >= 1)
        {
            std::cout << "FPS " << frame_count << std::endl;
            frame_count = 0;
            time_since_last_fps_calculation = 0;
        }
        else
        {
            frame_count++;
        }

        // Calculate mouse move and button
        double mouse_move_x = get_mouse_x() - get_last_mouse_x();
        if (get_mouse_x() >= 100000) mouse_move_x = 0;
        double mouse_move_y = get_mouse_y() - get_last_mouse_y();
        if (get_mouse_y() >= 100000) mouse_move_y = 0;
        set_left_mouse_button_state(0);
        set_mouse_move_x(mouse_move_x);
        set_mouse_move_y(mouse_move_y);
        set_right_mouse_button_state(0);
        if(glfwGetMouseButton(window(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            set_left_mouse_button_state(1);
        if (glfwGetMouseButton(window(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            set_right_mouse_button_state(1);

        // Update the keys
        get_pressed_keys()->clear();
        get_pressed_keys_frame()->clear();
        for (std::map<std::string, Key_State>::iterator it = get_keys_state()->begin(); it != get_keys_state()->end(); it++)
        {
            it->second = Key_State::Nothing; // Reset keys
        }

        for (std::map<std::string, unsigned int>::iterator it = a_keys.begin(); it != a_keys.end(); it++)
        {
            if (glfwGetKey(window(), it->second) == GLFW_PRESS)
            {
                (*get_keys_state())[it->first] = Key_State::Pressed;
                get_pressed_keys()->push_back(it->first);
            }
        }

        // Update key frame
        for (std::map<std::string, Key_State>::iterator it = get_keys_state()->begin(); it != get_keys_state()->end(); it++)
        {
            Key_State state = it->second;
            if (state != Key_State::Nothing)
            {
                if ((*get_keys_state_frame())[it->first] != state && (*get_keys_state_frame())[it->first] != Key_State::Already_Pressed)
                {
                    (*get_keys_state_frame())[it->first] = Key_State::Pressed;
                    get_pressed_keys_frame()->push_back(it->first);
                }
                else
                {
                    (*get_keys_state_frame())[it->first] = Key_State::Already_Pressed;
                }
            }
            else
            {
                (*get_keys_state_frame())[it->first] = Key_State::Nothing; // Reset keys
            }
        }

        // Update last mouse pos for future mouse pos calcul
        set_last_mouse_x(get_mouse_x());
        set_last_mouse_y(get_mouse_y());

        /*// Update the cursor texture
        if(overflighted_object != 0)
        {
            if(current_cursor() != overflighted_object->cursor_overflighted())
            {
                if(a_cursor != 0) glfwDestroyCursor(a_cursor);
                a_cursor = glfwCreateStandardCursor(overflighted_object->cursor_overflighted());
                glfwSetCursor(window, a_cursor);
                a_current_cursor = overflighted_object->cursor_overflighted();
            }
        }
        else if(a_cursor == 0)
        {
            a_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            glfwSetCursor(window, a_cursor);
            a_current_cursor = GLFW_ARROW_CURSOR;
        } //*/
    }
}
