#pragma once

#ifdef __ASTER_DEV
#include "../dev_part/scls_image.h"
#else
#include <scls_image.h>
#endif // __ASTER_DEV

// Define a name for an empty texture
#ifndef SCLS_GRAPHIC_NO_TEXTURE
#define SCLS_GRAPHIC_NO_TEXTURE "no_texture"
#endif // SCLS_GRAPHIC_NO_TEXTURE

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <random>
#include <memory>
#include <string>
#include <sstream>
#include <thread>
#include <utility>
#include <vector>

glm::vec3 calculate_forward(glm::vec3 rotation); // Return the forward position according to a rotation
float get_vector_x_angle(glm::vec3 vector); // Return the x angle of a vector (without euler angle)
float get_vector_x_angle(glm::vec3 vector, glm::vec3 forward, float rotation_y, unsigned int id = 0); // Return the x angle of a vector (euler angle)
float get_vector_y_angle(glm::vec3 vector, unsigned int id = 0); // Return the y angle of a vector
float normalize_angle(float angle); // Normalize an angle and return it
glm::vec3 normalize_rotation(glm::vec3 rotation); // Normalize a rotation and return it
glm::vec3 rotate_vector_y(glm::vec3 vector, float rotation, unsigned int id = 0); // Rotate a vector on the y axis
glm::vec3 rotate_vector_x(glm::vec3 vector, glm::vec3 rotation, glm::vec3 forward, unsigned int id = 0); // Rotate a vector on the x axis
glm::vec3 rotate_vector(glm::vec3 vector, glm::vec3 rotation, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation_multiplier = glm::vec3(1, 1, 1), bool protection = true, unsigned int id = 0); // Rotate a vector around a rotating point
std::string to_uppercase(std::string str); // Transform a string to an uppercase string

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    struct Transform_Animation {
        // Struct representing a transform object animation
        glm::vec3 base_position = glm::vec3(0, 0, 0);
        glm::vec3 base_rotation = glm::vec3(0, 0, 0);
        glm::vec3 base_scale = glm::vec3(1, 1, 1);

        float duration = 0;

        glm::vec3 final_position = glm::vec3(0, 0, 0);
        glm::vec3 final_rotation = glm::vec3(0, 0, 0);
        glm::vec3 final_scale = glm::vec3(1, 1, 1);

        bool change_plan_rotation = false;

        bool modify_position = true;
        bool modify_rotation = true;
        bool modify_scale = true;

        float state = 0;
    };

    class Transform_Object {
        // Class representing a object with a transform value
    public:
        Transform_Object(Transform_Object* a_parent = 0, glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Transform_Object contructor
        Transform_Object(const Transform_Object &copy); // Transform_Object copy constructor
        void add_animation(float duration, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale); // Add an animation to the object
        void add_animation(float duration, glm::vec3 base_position, glm::vec3 base_rotation, glm::vec3 base_scale, glm::vec3 final_position, glm::vec3 final_rotation, glm::vec3 final_scale); // Add an animation to the object with base transform
        void add_position_animation(float duration, glm::vec3 base_position, glm::vec3 final_position); // Add an animation to the object with the position
        void add_rotation_animation(float duration, glm::vec3 base_rotation, glm::vec3 final_rotation, bool change_plan_rotation = false); // Add an animation to the object with the rotation
        glm::mat4 apply_parent_position_model_matrix(glm::mat4 matrix); // Apply to a child matrix the parent position model
        glm::vec3 apply_parent_position_vector(glm::vec3 vector); // Apply the parent position to a vector
        glm::mat4 apply_parent_rotation_model_matrix(glm::mat4 matrix, Transform_Object* child = 0); // Apply to a child matrix the parent rotation model
        void calculate_direction(); // Calculate the direction vector of the transform object
        glm::mat4 get_model_matrix(); // Return the transformation matrix of the object
        void move(glm::vec3 a_mouvement); // Move the object
        void remove_child(Transform_Object* object); // Remove an object from the children
        void rescale(glm::vec3 a_scale); // Rescale the object
        void reset_animation(bool reset_position = true); // Reset the animation of the object
        inline virtual void rotate(glm::vec3 a_rotation, bool rotate_around = true) { set_rotation(get_rotation() + a_rotation); }; // Rotate the object
        void rotate_around_anchor(glm::vec3 a_rotation, glm::vec3 rotation_multiplier = glm::vec3(1, 1, 1)); // Rotate the object around the anchored point
        inline void rotate_plan(glm::vec3 a_rotation) // Rotate the plan of the object
        {
            set_plan_rotation(get_plan_rotation() + a_rotation);
        };
        void soft_reset(); // Reset softly the object
        virtual void update() { update_animation(); }; // Update the object
        void update_animation(); // Update the animations for the object
        ~Transform_Object(); // Transform_Object destructor

        // Getters
        inline glm::vec3 absolute_scale() {if(get_parent() == 0) return get_scale();return get_scale() * get_parent()->absolute_scale();};
        inline glm::vec3 get_absolute_plan_rotation(bool use_rotation_multiplier = false){
            if (get_parent() != 0)
            {
                return get_parent()->get_absolute_plan_rotation(use_rotation_multiplier) + get_plan_rotation(use_rotation_multiplier);
            }
            return get_plan_rotation(use_rotation_multiplier);
        }
        glm::vec3 get_absolute_position(Transform_Object* asker = 0);
        inline glm::vec3 get_anchored_position() { return anchored_position; };
        inline std::vector<Transform_Animation>* get_animations() { return &animations; };
        inline std::vector<Transform_Object*> *get_children() { return &children; };
        inline Transform_Animation* get_current_animation() { if (get_animations()->size() <= 0) return 0; return &((*get_animations())[get_animations()->size() - 1]); };
        inline glm::vec3 get_current_animation_position() {
            Transform_Animation* animation = get_current_animation();
            glm::vec3 new_position = animation->final_position;

            // Calculate new transform
            float animation_purcentate = animation->state / animation->duration;
            if (animation_purcentate < 1)
            {
                glm::vec3 position_difference = animation->final_position - animation->base_position;
                new_position = animation->base_position + position_difference * animation_purcentate;
            }
            return new_position;
        };
        inline glm::vec3 get_current_animation_rotation() {
            Transform_Animation* animation = get_current_animation();
            glm::vec3 new_rotation = animation->final_rotation;

            // Calculate new transform
            float animation_purcentate = animation->state / animation->duration;
            if (animation_purcentate < 1)
            {
                glm::vec3 rotation_difference = animation->final_rotation - animation->base_rotation;
                new_rotation = animation->base_rotation + rotation_difference * animation_purcentate;
            }
            return new_rotation;
        };
        inline glm::vec3 get_current_animation_scale() {
            Transform_Animation* animation = get_current_animation();
            glm::vec3 new_scale = animation->final_scale;

            // Calculate new transform
            float animation_purcentate = animation->state / animation->duration;
            if (animation_purcentate < 1)
            {
                glm::vec3 scale_difference = animation->final_scale - animation->base_scale;
                new_scale = animation->base_scale + scale_difference * animation_purcentate;
            }
            return new_scale;
        };
        inline glm::vec3 get_forward() { return forward; };
        glm::vec3 get_global_rotation_modifier();
        inline unsigned int get_id() { return id; };
        inline glm::vec3 get_movement() { return movement; };
        inline Transform_Object* get_parent() { return parent; }
        inline glm::vec3 get_parent_rotation_adder() { return a_parent_rotation_adder; };
        inline glm::vec3 get_parent_rotation_multiplier() { return parent_rotation_multiplier; };
        inline glm::vec3 get_plan_rotation(bool use_rotation_multiplier = false){
            if (use_rotation_multiplier) return plan_rotation * get_parent_rotation_multiplier();
            return plan_rotation;
        };
        inline glm::vec3 get_position() { return position; };
        inline glm::vec3 get_position_animation() { return position_animation; };
        inline glm::vec3 get_position_move_multiplier() { return position_move_multiplier; };
        inline glm::vec3 get_right() { return right; };
        inline glm::vec3 get_rotation() { return rotation; };
        inline glm::vec3 get_scale() { return scale; };
        inline glm::vec3 get_up() { return up; };
        inline bool is_animation_playing() { return animation_playing; };

        // Setters
        inline void set_anchored_position(glm::vec3 a_anchored_position)
        {
            anchored_position = a_anchored_position;
        };
        inline void set_parent(Transform_Object* new_parent)
        {
            if (get_parent() != 0)
            { get_parent()->remove_child(this); }
            parent = new_parent;
            if (new_parent != 0) { new_parent->get_children()->push_back(this); }
        };
        inline void set_parent_rotation_adder(glm::vec3 new_parent_rotation_adder) { a_parent_rotation_adder = new_parent_rotation_adder; };
        inline void set_parent_rotation_multiplier(glm::vec3 a_parent_rotation_multiplier) { parent_rotation_multiplier = a_parent_rotation_multiplier; };
        inline void set_movement(glm::vec3 new_movement) { movement = new_movement; };
        inline void set_plan_rotation(glm::vec3 new_plan_rotation)
        {
            plan_rotation = new_plan_rotation;
            calculate_direction();
        };
        inline void set_position(glm::vec3 new_position)
        {
            position = new_position;
        };
        inline void set_position_animation(glm::vec3 new_position) { position_animation = new_position; };
        inline void set_position_move_multipler(glm::vec3 a_position_move_multiplier) { position_move_multiplier = a_position_move_multiplier; };
        inline void set_rotation(glm::vec3 new_rotation) { rotation = new_rotation; };
        inline void set_scale(glm::vec3 new_scale) { scale = new_scale; };
        inline void start_animation() { animation_playing = true; };
        inline void stop_animation() { animation_playing = false; };
    private:
        bool animation_playing = false; // If the animation is playing or not
        std::vector<Transform_Animation> animations = std::vector<Transform_Animation>(); // List of all the animations in the object

        std::vector<Transform_Object*> children = std::vector<Transform_Object*>(); // List of all the children of the object
        Transform_Object* parent = 0; // Pointer to the parent of the object

        glm::vec3 forward = glm::vec3(1.0f, 0.0f, 0.0f); // Forward vector of the object
        glm::vec3 right = glm::vec3(0.0f, 0.0f, 1.0f); // Right vector for the object
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // Up vector for the object

        glm::vec3 anchored_position = glm::vec3(0.0f, 0.0f, 0.0f); // Base position of the object
        // ID of the transform object
        unsigned int id = 0;
        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f); // Movement of the object
        // Number of objects created
        static unsigned int object_count;
        glm::vec3 a_parent_rotation_adder = glm::vec3(0.0f, 0.0f, 0.0f); // Vector to add to a rotation coming from a parent
        glm::vec3 parent_rotation_multiplier = glm::vec3(1.0f, 1.0f, 1.0f); // Multiplier to apply to a rotation coming from a parent
        glm::vec3 plan_rotation = glm::vec3(0, 0, 0); // Rotation of the plan of the local transform
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the object
        glm::vec3 position_animation = glm::vec3(0.0f, 0.0f, 0.0f); // Position of the animation of the object
        glm::vec3 position_move_multiplier = glm::vec3(1.0, 1.0, 1.0); // Multiplier for the poisitions used in "move"
        glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // Rotation of the object
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); // Scale of the object
    };

    class Camera: public Transform_Object {
        // Class representing a camera into the game
    public:
        Camera(glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 a_scale = glm::vec3(1.0f, 1.0f, 1.0f)); // Camera constructor
        glm::mat4 get_projection(int window_height, int window_width); // Return the projection matrix
        glm::mat4 get_view(); // Return the view matrix
        ~Camera(); // Camera destructor

        // Getter
        inline float get_far() { return a_far; };
        inline float get_fov() { return fov; };
        inline glm::vec3 get_looked_position() { if (looks_forward()) return get_absolute_position() + get_forward(); return a_looked_position; };
        inline float get_sensitivity() { return sensitivity; };
        inline bool looks_forward() { return a_looks_forward; };
        inline void set_fov(float a_fov) { fov = a_fov; };
        inline void set_looked_position(glm::vec3 new_looked_position) { a_looked_position = new_looked_position; };
        inline void set_looks_forward(bool new_looks_forward) { a_looks_forward = new_looks_forward; };
    private:
        float a_far = 1000.0f; // Far projection for the camera
        float fov = 45.0f; // Fov of the camera
        float sensitivity = 30; // Sensitivity of the camera

        // Position looked by the camera
        glm::vec3 a_looked_position = glm::vec3(0, 0, 0);

        // If the camera looks forward or not
        bool a_looks_forward = true;
    };

    // State of a button
    enum Button_State {Clicked, Released, Already_Clicked};
    enum Key_State { Nothing, Pressed, Pressed_Repeated, Already_Pressed }; // Differents orientations for a map lev collection

    class _Window_Base_Struct {
        // Class representing the base struct in the game
    public:

        //*********
        //
        // _Window_Base_Struct hiddens methods
        //
        //*********

        // _Window_Base_Struct constructor
        _Window_Base_Struct(bool& cursor_on_window, double& mouse_x, double& mouse_y, int& window_width, int& window_height, std::string exec_path);
        // _Window_Base_Struct destructor
        virtual ~_Window_Base_Struct();

        //*********
        //
        // Window main functions
        //
        //*********

        // Return if a file formatted with the window context
        std::string file_formatted(std::string path);

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline std::string assets_directory_path() { return a_assets_directory_path; };
        inline Camera* camera() { return &a_camera; };
        inline std::string config_file_path() { return a_config_file_path; };
        inline std::string exec_path() { return a_exec_path; };
        inline glm::mat4 projection() {return camera()->get_projection(window_height(), window_width());};
        inline void set_assets_directory_path(std::string new_assets_directory_path) {
            new_assets_directory_path = file_formatted(new_assets_directory_path);
            if (std::filesystem::exists(new_assets_directory_path))
            {
                if (std::filesystem::is_directory(new_assets_directory_path))
                {
                    a_assets_directory_path = new_assets_directory_path;
                }
                else
                {
                    print("Warning", "SCLS Window", "The path \"" + new_assets_directory_path + "\" you want to set as the window assets directory is not a directory.");
                }
            }
            else
            {
                print("Warning", "SCLS Window", "The path \"" + new_assets_directory_path + "\" you want to set as the window assets directory does not exist.");
            }
        }

        //*********
        //
        // Window input handling attributes
        //
        //*********

        // Getters and setters (ONLY WITHOUT ATTRIUTES)
        inline double window_ratio() const {return static_cast<double>(window_width()) / static_cast<double>(window_height());};

        // Getters and setters (ONLY WITH ATTRIBUTES)

        // FPS
        inline double delta_time() { return a_delta_time; };

        // Keyboard
        inline bool key_pressed(std::string name) const { return a_keys_state_frame.at(name) == Key_State::Pressed || a_keys_state_frame.at(name) == Key_State::Already_Pressed || a_keys_state_frame.at(name) == Key_State::Pressed_Repeated; };
        inline bool key_pressed_or_repeated_pressed(std::string name) const { return a_keys_state_frame.at(name) == Key_State::Pressed || a_keys_state_frame.at(name) == Key_State::Pressed_Repeated; };
        inline unsigned short key_state(std::string name) const { return a_keys_state.at(name); };
        inline unsigned short key_state_frame(std::string name) const { return a_keys_state_frame.at(name); };
        inline double key_state_time(std::string name) const { return a_keys_state_time.at(name); };

        // Mouse and cursor
        inline GLFWcursor* cursor() {return a_cursor;};
        inline bool cursor_changed() const {return a_cursor_changed;};
        inline int cursor_state() const { return _cursor_state; };
        inline double last_mouse_x() const { return a_last_mouse_x; };
        inline double last_mouse_y() const { return a_last_mouse_y; };
        void load_mouse_buttons();
        inline unsigned short maximum_window_height() const { return a_max_window_height; };
        inline unsigned short maximum_window_width() const { return a_max_window_width; };
        inline unsigned short minimum_window_height() const { return a_min_window_height; };
        inline unsigned short minimum_window_width() const { return a_min_window_width; };
        inline bool mouse_button_clicked(unsigned int button) {return mouse_buttons_state()[button] != Button_State::Released;};
        inline bool mouse_button_clicked_during_this_frame(unsigned int button) {return mouse_buttons_state()[button] == Button_State::Clicked;};
        inline Button_State mouse_button_state(unsigned int button) {return a_mouse_buttons_state[button];};
        inline double mouse_move_x() const { return a_mouse_move_x; };
        inline double mouse_move_y() const { return a_mouse_move_y; };
        inline double mouse_x() const { return a_mouse_x; };
        inline double mouse_y() const { return a_mouse_y; };
        inline void set_cursor(GLFWcursor* new_cursor) {if(new_cursor != cursor()) {
            if(a_cursor != 0) glfwDestroyCursor(a_cursor);
            a_cursor = new_cursor;
            a_cursor_changed = true;
        }};
        virtual void set_maximum_window_height(unsigned short new_max_window_height) { a_max_window_height = new_max_window_height; };
        virtual void set_maximum_window_width(unsigned short new_max_window_width) { a_max_window_width = new_max_window_width; };
        virtual void set_minimum_window_height(unsigned short new_min_window_height) { a_min_window_height = new_min_window_height; };
        virtual void set_minimum_window_width(unsigned short new_min_window_width) { a_min_window_width = new_min_window_width; };
        inline double wheel_movement_y_during_this_frame() const {return a_wheel_movement_y;};
        inline int window_height() const { return a_window_height; };
        inline int window_width() const { return a_window_width; };

        //*********
        //
        // SCLS Text generator
        //
        //*********

        // Returns if the struct contains a text generator
        inline bool contains_text_image_generator(std::string generator_name) {
            for(std::map<std::string, Text_Image_Generator*>::iterator it = text_image_generators().begin();it!=text_image_generators().end();it++) {
                if(it->first == generator_name) return true;
            }
            return false;
        };
        // Returns a text generator in the struct, or 0 if it does not exist
        inline Text_Image_Generator* text_image_generator(std::string generator_name) {
            for(std::map<std::string, Text_Image_Generator*>::iterator it = text_image_generators().begin();it!=text_image_generators().end();it++) {
                if(it->first == generator_name) return it->second;
            }

            if(generator_name == "default") {
                Text_Image_Generator* to_return = new Text_Image_Generator();
                text_image_generators()["default"] = to_return;
                return to_return;
            }

            return 0;
        };
        // Returns the default text generator in the struct
        Text_Image_Generator* text_image_generator() {
            Text_Image_Generator* to_return = text_image_generator("default");
            return to_return;
        };

        // Getters and setters
        inline std::map<std::string, Text_Image_Generator*>& text_image_generators() {return a_text_image_generator;};

    protected:

        //*********
        //
        // Window main functions
        //
        //*********

        // Path to the config path file
        std::string a_config_file_path = "";

        //*********
        //
        // Window input handling
        //
        //*********

        // Getters and setters

        // FPS
        inline void set_delta_time(double new_delta_time) { a_delta_time = new_delta_time; };

        // Keyboard
        inline std::map<std::string, Key_State>& keys_state() { return a_keys_state; };
        inline std::map<std::string, Key_State>& keys_state_frame() { return a_keys_state_frame; };
        inline std::map<std::string, double>& keys_state_time() {return a_keys_state_time;};
        inline std::vector<std::string>& pressed_keys() { return a_pressed_keys; };
        inline std::vector <std::string>& pressed_keys_frame() { return a_pressed_keys_frame; };

        // Mouse and cursor
        inline bool is_cursor_on_window() { return a_cursor_on_window; };
        inline std::map<unsigned int, Button_State>& mouse_buttons_state() {return a_mouse_buttons_state;};
        inline void set_last_mouse_x(double new_last_mouse_x) { a_last_mouse_x = new_last_mouse_x; };
        inline void set_last_mouse_y(double new_last_mouse_y) { a_last_mouse_y = new_last_mouse_y; };
        inline void set_mouse_move_x(double new_mouse_move_x) { a_mouse_move_x = new_mouse_move_x; };
        inline void set_mouse_move_y(double new_mouse_move_y) { a_mouse_move_y = new_mouse_move_y; };
        inline void set_wheel_movement_y(double new_wheel_movement_y) {a_wheel_movement_y = new_wheel_movement_y;};
        inline void set_window_height(int new_height) { a_window_height = new_height; };
        inline void set_window_width(int new_width) { a_window_width = new_width; };

        // Attributes
        // If the cursor as changed during this frame or not
        bool a_cursor_changed = false;
        // State of the cursor
        int _cursor_state = GLFW_CURSOR_DISABLED;
        // Number of frame during this second
        unsigned short a_frame_count = 0;

        // Time since the last FPS calculation
        double a_time_since_last_fps_calculation = 0;

        //*********
        //
        // SCLS Text generator
        //
        //*********

        // Each text generator, with their name as key
        std::map<std::string, Text_Image_Generator*> a_text_image_generator = std::map<std::string, Text_Image_Generator*>();
    private:

        //*********
        //
        // Window main functions
        //
        //*********

        // Path to the assert path file
        std::string a_assets_directory_path = "";
        // Camera of the window
        Camera a_camera = Camera();
        // Path of the window executable
        const std::string a_exec_path = "";

        //*********
        //
        // Window input handling attributes
        //
        //*********

        // FPS
        // Time since the last frame of the window
        double a_delta_time = 0;

        // Keyboard
        // Current state of the keys
        std::map<std::string, Key_State> a_keys_state = std::map<std::string, Key_State>();
        // State of the modification in the keys during this frame
        std::map<std::string, Key_State> a_keys_state_frame = std::map<std::string, Key_State>();
        // Time since a keys is in its state
        std::map<std::string, double> a_keys_state_time = std::map<std::string, double>();
        // All the pressed keys
        std::vector <std::string> a_pressed_keys = std::vector <std::string>();
        // All the pressed keys during this frame
        std::vector <std::string> a_pressed_keys_frame = std::vector <std::string>();

        // Mouse and cursor
        // Pointer to the cursor
        GLFWcursor* a_cursor = 0;
        // Reference to the cursor_on_window bool
        bool &a_cursor_on_window;
        // Last X position of the mouse
        double a_last_mouse_x = 0;
        // Last Y position of the mouse
        double a_last_mouse_y = 0;
        // Maximum height of the graphic window
        unsigned short a_max_window_height = 10000;
        // Maximum width of the graphic window
        unsigned short a_max_window_width = 10000;
        // Minimum height of the graphic window
        unsigned short a_min_window_height = 100;
        // Minimum width of the graphic window
        unsigned short a_min_window_width = 100;
        // State of every mouse button
        std::map<unsigned int, Button_State> a_mouse_buttons_state = std::map<unsigned int, Button_State>();
        // Last X move of the mouse
        double a_mouse_move_x = 0;
        // Last Y move of the mouse
        double a_mouse_move_y = 0;
        // Reference to the mouse X pos
        double& a_mouse_x;
        // Reference to the mouse Y pos
        double& a_mouse_y;
        // Movement on the y axis of the wheel during this frame
        double a_wheel_movement_y = 0;
        // Reference to the height of the graphic window
        int& a_window_height;
        // Reference to the width of the graphic window
        int& a_window_width;
    };
}
