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
    // GL_Object main functions
    //
    //*********

    // GL_Object constructor used to do a page
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name) : a_name(name), a_window_struct(window_struct) {
        a_transform = std::make_shared<Transform_Object>(transform_parent);
        a_type.push_back("object");
    }

    // GL_Object most basic constructor with a name
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct, std::string name) : GL_Object(window_struct, 0, name) {

    }

    // GL_Object blank constructor
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct) : GL_Object(window_struct, "") {}

    // GL_Object most basic constructor with a transform parent
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent) : GL_Object(window_struct, transform_parent, "") {}

    // Most parent GL_Object constructor used for displaying
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : GL_Object(window_struct, transform_parent, name) {
        if(texture_name != "")a_texture = *window_struct->texture_shared_ptr(texture_name);
        a_vao = window_struct->vao(vao_name);
    }

    // GL_Object constructor used for displaying
    GL_Object::GL_Object(_Window_Advanced_Struct* window_struct, GL_Object* parent, std::string name, std::string texture_name, std::string vao_name) : GL_Object(window_struct, parent->transform(), name) {
        a_name = name;
        if(texture_name != "")a_texture = *window_struct->texture_shared_ptr(texture_name);
        a_vao = window_struct->vao(vao_name);

        set_parent(parent);
    }

    // GL_Object destructor
    GL_Object::~GL_Object() {
        if(only_texture_use()) {
            window_struct()->remove_texture(texture());
        }

        if(parent() != 0) {
            parent()->child_deleted(this);
        }

        delete_children();
    }

    // Delete a child of the object
    void GL_Object::delete_child(GL_Object* child) {
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
    void GL_Object::delete_children() {for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] != 0) delete children()[i];}children().clear();}

    //*********
    //
    // GL_Object graphic functions
    //
    //*********

    // Hidden parts of the render object
    void GL_Object::_render(glm::mat4 matrix) {
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
            GL_Object* ob = children()[i];
            if(ob->visible()) {
                ob->render();
            }
        }
    }
    void GL_Object::render() {glm::mat4 matrix = transform()->get_model_matrix();_render(matrix);}

    // Function called after every updates
    void GL_Object::last_update() {};
    // Reset the object without changing it
    void GL_Object::soft_reset() {for(int i = 0;i<static_cast<int>(children().size());i++){children()[i]->soft_reset();}};
    // Function called during every updates
    void GL_Object::update() {for(int i = 0;i<static_cast<int>(children().size());i++){children()[i]->update();}};
    // Function called when the events are updated
    void GL_Object::update_event(){for(int i = 0;i<static_cast<int>(children().size());i++){children()[i]->update_event();}};

    //*********
    //
    // GL_Object heritage functions
    //
    //*********

    // Clone the object
    void* GL_Object::clone(GL_Object* parent, std::string name, std::string texture_name, std::string vao_name) {GL_Object* to_return = new GL_Object(window_struct(), parent, name, texture_name, vao_name);return to_return;}

    //*********
    //
    // Loading handler
    //
    //*********

    // Handle an attribute from XML
    void GL_Object::set_xml_attribute(std::shared_ptr<XML_Text_Base> text, std::shared_ptr<__XML_Loader> loader_shared_ptr, int& i) {}
}
