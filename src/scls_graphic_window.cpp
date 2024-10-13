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

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    bool cursor_on_window = false; // If the cursor is on the window or not
    double global_mouse_x = 100000; // Global variable representing the X pos of the mouse
    double global_mouse_y = 100000; // Global variable representing the Y pos of the mouse
    // Movement in the y offset of the wheel of the mouse
    double wheel_y = 0;

    // Callback function for cursor enter in the window
    void _cursor_enter_callback(GLFWwindow* window, int entered) {
        if (entered) {
            // The cursor entered the content area of the window
            cursor_on_window = true;
        }
        else {
            // The cursor left the content area of the window
            cursor_on_window = false;
        }
    }

    // Callback function for mouse moving
    void _mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        global_mouse_x = xpos;
        global_mouse_y = ypos;
    }

    // Callback function for mouse wheel scrolling
    void _mouse_wheel_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        wheel_y += yoffset;
    }

    //*********
    //
    // Window input handling hiddens functions
    //
    //*********

    int _global_screen_width = 1280; // Global variable representing the width of the screen
    int _global_screen_height = 720; // Global variable representing the height of the screen
    // If the screen as been resized and processed or not
    bool _screen_resized = false;

    // Callback function for window resizing
    void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        _global_screen_height = height;
        _global_screen_width = width;
        _screen_resized = true;
        glViewport(0, 0, width, height);
    }

    //*********
    //
    // Window main functions
    //
    //*********

    // Window base constructor
    Window::Window(int a_window_width, int a_window_height, std::string a_exec_path): _Window_Advanced_Struct(cursor_on_window, global_mouse_x, global_mouse_y, _global_screen_width, _global_screen_height, a_exec_path) {

        // Load the keys and the mouse
        _load_keys();
        load_mouse_buttons();

        // Configurate base_struct
        camera()->set_position(glm::vec3(0.0, 0.0, 0.0));
        camera()->set_rotation(glm::vec3(0.0, 0.0, 0.0));
        set_window_height(a_window_height);
        set_window_width(a_window_width);

        // GLFW Window generation
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        a_window = glfwCreateWindow(window_width(), window_height(), "SCLS Graphic Benoit", NULL, NULL);
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

        // Callback functions
        glfwSetCursorPosCallback(a_window, _mouse_callback);
        glfwSetCursorEnterCallback(a_window, _cursor_enter_callback);
        glfwSetFramebufferSizeCallback(a_window, _framebuffer_size_callback);
        glfwSetScrollCallback(a_window, _mouse_wheel_scroll_callback);

        load_VAOs();
    }

    // Load the game from a config file
    void Window::load_from_config_file(std::string path) {
        if (path != config_file_path() && std::filesystem::exists(path)) // If the path is correct
        {
            a_config_file_path = path;
            std::vector<std::string> content = scls::cut_string(scls::read_file(config_file_path()), "\n");

            for (int i = 0; i < static_cast<int>(content.size()); i++) // Analyze each lines
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
        if(cursor() != 0) glfwDestroyCursor(cursor());
        pages_2d().clear();
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
        // Extended alphabet
        a_keys["&"] = GLFW_KEY_1;
        a_keys["ç"] = GLFW_KEY_9;
        a_keys["à"] = GLFW_KEY_0;
        a_keys["é"] = GLFW_KEY_2;
        a_keys["è"] = GLFW_KEY_7;
        a_keys["ù"] = GLFW_KEY_APOSTROPHE;

        // Numbers
        a_keys["="] = GLFW_KEY_EQUAL;
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
        a_keys["+"] = GLFW_KEY_KP_ADD;
        a_keys["-k"] = GLFW_KEY_KP_SUBTRACT;
        a_keys["*k"] = GLFW_KEY_KP_MULTIPLY;
        a_keys["/"] = GLFW_KEY_KP_DIVIDE;
        a_keys["."] = GLFW_KEY_KP_DECIMAL;
        a_keys[")"] = GLFW_KEY_MINUS;
        a_keys["^"] = GLFW_KEY_LEFT_BRACKET;
        a_keys["$"] = GLFW_KEY_RIGHT_BRACKET;
        a_keys["*"] = GLFW_KEY_BACKSLASH;

        // Poncutation
        a_keys["\""] = GLFW_KEY_3;
        a_keys["'"] = GLFW_KEY_4;
        a_keys["("] = GLFW_KEY_5;
        a_keys["-"] = GLFW_KEY_6;
        a_keys["_"] = GLFW_KEY_8;
        a_keys[":"] = GLFW_KEY_PERIOD;
        a_keys[";"] = GLFW_KEY_COMMA;
        a_keys[","] = GLFW_KEY_M;
        a_keys["!"] = GLFW_KEY_SLASH;
        a_keys["<"] = GLFW_KEY_WORLD_2;

        // Other
        a_keys["alt gr"] = GLFW_KEY_RIGHT_ALT;
        a_keys["backspace"] = GLFW_KEY_BACKSPACE;
        a_keys["down arrow"] = GLFW_KEY_DOWN;
        a_keys["enter"] = GLFW_KEY_ENTER;
        a_keys["left arrow"] = GLFW_KEY_LEFT;
        a_keys["left control"] = GLFW_KEY_LEFT_CONTROL;
        a_keys["left shift"] = GLFW_KEY_LEFT_SHIFT;
        a_keys["right arrow"] = GLFW_KEY_RIGHT;
        a_keys["right control"] = GLFW_KEY_RIGHT_CONTROL;
        a_keys["right shift"] = GLFW_KEY_RIGHT_SHIFT;
        a_keys["space"] = GLFW_KEY_SPACE;
        a_keys["tab"] = GLFW_KEY_TAB;
        a_keys["up arrow"] = GLFW_KEY_UP;

        for (std::map<std::string, unsigned int>::iterator it = a_keys.begin(); it != a_keys.end(); it++) {
            keys_state()[it->first] = Key_State::Nothing; // Reset keys
            keys_state_frame()[it->first] = Key_State::Nothing; // Reset keys
        }
    }

    // Properly resize the window
    void Window::resize_window(unsigned int width, unsigned int height) {
        glm::vec2 last_size = glm::vec2(width, height);
        glfwSetWindowSize(window(), width, height);
        if(is_resize_possible()) {
            glfwSetWindowSizeLimits(window(), minimum_window_width(), minimum_window_height(), maximum_window_width(), maximum_window_height());
        }
        else {
            glfwSetWindowSizeLimits(window(), width, height, width, height);
        }

        after_window_resizing(last_size);
    }

    //*********
    //
    // Window page handling
    //
    //*********

    //*********
    //
    // Window operating
    //
    //*********

    // Render the scene
    void Window::render() {
        // Clear OpenGL window
        glClearColor(background_color().red() / 255.0, background_color().green() / 255.0, background_color().blue() / 255.0, background_color().alpha() / 255.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the cursor texture
        if(cursor_changed() && cursor() != 0) {
            glfwSetCursor(window(), cursor());
        }
        else if(cursor() == 0) {
            set_cursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
            glfwSetCursor(window(), cursor());
        }
        a_cursor_changed = false;

        // Render 3D pages
        glDepthFunc(GL_LESS);
        if (displayed_pages_3d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_3d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->render();
            }
        }

        // Render 2D pages
        glDepthFunc(GL_ALWAYS);
        if (displayed_pages_2d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_2d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->render();
            }
        }

        // Update OpenGL
        glfwSwapBuffers(window());
        glfwPollEvents();
    }

    // Update one frame of the game
    void Window::update() {
        // Update 2D pages
        if (displayed_pages_2d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_2d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->update();
            }
        }

        // Update 3D pages
        if (displayed_pages_3d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_3d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->update();
            }
        }
    }

    // Update the event of the game during this frame
    void Window::update_event() {
        // Calculate delta time
        set_delta_time(glfwGetTime() - a_last_frame_time);
        a_last_frame_time = glfwGetTime();
        a_execution_time += delta_time();

        if (delta_time() > 0.1) set_delta_time(0);

        // FPS calculation
        a_time_since_last_fps_calculation += delta_time();
        if (a_time_since_last_fps_calculation >= 1)
        {
            print("SCLS", "FPS", a_frame_count);
            a_frame_count = 0;
            a_time_since_last_fps_calculation = 0;
        }
        else
        {
            a_frame_count++;
        }

        // Calculate the resizing of the window
        if(_screen_resized) {
            _screen_resized = false;
            after_window_resizing(glm::vec2(a_last_window_width, a_last_window_height));
            after_window_resizing(glm::vec2(a_last_window_width, a_last_window_height));
        }
        a_last_window_height = window_height();
        a_last_window_width = window_width();

        // Calculate mouse move and button
        double mouse_move_x = mouse_x() - last_mouse_x();
        if (mouse_x() >= 100000) mouse_move_x = 0;
        double mouse_move_y = mouse_y() - last_mouse_y();
        if (mouse_y() >= 100000) mouse_move_y = 0;
        set_mouse_move_x(mouse_move_x);
        set_mouse_move_y(mouse_move_y);
        for(std::map<unsigned int, Button_State>::iterator it = mouse_buttons_state().begin();it!=mouse_buttons_state().end();it++) {
            int state = glfwGetMouseButton(window(), it->first);
            if (state == GLFW_PRESS)
            {
                if(it->second == Button_State::Released) it->second = Button_State::Clicked;
                else it->second = Button_State::Already_Clicked;
            }
            else {
                it->second = Button_State::Released;
            }
        }
        set_wheel_movement_y(wheel_y);
        wheel_y = 0;

        // Update the keys
        pressed_keys().clear();
        pressed_keys_frame().clear();
        for (std::map<std::string, Key_State>::iterator it = keys_state().begin(); it != keys_state().end(); it++) {
            it->second = Key_State::Nothing; // Reset keys
        }
        for (std::map<std::string, double>::iterator it = keys_state_time().begin(); it != keys_state_time().end(); it++) {
            // Update the time
            it->second += delta_time();
        }

        for (std::map<std::string, unsigned int>::iterator it = a_keys.begin(); it != a_keys.end(); it++) {
            if (glfwGetKey(window(), it->second) == GLFW_PRESS) {
                keys_state()[it->first] = Key_State::Pressed;
                pressed_keys().push_back(it->first);
            }
        }

        // Update key frame
        for (std::map<std::string, Key_State>::iterator it = keys_state().begin(); it != keys_state().end(); it++) {
            Key_State state = it->second;
            if (state != Key_State::Nothing) {
                if (keys_state_frame()[it->first] != state && keys_state_frame()[it->first] != Key_State::Already_Pressed && keys_state_frame()[it->first] != Key_State::Pressed_Repeated)
                {
                    keys_state_frame()[it->first] = Key_State::Pressed;
                    keys_state_time()[it->first] = 0;
                    pressed_keys_frame().push_back(it->first);
                }
                else
                {
                    keys_state_frame()[it->first] = Key_State::Already_Pressed;

                    if(keys_state_time()[it->first] > 0.5) {
                        keys_state_time()[it->first] -= 0.02;
                        keys_state_frame()[it->first] = Key_State::Pressed_Repeated;
                    }
                }
            }
            else
            {
                keys_state_frame()[it->first] = Key_State::Nothing; // Reset keys
                keys_state_time()[it->first] = 0;
            }
        }

        // Update last mouse pos for future mouse pos calcul
        set_last_mouse_x(mouse_x());
        set_last_mouse_y(mouse_y());

        // Update the event of the 2D pages
        if (displayed_pages_2d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_2d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->update_event();
            }
        }

        // Update the event of the 3D pages
        if (displayed_pages_3d_names().size() > 0) {
            std::vector<std::shared_ptr<Object>> to_display = displayed_pages_3d();
            for(int i = 0;i<static_cast<int>(to_display.size());i++) {
                to_display[i].get()->update_event();
            }
        }
    }

    //*********
    //
    // Load the window
    //
    //*********

    // Create an object from a type
    std::shared_ptr<Object> Window::__create_loaded_object_from_type(std::string object_name, std::string object_type) {
        if(object_type == "gui") {
            std::shared_ptr<Object> to_return = *new_page_2d<GUI_Page>(object_name);
            return to_return;
        }
        return *new_page_2d<Object>(object_name);
    }

    // Create an page 3D from a type
    std::shared_ptr<Object> Window::__create_loaded_page_3d_from_type(std::string object_name, std::string object_type) { return *new_page_3d<Object>(object_name); }

    // Load the page from XML
    std::shared_ptr<Window_Loader> Window::load_from_xml(std::string window_path) {
        // Create the loader
        std::shared_ptr<Window_Loader> loader = std::make_shared<Window_Loader>(format_for_xml(remove_comment_out_of(read_file(window_path), "\"")));
        loader.get()->cutted = cut_string_by_balise(loader.get()->content, true);
        loader.get()->window_file_path = window_path;

        // Get the XML analyser
        XML_Text xml_text = XML_Text(remove_comment_out_of(read_file(window_path), "\""));
        std::vector<XML_Text>& cutted = xml_text.sub_texts();
        window_path = path_parent(window_path);

        // Check each balises
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            // Add a 2D page
            if(cutted[i].xml_balise_name() == "page_2d") {
                bool display_page = false;
                std::string page_name = "";
                std::string page_type = "";
                for(int j = 0;j<static_cast<int>(cutted[i].xml_balise_attributes().size());j++) {
                    std::string current_attribute_name = cutted[i].xml_balise_attributes()[j].name;
                    std::string current_attribute_value = cutted[i].xml_balise_attributes()[j].value;
                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        page_name = current_attribute_value;
                    }
                    else if(current_attribute_name == "type") {
                        // Get the type of the object
                        page_type = current_attribute_value;
                    }
                    else if(current_attribute_name == "display" || current_attribute_name == "displayed") {
                        // Display the page
                        display_page = true;
                    }
                }

                // Add the page
                if(page_name != "") {
                    __load_object_from_xml(page_name, page_type, cutted[i], loader);
                    if(display_page) display_page_2d(page_name);
                }
            }
            else if(cutted[i].xml_balise_name() == "page_3d") {
                // Add a 3D page
                bool display_page = false;
                std::string page_name = "";
                std::string page_type = "";
                for(int j = 0;j<static_cast<int>(cutted[i].xml_balise_attributes().size());j++) {
                    std::string current_attribute_name = cutted[i].xml_balise_attributes()[j].name;
                    std::string current_attribute_value = cutted[i].xml_balise_attributes()[j].value;
                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        page_name = current_attribute_value;
                    }
                    else if(current_attribute_name == "type") {
                        // Get the type of the object
                        page_type = current_attribute_value;
                    }
                    else if(current_attribute_name == "display" || current_attribute_name == "displayed") {
                        // Display the page
                        display_page = true;
                    }
                }

                // Add the page
                if(page_name != "") { __load_page_3d_from_xml(page_name, page_type, cutted[i], loader); }
            }
            else if(cutted[i].xml_balise_name() == "texture") {
                // Add a texture
                std::string name = "";
                std::string src = "";
                for(int j = 0;j<static_cast<int>(cutted[i].xml_balise_attributes().size());j++) {
                    // Get the datas about the attributes
                    std::string current_attribute_name = cutted[i].xml_balise_attributes()[j].name;
                    std::string current_attribute_value = cutted[i].xml_balise_attributes()[j].value;

                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        name = current_attribute_value;
                    }
                    else if(current_attribute_name == "src") {
                        // Get the name of the object
                        src = current_attribute_value;
                    }
                }

                // Format the path if necessary
                std::string final_src = src;
                if(!std::filesystem::exists(final_src)) final_src = window_path + "/" + final_src;

                if(!std::filesystem::exists(final_src)) {
                    print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to load, uses the \"" + src + "\" path, which does not exist.");
                }
                else {
                    // Add the texture
                    new_texture(name, final_src);
                }
            }
            else if(cutted[i].xml_balise_name() == "variable") {
                // Add a variable
                std::string name = "";
                for(int j = 0;j<static_cast<int>(cutted[i].xml_balise_attributes().size());j++) {
                    // Get the datas about the attributes
                    std::string current_attribute_name = cutted[i].xml_balise_attributes()[j].name;
                    std::string current_attribute_value = cutted[i].xml_balise_attributes()[j].value;

                    if(current_attribute_name == "name") {
                        // Get the value of the object
                        name = current_attribute_value;
                    }
                }
                // Add the variable
                if(name != "")load_variable(name, cutted[i].text());
            }
        }

        // Finish the loading of each objects
        if(loader.get()->created_objects.size() > 0) {
            while(loader.get()->created_objects_stack.size() > 0) {
                // Handle the stack
                std::shared_ptr<Object> object = loader.get()->created_objects_stack.top();
                loader.get()->created_objects_stack.pop();

                // Handle the object
                object.get()->after_xml_loading();
            }
        }

        // Return the loader
        return loader;
    }

    // Load an object in a page from XML
    void Window::__load_object_from_xml(std::string object_name, std::string object_type, XML_Text& object_content, std::shared_ptr<Window_Loader> loader) {
        // Create the object
        std::shared_ptr<Object> object = __create_loaded_object_from_type(object_name, object_type);
        loader.get()->created_objects[object_name] = object;
        loader.get()->created_objects_stack.push(object);

        // Parse the content
        for(int i = 0;i<static_cast<int>(object_content.sub_texts().size());i++) {
            // Display the page if necessary
            object.get()->set_xml_attribute(object_content.sub_texts()[i], loader, i);
        }
    }

    // Load a 3D page from XML
    void Window::__load_page_3d_from_xml(std::string object_name, std::string object_type, XML_Text& object_content, std::shared_ptr<Window_Loader> loader) {
        // Create the 3D page
        std::shared_ptr<Object> object = __create_loaded_page_3d_from_type(object_name, object_type);
        loader.get()->created_objects[object_name] = object;
        loader.get()->created_objects_stack.push(object);

        // Parse each attributes
        for(int i = 0;i<static_cast<int>(object_content.xml_balise_attributes().size());i++) {
            if(object_content.xml_balise_attributes()[i].name == "displayed" || object_content.xml_balise_attributes()[i].name == "display") {
                display_page_3d(object_name);
            }
        }

        // Parse the content
        for(int i = 0;i<static_cast<int>(object_content.sub_texts().size());i++) {
            // Display the page if necessary
            object.get()->set_xml_attribute(object_content.sub_texts()[i], loader, i);
        }
    }
}
