//******************
//
// scls_graphic_gl_part.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The 3D "Margaret" part allows the user to simply display a 3D graphic window.
//
// This file contains the source code of scls_graphic_gl_part.h.
//

#include "../headers/scls_graphic_gl_part.h"

namespace scls {

    //*********
    //
    // The Shaders handling
    //
    //*********

    // Predefined functions for the shaders
    namespace shader_content {
        std::string lighting_function() {
            std::string to_return = "";
            to_return += "vec4 apply_lighting(vec4 base_color) {";
            to_return += "vec3 norm = normalize(normal);\n";
            to_return += "vec3 lightDir = normalize(vec3(1000, 1000, 0) - frag_pos);\n";
            to_return += "float diff = max(dot(norm, lightDir), 0.0) / 1.5;\n";
            to_return += "vec4 diffuse = vec4(diff, diff, diff, 1.0);\n";
            to_return += "base_color *= diffuse + vec4(0.5, 0.5, 0.5, 0.0);\n";
            to_return += "return base_color;\n";
            to_return += "}\n";
            return to_return;
        }
    }

    // Built-in shaders
    std::string gui_default_fragment_shader() {
        std::string to_return = "#version 330 core\n";
        to_return += "in vec2 tex_pos;"; // Uniform / in/out variables
        to_return += "out vec4 FragColor;";
        to_return += "uniform vec4 background_color;";
        to_return += "uniform vec4 border_color;";
        to_return += "uniform vec4 border_width;";
        to_return += "uniform vec4 object_extremum;";
        to_return += "uniform vec4 object_rect;";
        to_return += "uniform vec2 scale;";
        to_return += "uniform sampler2D texture_0;";
        to_return += "uniform bool texture_binded;";
        to_return += "uniform vec4 texture_rect;";

        // Blend function
        to_return += Shader_Program::default_gui_blend_colors();

        // Main function
        to_return += "void main(){";
        // Check if the object should be displayed
        to_return += Shader_Program::default_gui_extremum_handling();
        to_return += "vec4 final_color = background_color;";
        to_return += Shader_Program::default_gui_border_handling();
        to_return += "else if(texture_binded){";
        // TEMPORARAY DISABLED && tex_pos[0] >= texture_rect[0] && tex_pos[1] >= texture_rect[1] && tex_pos[0] < texture_rect[0] + texture_rect[2] && tex_pos[1] < texture_rect[1] + texture_rect[3]

        // Object texture
        to_return += "vec2 final_tex_pos = tex_pos;";
        to_return += "final_tex_pos.x *= scale.x;";
        to_return += "final_tex_pos.y *= scale.y;\n";
        to_return += std::string("final_tex_pos -= texture_rect.xy;");
        //to_return += "while(final_tex_pos.x > texture_rect.z)final_tex_pos.x -= texture_rect.z;";
        //to_return += "while(final_tex_pos.y > 1.0/texture_rect.w){final_tex_pos.y -= 1.0/texture_rect.w;}";
        //to_return += "final_tex_pos /= texture_rect.zw;\n";
        //to_return += std::string("while(final_tex_pos.y < 0.0){final_tex_pos.y += 1.0;}while(final_tex_pos.y > 1.0){final_tex_pos.y -= 1.0;}");

        to_return += "final_color = blend_colors(texture(texture_0, final_tex_pos), final_color);}"; //*/
        to_return += "FragColor = final_color;}";
        return to_return;
    };

    // Shader_Program constructor
    Shader_Program::Shader_Program(Built_In_Shader shader_type) : Shader_Program(Shader_Program::get_built_in_vertex_shader(shader_type), Shader_Program::get_built_in_fragment_shader(shader_type)) {}

    // Shader_Program constructor
    Shader_Program::Shader_Program(std::string a_vertex_shader, std::string a_fragment_shader): fragment_shader(a_fragment_shader), vertex_shader(a_vertex_shader) {}

