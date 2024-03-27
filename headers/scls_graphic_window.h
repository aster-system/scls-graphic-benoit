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
#include "../headers/scene.h"

// Callback function for cursor enter in the window
void cursor_enter_callback(GLFWwindow* window, int entered);

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Callback function for mouse moving
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

class HUD : public HUD_Object {
	// Class representing an HUD
public:
	HUD(Advanced_Struct* a_advanced_struct, std::string a_name); // HUD constructor
	void add_hud_object(std::string name, HUD_Object* object); // Add an existing HUD to the game
	bool contains_hud_object(std::string name); // Return if the game contains an HUD Object
	bool is_hud() {return true;};
	virtual void load() {}; // Load the CLI after being selected as new current HUD
	template <class O = HUD_Object> // Template for adding a type of HUD object
	O* new_hud_object(std::string name, HUD_Object* parent, Texture* texture, std::string vao_name = "hud_default"); // Create a new HUD Object into the game
	template <class O = HUD_Object> // Template for adding a type of HUD object
	O* new_hud_object(std::string name, HUD_Object* parent, std::string texture_name, std::string vao_name = "hud_default"); // Create a new HUD Object into the game
	template <class O = HUD_Object> // Template for adding a type of HUD object
	O* new_hud_object(std::string name, HUD_Object* parent, unsigned short texture_width, unsigned short texture_height, glm::vec4 texture_color, std::string vao_name = "hud"); // Create a new HUD Object into the game
	void render(); // Render the HUD
	inline double scale_rendered_ratio() {return get_advanced_struct()->window_ratio();};
	void unload(); // Unload the objects in the HUD
	virtual void update(); // Update the HUD
	void update_object(); // Update all the objects in the HUD
	~HUD(); // HUD destructor

	// Getters and setters
	inline Advanced_Struct* get_advanced_struct() { return advanced_struct; };
	inline HUD_Object* get_hud_object(std::string a_name) { return hud_objects[a_name]; };
	inline std::map < std::string, HUD_Object*>* get_hud_objects() { return &hud_objects; };
	inline std::string get_name() { return name; };
private:
	std::string name; // Name of the HUD

	Advanced_Struct* advanced_struct = 0;
	std::map < std::string, HUD_Object*> hud_objects = std::map < std::string, HUD_Object*>(); // Each HUD_Object, with their name as key, in the HUD
};

class Window: public Advanced_Struct {
	// Class representing the game
public:
	Window(int a_window_width, int a_window_height, std::string a_exec_path, bool load_vaos = true); // Window constructor
	void add_hud(std::string name, HUD* object); // Add an existing HUD to the game
	void add_scene(std::string name, Scene* scene); // Add an existing scene to the game
	bool contains_hud(std::string name); // Return if the game contains an HUD Object
	bool contains_scene(std::string name); // Returns if the game contains a scene
	// Disable the cursor from the game
	inline void disable_cursor() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); _cursor_state = GLFW_CURSOR_DISABLED; };
	// Enable the cursor from the game
	inline void enable_cursor() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); _cursor_state = GLFW_CURSOR_NORMAL; };
	void load_keys(); // Load the keys in the game
	void load_from_config_file(std::string path); // Load the game from a config file
	template <class O = HUD> // Template for adding a type of HUD
	O* new_hud(std::string name); // Create a new HUD into the game
	Scene* new_scene(std::string name, std::string map_path = "", Map_Opening_Mode mode = Map_Opening_Mode::Simple, bool use_graphic = true, bool use_physic = true); // Create a scene into the game and return it
	void render(); // Render the scene
	void resize(unsigned int width, unsigned int height); // Properly resize the window
	bool run(); // Run the game by doing multiples call to update
	void update(); // Update one frame of the game
	void update_event(); // Update the event of the game during this frame
	~Window(); // Window destructor

	// Getters and setters
	inline bool continue_running() { return is_running; };
	inline unsigned long current_cursor() const {return a_current_cursor;};
	inline glm::vec4 get_background_color() { return background_color; };
	inline HUD* get_current_hud() { if (get_current_hud_name() == "") return 0; return (*get_huds())[get_current_hud_name()]; };
	inline Scene* get_current_scene() { if (get_current_scene_name() == "") return 0; return (*get_scenes())[get_current_scene_name()]; };
	inline std::string get_current_hud_name() { return current_hud; };
	inline std::string get_current_scene_name() { return current_scene; };
	inline std::string get_exec_directory() {
		std::vector<std::string> parts = scls::cut_string(get_exec_path(), "\\");
		std::string result = "";
		for (int i = 0; i < parts.size() - 1; i++) { result += parts[i] + "/"; }
		return result;
	} // Return the directory of the game exe
	inline HUD* get_hud(std::string a_name) { return huds[a_name]; };
	inline std::map < std::string, HUD*>* get_huds() { return &huds; };
	inline Scene* get_scene(std::string name) { if (contains_scene(name) && name != "") { return scenes[name]; } std::cout << "Matrix game : error ! The scene \"" << name << "\" does not exist." << std::endl; return 0; }
	inline std::map<std::string, Scene*> *get_scenes() { return &scenes; };
	inline bool is_cursor_on_window() { return a_cursor_on_window; };
    inline void set_background_color(glm::vec4 a_background_color) { background_color = a_background_color; };
	void set_current_hud(std::string a_name);
	void set_current_scene(std::string a_name);
	inline void set_is_running(bool a_run) { is_running = a_run; };
