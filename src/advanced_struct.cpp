#include "../headers/advanced_struct.h"

// Part constructor
Part::Part(glm::vec3 a_position, glm::vec3 a_rotation, glm::vec3 a_scale, std::string a_type, std::string a_texture_path, void* a_base_object) : position(a_position), rotation(a_rotation), scale(a_scale), type(a_type), texture_path(a_texture_path), base_object(a_base_object)
{

}

// Part copy constructor
Part::Part(const Part& copy) : Part(copy.position, copy.rotation, copy.scale, copy.type, copy.texture_path, copy.base_object)
{

}

// Part destructor
Part::~Part()
{
	if (base_object != 0)
	{
		delete base_object;
		base_object = 0;
	}
}

// Advanced_Struct constructor
Advanced_Struct::Advanced_Struct(double& a_mouse_x, double& a_mouse_y, int& a_window_width, int& a_window_height, std::string a_exec_path): Base_Struct(a_mouse_x, a_mouse_y, a_window_width, a_window_height, a_exec_path)
{

}

// Add an existing VBO into the game
void Advanced_Struct::add_vbo(std::string name, VBO* vbo)
{
	if (!contains_vbo(name))
	{
		(*get_vbos())[name] = vbo;
	}
	else
	{
		error("Matix", "The \"" + name + "\" texture you want to add already exists.");
	}
}

// Asscls::sign to a number a part
void Advanced_Struct::assign_part(unsigned int number, Part* part)
{
	if (contains_part(number)) { std::cout << "Matrix game : error ! The part \"" << number << "\" you want to add already exist." << std::endl; return; }
	(*get_parts())[number] = part;
}


// Returns if the struct contains a part
bool Advanced_Struct::contains_part(unsigned int number)
{
	std::map<unsigned int, Part*>* parts = get_parts();
	for (std::map<unsigned int, Part*>::iterator it = parts->begin(); it != parts->end(); it++)
	{
		if (it->first == number) { return true; } // Verify each part name (first element of map)
	}
	return false;
}

// Returns if the struct contains a texture
bool Advanced_Struct::contains_texture(std::string texture_name)
{
	std::map<std::string, Texture*>* textures = get_textures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		if (it->first == texture_name) { return true; } // Verify each texture path (first element of map)
	}
	return false;
}

// Returns if the struct contains a VAO
bool Advanced_Struct::contains_vao(std::string type)
{
	std::map<std::string, VAO*>* vaos = get_vaos();
	for (std::map<std::string, VAO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		if (it->first == type) { return true; } // Verify each vaos name (first element of map)
	}
	return false;
}

// Returns if the struct contains a VBO
bool Advanced_Struct::contains_vbo(std::string name)
{
	std::map<std::string, VBO*>* vaos = get_vbos();
	for (std::map<std::string, VBO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		if (it->first == name) { return true; } // Verify each vbos name (first element of map)
	}
	return false;
}

// Returns a part
Part* Advanced_Struct::get_part(unsigned int number)
{
	if (contains_part(number))
	{
		return (*get_parts())[number];
	}
	else
	{
		std::cout << "Matrix game : error ! The part \"" << number << "\" you want to get does not exist." << std::endl;
		return 0;
	}
}

// Returns a texture in the struct
Texture* Advanced_Struct::get_texture(std::string texture_name, bool copy_texture)
{
	if (contains_texture(texture_name))
	{
	    if(copy_texture)
        {
            Texture* to_copy = (*get_textures())[texture_name];
            Texture* to_return = new Texture(*to_copy);
            (*get_textures())[texture_name + ";copy" + std::to_string(to_copy->copy_count())] = to_return;
            return to_return;
        }
		return (*get_textures())[texture_name];
	}
	else
	{
		error("Matix", "The \"" + texture_name + "\" texture you want to use does not exists.");
	}
}

// Loads the HUD VAOs in the advanced struct
void Advanced_Struct::load_hud_VAOs()
{
	// Create the base shaders
	a_shaders_programs["hud_default"] = Shader_Program(Shader_Program::HUD_Default);

	// Define attributes for VAOs
	std::vector<Shader_Program_Variable> hud_attributes = get_base_hud_shader_program_variables();

	// Create VAOs
	VBO hud_vbo = VBO(hud_attributes, VBO::get_base_hud_vbo(hud_attributes), false);
	all_vaos["hud_default"] = new VAO(a_shaders_programs["hud_default"], hud_attributes, &hud_vbo); all_vaos["hud_default"]->load_vao();
}