    // Returns the vertex shader for a curve
    std::string Shader_Program::curved_vertex_shader() {
        std::string to_return = "#version 330 core\n";
        to_return += "layout(location = 1) in vec3 in_normal;\n";
        to_return += "layout(location = 0) in vec3 in_pos;\n";
        to_return += "layout(location = 2) in vec2 in_tex_pos;\n";
        to_return += "layout(location = 3) in vec4 in_tex_rect;\n";
        to_return += "layout(location = 4) in vec3 in_tex_multiplier;\n";
        to_return += "layout(location = 5) in float in_axis;\n";
        to_return += "out vec3 frag_pos;\n";
        to_return += "out vec3 normal;\n";
        to_return += "out vec3 tex_multiplier;\n";
        to_return += "out vec2 tex_pos;out vec4 tex_rect;\n";
        to_return += "uniform float curve_difference;\n";
        to_return += "uniform mat4 model;\n";
        to_return += "uniform mat4 projection;\n";
        to_return += "uniform mat4 view;\n";
        to_return += "void main(){\n";
        to_return += "frag_pos = vec3(model * vec4(in_pos, 1.0));\n";
        to_return += "normal = mat3(transpose(inverse(model))) * in_normal;\n";
        to_return += "tex_multiplier = in_tex_multiplier;\n";
        to_return += "tex_pos = in_tex_pos;\n";
        to_return += "tex_rect = in_tex_rect;\n";
        to_return += "vec3 needed_pos = in_pos;\n";
        to_return += "if(in_axis > 0){if(curve_difference > 0){needed_pos=vec3(needed_pos.x * curve_difference, needed_pos.y, needed_pos.z);}";
        to_return += "else if(curve_difference < 0){needed_pos=vec3(needed_pos.x * -curve_difference, needed_pos.y, needed_pos.z);}}\n";
        to_return += "gl_Position = projection * view * model * vec4(needed_pos.xyz, 1.0);\n}";
        return to_return;
    };

    // Returns the default fragment and vertex shader
    std::string Shader_Program::default_fragment_shader() {
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
            to_return += "final_color = vec4(1, 0, 0, 1);";
            to_return += "}";

            // Edit the color as necesary with lighting
            //to_return += "final_color = apply_lighting(final_color);\n";
            to_return += "FragColor = final_color;\n";
            to_return += "}";
            return to_return;
        };
    std::string Shader_Program::default_vertex_shader() {
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

    // Returns the "blend_colors" GLSL function
    std::string Shader_Program::default_gui_blend_colors() {
        std::string to_return = "vec4 blend_colors(vec4 color_1, vec4 color_2){";
        to_return += "if(color_1.w == 0 && color_2.w == 0) return vec4(0, 0, 0, 0);\n";
        to_return += "vec4 final_color = vec4(vec3(color_2.xyz), 1.0) * vec4(1.0 - (color_1.w));";
        to_return += "final_color += vec4(vec3(color_1.xyz), 1.0) * vec4(color_1.w);";
        to_return += "final_color.w = max(color_1.w, color_2.w);";
        to_return += "return final_color;}";
        return to_return;
    }

    // Load the Shader
    void Shader_Program::load_shader() {
        // Convert the fragment and vertex shader (string) to an char*
        char* char_array_fragment = new char[get_fragment_shader().length() + 1];
        char_array_fragment[get_fragment_shader().length()] = '\0';
        for (int i = 0; i < static_cast<int>(get_fragment_shader().length()); i++) { char_array_fragment[i] = get_fragment_shader()[i]; }
        char* char_array_vertex = new char[get_vertex_shader().length() + 1];
        char_array_vertex[get_vertex_shader().length()] = '\0';
        for (int i = 0; i < static_cast<int>(get_vertex_shader().length()); i++) { char_array_vertex[i] = get_vertex_shader()[i]; }

        // Pass the fragment shader into the GPU and compile it
        unsigned int fragment = 0;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &char_array_fragment, NULL);
        glCompileShader(fragment);
        delete[] char_array_fragment;

        // Pass the vertex shader into the GPU and compile it
        unsigned int vertex = 0;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &char_array_vertex, NULL);
        glCompileShader(vertex);
        delete[] char_array_vertex;

