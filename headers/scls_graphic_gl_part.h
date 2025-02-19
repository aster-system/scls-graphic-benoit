//******************
//
// scls_graphic_gl_part.h
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The 3D "Margaret" part allows the user to simply display a 3D graphic window.
//
// This file contains the lowest level OpenGL handling.
//

#ifndef SCLS_GRAPHIC_GL_PART
#define SCLS_GRAPHIC_GL_PART

#include "../headers/scls_graphic_window_base_struct.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // The Shaders handling
    //
    //*********

    // Predefined functions for the shaders
    namespace shader_content {
        // Base lighting function for a shader
        extern std::string lighting_function();
    }

    struct Shader_Program_Variable {
        // Struct representing the values for a "in" variable in the shader program
        // Type of the shader program
        unsigned short type = GL_FLOAT; // 0 = GL_FLOAT
        // Size of the variable, in vector part
        unsigned short vector_size = 1;
    };

    class Shader_Program {
        // Class representing a shader program interface
    public:
        // Every type of built-in shaders
        enum Built_In_Shader {
            Curved,
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
        void set_uniformb_value(std::string name, bool v); // Change the value of a uniform bool value
        void set_uniform1f_value(std::string name, float v1); // Change the value of a uniform float value
        void set_uniform2f_value(std::string name, float v1, float v2); // Change the value of a uniform vec2 float value
        void set_uniform3f_value(std::string name, float v1, float v2, float v3); // Change the value of a uniform vec3 float value
        void set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4); // Change the value of a uniform vec4 float value
        void set_uniform4f_value(std::string name, glm::vec4 v); // Change the value of a uniform vec4 float value
        // Change the value of a uniform color value
        inline void set_uniform4f_value(std::string name, scls::Color color){set_uniform4f_value(name, static_cast<double>(color.red()) / 255.0, static_cast<double>(color.green()) / 255.0, static_cast<double>(color.blue()) / 255.0, static_cast<double>(color.alpha()) / 255.0);};
        void set_uniform4fv_value(std::string name, glm::mat4 fv); // Change the value of a uniform mat4 float value
        void use(); // Start using the shader
        ~Shader_Program(); // Shader_Program destructor

        // Getters and setters
        inline std::string get_fragment_shader() const { return fragment_shader; };
        inline std::string get_vertex_shader() const { return vertex_shader; };
        inline bool loaded() {return a_loaded;};

        // Return the default shaders
        static std::string default_gui_blend_colors();
        static std::string default_gui_border_handling() {return std::string("if(tex_pos[0] < border_width.y || tex_pos[1] < border_width.z || tex_pos[0] > 1.0 - border_width.w || tex_pos[1] > 1.0 - border_width.x){final_color = blend_colors(border_color, final_color);}");};
        static std::string default_gui_extremum_handling() {return std::string("if(tex_pos.y > object_extremum.w || tex_pos.y < object_extremum.y){discard;}");};
        static std::string get_built_in_fragment_shader(Built_In_Shader shader_type) {if (shader_type == Default || shader_type == Curved) {return get_default_fragment_shader();}return get_default_hud_fragment_shader();};
        static std::string get_built_in_vertex_shader(Built_In_Shader shader_type) {if (shader_type == Curved) {return curved_vertex_shader();}else if (shader_type == Default){return get_default_vertex_shader();}return get_default_hud_vertex_shader();};
        static std::string get_default_fragment_shader() {
            std::string to_return = "#version 330 core\n";
            to_return += "in vec3 frag_pos;\n";
            to_return += "in vec3 normal;\n";
            to_return += "in vec3 tex_multiplier;\n";
            to_return += "in vec2 tex_pos;\n";
            to_return += "in vec4 tex_rect;\n";
            to_return += "out vec4 FragColor;\n";
            to_return += "uniform bool reverse_texture_x;\n";
            to_return += "uniform bool reverse_texture_y;\n";
            to_return += "uniform vec3 scale;\n";
            to_return += "uniform sampler2D texture_0;\n";
            to_return += "uniform bool texture_binded;\n";

            // Use the lighting function
            //to_return += shader_content::lighting_function();

            // Get the good position of the object
            to_return += "void main(){\n";
            to_return += "vec4 final_color = vec4(0, 0, 0, 0);\n";
            to_return += "if(texture_binded){";
            to_return += "  vec2 final_tex_pos = tex_pos * tex_rect.zw;";
            to_return += "  if(reverse_texture_x){final_tex_pos.x = 1.0 - final_tex_pos.x;}";
            to_return += "  if(reverse_texture_y){final_tex_pos.y = 1.0 - final_tex_pos.y;}";
            to_return += "  if (tex_multiplier.x == 1) final_tex_pos.x = final_tex_pos.x * scale.x;";
            to_return += "  else if (tex_multiplier.x == 2) final_tex_pos.y = final_tex_pos.y * scale.x;";
            to_return += "  if (tex_multiplier.y == 1) final_tex_pos.x = final_tex_pos.x * scale.y;";
            to_return += "  else if (tex_multiplier.y == 2) final_tex_pos.y = final_tex_pos.y * scale.y;\n";
            to_return += "  if(tex_multiplier.z == 1) final_tex_pos.x = final_tex_pos.x * scale.z;\n";
            to_return += "  else if (tex_multiplier.z == 2)final_tex_pos.y = final_tex_pos.y * scale.z;\n";
            to_return += "  while(final_tex_pos.x > tex_rect[2])final_tex_pos.x -= tex_rect[2];\n";
            to_return += "  while(final_tex_pos.y > tex_rect[3])final_tex_pos.y -= tex_rect[3];\n";
            to_return += "  final_color = texture(texture_0, tex_rect.xy + final_tex_pos);\n";
            to_return += "} else {";
            to_return += "final_color = vec4(1, 1, 1, 1);";
            to_return += "}";

            // Edit the color as necesary with lighting
            //to_return += "final_color = apply_lighting(final_color);\n";
            to_return += "FragColor = final_color;\n";
            to_return += "}";
            return to_return;
        };
        static std::string get_default_hud_fragment_shader() {
            std::string to_return = "#version 330 core\n";
            to_return += "in vec2 tex_pos;"; // Uniform / in/out variables
            to_return += "out vec4 FragColor;";
            to_return += "uniform vec4 background_color;";
            to_return += "uniform vec4 border_color;";
            to_return += "uniform vec4 border_width;";
            to_return += "uniform vec4 object_extremum;";
            to_return += "uniform vec4 object_rect;";
            to_return += "uniform sampler2D texture_0;";
            to_return += "uniform bool texture_binded;";
            to_return += "uniform vec4 texture_rect;";

            // Blend function
            to_return += default_gui_blend_colors();

            // Main function
            to_return += "void main(){";
            // Check if the object should be displayed
            to_return += default_gui_extremum_handling();
            to_return += "vec4 final_color = background_color;";
            to_return += default_gui_border_handling();
            to_return += "else if(texture_binded && tex_pos[0] >= texture_rect[0] && tex_pos[1] >= texture_rect[1] && tex_pos[0] < texture_rect[0] + texture_rect[2] && tex_pos[1] < texture_rect[1] + texture_rect[3]){";
            to_return += "vec2 real_tex_pos = tex_pos - texture_rect.xy;";
            to_return += "real_tex_pos /= texture_rect.zw;";
            to_return += "final_color = blend_colors(texture(texture_0, real_tex_pos), final_color);}"; //*/
            to_return += "FragColor = final_color;}";
            return to_return;
        };
        static std::string get_default_vertex_shader() {
            std::string to_return = "#version 330 core\n";
            to_return += "layout(location = 1) in vec3 in_normal;\n";
            to_return += "layout(location = 0) in vec3 in_pos;\n";
            to_return += "layout(location = 2) in vec2 in_tex_pos;\n";
            to_return += "layout(location = 3) in vec4 in_tex_rect;\n";
            to_return += "layout(location = 4) in vec3 in_tex_multiplier;\n";
            to_return += "out vec3 frag_pos;\n";
            to_return += "out vec3 normal;\n";
            to_return += "out vec3 tex_multiplier;\n";
            to_return += "out vec2 tex_pos;out vec4 tex_rect;\n";
            to_return += "uniform mat4 model;\n";
            to_return += "uniform mat4 projection;\n";
            to_return += "uniform mat4 view;\n";
            to_return += "void main(){\n";
            to_return += "frag_pos = vec3(model * vec4(in_pos, 1.0));\n";
            to_return += "normal = mat3(transpose(inverse(model))) * in_normal;\n";
            to_return += "tex_multiplier = in_tex_multiplier;\n";
            to_return += "tex_pos = in_tex_pos;\n";
            to_return += "tex_rect = in_tex_rect;\n";
            to_return += "gl_Position = projection * view * model * vec4(in_pos.xyz, 1.0);\n}";
            return to_return;
        };
        static std::string curved_vertex_shader();
        static std::string get_default_hud_vertex_shader() {
            std::string to_return = "#version 330 core\n";
            to_return += "layout(location = 0) in vec3 position;";
            to_return += "layout(location = 1) in vec2 texture_position;";
            to_return += "out vec2 tex_pos;";
            to_return += "uniform mat4 model;";
            to_return += "void main(){";
            to_return += "tex_pos = texture_position;";
            to_return += "gl_Position = model * vec4(position.xyz, 1.0);}";
            return to_return;
        };
    private:
        bool a_loaded = false;
        unsigned int shader_program = 0; // Handle to the shader program

        std::string fragment_shader = ""; // Content of the fragment shader
        std::string vertex_shader = ""; // Content of the vertex shader
    };

    //*********
    //
    // The VBO class
    //
    //*********

    class VBO {
        // Class representing a VBO interface
    public:
        // Most basic VBO constructor
        VBO();
        VBO(std::vector<Shader_Program_Variable> attributes, std::vector<double> datas, bool use_ebo = true); // VBO complete constructor
        VBO(std::vector<Shader_Program_Variable> attributes, bool fill_datas = true, bool use_ebo = true); // VBO constructor
        void bind(); // Bind the VBO into the GPU memory
        void bind_buffer(); // Bind the buffer data of the VBO
        unsigned int get_vertice_number(); // Returns the number of vertices into the VBO
        // Load the VBO from binary
        void load_from_binary(char* binary);
        // Load the vertices from a file
        void load_from_file(std::string path);
        // Load the VBO into the GPU memory
        void load_vbo();
        // Create a new VBO from this one
        VBO* new_copy();
        // Unbind the VBO from the GPU memory
        void unbind();
        // VBO destructor
        ~VBO();

        // Returns the datas convert in float
        inline std::vector<float> datas_to_float() {std::vector<float> datas = std::vector<float>(a_datas.size());for(int i = 0;i<static_cast<int>(a_datas.size());i++) {datas[i] = a_datas[i];}return datas;};

        // Built-in VBOs
        // Returns the datas for a cube vbo
        static std::vector<double> cube_vbo() {
            std::vector<double> a_datas = std::vector<double>();
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(1.0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(1.0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(1.0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(1.0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(-1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.75);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.5);
            a_datas.push_back(0.66667);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(-0.5);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0);
            a_datas.push_back(0.33333);
            a_datas.push_back(0.25);
            a_datas.push_back(1.0);
            a_datas.push_back(-1.0);
            a_datas.push_back(0.0);
            return a_datas;
        };
        // Returns the datas for a gui vbo
        static std::vector<double> gui_vbo(std::vector<Shader_Program_Variable> attributes) {
            std::vector<double> a_datas = std::vector<double>();
            a_datas.push_back(1.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);

            a_datas.push_back(1.0);
            a_datas.push_back(1.0);

            if (attributes.size() > 2) {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);

            a_datas.push_back(0.0);
            a_datas.push_back(1.0);

            if (attributes.size() > 2)
            {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.0);

            a_datas.push_back(1.0);
            a_datas.push_back(0.0);

            if (attributes.size() > 2)
            {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            a_datas.push_back(1.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.0);

            a_datas.push_back(1.0);
            a_datas.push_back(0.0);

            if (attributes.size() > 2)
            {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            a_datas.push_back(0.0);
            a_datas.push_back(0.0);
            a_datas.push_back(0.0);

            a_datas.push_back(0.0);
            a_datas.push_back(0.0);

            if (attributes.size() > 2)
            {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            a_datas.push_back(0.0);
            a_datas.push_back(1.0);
            a_datas.push_back(0.0);

            a_datas.push_back(0.0);
            a_datas.push_back(1.0);

            if (attributes.size() > 2)
            {
                a_datas.push_back(0.0);
                a_datas.push_back(0.0);
                a_datas.push_back(1.0);
                a_datas.push_back(1.0);

                a_datas.push_back(0.0);
                a_datas.push_back(-1.0);
                a_datas.push_back(1.0);
            }

            return a_datas;
        }

        // Getters and setters
        inline std::vector<Shader_Program_Variable> *get_attributes() { return &a_attributes; };
        inline std::vector<double> get_datas() { return a_datas; };
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
        std::vector<double> a_datas = std::vector<double>(); // Each vertices in the VBO
    };

    //*********
    //
    // The VAO class
    //
    //*********

    class VAO {
        // Class representing a VAO interface
    public:
        // Most basic VAO constructor
        VAO(const std::shared_ptr<VBO>& vbo);
        // Most usefull VAO constructor
        VAO(Shader_Program shader_program, const std::shared_ptr<VBO>& vbo);
        VAO(std::string shader_path, const std::shared_ptr<VBO>& vbo);

        // Bind the VAO into the GPU memory
        virtual void bind(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));
        Shader_Program load_shader_program(std::string shader_path); // Load and return a shader
        // Load the VAO into the GPU memory
        void load_vao();
        virtual void render(glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0)); // Render the VAO
        unsigned int triangle_number(); // Returns the number of triangle to draw
        virtual ~VAO(); // VAO destructor


        // Getters and setters
        inline Shader_Program* get_shader_program() { return a_shader_program; };
        inline unsigned int& get_vao() { return vao; };
        inline VBO* get_vbo() { return a_vbo.get(); };
        inline bool loaded() {return a_loaded;};
        inline std::shared_ptr<VBO>& vbo_shared_ptr() {return a_vbo;};
    protected:
        bool a_loaded = false; //If the VAO is loaded or not
        unsigned int vao = 0; // Handle to the VAO

        // Pointer to the shader program
        Shader_Program *a_shader_program = 0;
        // Shared pointer to the VBO
        std::shared_ptr<VBO> a_vbo = 0;
    };

    //*********
    //
    // The Texture class
    //
    //*********

    class Texture {
        // Class representing a texture interface
    public:
        Texture(std::string a_texture_path, bool a_resize = true); // Texture constructor
        // Texture constructor much modulable
        Texture(unsigned short width, unsigned short height, glm::vec4 color, bool a_resize = true);
        // Texture constructor much modulable
        Texture(unsigned short width, unsigned short height, scls::Color color, bool a_resize = true);
        // Texture copy constructor
        Texture(Texture& texture_to_copy);
        // Most basic texture constructor
        Texture();
        void bind(); // Bind the texture into the GPU memory
        void change_texture(); // Change the texture of the texture according to image
        // Load the texture into the GPU memory
        void load_texture();
        ~Texture(); // Texture destructor

        // Set the image from a path
        inline void set_image_by_path(std::string path) { set_image(new scls::Image(path)); };

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline Fraction image_ratio() {return Fraction(get_image()->width(), get_image()->height());};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned short copy_count() {return a_copy_count;};
        inline scls::Image* get_image() { return a_image.get(); };
        inline glm::vec2 get_texture_size() { return glm::vec2(width, height); };
        inline std::string get_texture_path() { return texture_path; };
        inline std::shared_ptr<scls::Image>& image_shared_ptr() { return a_image; };
        inline bool loaded() {return a_loaded;};
        inline void set_image(scls::Image* new_image) {
            if(new_image == 0 && get_image() != 0) {
                glDeleteTextures(1, &texture_id);
            }
            else if(get_image() == 0 && new_image != 0) {
                glGenTextures(1, &texture_id);
            }

            a_image.reset(new_image);
            if(get_image() != 0 && get_image()->flip_x_number() % 2 == 0) get_image()->flip_x();
            change_texture();
        };
        inline void set_image(std::shared_ptr<scls::Image> new_image) {
            if(new_image.get() == 0 && get_image() != 0) {
                glDeleteTextures(1, &texture_id);
            }
            else if(get_image() == 0 && new_image.get() != 0) {
                glGenTextures(1, &texture_id);
            }

            a_image = new_image;
            if(get_image() != 0 && get_image()->flip_x_number() % 2 == 0) get_image()->flip_x();
            change_texture();
        };
        inline bool use_resize() { return resize; };
    private:
        // Number of copy of this texture
        unsigned short a_copy_count = 0;
        int height = 0; // Height of the texture
        // SCLS image of this texture
        std::shared_ptr<scls::Image> a_image;
        bool a_loaded = false;
        bool resize = true; // If the shader resize the texture or not
        unsigned int texture_id = 0; // Handle to the texture
        std::string texture_path = ""; // Path of the texture
        int width = 0; // Width of the texture
    };
}

#endif // SCLS_GRAPHIC_GL_PART
