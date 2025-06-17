#pragma once

#include "scls_graphic_window_base_struct.h"
#include "scls_graphic_gl_part.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // _Window_Advanced_Struct hiddens functions
    //
    //*********

    // Returns the base shader program variable for an HUD form
    inline std::vector<Shader_Program_Variable> _base_hud_shader_program_variables() {
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
    inline std::vector<Shader_Program_Variable> _base_3d_shader_program_variables() {
        // Create base Shader_Program_Variable for the shader program
        std::vector<Shader_Program_Variable> base_3d_attributes = std::vector<Shader_Program_Variable>();
        Shader_Program_Variable v1 = Shader_Program_Variable();
        Shader_Program_Variable v2 = Shader_Program_Variable();
        Shader_Program_Variable v3 = Shader_Program_Variable();
        Shader_Program_Variable v4 = Shader_Program_Variable();
        Shader_Program_Variable v5 = Shader_Program_Variable();
        v1.vector_size = 3;
        v2.vector_size = 3;
        v3.vector_size = 2;
        v4.vector_size = 4;
        v5.vector_size = 3;
        base_3d_attributes.push_back(v1);
        base_3d_attributes.push_back(v2);
        base_3d_attributes.push_back(v3);
        base_3d_attributes.push_back(v4);
        base_3d_attributes.push_back(v5);

        return base_3d_attributes;
    };

    class _Window_Advanced_Struct : public _Window_Base_Struct {
        // Class representing the advanced struct in the game
    public:

        //*********
        //
        // _Window_Advanced_Struct hiddens methods
        //
        //*********

        // _Window_Advanced_Struct constructor
        _Window_Advanced_Struct(bool& cursor_on_window, double& mouse_x, double& mouse_y, int& window_width, int& window_height, std::string a_exec_path);
        // _Window_Advanced_Struct destructor
        virtual ~_Window_Advanced_Struct();

        //*********
        //
        // Window model storage
        //
        //*********

        // Add an existing shader program into the game
        inline Shader_Program* add_shader_program(std::string shader_program_name, const Shader_Program& shader_program) {
            if (!contains_shader_program(shader_program_name)) {a_shaders_programs[shader_program_name] = shader_program;return &a_shaders_programs[shader_program_name];}
            else print("Warning", "SCLS Window", "The \"" + shader_program_name + "\" Shader Program you want to add already exists.");
            return 0;
        };
        // Add an existing VBO into the game
        inline std::shared_ptr<VBO>* add_vbo(std::string name, const std::shared_ptr<VBO>& vbo) {
            if (!contains_vbo(name)) {vbos()[name] = vbo;return &(vbos()[name]);}
            print("Warning", "SCLS Window", "The \"" + name + "\" VBO you want to add already exists.");
            return 0;
        };

        // Returns if the struct contains a shader program
        inline bool contains_shader_program(std::string name) {for (std::map<std::string, Shader_Program>::iterator it = a_shaders_programs.begin(); it != a_shaders_programs.end(); it++) {if (it->first == name) return true;}return false;};
        // Returns if the struct contains a textures
        inline bool contains_texture(std::string name) {for (std::map<std::string, std::shared_ptr<Texture>>::iterator it = textures().begin(); it != textures().end(); it++) {if (it->first == name) return true;}return false;};
        // Returns if the struct contains a VAO
        inline bool contains_vao(std::string name) {for (std::map<std::string, std::shared_ptr<VAO>>::iterator it = vaos().begin(); it != vaos().end(); it++) {if (it->first == name) return true;}return false;};
        // Returns if the struct contains a VBO
        inline bool contains_vbo(std::string name) {for (std::map<std::string, std::shared_ptr<VBO>>::iterator it = vbos().begin(); it != vbos().end(); it++) {if (it->first == name) return true;}return false;};

        // Displays a page 2D
        virtual void display_page_2d(std::string new_page_2d) = 0;
        // Hides all page 2D
        virtual void hide_all_pages_2d() = 0;

        // Loads the built-in VAOs in the _Window_Advanced_Struct
        void load_VAOs();
        // Remove a texture in the window
        void remove_texture(Texture* texture);
        // Unload all the textures
        void unload_textures();
        // Unload all the VAOs
        void unload_vaos();
        // Unload all the VBOs
        void unload_vbos();

        // Create new models
        // Create a texture to the window
        Texture* new_texture(std::string name, std::string path, bool texture_resize = false);
        std::shared_ptr<Texture>* new_texture_shared_ptr(std::string name, unsigned short width, unsigned short height, Color color);
        Texture* new_texture(std::string name, unsigned short width, unsigned short height, Color color) {std::shared_ptr<Texture>* current_ptr = new_texture_shared_ptr(name, width, height, color);if(current_ptr == 0){return 0;} return current_ptr->get();};
        Texture* new_texture(std::string name);
        std::shared_ptr<Texture>* new_texture_shared_ptr(std::string name, std::shared_ptr<__Image_Base> image, bool texture_resize);
        inline Texture* new_texture(std::string name, std::shared_ptr<__Image_Base> image){std::shared_ptr<Texture>* temp = new_texture_shared_ptr(name, image, true);if(temp==0){return 0;}return temp->get();};
        inline Texture* new_texture(std::string name, std::shared_ptr<__Image_Base> image, bool texture_resize){std::shared_ptr<Texture>* temp = new_texture_shared_ptr(name, image, texture_resize);if(temp==0){return 0;}return temp->get();};
        // Create a new VAO into the window with the most basic constructor
        VAO* new_vao(std::string name, char* binary);
        // Create a new VAO into the window
        inline VAO* new_vao(std::string name, std::string vbo, std::string shader = "default"){return new_vao_shared_ptr(name, vbo, shader)->get();};
        std::shared_ptr<VAO>* new_vao_shared_ptr(std::string name, std::string vbo, std::string shader = "default");
        // Create a new VBO into the game
        std::shared_ptr<VBO>* new_vbo(std::string name);

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        Texture* texture(std::string texture_name, bool copy_texture = false);
        std::shared_ptr<Texture>* texture_shared_ptr(std::string texture_name);
        inline VAO* vao(std::string vao_name) { if(contains_vao(vao_name)) return vaos()[vao_name].get(); return 0; };
        inline std::shared_ptr<VAO>* vao_shared_ptr(std::string vao_name) { if(contains_vao(vao_name)) return &vaos()[vao_name]; return 0; };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline std::map<std::string, std::shared_ptr<Texture>>& textures() { return a_textures; };
        inline std::map<std::string, std::shared_ptr<VAO>>& vaos() { return a_vaos; };
        inline std::map<std::string, std::shared_ptr<VBO>>& vbos() { return a_vbos; };
    private:
        //*********
        //
        // Window model storage
        //
        //*********

        // Each shaders, with their name as key, in the window
        std::map<std::string, Shader_Program> a_shaders_programs = std::map<std::string, Shader_Program>();
        // Each Textures, with their name as key, in the window
        std::map<std::string, std::shared_ptr<Texture>> a_textures = std::map<std::string, std::shared_ptr<Texture>>();
        // Each VAOs, with their name as key, in the window
        std::map<std::string, std::shared_ptr<VAO>> a_vaos = std::map<std::string, std::shared_ptr<VAO>>();
        // Each VBOs base, with their name as key, in the window
        std::map<std::string, std::shared_ptr<VBO>> a_vbos = std::map<std::string, std::shared_ptr<VBO>>();
    };
}