        // Check if the shader compilation gave an error
        int  success = 0;
        char infoLog[512];
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        success = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Generate the shader program
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex);
        glAttachShader(shader_program, fragment);
        glLinkProgram(shader_program);

        // Check if the shader program didn't send an error
        success = 0;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Delete useless ressources
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Create a new Shader_Program from this one
    Shader_Program* Shader_Program::new_copy() {Shader_Program* copy = new Shader_Program(vertex_shader, fragment_shader);return copy;}

    // Pass variable to the shader program
    void Shader_Program::pass_variable(std::vector<Shader_Program_Variable> *variables) {
        // Define necessary variable size for binding
        unsigned int total_size = 0;
        for (std::vector<Shader_Program_Variable>::iterator it = variables->begin(); it != variables->end(); it++) {
            unsigned int type_size = 0;
            if (it->type == GL_FLOAT) {type_size = sizeof(float);}
            else if(it->type == GL_INT){type_size = sizeof(int);}
            total_size += it->vector_size * type_size;
        }
        int64_t current_size = 0;
        unsigned int variable_number = 0;

        use();
        for (std::vector<Shader_Program_Variable>::iterator it = variables->begin(); it != variables->end(); it++) {
            unsigned int type_size = 0;
            if (it->type == GL_FLOAT) { type_size = sizeof(float); }
            else if(it->type == GL_INT){type_size = sizeof(int);}

            glVertexAttribPointer(variable_number, it->vector_size, GL_FLOAT, GL_FALSE, total_size, reinterpret_cast<void*>(current_size));
            glEnableVertexAttribArray(variable_number); // Pass each variables to the shader
            current_size += it->vector_size * type_size;
            variable_number++;
        }
    }

    // Change the value of a uniform bool value
    void Shader_Program::set_uniformb_value(std::string name, bool v) {
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        use();
        glUniform1f(uniform_location, v);
    }

    // Change the value of a uniform float value
    void Shader_Program::set_uniform1f_value(std::string name, float v1) {
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        use();
        glUniform1f(uniform_location, v1);
    }

    // Change the value of a uniform vec2 float value
    void Shader_Program::set_uniform2f_value(std::string name, float v1, float v2) {
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        use();
        glUniform2f(uniform_location, v1, v2);
    }

    // Change the value of a uniform vec3 float value
    void Shader_Program::set_uniform3f_value(std::string name, float v1, float v2, float v3) {
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        use();
        glUniform3f(uniform_location, v1, v2, v3);
    }

    // Change the value of a uniform vec4 float value
    void Shader_Program::set_uniform4f_value(std::string name, float v1, float v2, float v3, float v4) {
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        use();
        glUniform4f(uniform_location, v1, v2, v3, v4);
    }

    // Change the value of a uniform vec4 float value
    void Shader_Program::set_uniform4f_value(std::string name, glm::vec4 v) {
        set_uniform4f_value(name, v[0], v[1], v[2], v[3]);
    }

    // Change the value of a matrix mat4 float value
    void Shader_Program::set_uniform4fv_value(std::string name, glm::mat4 fv) {
        use();
        int uniform_location = glGetUniformLocation(shader_program, name.c_str());
        glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(fv));
    }

    // Start using the shader
    void Shader_Program::use() {glUseProgram(shader_program);}

    // Shader_Program destructor
    Shader_Program::~Shader_Program() {if(loaded()){glDeleteProgram(shader_program);} }

    //*********
    //
    // The VBO class
    //
    //*********

    // Most basic VBO constructor
    VBO::VBO() {}

    // VBO complete constructor
    VBO::VBO(std::vector<Shader_Program_Variable> attributes, std::vector<double> datas, bool use_ebo): VBO() {
        a_attributes = attributes;
        a_datas = datas;
        a_use_ebo = use_ebo;

        load_vbo();
    }

    // VBO constructor
    VBO::VBO(std::vector<Shader_Program_Variable> attributes, bool fill_datas, bool use_ebo): VBO(attributes, gui_vbo(a_attributes), use_ebo) {}

    // Bind the VBO into the GPU memory
    void VBO::bind() {glBindBuffer(GL_ARRAY_BUFFER, vbo);}

    // Bind the buffer data of the VBO
    void VBO::bind_buffer() {
        std::vector<float> datas = datas_to_float();
        float* arr = datas.data();

        // Bind the data into the GPU memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * datas.size(), arr, GL_STATIC_DRAW);

        // Bind the EBO if the VBO use them
        if(a_use_ebo) {
            unsigned int* arr_2 = get_indices_in_array();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * get_indices().size(), arr_2, GL_STATIC_DRAW);
        }
    }

    // Returns the datas for a cube vbo
    std::vector<double> VBO::cube_vbo() {
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
    }

    // Returns the number of vertices into the VBO
    unsigned int VBO::get_vertice_number() {
        std::vector<float> datas = datas_to_float();
        unsigned int attribute_size = 0;
        for(int i = 0;i<static_cast<int>(get_attributes()->size());i++) {
            unsigned int type_size = 0;
            if ((*get_attributes())[i].type == GL_FLOAT) { type_size = sizeof(float); }
            attribute_size += (*get_attributes())[i].vector_size * type_size; // Get the total size of the datas
        }
        return (datas.size() * sizeof(float)) / attribute_size; // Divise the total size with the size of one float
    }

    // Returns the datas for a gui vbo
    std::vector<double> VBO::gui_vbo(std::vector<Shader_Program_Variable> attributes) {
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
    std::vector<double> VBO::gui_vbo(){std::vector<Shader_Program_Variable> attributes = std::vector<Shader_Program_Variable>(5);return gui_vbo(attributes);};

    // Load the VBO from binary
    void VBO::load_from_binary(char* binary) {
        unsigned int cursor_pos = 0;
        unsigned char attribute_number = binary[0]; cursor_pos++;
        unsigned int attribute_size = 0;
        for (int i = 0; i < attribute_number; i++) {
            unsigned short type = scls::__extract_2bytes_from_char_array(binary, cursor_pos, true); cursor_pos += 2;
            unsigned short vector_size = scls::__extract_2bytes_from_char_array(binary, cursor_pos, true); cursor_pos += 2;

            Shader_Program_Variable variable; variable.type = type; variable.vector_size = vector_size;
            a_attributes.push_back(variable);
            attribute_size += vector_size;
        }

        // Get each datas
        a_datas.clear();
        unsigned int datas_number = scls::__extract_4bytes_from_char_array(binary, cursor_pos, true); cursor_pos += 4;
        for (unsigned int i = 0; i < datas_number * attribute_size; i++) {
            double data = scls::__extract_double_from_char_array(binary, cursor_pos); cursor_pos += 8;
            a_datas.push_back(static_cast<float>(data));
        }
    }

    // Load the vertices from a file
    void VBO::load_from_file(std::string path) {
        std::string content;
        std::ifstream file;

        // ensure ifstream objects can throw exceptions:
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            file.open(path);
            std::stringstream stream;

            // read file's buffer contents into streams
            stream << file.rdbuf();

            // close file handlers
            file.close();

            // convert stream into string
            content = stream.str();
        }
        catch (const std::ifstream::failure& e){scls::print("SCLS Graphic \"Benoit\"", std::string("Can't open the VBO file \"") + path + std::string("\"."));}

        // Fill the datas
        a_datas.clear();
        std::vector<std::string> cutted = scls::cut_string(content, " ");
        for (int i = 0; i < static_cast<int>(cutted.size()); i++){a_datas.push_back(atof(cutted[i].c_str()));}
    }

    // Load the VBO into the GPU memory
    void VBO::load_vbo() {glGenBuffers(1, &vbo);if (a_use_ebo){glGenBuffers(1, &ebo);}}

    // Create a new VBO from this one
    VBO* VBO::new_copy() {VBO* vbo = new VBO();vbo->a_attributes = a_attributes;vbo->a_datas = a_datas;vbo->a_indices = a_indices;vbo->a_use_ebo = a_use_ebo;return vbo;}

    // Unbind the VBO from the GPU memory
    void VBO::unbind() {glBindBuffer(GL_ARRAY_BUFFER, 0);}

    // VBO destructor
    VBO::~VBO() {glDeleteBuffers(1, &vbo);}

    //*********
    //
    // The VAO class
    //
    //*********

    // Most basic VAO constructor
    VAO::VAO(const std::shared_ptr<VBO>& vbo) : a_vbo(vbo) {}

    // Most usefull VAO constructor
    VAO::VAO(Shader_Program shader_program, const std::shared_ptr<VBO>& vbo) : VAO(vbo) {a_shader_program = shader_program.new_copy();}

    // VAO constructor
    VAO::VAO(std::string shader_path, const std::shared_ptr<VBO>& vbo) : VAO(load_shader_program(shader_path), vbo) {}

    // Bind the VAO into the GPU memory
    void VAO::bind(glm::vec3 scale) {a_shader_program->use();glBindVertexArray(vao);a_shader_program->set_uniform3f_value("scale", scale.x, scale.y, scale.z);}

    // Load and return a shader
    Shader_Program VAO::load_shader_program(std::string shader_path) {
        std::string vertex_content;
        std::string fragment_content;
        std::ifstream vertex_file;
        std::ifstream fragment_file;

        // ensure ifstream objects can throw exceptions:
        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vertex_file.open(shader_path + ".vert");
            fragment_file.open(shader_path + ".frag");
            std::stringstream vertex_stream, fragment_stream;

            // read file's buffer contents into streams
            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();

            // close file handlers
            vertex_file.close();
            fragment_file.close();

            // convert stream into string
            vertex_content = vertex_stream.str();
            fragment_content = fragment_stream.str();
        }
        catch (const std::ifstream::failure& e){scls::print("SCLS Graphic \"Benoit\"", std::string("Can't open the Shader Program file \"") + shader_path + std::string("\"."));}
        const char* vertex_code = vertex_content.c_str();
        const char* fragment_code = fragment_content.c_str();

        Shader_Program shader = Shader_Program(vertex_code, fragment_code); // Create the shader program
        return shader;
    }

    // Load the VAO
    void VAO::load_vao() {
        a_shader_program->load_shader();

        // Create the VAO into the GPU memory
        glGenVertexArrays(1, &vao);
        a_vbo->load_vbo();

        // Bind the VAO and the VBO
        glBindVertexArray(vao);
        a_vbo->bind_buffer();

        // Pass the "in" variables into the shader program
        a_shader_program->pass_variable(a_vbo->get_attributes());

        // Unbind all
        a_vbo->unbind();
        glBindVertexArray(0);

        a_loaded = true;
    }

    // Render the VAO
    void VAO::render(glm::vec3 scale) {
        bind(scale);
        if (get_vbo()->is_using_ebo()){glDrawElements(GL_TRIANGLES, get_vbo()->get_indices().size(), GL_UNSIGNED_INT, 0);}
        else {glDrawArrays(GL_TRIANGLES, 0, triangle_number() * 3.0);}
    }

    // Returns the number of triangle to draw
    unsigned int VAO::triangle_number() {return get_vbo()->get_vertice_number() / 3.0;}

    // VAO destructor
    VAO::~VAO() {delete get_shader_program(); a_shader_program = 0;glDeleteVertexArrays(1, &vao);}

    //*********
    //
    // The Texture class
    //
    //*********

    // Texture constructor
    Texture::Texture(std::string a_texture_path, bool a_resize): a_image(new __Image_Base()), resize(a_resize), texture_path(a_texture_path) {if(a_texture_path != std::string()){get_image()->load_from_path(texture_path);get_image()->flip_x();load_texture();}}
    Texture::Texture(std::shared_ptr<__Image_Base> image, bool a_resize) : Texture("", false) {a_image=image;load_texture();}
    Texture::Texture(unsigned short width, unsigned short height, glm::vec4 color, bool a_resize) : Texture("", false) {a_image.reset(new __Image_Base(width, height, color[0], color[1], color[2], color[3]));load_texture();}
    Texture::Texture(unsigned short width, unsigned short height, scls::Color color, bool a_resize) : Texture("", false) {a_image.reset(new __Image_Base(width, height, color));load_texture();}
    Texture::Texture() : Texture(1, 1, scls::Color(255, 0, 0), false) {}
    Texture::Texture(Texture& texture_to_copy) : Texture(texture_to_copy.width, texture_to_copy.height, glm::vec4(0, 0, 0, 0), texture_to_copy.use_resize()) {get_image()->paste(texture_to_copy.get_image(), 0, 0, 1.0); change_texture();texture_to_copy.a_copy_count++;}

    // Bind the texture into the GPU memory
    void Texture::bind() { glBindTexture(GL_TEXTURE_2D, texture_id); }

    // Change the texture of the texture according to image
    void Texture::change_texture() {
        if(get_image() == 0) return;

        height = get_image()->height();
        width = get_image()->width();

        char* texture = get_image()->datas()->datas();

        // Load the texture
        glBindTexture(GL_TEXTURE_2D, texture_id);
        GLint color_format = GL_RGBA; if(get_image()->color_type() != 6) color_format = GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, color_format, GL_UNSIGNED_BYTE, texture);
        glGenerateMipmap(GL_TEXTURE_2D);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Good
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Far of the texture
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Near of the texture
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Good
    }

    // Load the texture into the GPU memory
    void Texture::load_texture() {if(!loaded()){glGenTextures(1, &texture_id);}a_loaded = true;if(get_image() != 0){change_texture();}}

    // Texture destructor
    Texture::~Texture() { a_image.reset(); glDeleteTextures(1, &texture_id); }
}
