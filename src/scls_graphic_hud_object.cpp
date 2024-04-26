//******************
//
// scls_graphic_hud_object.cpp
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
// This file contains the code in scls_graphic_gui_object.h
//

#include "../headers/scls_graphic_hud_object.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    //*********
    //
    // HUD Object main functions
    //
    //*********

    // Most basic HUD_Object constructor
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct, std::string name) :Object(window_struct, name) {
        a_type.push_back(SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME);
    }

    // Most parent HUD_Object constructor used for displaying
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), transform_parent, name, texture_name, vao_name) {
        a_type.push_back(SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME);
    }

    // HUD_Object constructor used for displaying
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), parent, name, texture_name, vao_name) {
        a_type.push_back(SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME);
    }

    // Render the object on the window
    void HUD_Object::render() {
        // Create the upgraded matrix
        glm::mat4 matrix = transform()->get_model_matrix();

        vao()->get_shader_program()->set_uniform4f_value("background_color", glm::vec4(background_color().red() / 255.0, background_color().green() / 255.0, background_color().blue() / 255.0, background_color().alpha() / 255.0));
        vao()->get_shader_program()->set_uniform4f_value("border_color", glm::vec4(0, 0, 0, 1));
        vao()->get_shader_program()->set_uniform4f_value("border_width", border_width());
        vao()->get_shader_program()->set_uniform2f_value("one_pixel", one_pixel_scale()[0], one_pixel_scale()[1]);
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", texture_rect());

        // Call the hidden render part
        _render(matrix);
    }

    // Update the size of the HUD elements
    void HUD_Object::update_hud_scale() {
        // Check border
        if(a_last_border_width_definition_type == _Border_Width_Definition::Pixel_Border_Width) {
            set_pixel_border_width(a_last_border_width);
        }

        // Check size
        if(a_last_scale_definition_type == _Scale_Definition::Object_Scale) {
            set_object_scale(a_last_scale);
        }
    }

    // HUD_Object destructor
    HUD_Object::~HUD_Object() {

    }

    //*********
    //
    // HUD Text main functions
    //
    //*********

    // Most parent HUD_Object constructor used for displaying
    HUD_Text::HUD_Text(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), transform_parent, name, texture_name, vao_name) {

    }

    // HUD_Text constructor used for displaying
    HUD_Text::HUD_Text(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), parent, name, texture_name, vao_name) {

    }

    // Update the text
    void HUD_Text::update() {
        // Update the texture of the text
        update_text_texture();
    }

    // Update the text texture
    void HUD_Text::update_text_texture() {
        if(!a_modified) return;

        // Create the texture
        Text_Image_Data datas; datas.red = font_color().red(); datas.green = font_color().green(); datas.blue = font_color().blue(); datas.alpha = font_color().alpha();
        datas.background_red = background_color().red(); datas.background_green = background_color().green(); datas.background_blue = background_color().blue(); datas.background_alpha = background_color().alpha();
        datas.font = scls::get_system_font(font_family()); datas.font_size = font_size(); datas.alignment = text_alignment();
        Image* new_image_texture = text_image(text(), datas);

        // Update the texture
        texture()->set_image(new_image_texture);
        a_modified = false;
    }

    // HUD_Text destructor
    HUD_Text::~HUD_Text() {

    }

    //*********
    //
    // HUD page
    //
    //*********

    // HUD_Page most basic constructor
    HUD_Page::HUD_Page(_Window_Advanced_Struct* window_struct, std::string name) : HUD_Object(window_struct, name) {
        set_vao("hud_default");
        set_vao("hud_default");
    }

    // Render the page
    void HUD_Page::render(){
        HUD_Object::render();

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            Object* ob = children()[i];
            if(ob->visible()) {
                ob->render();
            }
        }
    };

    // Update the event of the page
    void HUD_Page::update_event() {
        // Soft reset the page
        soft_reset();

        // Check the overflighted cursor
        HUD_Object* current_overflighted_object = 0;
        std::vector<Object*>* to_analyse = (&children());
        for(int i = 0;i<static_cast<int>(to_analyse->size());i++) {
            Object* analyzed_object = (*to_analyse)[to_analyse->size() - (i + 1)];
            if(analyzed_object->type(1) == SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME) {
                HUD_Object* current_object = reinterpret_cast<HUD_Object*>(analyzed_object);
                if(current_object->is_in_pixel(window_struct()->mouse_x(), window_struct()->window_height() - window_struct()->mouse_y())) {
                    current_overflighted_object = current_object;
                    i = -1;
                    to_analyse = (&current_overflighted_object->children());
                }
            }
        }

        // Update the cursor texture
        if(a_overflighted_object != current_overflighted_object) {
            if(current_overflighted_object == 0) {
                window_struct()->set_cursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
            }
            else {
                window_struct()->set_cursor(glfwCreateStandardCursor(current_overflighted_object->overflighted_cursor()));
            }
        }
        a_overflighted_object = current_overflighted_object;
        if(a_overflighted_object != 0) {
            a_overflighted_object->set_is_overflighted(true);
        }
    }

    // HUD_Page destructor
    HUD_Page::~HUD_Page(){

    }
}