// Loads the VAOs in the advanced struct
void Advanced_Struct::load_VAOs()
{
	// Create the base shaders
	a_shaders_programs["default"] = Shader_Program();

	// Create a base texture
	(*get_textures())["transparent"] = new Texture(5, 5, glm::vec4(0.0, 0.0, 0.0, 0.0));
	(*get_textures())["white"] = new Texture(5, 5, glm::vec4(1.0, 1.0, 1.0, 1.0));

	load_hud_VAOs();
}

// Add a texture to the game
Texture* Advanced_Struct::new_texture(std::string name, std::string path, bool texture_resize)
{
	if (!contains_texture(name))
	{
		Texture* texture = new Texture(path, texture_resize);
		(*get_textures())[name] = texture;
		return texture;
	}
	else
	{
		error("Matix", "The \"" + name + "\" texture you want to add already exists.");
	}
}

// Returns a texture in the struct
Texture* Advanced_Struct::new_texture(std::string texture_name, unsigned short width, unsigned short height, glm::vec4 color)
{
	if (!contains_texture(texture_name))
	{
		Texture* texture = new Texture(width, height, color);
		(*get_textures())[texture_name] = texture;
		return texture;
	}
	else
	{
		error("Matix", "The \"" + texture_name + "\" texture you want to add already exists.");
	}
}

// Add a texture to the game with the most basic constructor
Texture* Advanced_Struct::new_texture(std::string name, bool texture_resize)
{
	if (!contains_texture(name))
	{
		Texture* texture = new Texture();
		(*get_textures())[name] = texture;
		return texture;
	}
	else
	{
		error("Matix", "The \"" + name + "\" texture you want to add already exists.");
	}
}

// Create a new VAO into the game with the most basic constructor
VAO* Advanced_Struct::new_vao(std::string name, char* binary)
{
	if (contains_vao(name))
	{
		std::cout << "Matrix game : error ! The \"" << name << "\" VAO already exists." << std::endl;
		return 0;
	}

	unsigned int current_pos = 0;
	unsigned char shader_size = binary[current_pos]; current_pos++;
	std::string shader = "";
	for (int i = 0; i < shader_size; i++)
	{
		shader += binary[current_pos]; current_pos++;
	}

	unsigned char vbo_size = binary[current_pos]; current_pos++;
	std::string vbo = "";
	for (int i = 0; i < vbo_size; i++)
	{
		vbo += binary[current_pos]; current_pos++;
	}

	return new_vao(name, vbo, shader);
}

// Create a new VAO into the game
VAO* Advanced_Struct::new_vao(std::string name, std::string vbo, std::string shader)
{
	if (contains_vao(name))
	{
		std::cout << "Matrix game : error ! The \"" << name << "\" VAO already exists." << std::endl;
		return 0;
	}

	if (!contains_vbo(vbo))
	{
		std::cout << "Matrix game : error ! The \"" << name << "\" VAO use the \"" << vbo << "\" VBO, which does not exist." << std::endl;
		return 0;
	}

	all_vaos[name] = new VAO(&a_shaders_programs[shader], a_vbos[vbo]);
	all_vaos[name]->load_vao();
	return all_vaos[name];
}

// Create a new VBO into the game
VBO* Advanced_Struct::new_vbo(std::string name)
{
	if (!contains_vbo(name))
	{
		VBO* vbo = new VBO();
		add_vbo(name, vbo);
		return vbo;
	}
	else
	{
		error("Matix", "The \"" + name + "\" texture you want to add already exists.");
	}
	return 0;
}

// Unload all the textures
void Advanced_Struct::unload_textures()
{
	std::map<std::string, Texture*>* textures = get_textures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
	{
		delete it->second; // Delete textures
		it->second = 0;
	}
}

// Advanced_Struct destructor
Advanced_Struct::~Advanced_Struct()
{
	std::map<std::string, VAO*>* vaos = get_all_vaos();
	for (std::map<std::string, VAO*>::iterator it = vaos->begin(); it != vaos->end(); it++)
	{
		delete it->second; // Delete VAOs
		it->second = 0;
	}

	unload_textures();

	std::map<std::string, VBO*>* vbos = get_vbos();
	for (std::map<std::string, VBO*>::iterator it = vbos->begin(); it != vbos->end(); it++)
	{
		delete it->second; // Delete VBOs
		it->second = 0;
	}

	std::map<unsigned int, Part*>* parts = get_parts();
	for (std::map<unsigned int, Part*>::iterator it = parts->begin(); it != parts->end(); it++)
	{
		delete it->second; // Delete parts
		it->second = 0;
	}
}
