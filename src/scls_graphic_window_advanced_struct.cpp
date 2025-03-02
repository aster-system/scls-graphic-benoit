#include "../headers/scls_graphic_window_advanced_struct.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // _Window_Advanced_Struct hiddens methods
    //
    //*********

    // _Window_Advanced_Struct constructor
    _Window_Advanced_Struct::_Window_Advanced_Struct(bool& cursor_on_window, double& mouse_x, double& mouse_y, int& window_width, int& window_height, std::string exec_path): _Window_Base_Struct(cursor_on_window, mouse_x, mouse_y, window_width, window_height, exec_path) {}

    // _Window_Advanced_Struct destructor
    _Window_Advanced_Struct::~_Window_Advanced_Struct() {unload_textures();unload_vaos();unload_vbos();}

    //*********
    //
    // Window model storage
    //
    //*********

    // Loads the VAOs in the advanced struct
    void _Window_Advanced_Struct::load_VAOs() {
        // Create the base shaders
        Shader_Program shader_to_add = Shader_Program(); add_shader_program("default", shader_to_add);
        a_shaders_programs["gui_default"] = Shader_Program(Shader_Program::HUD_Default);
        a_shaders_programs["curved"] = Shader_Program(Shader_Program::Curved);

        // Create a base texture
        textures()["black"] = std::make_shared<Texture>(5, 5, glm::vec4(0, 0, 0, 255));
        textures()["transparent"] = std::make_shared<Texture>(5, 5, glm::vec4(0, 0, 0, 0));
        textures()["white"] = std::make_shared<Texture>(5, 5, glm::vec4(255, 255, 255, 255));

        // Define attributes for VAOs
        std::vector<Shader_Program_Variable> object_3d_attributes = _base_3d_shader_program_variables();
        std::vector<Shader_Program_Variable> gui_attributes = _base_hud_shader_program_variables();

        // Create VAOs
        // GUI VAO
        std::shared_ptr<VBO> gui_vbo = std::make_shared<VBO>(gui_attributes, VBO::gui_vbo(gui_attributes), false); add_vbo("gui_default", gui_vbo);
        vaos()["gui_default"] = std::make_shared<VAO>(a_shaders_programs["gui_default"], gui_vbo);
        vaos()["gui_default"].get()->load_vao();
        // Cube VAO
        //std::shared_ptr<VBO> cube_vbo = std::make_shared<VBO>(object_3d_attributes, VBO::cube_vbo(), false); add_vbo("cube", cube_vbo);
        //vaos()["cube"] = std::make_shared<VAO>(a_shaders_programs["default"], object_3d_attributes, cube_vbo);
        //vaos()["cube"].get()->load_vao();
        // One faced cube VAO
        std::shared_ptr<VBO> one_faced_cube_vbo = std::make_shared<VBO>();
        one_faced_cube_vbo.get()->load_from_binary(model_maker::regular_polygon_3d(4).get()->binary_vbo_complete().get()->datas());
        add_vbo("one_faced_cube", one_faced_cube_vbo); new_vao("one_faced_cube", "one_faced_cube", "default");
        vaos()["cube"] = vaos()["one_faced_cube"];
        // Curved one faced cube VAO
        std::shared_ptr<VBO> one_faced_cube_vbo_curved = std::make_shared<VBO>();
        std::shared_ptr<model_maker::Solid> needed_solid = model_maker::regular_polygon_3d(4); needed_solid.get()->set_x(0.5);
        one_faced_cube_vbo_curved.get()->load_from_binary(needed_solid.get()->binary_vbo_complete(model_maker::Point::_VT_Curved).get()->datas());
        add_vbo("one_faced_cube_curved", one_faced_cube_vbo_curved); new_vao("one_faced_cube_curved", "one_faced_cube_curved", "curved");
        vaos()["cube_curved"] = vaos()["one_faced_cube_curved"];
        //int* a = 0; *a += 5;
    }

    // Returns a texture in the struct
    Texture* _Window_Advanced_Struct::texture(std::string texture_name, bool copy_texture) {
        if (contains_texture(texture_name))
        {
            if(copy_texture)
            {
                Texture* to_copy = textures()[texture_name].get();
                std::shared_ptr<Texture> to_return = std::make_shared<Texture>(*to_copy);
                textures()[texture_name + ";copy" + std::to_string(to_copy->copy_count())] = to_return;
                return to_return.get();
            }
            return textures()[texture_name].get();
        }
        scls::print("Warning", "SCLS Window", "The \"" + texture_name + "\" texture you want to use does not exists.");
        return 0;
    }

    // Returns a texture shared pointer in the struct
    std::shared_ptr<Texture>* _Window_Advanced_Struct::texture_shared_ptr(std::string texture_name) {
        if (contains_texture(texture_name)) { return &textures()[texture_name]; }
        scls::print("Warning", "SCLS Window", "The \"" + texture_name + "\" texture you want to use does not exists.");
        return 0;
    }

    // Unload all the textures
    void _Window_Advanced_Struct::unload_textures() { textures().clear(); }

    // Unload all the VAOs
    void _Window_Advanced_Struct::unload_vaos() { vaos().clear(); }

    // Unload all the VBOs
    void _Window_Advanced_Struct::unload_vbos() { vbos().clear(); }

    // Add a texture to the game
    Texture* _Window_Advanced_Struct::new_texture(std::string name, std::string path, bool texture_resize) {
        if(std::filesystem::exists(path)) {
            if (!contains_texture(name)) {
                std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, texture_resize);
                texture.get()->change_texture();
                textures()[name] = texture;

                return texture.get();
            }
            scls::print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to add already exists.");
            return 0;
        }
        scls::print("Warning", "SCLS Window", "The \"" + name + "\" texture uses the \"" + path + "\" path, which does not exist.");
        return 0;
    }
    std::shared_ptr<Texture>* _Window_Advanced_Struct::new_texture_shared_ptr(std::string name, unsigned short width, unsigned short height, Color color) {
        if (!contains_texture(name)) {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(width, height, color);
            texture.get()->change_texture();
            textures()[name] = texture;
            return &textures()[name];
        }
        scls::print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to add already exists.");
        return 0;
    }
    Texture* _Window_Advanced_Struct::new_texture(std::string name) {
        if (!contains_texture(name))
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>();
            textures()[name] = texture;
            return texture.get();
        }
        scls::print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to add already exists.");
        return 0;
    }
    std::shared_ptr<Texture>* _Window_Advanced_Struct::new_texture_shared_ptr(std::string name, std::shared_ptr<Image> image, bool texture_resize) {
        if (!contains_texture(name)) {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(image, texture_resize);
            texture.get()->change_texture();
            textures()[name] = texture;
            return &textures()[name];
        }
        scls::print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to add already exists.");
        return 0;
    }

    // Create a new VAO into the game with the most basic constructor
    VAO* _Window_Advanced_Struct::new_vao(std::string name, char* binary) {
        if (!contains_vao(name)) {
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
        print("Warning", "SCLS Window", "The \"" + name + "\" VAO already exists.");
        return 0;
    }

    // Create a new VAO into the game
    std::shared_ptr<VAO>* _Window_Advanced_Struct::new_vao_shared_ptr(std::string name, std::string vbo, std::string shader) {
        if (!contains_vao(name)) {
            // Check the shader and the VBO
            if(!contains_vbo(vbo)) {
                print("Warning", "SCLS Window", "The \"" + name + "\" VAO use the \"" + vbo + "\" VBO, which does not exist.");
                return 0;
            }
            if(!contains_shader_program(shader)) {print("Warning", "SCLS Window", "The \"" + name + "\" VAO use the \"" + shader + "\" Shader, which does not exist.");}

            vaos()[name] = std::make_shared<VAO>(a_shaders_programs[shader], vbos()[vbo]);
            vaos()[name].get()->load_vao();
            return &vaos()[name];
        }
        print("Warning", "SCLS Window", "The \"" + name + "\" VAO already exists.");
        return 0;
    }

    // Create a new VBO into the game
    std::shared_ptr<VBO>* _Window_Advanced_Struct::new_vbo(std::string name) {
        if (!contains_vbo(name)) {
            std::shared_ptr<VBO> vbo = std::make_shared<VBO>();
            return add_vbo(name, vbo);
        }
        print("Warning", "SCLS Window", "The \"" + name + "\" texture you want to add already exists.");
        return 0;
    }

    // Remove a texture in the window
    void _Window_Advanced_Struct::remove_texture(Texture* texture) {for (std::map<std::string, std::shared_ptr<Texture>>::iterator it = textures().begin(); it != textures().end(); it++) {if (it->second.get() == texture) {textures().erase(it);return;}}}
}
