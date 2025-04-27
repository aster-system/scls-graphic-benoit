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

    // Object constructor used to do a page
    Object::Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name) : a_name(name), a_window_struct(window_struct) {
        a_transform = new Transform_Object(transform_parent);
        a_type.push_back("object");
    }

    // Object most basic constructor with a name
    Object::Object(_Window_Advanced_Struct* window_struct, std::string name) : Object(window_struct, 0, name) {

    }

    // Object blank constructor
    Object::Object(_Window_Advanced_Struct* window_struct) : Object(window_struct, "") {}

    // Object most basic constructor with a transform parent
    Object::Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent) : Object(window_struct, transform_parent, "") {}

    // Most parent Object constructor used for displaying
    Object::Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : Object(window_struct, transform_parent, name) {
        if(texture_name != "")a_texture = *window_struct->texture_shared_ptr(texture_name);
        a_vao = window_struct->vao(vao_name);
    }

    // Object constructor used for displaying
    Object::Object(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : Object(window_struct, parent->transform(), name) {
        a_name = name;
        if(texture_name != "")a_texture = *window_struct->texture_shared_ptr(texture_name);
        a_vao = window_struct->vao(vao_name);

        set_parent(parent);
    }

    // Object destructor
    Object::~Object() {
        if(only_texture_use()) {
            window_struct()->remove_texture(texture());
        }

        if(parent() != 0) {
            parent()->child_deleted(this);
        }

        delete_children();
        delete a_transform; a_transform = 0;
    }

    // Delete a child of the object
    void Object::delete_child(Object* child) {
        if(child == 0) return;

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i] == child) {
                children().erase(children().begin() + i);
                delete child;
                return;
            }
        }
    }

    // Delete all children of the object
    void Object::delete_children() {for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] != 0) delete children()[i];}children().clear();}

    //*********
    //
    // Object graphic functions
    //
    //*********

    // Hidden parts of the render object
    void Object::_render(glm::mat4 matrix) {
        if(vao() != 0) {
            // Write some uniform variables into the shader
            vao()->get_shader_program()->set_uniform4fv_value("model", matrix);
            if(texture() == 0) {
                vao()->get_shader_program()->set_uniformb_value("texture_binded", false);
            }
            else {
                texture()->bind(); // Bind the texture
                vao()->get_shader_program()->set_uniformb_value("texture_binded", true);
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

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            Object* ob = children()[i];
            if(ob->visible()) {
                ob->render();
            }
        }
    }
    void Object::render() {glm::mat4 matrix = transform()->get_model_matrix();_render(matrix);}

    //*********
    //
    // Object heritage functions
    //
    //*********

    // Clone the object
    void* Object::clone(Object* parent, std::string name, std::string texture_name, std::string vao_name) {Object* to_return = new Object(window_struct(), parent, name, texture_name, vao_name);return to_return;}

    //*********
    //
    // Loading handler
    //
    //*********

    // Handle an attribute from XML
    void Object::set_xml_attribute(std::shared_ptr<XML_Text> text, std::shared_ptr<__XML_Loader> loader_shared_ptr, int& i) {}
}