private:
    // Current displayed cursor
    unsigned long a_current_cursor = GLFW_ARROW_CURSOR;
    // Pointer to the cursor
    GLFWcursor* a_cursor = 0;
	std::string current_hud = ""; // Name of the current hud loaded
	std::string current_scene = ""; // Name of the current scene loaded
	// Reference to the cursor_on_window bool
	bool &a_cursor_on_window;
	bool is_running = true; // If the game should continue running or not
	float last_frame_time = 0; // Time when the last frame occurs, for calculating delta_time and FPS

	glm::vec4 background_color = glm::vec4(0.0f, (1.0f/255.0f) * 204.0f, (1.0f / 255.0f) * 204.0f, 1.0f); // Background color of the game
	std::map < std::string, HUD*> huds = std::map < std::string, HUD*>(); // Each HUD, with their name as key, in the game
	// HUD_Object overflighted by the cursor
	HUD_Object* overflighted_object = 0;
	std::map<std::string, Scene *> scenes = std::map<std::string, Scene *>(); // Each scenes, with their name as key, in the game
	GLFWwindow* window = 0; // Pointer to the GLFW window
};

// Create a new HUD Object into the HUD
template <class O> // Template for adding a type of HUD object
O* HUD::new_hud_object(std::string name, HUD_Object* parent, std::string texture_name, std::string vao_name) {
	return new_hud_object<O>(name, parent, get_advanced_struct()->get_texture(texture_name), vao_name);
}

// Create a new HUD Object into the game
template <class O> // Template for adding a type of HUD object
O* HUD::new_hud_object(std::string name, HUD_Object* parent, Texture* texture, std::string vao_name) {
    if (contains_hud_object(name)) { std::cout << "HUD \"" << get_name() << "\" error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

	// Load the VAO
	VAO* vao = (*get_advanced_struct()->get_all_vaos())[vao_name];

	O* new_object = new O(get_advanced_struct(), name, parent, texture, vao);
	add_hud_object(name, new_object);
	return new_object;
}

// Create a new HUD Object into the HUD
template <class O> // Template for adding a type of HUD object
O* HUD::new_hud_object(std::string name, HUD_Object* parent, unsigned short texture_width, unsigned short texture_height, glm::vec4 texture_color, std::string vao_name) {
	if (contains_hud_object(name)) { std::cout << "HUD \"" << get_name() << "\" error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

	bool texture_resize = false; // Load the texture
	Texture* texture = get_advanced_struct()->new_texture(name, texture_width, texture_height, texture_color);

	// Load the VAO
	VAO* vao = (*get_advanced_struct()->get_all_vaos())[vao_name];

	O* new_object = new O(get_advanced_struct(), name, parent, texture, vao);
	add_hud_object(name, new_object);
	return new_object;
}

// Create a new HUD Object into the game
template <class O> // Template for adding a type of HUD
O* Window::new_hud(std::string name) {
	if (contains_scene(name)) { std::cout << "Matix game : error ! The objects \"" << name << "\" you want to create already exists." << std::endl; return 0; }

	O* new_object = new O(this, name);
	add_hud(name, new_object);
	return new_object;
}

#endif // SCLS_WINDOW
