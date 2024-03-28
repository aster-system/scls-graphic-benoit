#pragma once

#include "../headers/base_struct.h"
#include "../headers/model.h"

// Returns the base shader program variable for an HUD form
inline std::vector<Shader_Program_Variable> get_base_hud_shader_program_variables()
{
	// Create base Shader_Program_Variable for the shader program
	std::vector<Shader_Program_Variable> hud_attributes = std::vector<Shader_Program_Variable>();
	Shader_Program_Variable v1 = Shader_Program_Variable();
	Shader_Program_Variable v2 = Shader_Program_Variable();
	v1.vector_size = 3;
	v2.vector_size = 2;
	hud_attributes.push_back(v1);
	hud_attributes.push_back(v2);

	return hud_attributes;
};

// Returns the base shader program variable for a 3D form
inline std::vector<Shader_Program_Variable> get_base_3d_shader_program_variables()
{
	// Create base Shader_Program_Variable for the shader program
	std::vector<Shader_Program_Variable> base_3d_attributes = std::vector<Shader_Program_Variable>();
	Shader_Program_Variable v1 = Shader_Program_Variable();
	Shader_Program_Variable v2 = Shader_Program_Variable();
	Shader_Program_Variable v3 = Shader_Program_Variable();
	Shader_Program_Variable v4 = Shader_Program_Variable();
	v1.vector_size = 3;
	v2.vector_size = 2;
	v3.vector_size = 4;
	v4.vector_size = 3;
	base_3d_attributes.push_back(v1);
	base_3d_attributes.push_back(v2);
	base_3d_attributes.push_back(v3);
	base_3d_attributes.push_back(v4);

	return base_3d_attributes;
};

class Advanced_Struct : public Base_Struct
{
	// Class representing the advanced struct in the game
public:
	Advanced_Struct(double& a_mouse_x, double& a_mouse_y, int& a_window_width, int& a_window_height, std::string a_exec_path); // Advanced_Struct constructor
	// Add an existing VBO into the game
	void add_vbo(std::string name, VBO* vbo);
	bool contains_texture(std::string texture_name); // Returns if the struct contains a textures
	bool contains_vao(std::string type); // Returns if the struct contains a VAO
	// Returns if the struct contains a VBO
	bool contains_vbo(std::string name);
	Texture* get_texture(std::string texture_name, bool copy_texture = false); // Returns a texture in the struct
	void load_hud_VAOs(); // Loads the HUD VAOs in the advanced struct
	void load_VAOs(); // Loads the VAOs in the advanced struct
	Texture* new_texture(std::string name, std::string path, bool texture_resize = false); // Add a texture to the game
	Texture* new_texture(std::string texture_name, unsigned short width, unsigned short height, glm::vec4 color); // Returns a texture in the struct
	Texture* new_texture(std::string name, bool texture_resize = false); // Add a texture to the game with the most basic constructor
	// Create a new VAO into the game with the most basic constructor
	VAO* new_vao(std::string name, char* binary);
	// Create a new VAO into the game
	VAO* new_vao(std::string name, std::string vbo, std::string shader = "default");
	// Create a new VBO into the game
	VBO* new_vbo(std::string name);
	void unload_textures(); // Unload all the textures
	~Advanced_Struct(); // Advanced_Struct destructor

	// Getters and setters
	inline std::map<std::string, VAO*> *get_all_vaos() { return &all_vaos; };
	inline std::map<std::string, Texture*>* get_textures() { return &textures; };
	inline VAO* get_vao(std::string vao_name) {if(!contains_vao(vao_name)) return 0; return all_vaos[vao_name];};
	inline std::map<std::string, VAO*>* get_vaos() { return &all_vaos; };
	inline std::map<std::string, VBO*>* get_vbos() { return &a_vbos; };
private:
	// Each shaders, with their name as key, in the game
	std::map<std::string, Shader_Program> a_shaders_programs = std::map<std::string, Shader_Program>();
	// Each VBOs base, with their name as key, in the game
	std::map<std::string, VBO*> a_vbos = std::map<std::string, VBO*>();
	std::map<std::string, Texture*> textures = std::map<std::string, Texture*>(); // Each texture, with their name as key, in the game
	std::map<std::string, VAO*> all_vaos = std::map<std::string, VAO*>(); // Each vaos, with their type as key, in the game
};
