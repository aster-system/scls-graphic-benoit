//******************
//
// scls_graphic_object.cpp
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
// This file contains the source code of scls_graphic_object.h.
//

#include "../headers/scls_graphic_object.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
    //
    // Object main functions
    //
    //*********

    // Object most basic constructor
    Object::Object(_Window_Advanced_Struct* window_struct) : a_window_struct(window_struct) {
        a_transform = new Transform_Object();
    }

    // Object constructor used for displaying
    Object::Object(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name) : Object(window_struct) {
        a_name = name;
        if(texture_name != "")a_texture = window_struct->texture(texture_name);
        a_vao = window_struct->vao(vao_name);
    }

    // Object destructor
    Object::~Object() {
        delete a_transform; a_transform = 0;
    }

    //*********
    //
    // Object graphic functions
    //
    //*********

    // Hidden parts of the render object
    void Object::_render(glm::mat4 matrix) {
        // Write some uniform variables into the shader
        vao()->get_shader_program()->set_uniform4fv_value("model", matrix);

        if(texture() != 0) {
            texture()->bind(); // Bind the texture
        }

        if (texture() != 0 && texture()->use_resize())
        {
            vao()->render(transform()->get_scale()); // Render the object with scaling
        }
        else
        {
            vao()->render(); // Render the object without scaling
        }
    }

    // Render the object on the window
    void Object::render() {
        // Create the upgraded matrix
        glm::mat4 matrix = transform()->get_model_matrix();

        // Call the hidden render part
        _render(matrix);
    }

    //*********
    //
    // Object heritage functions
    //
    //*********

    // Clone the object
    void* Object::clone(std::string name, std::string texture_name, std::string vao_name) {
        Object* to_return = new Object(window_struct(), name, texture_name, vao_name);

        return to_return;
    }
}
