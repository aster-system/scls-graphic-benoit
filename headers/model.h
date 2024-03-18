#pragma once

#include "../headers/base_struct.h"

struct Shader_Program_Variable
{
	// Struct representing the values for a "in" variable in the shader program
	unsigned short type = GL_FLOAT; // 0 = GL_FLOAT
	unsigned short vector_size = 1;
};

class Shader_Program
{
	// Class representing a shader program interface
public:
	// Every type of built-in shaders
	enum Built_In_Shader {
		Default,
		HUD_Default
	};

	Shader_Program(Built_In_Shader shader_type = Default); // Shader_Program constructor
	Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader); // Shader_Program constructor
	// Load the Shader
	void load_shader();
	// Create a new Shader_Program from this one
	Shader_Program* new_copy();
	void pass_variable(std::vector<Shader_Program_Variable> *variables); // Pass variable to the shader program
	void set_uniform1f_value(std::string name, float v1); // Change the value of a uniform float value
	void set_uniform2f_value(std::string name, float v1, float v2); // Change the value of a uniform vec2 float value
	void set_uniform3f_value(std::string name, float v1, float v2, float v3); // Change the value of a uniform vec3 float value
	void set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4); // Change the value of a uniform vec4 float value
	void set_uniform4f_value(std::string name, glm::vec4 v); // Change the value of a uniform vec4 float value
	void set_uniform4fv_value(std::string name, glm::mat4 fv); // Change the value of a uniform mat4 float value
	void use(); // Start using the shader
	~Shader_Program(); // Shader_Program destructor

	// Getters and setters
	inline std::string get_fragment_shader() const { return fragment_shader; };
	inline std::string get_vertex_shader() const { return vertex_shader; };

	// Return the default shaders
	static std::string get_built_in_fragment_shader(Built_In_Shader shader_type)
	{
		if (shader_type == Default)
		{
			return get_default_fragment_shader();
		}
		else if (shader_type == HUD_Default)
		{
			return get_default_hud_fragment_shader();
		}
	};
	static std::string get_built_in_vertex_shader(Built_In_Shader shader_type)
	{
		if (shader_type == Default)
		{
			return get_default_vertex_shader();
		}
		else if (shader_type == HUD_Default)
		{
			return get_default_hud_vertex_shader();
		}
	};
	static std::string get_default_fragment_shader()
	{
		return "#version 330 core\nin vec3 tex_multiplier;in vec2 tex_pos;in vec4 tex_rect;out vec4 FragColor;uniform vec3 scale;uniform sampler2D texture_0;void main(){vec2 final_tex_pos = (tex_pos - tex_rect.xy);if (tex_multiplier.x == 0)final_tex_pos.x = final_tex_pos.x * scale.x;else if (tex_multiplier.x == 1)final_tex_pos.y = final_tex_pos.y * scale.x;if (tex_multiplier.y == 0)final_tex_pos.x = final_tex_pos.x * scale.y;else if (tex_multiplier.y == 1)final_tex_pos.y = final_tex_pos.y * scale.y;if (tex_multiplier.z == 0)final_tex_pos.x = final_tex_pos.x * scale.z;else if (tex_multiplier.z == 1)final_tex_pos.y = final_tex_pos.y * scale.z;while (final_tex_pos.x > tex_rect[2])final_tex_pos.x -= tex_rect[2];while (final_tex_pos.y > tex_rect[3])final_tex_pos.y -= tex_rect[3];FragColor = texture(texture_0, tex_rect.xy + final_tex_pos);}";
	};
	static std::string get_default_hud_fragment_shader()
	{
	    // Experimental : "#version 330 core\nin vec2 tex_pos;out vec4 FragColor;uniform vec4 border_color;uniform vec4 border_width;uniform sampler2D texture_0;void main(){vec4 color = border_color;if(tex_pos[0] >= border_width[1] && tex_pos[0] <= 1.0 - border_width[3] && tex_pos[1] >= border_width[0] && tex_pos[1] <= 1.0 - border_width[2]){vec2 texture_pos = tex_pos;texture_pos[0]-=border_width[1];texture_pos[1]-=border_width[0];texture_pos[0]*=(1.0+(border_width[1]+border_width[3]));texture_pos[1]*=(1.0+(border_width[0]+border_width[2]));color = texture(texture_0, texture_pos);}FragColor = color;}"
		return "#version 330 core\nin vec2 tex_pos;out vec4 FragColor;uniform vec4 border_color;uniform vec4 texture_rect;uniform sampler2D texture_0;void main(){vec4 color = border_color;if(tex_pos[0] >= texture_rect[0] && tex_pos[0] <= texture_rect[0] + texture_rect[2] && tex_pos[1] >= texture_rect[1] && tex_pos[1] <= texture_rect[1] + texture_rect[3]){color = texture(texture_0, tex_pos);}FragColor = color;}";
	};
	static std::string get_default_vertex_shader()
	{
		return "#version 330 core\nlayout(location = 0) in vec3 aPos;layout(location = 1) in vec2 in_tex_pos;layout(location = 2) in vec4 in_tex_rect;layout(location = 3) in vec3 in_tex_multiplier;out vec3 tex_multiplier;out vec2 tex_pos;out vec4 tex_rect;uniform mat4 model;uniform mat4 projection;uniform mat4 view;void main(){tex_multiplier = in_tex_multiplier;tex_pos = in_tex_pos;tex_rect = in_tex_rect;gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);}";
	};
	static std::string get_default_hud_vertex_shader()
	{
		return "#version 330 core\nlayout(location = 0) in vec3 position;layout(location = 1) in vec2 texture_position;out vec2 tex_pos;uniform mat4 model;void main(){tex_pos = texture_position;gl_Position = model * vec4(position.xyz, 1.0);}";
	};
