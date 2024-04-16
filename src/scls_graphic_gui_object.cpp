//******************
//
// scls_graphic_gui_object.cpp
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

#include "../headers/scls_graphic_gui_object.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    //*********
    //
    // HUD page
    //
    //*********

    // HUD_Page most basic constructor
    HUD_Page::HUD_Page(_Window_Advanced_Struct* window_struct, std::string name) : _Page(window_struct, name) {

    }

    // HUD_Page destructor
    HUD_Page::~HUD_Page(){

    }

    //*********
    //
    // HUD Object main functions
    //
    //*********

    // HUD_Object most basic constructor
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct)) {

    }

    // HUD_Object constructor used for displaying
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), name, texture_name, vao_name) {

    }

    // Render the object on the window
    void HUD_Object::render() {
        // Create the upgraded matrix
        glm::mat4 matrix = transform()->get_model_matrix();
        // Apply the scale according to the texture
        matrix = glm::scale(matrix, glm::vec3(texture_ratio() / window_struct()->window_ratio(), 1, 1));

        vao()->get_shader_program()->set_uniform4f_value("background_color", glm::vec4(background_color().red() / 255.0, background_color().green() / 255.0, background_color().blue() / 255.0, background_color().alpha() / 255.0));
        vao()->get_shader_program()->set_uniform4f_value("border_color", glm::vec4(0, 0, 0, 1));
        vao()->get_shader_program()->set_uniform4f_value("border_width", border_width() / glm::vec4(1, texture_ratio(), 1, texture_ratio()));
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", texture_rect());

        // Call the hidden render part
        _render(matrix);
    }

    // HUD_Object destructor
    HUD_Object::~HUD_Object() {

    }

    //*********
    //
    // HUD Text main functions
    //
    //*********

    // HUD_Text most basic constructor
    HUD_Text::HUD_Text(_Window_Advanced_Struct* window_struct) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct)) {

    }

    // HUD_Text constructor used for displaying
    HUD_Text::HUD_Text(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), name, texture_name, vao_name) {

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
        datas.font = scls::get_system_font(font_family()); datas.font_size = font_size();
        Image* new_image_texture = text_image(text(), datas);

        // Update the texture
        texture()->set_image(new_image_texture);
        a_modified = false;
    }

    // HUD_Text destructor
    HUD_Text::~HUD_Text() {

    }
}
