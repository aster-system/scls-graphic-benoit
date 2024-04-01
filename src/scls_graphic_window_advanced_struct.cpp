#include "../headers/scls_graphic_window_advanced_struct.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // _Window_Advanced_Struct hiddens methods
    //
    //*********

    // _Window_Advanced_Struct constructor
    _Window_Advanced_Struct::_Window_Advanced_Struct(double& mouse_x, double& mouse_y, int& window_width, int& window_height, std::string exec_path): Base_Struct(mouse_x, mouse_y, window_width, window_height, exec_path) {

    }

    // _Window_Advanced_Struct destructor
    _Window_Advanced_Struct::~_Window_Advanced_Struct() {
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
    }

    //*********
    //
    // Window model storage
    //
    //*********

    // Returns a texture in the struct
    Texture* _Window_Advanced_Struct::texture(std::string texture_name, bool copy_texture) {
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
        error("SCLS Graphic Window", "The \"" + texture_name + "\" texture you want to use does not exists.");
        return 0;
    }







    // Loads the HUD VAOs in the advanced struct
    void _Window_Advanced_Struct::load_hud_VAOs() {
        // Create the base shaders
        a_shaders_programs["hud_default"] = Shader_Program(Shader_Program::HUD_Default);

        // Define attributes for VAOs
        std::vector<Shader_Program_Variable> hud_attributes = get_base_hud_shader_program_variables();

        // Create VAOs
        VBO hud_vbo = VBO(hud_attributes, VBO::get_base_hud_vbo(hud_attributes), false);
        all_vaos["hud_default"] = new VAO(a_shaders_programs["hud_default"], hud_attributes, &hud_vbo); all_vaos["hud_default"]->load_vao();
    }

    // Loads the VAOs in the advanced struct
    void _Window_Advanced_Struct::load_VAOs() {
        // Create the base shaders
        a_shaders_programs["default"] = Shader_Program();

        // Create a base texture
        (*get_textures())["transparent"] = new Texture(5, 5, glm::vec4(0.0, 0.0, 0.0, 0.0));
        (*get_textures())["white"] = new Texture(5, 5, glm::vec4(1.0, 1.0, 1.0, 1.0));

        load_hud_VAOs();
    }

    // Add a texture to the game
    Texture* _Window_Advanced_Struct::new_texture(std::string name, std::string path, bool texture_resize) {
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
    Texture* _Window_Advanced_Struct::new_texture(std::string texture_name, unsigned short width, unsigned short height, glm::vec4 color) {
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
    Texture* _Window_Advanced_Struct::new_texture(std::string name, bool texture_resize) {
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
    VAO* _Window_Advanced_Struct::new_vao(std::string name, char* binary) {
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
    VAO* _Window_Advanced_Struct::new_vao(std::string name, std::string vbo, std::string shader) {
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
    VBO* _Window_Advanced_Struct::new_vbo(std::string name) {
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
    void _Window_Advanced_Struct::unload_textures() {
        std::map<std::string, Texture*>* textures = get_textures();
        for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++)
        {
            delete it->second; // Delete textures
            it->second = 0;
        }
    }
}