private:
	unsigned int shader_program = 0; // Handle to the shader program

	std::string fragment_shader = ""; // Content of the fragment shader
	std::string vertex_shader = ""; // Content of the vertex shader
};

class VBO
{
	// Class representing a VBO interface
public:
	// Most basic VBO constructor
	VBO();
	VBO(std::vector<Shader_Program_Variable> attributes, std::vector<float> datas, bool use_ebo = true); // VBO complete constructor
	VBO(std::vector<Shader_Program_Variable> attributes, bool fill_datas = true, bool use_ebo = true); // VBO constructor
	void bind(); // Bind the VBO into the GPU memory
	void bind_buffer(); // Bind the buffer data of the VBO
	unsigned int get_vertice_number(); // Returns the number of vertices into the VBO
	// Load the VBO from binary
	void load_from_binary(char* binary);
	void load_from_file(std::string path); // Load the vertices from a file
	// Load the VBO into the GPU memory
	void load_vbo();
	// Create a new VBO from this one
	VBO* new_copy();
	void unbind(); // Unbind the VBO from the GPU memory
	~VBO(); // VBO destructor

	// Getters and setters
	inline std::vector<Shader_Program_Variable> *get_attributes() { return &a_attributes; };
	static std::vector<float> get_base_hud_vbo(std::vector<Shader_Program_Variable> attributes) {
		std::vector<float> a_datas = std::vector<float>();
		a_datas.push_back(0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(1.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(1.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(0.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(1.0f);
		a_datas.push_back(0.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(-0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(0.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		a_datas.push_back(-0.5f);
		a_datas.push_back(0.5f);
		a_datas.push_back(0.0f);

		a_datas.push_back(0.0f);
		a_datas.push_back(1.0f);

		if (attributes.size() > 2)
		{
			a_datas.push_back(0.0f);
			a_datas.push_back(0.0f);
			a_datas.push_back(1.0f);
			a_datas.push_back(1.0f);

			a_datas.push_back(0.0f);
			a_datas.push_back(-1.0f);
			a_datas.push_back(1.0f);
		}

		return a_datas;
	}
	inline std::vector<float> get_datas() { return a_datas; };
	inline std::vector<unsigned int> get_indices() { return a_indices; };
	inline unsigned int* get_indices_in_array() { return a_indices.data(); };
	inline unsigned int& get_vbo() { return vbo; };
	inline bool is_using_ebo() { return a_use_ebo; };
	inline void set_attributes(std::vector<Shader_Program_Variable> attributes) { a_attributes = attributes; };
private:
	unsigned int ebo = 0; // Handle to the EBO
	unsigned int vbo = 0; // Handle to the VBO

	bool a_use_ebo = false; // If the VBO use EBO
	std::vector<Shader_Program_Variable> a_attributes = std::vector<Shader_Program_Variable>(); // Each Shader_Program_Variable in the VBO
protected:
	std::vector<unsigned int> a_indices = std::vector<unsigned int>(); // Each EBOs in the VBO
	std::vector<float> a_datas = std::vector<float>(); // Each vertices in the VBO
};

class VAO
{
	// Class representing a VAO interface
public:
	// Most basic VAO constructor
	VAO();
	// Most usefull VAO constructor
	VAO(Shader_Program* shader_program, VBO* vbo);
	VAO(std::string shader_path, std::vector<Shader_Program_Variable> a_attributes, VBO* vbo); // VAO constructor
	// VAO constructor
	VAO(Shader_Program* shader_program, std::vector<Shader_Program_Variable> a_attributes, VBO* vbo);
	// VAO constructor
	VAO(Shader_Program shader_program, std::vector<Shader_Program_Variable> a_attributes, VBO* vbo);
	virtual void bind(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0)); // Bind the VAO into the GPU memory
	Shader_Program load_shader_program(std::string shader_path); // Load and return a shader
	// Load the VAO into the GPU memory
	void load_vao();
	virtual void render(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0)); // Render the VAO
	unsigned int triangle_number(); // Returns the number of triangle to draw
	~VAO(); // VAO destructor


	// Getters and setters
	inline Shader_Program* get_shader_program() { return a_shader_program; };
	inline unsigned int& get_vao() { return vao; };
	inline VBO* get_vbo() { return a_vbo; };
protected:
	unsigned int vao = 0; // Handle to the VAO

	// Pointer to the shader program
	Shader_Program *a_shader_program = 0;
	VBO *a_vbo = 0; // Pointer to the VBO
};

class Texture
{
	// Class representing a texture interface
public:
	Texture(std::string a_texture_path, bool a_resize = true); // Texture constructor
	// Texture constructor much modulable
	Texture(unsigned short width, unsigned short height, glm::vec4 color, bool a_resize = true);
	// Most basic texture constructor
	Texture();
	void bind(); // Bind the texture into the GPU memory
	void change_texture(); // Change the texture of the texture according to image
	// Load the texture into the GPU memory
	void load_texture();
	~Texture(); // Texture destructor

	// Getters and setters
	inline basix::Image* get_image() { return a_image; };
	inline glm::vec2 get_texture_size() { return glm::vec2(width, height); };
	inline std::string get_texture_path() { return texture_path; };
	inline double image_ratio() {return static_cast<double>(get_image()->get_width()) / static_cast<double>(get_image()->get_height());};
	inline void set_image(basix::Image* new_image) {delete a_image;a_image = new_image; a_image->flip_x(); change_texture();};
	inline bool use_resize() { return resize; };
private:
	int height = 0; // Height of the texture
	// Basix image of this texture
	basix::Image* a_image = 0;
	bool resize = true; // If the shader resize the texture or not
	unsigned int texture_id = 0; // Handle to the texture
	std::string texture_path = ""; // Path of the texture
	int width = 0; // Width of the texture
};
