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
    // GUI Object main functions
    //
    //*********

    // Most basic GUI_Object constructor
    GUI_Object::GUI_Object(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : __GUI_Object_Core(window, parent), a_name(name), a_parent(parent) {
        a_texture = (*window.new_texture_shared_ptr(name, 1, 1, Color(255, 255, 255, 0)));
        a_vao = (*window.vao_shared_ptr("gui_default"));
    }

    // Function called after that the window is resized
    void GUI_Object::after_resizing(){ calculate_transformation(true); }

    // Deletes a child of the object and returns if the child has been correctly deleted
    bool GUI_Object::delete_child(GUI_Object* object) {
        // Check if the child is here
        for(int i = 0;i<static_cast<int>(children().size());i++) { if(children()[i].get() == object) { children().erase(children().begin() + i); return true; } }

        // Check if the child is the child of a child
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i].get()->delete_child(object)) return true;
        }
        return false;
    }

    // Render the object
    void GUI_Object::render(glm::vec3 scale_multiplier) {
        if(a_transformation_updated) calculate_transformation(true);

        // Handle the extremum
        glm::vec4 final_extremum = object_extremum();
        if(final_extremum[1] >= 1 || final_extremum[3] <= 0) return;

        // Handle the matrix
        glm::mat4 matrix = glm::mat4(1.0);
        glm::vec2 absolute_position_to_apply = glm::vec2(x_in_adapted_absolute_scale().to_double(), y_in_adapted_absolute_scale().to_double());
        glm::vec2 absolute_scale_to_apply = glm::vec2(width_in_adapted_absolute_scale().to_double(), height_in_adapted_absolute_scale().to_double());
        absolute_scale_to_apply *= glm::vec2(scale_multiplier[0], scale_multiplier[1]);
        matrix = glm::translate(matrix, glm::vec3(-1.0, -1.0, 0.0));
        matrix = glm::translate(matrix, glm::vec3(absolute_position_to_apply[0], absolute_position_to_apply[1], 0));
        matrix = glm::scale(matrix, glm::vec3(absolute_scale_to_apply[0], absolute_scale_to_apply[1], 1));

        // Handle the background color
        vao()->get_shader_program()->set_uniform4f_value("background_color", background_color());

        // Handle the border
        vao()->get_shader_program()->set_uniform4f_value("border_color", border_color());
        vao()->get_shader_program()->set_uniform4f_value("border_width", border_width_in_scale());

        // Handle the rect of the texture
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", texture_rect());

        // Handle the texture and the VAO
        if(texture() == 0) {
            vao()->get_shader_program()->set_uniformb_value("texture_binded", false);
        }
        else {
            texture()->bind(); // Bind the texture
            vao()->get_shader_program()->set_uniformb_value("texture_binded", true);
        }
        vao()->get_shader_program()->set_uniform4fv_value("model", matrix);
        vao()->get_shader_program()->set_uniform4f_value("object_extremum", final_extremum);
        vao()->get_shader_program()->set_uniform4f_value("object_rect", glm::vec4(0, 0, 1.0, 1.0));
        vao()->render();

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i] != 0 && children()[i]->visible()) children()[i]->render(scale_multiplier);
        }
    }

    // GUI_Object destructor
    GUI_Object::~GUI_Object() {
        delete_children();

        if(parent() != 0) {
            parent()->child_deleted(this);
        }

        window_struct().remove_texture(texture());

        a_shared_ptr = 0;
    }

    //*********
    //
    // Loading handler
    //
    //*********

    // Loads the object from XML
    void GUI_Object::__load_from_xml(std::shared_ptr<XML_Text> text, std::shared_ptr<__GUI_Page_Loader> loader, std::string event) {
        // Parse the content
        for(int i = 0;i<static_cast<int>(text.get()->sub_texts().size());i++) {
            std::shared_ptr<XML_Text> current_text = text.get()->sub_texts()[i];
            std::string current_balise_name = current_text.get()->xml_balise_name();

            // Apply the XML balise
            if(current_balise_name == "when") {
                // Load the object look at a certain even
                std::vector<_Text_Balise_Part> final_content = std::vector<_Text_Balise_Part>();
                std::string current_event = "";
                // Get each attributes
                for(int j = 0;j<static_cast<int>(current_text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = current_text.get()->xml_balise_attributes()[j];
                    std::string& current_attribute_name = current_attribute.name;
                    std::string& current_attribute_value = current_attribute.value;

                    if(current_attribute_name == "event") {
                        // Value of the event
                        current_event = current_attribute_value;
                    }
                }

                if(event == "") {
                    // Apply the attribute for the event
                    __load_from_xml(current_text, loader, current_event);
                }
                else {
                    print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't load an event inside an another event with XML.");
                }
            }
            else if(current_balise_name != "parent") {
                set_xml_attribute(current_text, event, loader, i);
            }
        }
    }

    // Handle an attribute from XML
    void GUI_Object::set_xml_attribute(std::shared_ptr<XML_Text> text, std::string event, std::shared_ptr<__GUI_Page_Loader> loader, int& i) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "background_color") {
            // Load the background color
            Color background_color(0, 0, 0, 255);
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "red") {
                    // Red part of the color
                    background_color.set_red(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "green") {
                    // Red part of the color
                    background_color.set_green(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "blue") {
                    // Red part of the color
                    background_color.set_blue(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "alpha") {
                    // Alpha part of the color
                    background_color.set_alpha(std::stoi(current_attribute_value));
                }
            }
            set_background_color(background_color);
        }
        else if(xml_attribute_name == "border") {
            // Load the border
            Color border_color(0, 0, 0, 255);
            Fraction border_bottom = Fraction(0);
            Fraction border_left = Fraction(0);
            Fraction border_right = Fraction(0);
            Fraction border_top = Fraction(0);
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "red") {
                    // Red part of the color
                    border_color.set_red(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "green") {
                    // Red part of the color
                    border_color.set_green(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "blue") {
                    // Red part of the color
                    border_color.set_blue(std::stoi(current_attribute_value));
                }
                else if(current_attribute_name == "bottom") {
                    // Bottom width of the border
                    border_bottom = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "left") {
                    // Left width of the border
                    border_left = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "right") {
                    // Right width of the border
                    border_right = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "top") {
                    // Top width of the border
                    border_top = Fraction::from_std_string(current_attribute_value);
                }
            }
            set_border_color(border_color);
            set_border_width_in_pixel(border_top.to_double());
        }
        else if(xml_attribute_name == "cursor") {
            if(event == "overflighted") {
                // Load the cursor of the object when overflighted
                unsigned long cursor = GLFW_ARROW_CURSOR;
                for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                    std::string current_attribute_name = current_attribute.name;
                    std::string current_attribute_value = current_attribute.value;

                    if(current_attribute_name == "type") {
                        // Value of the cursor
                        if(current_attribute_value == "pointing_hand_cursor") { cursor = GLFW_HAND_CURSOR; }
                    }
                }
                // Set the cursor
                set_overflighted_cursor(cursor);
            }
        }
        else if(xml_attribute_name == "height") {
            // Load the height of the object
            Fraction height = Fraction(1);
            _Size_Definition height_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "value") {
                    // Value of the height
                    height = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the height
                    if(current_attribute_value == "scale") height_type = _Size_Definition::Scale_Size;
                    else if(current_attribute_value == "pixel") height_type = _Size_Definition::Pixel_Size;
                }
            }
            // Set the height
            if(height_type == _Size_Definition::Pixel_Size) set_height_in_pixel(height.to_double());
            else set_height_in_scale(height);
        }
        else if(xml_attribute_name == "texture") {
            // Load the texture position
            Alignment_Texture texture_alignment = Alignment_Texture::T_Fill;
            Alignment_Horizontal texture_alignment_horizontal = Alignment_Horizontal::H_Center;
            Alignment_Vertical texture_alignment_vertical = Alignment_Vertical::V_Center;
            std::string texture_name = "";
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "name") {
                    // Value of the name
                    texture_name = current_attribute_value;
                }
                else if(current_attribute_name == "alignment") {
                    // Alignment of the texture
                    if(current_attribute_value == "fill") texture_alignment = Alignment_Texture::T_Fill;
                    else if(current_attribute_value == "fit") texture_alignment = Alignment_Texture::T_Fit;
                    else if(current_attribute_value == "fit_horizontally") texture_alignment = Alignment_Texture::T_Fit_Horizontally;
                    else if(current_attribute_value == "fit_vertically") texture_alignment = Alignment_Texture::T_Fit_Vertically;
                    else if(current_attribute_value == "user_defined") texture_alignment = Alignment_Texture::T_User_Defined;
                    else{print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't set the value of the texture alignment to \"" + current_attribute_value + "\".");}
                }
                else if(current_attribute_name == "alignment_vertical" || current_attribute_name == "alignment_v") {
                    // Vertical alignment of the texture
                    if(current_attribute_value == "bottom") texture_alignment_vertical = Alignment_Vertical::V_Bottom;
                    else if(current_attribute_value == "top") texture_alignment_vertical = Alignment_Vertical::V_Top;
                    else{print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't set the value of the texture vertical alignment to \"" + current_attribute_value + "\".");}
                }
                else if(current_attribute_name == "alignment_horizontal" || current_attribute_name == "alignment_h") {
                    // Horizontal alignment of the texture
                    if(current_attribute_value == "left") texture_alignment_horizontal = Alignment_Horizontal::H_Left;
                    else if(current_attribute_value == "right") texture_alignment_horizontal = Alignment_Horizontal::H_Right;
                    else print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't set the value of the texture vertical alignment to \"" + current_attribute_value + "\".");
                }
                else{print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't find the attribute \"" + current_attribute_value + "\" of the texture.");}
            }
            // Set the goot texture
            if(texture_name != "") set_texture(texture_name);
            set_texture_alignment(texture_alignment);
            set_texture_alignment_horizontal(texture_alignment_horizontal);
            set_texture_alignment_vertical(texture_alignment_vertical);
        }
        else if(xml_attribute_name == "width") {
            // Load the width of the object
            Fraction width = Fraction(1);
            _Size_Definition width_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "value") {
                    // Value of the width
                    width = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the width
                    if(current_attribute_value == "scale") width_type = _Size_Definition::Scale_Size;
                    else if(current_attribute_value == "pixel") width_type = _Size_Definition::Pixel_Size;
                }
            }
            // Set the width
            if(width_type == _Size_Definition::Pixel_Size) set_width_in_pixel(width.to_double());
            else set_width_in_scale(width);
        }
        else if(xml_attribute_name == "x") {
            // Load the X position
            unsigned char attachment_horizontal = 0;
            Fraction attachment_horizontal_offset = Fraction(0);
            std::string attached_object_vertical = "";
            Fraction x = Fraction(1);
            _Size_Definition x_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "value") {
                    // Value of the width
                    x = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the width
                    if(current_attribute_value == "scale") x_type = _Size_Definition::Scale_Size;
                    else if(current_attribute_value == "pixel") x_type = _Size_Definition::Pixel_Size;
                    else if(current_attribute_value == "object_scale") x_type = _Size_Definition::Object_Scale_Size;
                }
                else if(current_attribute_name == "attachment" || current_attribute_name == "attach") {
                    // Use the attachment
                    if(current_attribute_value == "left") attachment_horizontal = 1;
                    else if(current_attribute_value == "right") attachment_horizontal = 2;
                }
                else if(current_attribute_name == "offset") {
                    // Add an attachment offset
                    attachment_horizontal_offset = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "attached_object" || current_attribute_name == "attached_object_name") {
                    // Add an attachment object
                    attached_object_vertical = current_attribute_value;
                }
            }
            if(attachment_horizontal == 0) {
                // Set the x
                if(x_type == _Size_Definition::Object_Scale_Size) set_x_in_object_scale(x);
                else if(x_type == _Size_Definition::Pixel_Size) set_x_in_pixel(x.to_double());
                else set_x_in_scale(x);
            }
            else {
                // Attach the object
                std::shared_ptr<__GUI_Object_Core> other_object;
                if(attached_object_vertical != "") other_object = loader.get()->created_objects[attached_object_vertical];
                if(attachment_horizontal == 1) {
                    if(other_object.get() == 0) attach_left_in_parent(attachment_horizontal_offset.to_double());
                    //else attach_bottom_of_object_in_parent(other_object, attachment_horizontal_offset);
                }
                else if(attachment_horizontal == 2) {
                    if(other_object.get() == 0) attach_right_in_parent(attachment_horizontal_offset.to_double());
                    else attach_right_of_object_in_parent(other_object, attachment_horizontal_offset.to_double());
                } //*/
            }
        }
        else if(xml_attribute_name == "y") {
            // Load the Y position
            unsigned char attachment_vertical = 0;
            Fraction attachment_vertical_offset = Fraction(0);
            std::string attached_object_vertical = "";
            Fraction y = Fraction(1);
            _Size_Definition y_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "value") {
                    // Value of the width
                    try {y = Fraction::from_std_string(current_attribute_value);}
                    catch(std::invalid_argument) {
                        y = Fraction(0); set_can_print(true);
                        print("Warning", "SCLS GUI Object \"" + name() + "\"", "Unvalid Y value \"" + current_attribute_value + "\".");
                    }
                }
                else if(current_attribute_name == "type") {
                    // Type of the width
                    if(current_attribute_value == "scale") y_type = _Size_Definition::Scale_Size;
                    else if(current_attribute_value == "pixel") y_type = _Size_Definition::Pixel_Size;
                    else if(current_attribute_value == "object_scale") y_type = _Size_Definition::Object_Scale_Size;
                }
                else if(current_attribute_name == "attachment" || current_attribute_name == "attach") {
                    // Use the attachment
                    if(current_attribute_value == "top") attachment_vertical = 1;
                    else if(current_attribute_value == "bottom")attachment_vertical = 2;
                }
                else if(current_attribute_name == "offset") {
                    // Add an attachment offset
                    attachment_vertical_offset = Fraction::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "attached_object" || current_attribute_name == "attached_object_name") {
                    // Add an attachment object
                    attached_object_vertical = current_attribute_value;
                }
            }
            if(attachment_vertical == 0) {
                // Set the y
                if(y_type == _Size_Definition::Object_Scale_Size) set_y_in_object_scale(y);
                else if(y_type == _Size_Definition::Pixel_Size) set_y_in_pixel(y.to_double());
                else set_y_in_scale(y);
            }
            else {
                // Attach the object
                std::shared_ptr<__GUI_Object_Core> other_object;
                if(attached_object_vertical != "") other_object = loader.get()->created_objects[attached_object_vertical];
                if(attachment_vertical == 1) {
                    if(other_object.get() == 0) attach_top_in_parent(attachment_vertical_offset);
                }
                else if(attachment_vertical == 2) {
                    if(other_object.get() == 0) attach_bottom_in_parent(attachment_vertical_offset);
                    else attach_bottom_of_object_in_parent(other_object, attachment_vertical_offset);
                }
            }
        }
    }

    //*********
    //
    // Transform handling
    //
    //*********

    // Calculate the adapted scale
    void GUI_Object::calculate_transformation(bool force, bool with_children) {
        if(!a_transformation_updated && !force) return;

        // Create the new transformation
        last_transformation_shared_ptr() = transformation_shared_ptr();
        __create_new_transformation(last_transformation_shared_ptr());

        // Calculate the transformation
        _apply_calculate_transformation(transformation_shared_ptr());

        a_transformation_updated = false;
        transformation()->calculate_transformation();

        // Check for the attachment horizontal
        if(a_transform_attachment.attachment_horizontal_type == 1) {
            __move_left_in_parent(a_transform_attachment.attachment_horizontal_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_horizontal_type == 2) {
            __move_right_in_parent(a_transform_attachment.attachment_horizontal_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_horizontal_type == 4) {
            __move_right_of_object_in_parent(a_transform_attachment.attached_object_horizontal, a_transform_attachment.attachment_horizontal_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }

        // Check for the attachment vertical
        if(a_transform_attachment.attachment_vertical_type == 1) {
            __move_top_in_parent(a_transform_attachment.attachment_vertical_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 2) {
            __move_bottom_in_parent(a_transform_attachment.attachment_vertical_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 3) {
            __move_top_of_object_in_parent(a_transform_attachment.attached_object_vertical, a_transform_attachment.attachment_vertical_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 4) {
            __move_bottom_of_object_in_parent(a_transform_attachment.attached_object_vertical, a_transform_attachment.attachment_vertical_offset.to_double());
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }

        // Apply to children
        if(with_children) {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i].get() != 0) {a_children[i].get()->after_resizing();}}}

        // Call the virtual function
        __GUI_Object_Core::calculate_transformation();
    }

    // Create the new transformation
    void GUI_Object::__create_new_transformation(std::shared_ptr<__GUI_Transformation> last_transformation_to_use) {
        std::shared_ptr<__GUI_Transformation> parent_transformation;
        if(parent() != 0) {
            parent()->calculate_transformation(false, false);
            parent_transformation = parent()->transformation_shared_ptr();
        }
        if(transformation() == 0) transformation_shared_ptr() = std::make_shared<__GUI_Transformation>(0, 0, parent_transformation);
        else { transformation_shared_ptr() = std::make_shared<__GUI_Transformation>(*transformation()); }
        transformation()->parent_shared_ptr() = parent_transformation;
        transformation()->set_window_height(window_struct().window_height());
        transformation()->set_window_width(window_struct().window_width());
    };

    // Delete the children of an object
    void GUI_Object::delete_children() { children().clear(); }

    // Returns the rect of the fitted texture
    glm::vec4 GUI_Object::fitted_texture_rect() {
        if(texture()->image_ratio() > (width_in_absolute_scale() / height_in_absolute_scale_and_window())) {
            return fitted_horizontally_texture_rect();
        }
        return fitted_vertically_texture_rect();
    }

    // Returns the rect of the horizontally fitted texture
    // PROBLEMATIC WITH PRIME NUMBER SIZED TEXTURES
    glm::vec4 GUI_Object::fitted_horizontally_texture_rect() {
        // Get the good size and X position
        Fraction height_texture = Fraction((floor((Fraction(width_in_pixel()) / texture()->image_ratio()).to_double()) - 0.5) * 2.0) / Fraction(height_in_pixel() * 2.0);
        Fraction width_texture = Fraction(1);
        Fraction x_texture = Fraction(0);
        // Get the good Y position
        Fraction y_texture = Fraction(0);
        if(texture_alignment_vertical() == Alignment_Vertical::V_Center) y_texture = Fraction(1, 2) - height_texture / 2.0;
        else if(texture_alignment_vertical() == Alignment_Vertical::V_Top) y_texture = Fraction(1) - height_texture;

        return glm::vec4(x_texture.to_double(), y_texture.to_double(), width_texture.to_double(), height_texture.to_double());
    }

    // Returns the rect of the vertically fitted texture
    // PROBLEMATIC WITH PRIME NUMBER SIZED TEXTURES
    glm::vec4 GUI_Object::fitted_vertically_texture_rect() {
        Fraction height_texture = Fraction((static_cast<double>(height_in_pixel()) - 0.5) * 2.0) / Fraction(height_in_pixel() * 2.0);
        Fraction width_texture = Fraction((floor((Fraction(height_in_pixel()) * texture()->image_ratio()).to_double()) - 0.5) * 2.0) / Fraction(width_in_pixel() * 2.0);
        Fraction y_texture = Fraction(0);
        // Get the good Y position
        Fraction x_texture = Fraction(0);
        if(texture_alignment_horizontal() == Alignment_Horizontal::H_Center) x_texture = Fraction(1, 2) - width_texture / 2.0;
        else if(texture_alignment_horizontal() == Alignment_Horizontal::H_Right) x_texture = Fraction(1) - width_texture;

        return glm::vec4(x_texture.to_double(), y_texture.to_double(), width_texture.to_double(), height_texture.to_double());
    }

    // Returns the position of the mouse in scale
    glm::vec2 GUI_Object::mouse_position_in_scale() {
        glm::vec2 mouse_pos = glm::vec2(window_struct().mouse_x(), window_struct().window_height() - window_struct().mouse_y());
        mouse_pos -= glm::vec2(x_in_absolute_pixel(), y_in_absolute_pixel());
        mouse_pos /= glm::vec2(width_in_pixel(), height_in_pixel());
        return mouse_pos;
    }

    // Returns the rect of user defined texture
    glm::vec4 GUI_Object::user_defined_texture_rect() {
        Fraction height_texture = texture_height_in_scale();
        Fraction width_texture = texture_width_in_scale();
        Fraction x_texture = Fraction(0);
        Fraction y_texture = Fraction(0);
        if(texture_alignment_horizontal() == Alignment_Horizontal::H_Center) x_texture = Fraction(1, 2) - width_texture / Fraction(2);
        else if(texture_alignment_horizontal() == Alignment_Horizontal::H_Right) x_texture = Fraction(1) - width_texture;
        if(texture_alignment_vertical() == Alignment_Vertical::V_Center) y_texture = Fraction(1, 2) - height_texture /Fraction(2);
        else if(texture_alignment_vertical() == Alignment_Vertical::V_Top) y_texture = Fraction(1) - height_texture;
        return glm::vec4(x_texture.to_double(), y_texture.to_double(), width_texture.to_double(), height_texture.to_double());
    }

    //*********
    //
    // GUI Main Object main functions
    //
    //*********

    // Update the object for the events
    void GUI_Main_Object::update_event() {
        // Check the overflighted cursor
        GUI_Object* current_overflighted_object = this;
        for(int i = 0;i<static_cast<int>(current_overflighted_object->children().size());i++) {
            GUI_Object* current_object = current_overflighted_object->children()[i].get();
            if(current_object->visible() && !current_object->ignore_click() && current_object->is_in_rect_in_pixel(window_struct().mouse_x(), window_struct().window_height() - window_struct().mouse_y())) {
                current_overflighted_object = current_object;
                i = -1;
            }
        }

        // Update the cursor texture
        if(a_overflighted_object != current_overflighted_object) {
            if(current_overflighted_object == 0) {
                window_struct().set_cursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
            }
            else {
                window_struct().set_cursor(glfwCreateStandardCursor(current_overflighted_object->overflighted_cursor()));
            }
        }
        a_overflighted_object = current_overflighted_object;
        GUI_Object* current_parent = a_overflighted_object;
        short state = 0;
        while(current_parent != 0) {
            current_parent->set_overflighting_state(state);
            current_parent = current_parent->parent();
            state++;
        }

        // Check the focused object
        if(window_struct().mouse_button_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(a_overflighted_object != 0) {
                a_focused_object = a_overflighted_object;
            }
            else {
                a_focused_object = 0;
            }
        }
        current_parent = a_focused_object;
        state = 0;
        while(current_parent != 0) {
            current_parent->set_focusing_state(state);
            current_parent = current_parent->parent();
            state++;
        }

        GUI_Object::update_event();
    }

    //*********
    //
    // GUI_Scroller main functions
    //
    //*********

    // Most basic GUI_Scroller constructor
    GUI_Scroller::GUI_Scroller(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) {
        a_scroller_children = *_create_scroller_children();
        a_scroller_children.get()->set_height_in_scale(Fraction(1));
        a_scroller_children.get()->set_width_in_scale(Fraction(1));
        a_scroller_children.get()->move_left_in_parent(1);
        a_scroller_children.get()->move_top_in_parent(1);
        a_created_objects_parent = a_scroller_children;
    }

    // GUI_Scroller destructor
    GUI_Scroller::~GUI_Scroller() {

    }

    // Check if a scroll should occurs
    void GUI_Scroller::check_scroll() {
        if(has_child_overflighted() || is_overflighted()) {
            double final_movement = window_struct().wheel_movement_y_during_this_frame();
            if(final_movement != 0) {
                scroll_y(Fraction(final_movement));
            }
        }
    }

    // Check the browser scroller
    void GUI_Scroller::check_scroller(bool reset) {
        int last_y_position = (a_scroller_children.get()->last_transformation()->y_in_pixel() + a_scroller_children.get()->last_transformation()->height_in_pixel()) - (a_scroller_children.get()->last_transformation()->parent()->height_in_pixel());
        GUI_Object::after_resizing();
        Fraction tallest_point = Fraction(0);
        for(int i = 0;i<static_cast<int>(a_scroller_children->children().size());i++) {
            GUI_Object* current_object = a_scroller_children->children()[i].get();
            if(Fraction(current_object->y_in_pixel()) + Fraction(current_object->height_in_pixel()) > tallest_point) {
                tallest_point = Fraction(current_object->y_in_pixel()) + current_object->height_in_pixel();
            }
        }
        a_scroller_children->set_height_in_pixel(static_cast<unsigned int>(tallest_point.to_double()));
        if(scroller_vertical_alignment() == Alignment_Vertical::V_Top) {
            a_scroller_children->move_top_in_parent(1);
        }
        else if(scroller_vertical_alignment() == Alignment_Vertical::V_Bottom) {
            a_scroller_children->move_bottom_in_parent(1);
        }

        // Resize the children
        if(a_scroller_children.get()->width_in_pixel() != width_in_pixel() - 2) a_scroller_children->set_width_in_pixel(width_in_pixel() - 2);

        /*// Calculate according to the last position
        int children_height = static_cast<int>(a_scroller_children->height_in_pixel());
        int height = static_cast<int>(height_in_pixel());
        if(!reset) {
            if(last_y_position >= 1) {
                if(height >= children_height || last_y_position < children_height - height) {
                    a_scroller_children->move_top_in_parent(-last_y_position);
                }
                else {
                    a_scroller_children->move_bottom_in_parent(1);
                }
            }
        } //*/
        //calculate_transformation(true, true);
    }

    // Private function to create the children scroller
    std::shared_ptr<GUI_Object>* GUI_Scroller::_create_scroller_children() {std::shared_ptr<GUI_Object>* to_return = GUI_Object::new_object<GUI_Object>(name() + "_children_scroller");return to_return;}

    // Scroll the scroller
    void GUI_Scroller::scroll_y(Fraction movement) {
        if(scroller_vertical_alignment() == Alignment_Vertical::V_Top) {
            if(a_scroller_children->height_in_scale() > 1) {
                movement *= Fraction(15);
                Fraction final_pos = (a_scroller_children->y_in_pixel()) - movement;
                a_scroller_children->set_y_in_pixel(final_pos);
            }
            else {
                a_scroller_children->move_top_in_parent(1);
            }
            a_scroller_children.get()->calculate_transformation(true, false);
            if(a_scroller_children.get()->children().size() > 0) a_scroller_children.get()->children()[a_scroller_children.get()->children().size()-1].get()->calculate_transformation(true, true);
        }
    }

    //*********
    //
    // GUI_Scroller_Choice main functions
    //
    //*********

    // Most basic GUI_Scroller_Choice constructor
    GUI_Scroller_Choice::GUI_Scroller_Choice(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : GUI_Scroller(window, name, parent) {}

    // GUI_Scroller_Choice destructor
    GUI_Scroller_Choice::~GUI_Scroller_Choice() {}

    // Select an object
    void GUI_Scroller_Choice::select_object(std::string object_name) {
        GUI_Object* object = scroller_children()->child_by_name(object_name);
        if(object != 0) {
            // Add the contained object
            a_choice_modified = true;
            if(contains_selected_object(object_name)) {
                if(a_unselect_object_on_confirmation) unselect_object(object_name);
                a_currently_confirmed_objects.push_back(object_name);
            }
            else {
                a_currently_selected_objects.push_back(object_name);

                // Remove the too much objects
                while(a_currently_selected_objects.size() > max_number_selected_object()) {
                    unselect_object(a_currently_selected_objects[0]);
                }
            }

            // Apply the graphic modification
            object->set_background_color(selected_objects_style().a_background_color);
        }
    };

    // Unselect an object
    void GUI_Scroller_Choice::unselect_object(std::string object_name) {
        GUI_Object* object = scroller_children()->child_by_name(object_name);
        for(int i = 0;i<static_cast<int>(currently_selected_objects().size());i++) {
            if(currently_selected_objects()[i]==object_name) {
                currently_selected_objects().erase(currently_selected_objects().begin()+i,currently_selected_objects().begin()+i+1);
                if(object != 0) object->set_background_color(unselected_objects_style().a_background_color);
                return;
            }
        }
    }

    // Update the even in the scroller
    void GUI_Scroller_Choice::update_event() {
        GUI_Scroller::update_event();

        // Check each buttons
        for(int i = 0;i<static_cast<int>(scroller_children()->children().size());i++) {
            if(scroller_children()->children()[i].get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                // If the button is clicked
                std::string object_name = scroller_children()->children()[i].get()->name();
                select_object(object_name);
            }
        } check_number_selected_object();
    }

    //*********
    //
    // __GUI_Text_Metadatas main functions
    //
    //*********

    // Handle an attribute from XML
    void __GUI_Text_Metadatas::set_xml_attribute(std::shared_ptr<XML_Text> text, std::string event, std::shared_ptr<__GUI_Page_Loader> loader, int& i) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "content") {
            // Get each attributes
            std::string needed_text = text.get()->text();
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                // Load the font size of the text
                if(current_attribute_name == "font_size") {
                    // Value of the font size
                    set_font_size(Fraction::from_std_string(current_attribute_value).to_double());
                } else if(current_attribute_name == "max_width") {
                    // Value of the max width
                    set_max_width(Fraction::from_std_string(current_attribute_value).to_double());
                } else if(current_attribute_name == "src") {
                    // Loads the content of a balise from a file
                    if(current_attribute_value[0] == '\"'){current_attribute_value = current_attribute_value.substr(1, current_attribute_value.size() - 1);}
                    if(current_attribute_value[current_attribute_value.size()-1] == '\"'){current_attribute_value = current_attribute_value.substr(0, current_attribute_value.size() - 1);}
                    if(std::filesystem::exists(current_attribute_value)) {
                        needed_text = read_file(current_attribute_value);
                    } else {
                        print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The path \"" + current_attribute_value + "\" you want to load as the content of this text does not exist.");
                    }
                }
            }
            // Get the content of the text
            needed_text = scls::format_string_tabulations(scls::format_string_break_line(needed_text, " "), "");
            set_text(needed_text);
        }
        else if(xml_attribute_name == "font_size") {
            // Load the font size of the text
            Fraction final_font_size = Fraction(1);
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "value") {
                    // Value of the height
                    final_font_size = Fraction::from_std_string(current_attribute_value);
                }
            }
            // Set the font size
            set_font_size(final_font_size.to_double());
        }
        else if(xml_attribute_name == "text_style") {
            // Load datas about the text
            Color final_color = font_color();
            Fraction final_font_size = Fraction(font_size());
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                if(current_attribute_name == "color") {
                    // Value of the color
                    final_color = Color::from_std_string(current_attribute_value);
                }
                else if(current_attribute_name == "font_size") {
                    // Value of the font size
                    final_font_size = Fraction::from_std_string(current_attribute_value);
                }
            }
            // Set the datas
            set_font_color(final_color);
            set_font_size(final_font_size.to_double());
        }
        else {
            // Load a lowest level attribute
            GUI_Object::set_xml_attribute(text, event, loader, i);
        }
    }

    //*********
    //
    // GUI Text main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text::GUI_Text(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {}

    // Function called after that the window is resized
    void GUI_Text::after_resizing(){
        __GUI_Text_Metadatas::after_resizing();
        if(max_width() != -1) {set_max_width(width_in_pixel());update_text_texture(scls::Image_Generation_Type::IGT_Size);}
    }

    // Update the texture of the text
    void GUI_Text::update_text_texture(scls::Image_Generation_Type generation_type) {
        if(text() != "") {
            // Create the text
            glm::vec2 position_to_apply = glm::vec2(x_in_pixel(), y_in_pixel());
            attached_text_image_block()->global_style().background_color = background_color();
            attached_text_image_block()->global_style().color = font_color();
            attached_text_image_block()->global_style().font_size = font_size();
            attached_text_image_block()->global_style().max_width = max_width();
            // a_text_image->set_cursor_position(cursor_position_in_formatted_text());
            // a_text_image->set_use_cursor(use_cursor());

            // Apply the text
            std::shared_ptr<Image> image_to_paste = attached_text_image_block()->image_shared_pointer(generation_type);
            texture()->set_image(image_to_paste);
        }
        else {
            texture()->set_image(0);
        }
    };

    //*********
    //
    // GUI_Text_Input main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text_Input::GUI_Text_Input(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {
        set_text_image_type(Block_Type::BT_Keep_Block_And_Line_In_Memory);
    }

    // Add a text to the input at the cursor position
    void GUI_Text_Input::add_text(std::string text_to_add) {
        // Prepare the needed datas to add (the text is treated as unformatted here)
        if(text_to_add != "") {
            if(attached_text_image() == 0) {
                set_text(text_to_add);
                set_cursor_position_in_formatted_text(text_to_add.size());
            } else {
                // Add the lines if needed
                unsigned int cursor_position = attached_text_image()->cursor_position_in_full_text();
                int new_line = count_string(text_to_add, "</br>");
                for(int i = 0;i<new_line;i++) {
                    if((attached_text_image()->line_number_at_position(cursor_position) - line_offset()) < children().size()) {
                        children().insert(children().begin() + (attached_text_image()->line_number_at_position(cursor_position) - line_offset()) + 1, 0);
                    }
                    else {
                        children().push_back(0);
                    }
                }
                // Update the text
                attached_text_image()->add_text(text_to_add);
                a_input_during_this_frame = true;
                a_text_modified = true;
                update_text_texture();
            }
        }
    };

    // Function called after that the window is resized
    void GUI_Text_Input::after_resizing(){
        GUI_Object::after_resizing();
        place_lines();
    }

    // Format a char
    std::string GUI_Text_Input::_format(std::string letter, bool apply_alt, bool apply_capitalisation) {
        std::string result = "";
        std::string to_analyse = "";

        for(int i = 0;i<static_cast<int>(letter.size());i++) {
            to_analyse += letter[i];
            if(!is_character_utf_8(letter[i])) {
                result += _format_one_letter(to_analyse, apply_alt, apply_capitalisation);
                to_analyse = "";
            }
        }
        // Last analyse
        if(to_analyse != "") result += _format_one_letter(to_analyse, apply_alt, apply_capitalisation);

        return result;
    }

    // Capitalize a std::string
    std::string GUI_Text_Input::_format_one_letter(std::string letter, bool apply_alt, bool apply_capitalisation) {
        if(apply_alt) {
            // Top bar
            if(letter == "é") letter = "~";
            if(letter == "\"") letter = "#";
            if(letter == "'") letter = "{";
            if(letter == "(") letter = "[";
            if(letter == "-") letter = "|";
            if(letter == "è") letter = "`";
            if(letter == "_") letter = "\\";
            if(letter == "ç") letter = "^";
            if(letter == "à") letter = "@";
            if(letter == ")") letter = "]";
        }
        if(apply_capitalisation) {
            // Alphabet letter
            if(letter == "a") letter = "A";
            if(letter == "b") letter = "B";
            if(letter == "c") letter = "C";
            if(letter == "d") letter = "D";
            if(letter == "e") letter = "E";
            if(letter == "f") letter = "F";
            if(letter == "g") letter = "G";
            if(letter == "h") letter = "H";
            if(letter == "i") letter = "I";
            if(letter == "j") letter = "J";
            if(letter == "k") letter = "K";
            if(letter == "l") letter = "L";
            if(letter == "m") letter = "M";
            if(letter == "n") letter = "N";
            if(letter == "o") letter = "O";
            if(letter == "p") letter = "P";
            if(letter == "q") letter = "Q";
            if(letter == "r") letter = "R";
            if(letter == "s") letter = "S";
            if(letter == "t") letter = "T";
            if(letter == "u") letter = "U";
            if(letter == "v") letter = "V";
            if(letter == "w") letter = "W";
            if(letter == "x") letter = "X";
            if(letter == "y") letter = "Y";
            if(letter == "z") letter = "Z";
            // Extended alphabet
            if(letter == "ù") letter = "%";
            if(letter == "^") letter = "¨";

            // Top bar
            if(letter == "&") letter = "1";
            if(letter == "é") letter = "2";
            if(letter == "\"") letter = "3";
            if(letter == "'") letter = "4";
            if(letter == "(") letter = "5";
            if(letter == "-") letter = "6";
            if(letter == "è") letter = "7";
            if(letter == "_") letter = "8";
            if(letter == "ç") letter = "9";
            if(letter == "à") letter = "0";

            // Ponctuation
            if(letter == ":") letter = "/";
            if(letter == ";") letter = ".";
            if(letter == ",") letter = "?";
            if(letter == "!") letter = "§";

            // Other
            if(letter == ")") letter = "°";
            if(letter == "=") letter = "+";
            if(letter == "$") letter = "£";
            if(letter == "*") letter = "µ";
            if(letter == "<") letter = ">";
        }

        // Set the last descriptive character
        if(a_last_descriptive_character == ""){
            if(letter == "^" || letter == "¨") {
                a_last_descriptive_character = letter;
                letter = "";
            }
        }
        else {
            if(a_last_descriptive_character == "^") {
                if(letter == "e") letter = "ê";
                else if(letter == "E") letter = "Ê";
                else if(letter == "a") letter = "â";
                else if(letter == "A") letter = "Â";
                else if(letter == "u") letter = "û";
                else if(letter == "U") letter = "Û";
                else if(letter == "i") letter = "î";
                else if(letter == "I") letter = "Î";
                else if(letter == "o") letter = "ô";
                else if(letter == "O") letter = "Ô";
                else {
                    if(letter == " ") letter = a_last_descriptive_character;
                    else letter = a_last_descriptive_character + letter;
                }
            }
            else if(a_last_descriptive_character == "¨") {
                if(letter == "e") letter = "ë";
                else if(letter == "E") letter = "Ë";
                else if(letter == "a") letter = "ä";
                else if(letter == "A") letter = "Ä";
                else if(letter == "u") letter = "ü";
                else if(letter == "U") letter = "Ü";
                else if(letter == "i") letter = "ï";
                else if(letter == "I") letter = "Ï";
                else if(letter == "o") letter = "ö";
                else if(letter == "O") letter = "Ö";
                else {
                    if(letter == " ") letter = a_last_descriptive_character;
                    else letter = a_last_descriptive_character + letter;
                }
            }
            a_last_descriptive_character = "";
        }

        return letter;
    }

    // Input the inputed text
    void GUI_Text_Input::input_text() {
        std::string final_text = text();
        std::string to_add = "";

        // Handle letters
        bool should_control = (window_struct().key_state("left control") == Key_State::Pressed || window_struct().key_state("right control") == Key_State::Pressed);
        bool should_alt = (window_struct().key_state("alt gr") == Key_State::Pressed);
        bool should_capitalise = (window_struct().key_state("left shift") == Key_State::Pressed || window_struct().key_state("right shift") == Key_State::Pressed);
        if(window_struct().key_pressed_or_repeated_pressed("a")) { to_add += "a";  }
        if(window_struct().key_pressed_or_repeated_pressed("b")) { to_add += "b";  }
        if(window_struct().key_pressed_or_repeated_pressed("c")) { to_add += "c";  }
        if(window_struct().key_pressed_or_repeated_pressed("d")) { to_add += "d";  }
        if(window_struct().key_pressed_or_repeated_pressed("e")) { to_add += "e";  }
        if(window_struct().key_pressed_or_repeated_pressed("f")) { to_add += "f";  }
        if(window_struct().key_pressed_or_repeated_pressed("g")) { to_add += "g";  }
        if(window_struct().key_pressed_or_repeated_pressed("h")) { to_add += "h";  }
        if(window_struct().key_pressed_or_repeated_pressed("i")) { to_add += "i";  }
        if(window_struct().key_pressed_or_repeated_pressed("j")) { to_add += "j";  }
        if(window_struct().key_pressed_or_repeated_pressed("k")) { to_add += "k";  }
        if(window_struct().key_pressed_or_repeated_pressed("l")) { to_add += "l";  }
        if(window_struct().key_pressed_or_repeated_pressed("m")) { to_add += "m";  }
        if(window_struct().key_pressed_or_repeated_pressed("n")) { to_add += "n";  }
        if(window_struct().key_pressed_or_repeated_pressed("o")) { to_add += "o";  }
        if(window_struct().key_pressed_or_repeated_pressed("p")) { to_add += "p";  }
        if(window_struct().key_pressed_or_repeated_pressed("q")) { to_add += "q";  }
        if(window_struct().key_pressed_or_repeated_pressed("r")) { to_add += "r";  }
        if(window_struct().key_pressed_or_repeated_pressed("s")) { to_add += "s";  }
        if(window_struct().key_pressed_or_repeated_pressed("t")) { to_add += "t";  }
        if(window_struct().key_pressed_or_repeated_pressed("u")) { to_add += "u";  }
        if(window_struct().key_pressed_or_repeated_pressed("v")) {
            if(should_control) {
                to_add += clipboard_datas();
            }
            else {
                to_add += "v";
            }
        }
        if(window_struct().key_pressed_or_repeated_pressed("w")) { to_add += "w";  }
        if(window_struct().key_pressed_or_repeated_pressed("x")) { to_add += "x";  }
        if(window_struct().key_pressed_or_repeated_pressed("y")) { to_add += "y";  }
        if(window_struct().key_pressed_or_repeated_pressed("z")) { to_add += "z";  }
        // Extended alphabet letter
        if(window_struct().key_pressed_or_repeated_pressed("ù")) { to_add += "ù";  }

        // Handle numbers
        if(window_struct().key_pressed_or_repeated_pressed("0")) { to_add += "0";  }
        if(window_struct().key_pressed_or_repeated_pressed("1")) { to_add += "1";  }
        if(window_struct().key_pressed_or_repeated_pressed("2")) { to_add += "2";  }
        if(window_struct().key_pressed_or_repeated_pressed("3")) { to_add += "3";  }
        if(window_struct().key_pressed_or_repeated_pressed("4")) { to_add += "4";  }
        if(window_struct().key_pressed_or_repeated_pressed("5")) { to_add += "5";  }
        if(window_struct().key_pressed_or_repeated_pressed("6")) { to_add += "6";  }
        if(window_struct().key_pressed_or_repeated_pressed("7")) { to_add += "7";  }
        if(window_struct().key_pressed_or_repeated_pressed("8")) { to_add += "8";  }
        if(window_struct().key_pressed_or_repeated_pressed("9")) { to_add += "9";  }
        if(window_struct().key_pressed_or_repeated_pressed("=")) { to_add += "=";  }
        if(window_struct().key_pressed_or_repeated_pressed("+")) { to_add += "+";  }
        if(window_struct().key_pressed_or_repeated_pressed("/")) { to_add += "/";  }
        if(window_struct().key_pressed_or_repeated_pressed(".")) { to_add += ".";  }

        // Handle numbers / special characters
        if(window_struct().key_pressed_or_repeated_pressed("&")) { to_add += "&";  }
        if(window_struct().key_pressed_or_repeated_pressed("é")) { to_add += "é";  }
        if(window_struct().key_pressed_or_repeated_pressed("\"")) { to_add += "\"";  }
        if(window_struct().key_pressed_or_repeated_pressed("'")) { to_add += "'";  }
        if(window_struct().key_pressed_or_repeated_pressed("(")) { to_add += "(";  }
        if(window_struct().key_pressed_or_repeated_pressed("-")) { to_add += "-";  }
        if(window_struct().key_pressed_or_repeated_pressed("è")) { to_add += "è";  }
        if(window_struct().key_pressed_or_repeated_pressed("_")) { to_add += "_";  }
        if(window_struct().key_pressed_or_repeated_pressed("ç")) { to_add += "ç";  }
        if(window_struct().key_pressed_or_repeated_pressed("à")) { to_add += "à";  }
        if(window_struct().key_pressed_or_repeated_pressed("^")) { to_add += "^";  }
        if(window_struct().key_pressed_or_repeated_pressed("*")) { to_add += "*";  }

        // Handle ponctuation
        if(window_struct().key_pressed_or_repeated_pressed(":")) { to_add += ":";  }
        if(window_struct().key_pressed_or_repeated_pressed(";")) { to_add += ";";  }
        if(window_struct().key_pressed_or_repeated_pressed(",")) { to_add += ",";  }
        if(window_struct().key_pressed_or_repeated_pressed("!")) { to_add += "!";  }
        if(window_struct().key_pressed_or_repeated_pressed("<")) { to_add += "<";  }

        // Handle special characters
        if(window_struct().key_pressed_or_repeated_pressed("backspace") == Key_State::Pressed && final_text.size() > 0 && cursor_position_in_unformatted_text() > 0) {
            remove_text(1);
        }
        if(window_struct().key_pressed_or_repeated_pressed("space")) { to_add += " ";  }
        if(window_struct().key_pressed_or_repeated_pressed("tab")) { to_add += "    ";  }
        if(window_struct().key_pressed_or_repeated_pressed(")")) { to_add += ")";  }
        if(window_struct().key_pressed_or_repeated_pressed("$")) { to_add += "$";  }
        to_add = _format(to_add, should_alt, should_capitalise);
        to_add = format_string_from_plain_text(to_add);

        if(window_struct().key_pressed_or_repeated_pressed("-k")) { to_add += "-";  }
        if(window_struct().key_pressed_or_repeated_pressed("*k")) { to_add += "*";  }
        if(window_struct().key_pressed_or_repeated_pressed("enter") == Key_State::Pressed) { to_add += "</br>";  }

        add_text(to_add);
    }

    // Move the cursor in the text
    void GUI_Text_Input::move_cursor(int movement) {
        int final_cursor_position = cursor_position_in_formatted_text() + movement;
        if(movement < 0) {
            if(final_cursor_position < 0) {
                final_cursor_position = 0;
            }
        }
        else if(movement > 0) {
            if(final_cursor_position > static_cast<int>(plain_text_size())) {
                final_cursor_position = plain_text_size();
            }
        }
        else if(final_cursor_position == static_cast<int>(cursor_position_in_formatted_text())) return;
        set_cursor_position_in_formatted_text(final_cursor_position);

        update_text_texture();
    }

    // Moves the cursor at a pixel position
    void GUI_Text_Input::move_cursor_at_position_in_scale(glm::vec2 position) {
        if(children().size() <= 0) return;
        GUI_Object* current_object = children()[0].get(); bool good = false; unsigned int i = 0;
        while(current_object->y_in_scale() > position[1] && i < children().size()) {
            i++;
            if(i < children().size()) {
                current_object = children()[i].get();
            }
        }
        if(i >= children().size()) {
            i = children().size() - 1;
            current_object = children()[children().size() - 1].get();
        }

        move_cursor(attached_text_image()->cursor_position_in_plain_text_at_line_and_x(i + line_offset(), position[0] * width_in_pixel()) - cursor_position_in_formatted_text());
    }

    // Moves the cursor in the Y axis
    void GUI_Text_Input::move_cursor_y(int movement) {
        if(attached_text_image() == 0) return;

        Text_Image_Line* cursor_line = attached_text_image()->cursor_line();
        if(cursor_line != 0) {
            int cursor_number = (cursor_line->datas().line_number) + movement;
            if(cursor_number < 0) {
                move_cursor(-cursor_position_in_formatted_text());
            }
            else if(cursor_number >= attached_text_image()->lines().size()) {
                move_cursor(plain_text_size() - cursor_position_in_formatted_text());
            }
            else {
                move_cursor(attached_text_image()->cursor_position_in_plain_text_at_line_and_x(cursor_number, cursor_line->cursor_x()) - cursor_position_in_formatted_text());
            }
        }
    }

    // Place the lines in the text
    void GUI_Text_Input::place_lines() {
        GUI_Object* last_object = 0;
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            GUI_Object* current_object = children()[i].get();
            if(current_object != 0) {
                current_object->set_height_in_pixel(current_object->texture()->get_image()->height());
                current_object->move_left_in_parent();
                if(last_object == 0) current_object->move_top_in_parent();
                else current_object->move_bottom_of_object_in_parent(last_object);
                last_object = current_object;
            }
        }
    }

    // Remove a text to the input at the cursor position
    void GUI_Text_Input::remove_text(unsigned int size_to_delete) {
        // Preparate the needed datas
        // Remove the text
        unsigned int line_to_delete = attached_text_image()->line_number_at_position(cursor_position_in_unformatted_text());
        unsigned int removed_lines = attached_text_image()->remove_text(size_to_delete);
        // Remove the needed children
        int final_size = static_cast<int>(children().size()) + line_offset();
        if(removed_lines > 0) {for(int i = line_to_delete;i<final_size;i++) { children().erase(children().begin() + line_to_delete);}}
        update_text_texture();
    }

    // Update the cursor behavior
    void GUI_Text_Input::update_cursor() {
        if(window_struct().key_pressed_or_repeated_pressed("left arrow")) {move_cursor(-1);}
        if(window_struct().key_pressed_or_repeated_pressed("right arrow")) {move_cursor(1);}

        if(window_struct().key_pressed_or_repeated_pressed("up arrow")) {move_cursor_y(-1);}
        if(window_struct().key_pressed_or_repeated_pressed("down arrow")) {move_cursor_y(1);}

        if(is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT) || has_child_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            move_cursor_at_position_in_scale(mouse_position_in_scale());
        }

        // Handle wheel scrolling
        if(has_child_overflighted() ||is_overflighted()) {
            double final_movement = window_struct().wheel_movement_y_during_this_frame();
            if(final_movement > 0 && a_line_offset > 0) final_movement = -1;
            else if(final_movement < 0 && a_line_offset < attached_text_image()->lines_datas().size()) final_movement = 1;
            else final_movement = 0;
            if(final_movement != 0) {
                a_line_offset += final_movement;
                delete_children();
                update_text_texture();
            }
        }
    }

    // Update the text
    void GUI_Text_Input::update_event() {
        GUI_Object::update_event();
        if(visible() && (is_focused() || has_child_focused())) {
            input_text();
            update_cursor();
        }
    }

    // Update the texture of the text
    void GUI_Text_Input::update_text_texture() {
        // Configure the text image
        if(attached_text_image() == 0) attached_text_image_block()->generate_blocks();
        if(attached_text_image() == 0) return;
        attached_text_image()->global_style().color = font_color();
        attached_text_image()->global_style().font = font();
        attached_text_image()->set_cursor_position_in_plain_text(cursor_position_in_formatted_text());
        attached_text_image()->set_use_cursor(true);

        // Configure the needed creation settings
        attached_text_image()->reset_line_generation();

        // Delete the useless children modified
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(i >= static_cast<int>(attached_text_image()->lines_datas().size())) {
                 children().erase(children().begin() + i); i--;
            }
            else if(children()[i] != 0 && (i + line_offset() < attached_text_image()->lines().size() && (attached_text_image()->lines()[i + line_offset()] == 0 || attached_text_image()->lines()[i + line_offset()]->has_been_modified()))) {
                children()[i].reset();
            }
        }

        // Add lines if needed
        while(children().size() < static_cast<int>(attached_text_image()->lines_datas().size() - line_offset())) {
            children().push_back(0);
        }

        unsigned short line_max_number = 0;
        unsigned int total_height = 0;
        for(int i = 0;i + line_offset()<static_cast<int>(attached_text_image()->lines_datas().size());i++) {
            // Create the configuration for the line
            Text_Image_Line* line_to_apply = attached_text_image()->generate_next_line(i + line_offset());
            if(line_to_apply != 0) {
                // Check the total height
                if(total_height > height_in_pixel()) break;

                if(line_to_apply->has_been_modified() || (i < static_cast<int>(children().size()) && children()[i] == 0) || i >= static_cast<int>(children().size())) {
                    Image* image_to_apply = line_to_apply->image();
                    if(image_to_apply != 0) {
                        line_to_apply->set_has_been_modified(false);

                        // Generate the object for the line
                        std::string final_name = name() + "_gen_" + std::to_string(a_generation) + "_line_" + std::to_string(i);
                        std::shared_ptr<GUI_Object> new_line = *new_object<GUI_Object>(final_name, 0, i * 60);
                        new_line.get()->set_ignore_click(true);
                        new_line.get()->set_width_in_scale(Fraction(1)); new_line.get()->set_height_in_pixel(image_to_apply->height());
                        new_line.get()->set_texture_alignment_horizontal(Alignment_Horizontal::H_Left);
                        new_line.get()->texture()->set_image(line_to_apply->shared_image());

                        // Place the children
                        if(i < static_cast<int>(children().size()) && (children()[i].get() == 0 || line_to_apply->has_been_modified())) { children()[i] = children()[children().size() - 1]; children().pop_back(); }
                        total_height += image_to_apply->height();
                    }
                }
                else {
                    Image* image_to_apply = line_to_apply->image();
                    if(image_to_apply != 0) total_height += image_to_apply->height();
                }
            }
        }

        // End configure the needed creation settings
        attached_text_image()->delete_useless_generated_lines();

        // Delete the useless children in more
        for(int i = 0;i<static_cast<int>(children().size()) - static_cast<int>(attached_text_image()->lines_datas().size());i++) {children().pop_back();}

        // Delete empty children
        for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i].get() == 0) {children().erase(children().begin() + i); i--;}}

        place_lines(); a_generation++;
    };

    //*********
    //
    // GUI_File_Explorer main function
    //
    //*********

    // Most parent GUI_File_Explorer constructor used for displaying
    GUI_File_Explorer::GUI_File_Explorer(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) { load(); }

    // Function called after that the window is resized
    void GUI_File_Explorer::after_resizing() {
         place_all();
    }

    // Returns if a file is chosen during this frame
    bool GUI_File_Explorer::file_chosen() {
        if(!choose_clicked() && !a_file_chose) return false;
        return true;
    }

    // Load the explorer
    void GUI_File_Explorer::load() {
        // Browser of the file explorer
        a_browser = *new_object<GUI_Scroller>("browser");
        a_browser.get()->set_background_color(scls::Color(255, 255, 255));
        a_browser.get()->set_border_width_in_pixel(1);
        a_browser.get()->set_height_in_scale(Fraction(4, 5));
        a_browser.get()->set_width_in_scale(Fraction(4, 5));
        // Button to chose a file
        a_choose_button = *new_object<GUI_Text>("choose_button");
        a_choose_button.get()->set_border_width_in_pixel(1);
        a_choose_button.get()->set_font_size(50);
        a_choose_button.get()->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_choose_button.get()->set_text(choose_button_text());
        a_choose_button.get()->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_choose_button.get()->set_height_in_scale(Fraction(1, 10));
        a_choose_button.get()->set_width_in_scale(Fraction(3, 20));
        // Final path of the file explorer
        a_final_path = *new_object<GUI_Text>("final_path");
        a_final_path.get()->set_font_size(50);
        a_final_path.get()->set_text(final_path_text());
        a_final_path.get()->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_final_path.get()->set_height_in_scale(Fraction(1, 10));
        a_final_path.get()->set_width_in_scale(Fraction(4, 5));
        // Top bar of the file explorer
        a_top_bar = *new_object<GUI_Object>("top_bar");
        a_top_bar.get()->set_background_color(scls::Color(209, 209, 209));
        a_top_bar.get()->set_height_in_pixel(30);
        a_top_bar.get()->set_width_in_scale(Fraction(1));
        a_top_bar.get()->set_x_in_scale(Fraction(0));

        place_all();
    }

    // Place all the elements in the file explorer
    void GUI_File_Explorer::place_all() {
        GUI_Object::after_resizing();

        // Place each object
        a_choose_button->move_bottom_in_parent();
        a_choose_button->move_left_in_parent();
        a_final_path->set_height_in_scale(Fraction(1, 10));
        a_final_path->set_width_in_scale(Fraction(4, 5));
        a_final_path->move_bottom_in_parent();
        a_final_path->move_right_in_parent();
        a_browser->move_right_in_parent();
        a_browser->move_top_of_object_in_parent(a_final_path);
        a_top_bar->set_height_in_pixel(30);
        a_top_bar->set_x_in_scale(Fraction(0));
        a_top_bar->move_top_in_parent();

        place_browser_buttons();
        place_top_bar_buttons();
    }

    // Place correctly all the buttons in the browser
    void GUI_File_Explorer::place_browser_buttons() {
        std::shared_ptr<GUI_Text> last_button = 0;
        for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
            std::shared_ptr<GUI_Text> current_button = a_browser_buttons[static_cast<int>(a_browser_buttons.size()) - (i + 1)];
            current_button.get()->move_left_in_parent(1);
            if(last_button.get() == 0) current_button.get()->move_bottom_in_parent();
            else current_button.get()->move_top_of_object_in_parent(last_button);
            last_button = current_button;
        }
    }

    // Place correctly all the buttons in the top bar
    void GUI_File_Explorer::place_top_bar_buttons() {
        std::shared_ptr<GUI_Text> last_button;
        for(int i = 0;i<static_cast<int>(a_top_bar_buttons.size());i++) {
            a_top_bar_buttons[i].get()->set_height_in_pixel(a_top_bar_buttons[i]->texture()->get_image()->height());
            a_top_bar_buttons[i].get()->set_width_in_pixel(a_top_bar_buttons[i]->texture()->get_image()->width());
            a_top_bar_buttons[i].get()->move_top_in_parent();
            if(last_button.get() == 0) a_top_bar_buttons[i].get()->move_left_in_parent();
            else a_top_bar_buttons[i].get()->move_right_of_object_in_parent(last_button);
            last_button = a_top_bar_buttons[i];
        }
    }

    // Set the current path to a new path
    void GUI_File_Explorer::set_path(std::string path) {
        // Format the path
        if(path[path.size() - 1] == '/' && path[path.size() - 2] != ':') path = path.substr(0, path.size() - 1);
        #if defined(__linux__)
        if(path[0] != '/') path = "/" + path;
        #endif // defined

        path = replace(path, "\\", "/");
        if(a_current_path == "" || contains_selected_file(file_name(path, true))) {
            if(std::filesystem::is_directory(path)) {
                // Handle a directory
                a_browser_buttons_to_modify.clear();
                a_current_path = path;
                a_currently_selected_files = "";
                update_browser();
                update_top_bar();
            }
            else {
                a_file_chose = true;
            }
        }
        else {
            // Handle a single file
            for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
                std::string file = a_browser_buttons[i]->plain_text();
                if(contains_selected_file(file) || file == file_name(path, true)) {
                    a_browser_buttons_to_modify.push_back(i);
                }
            }
            if(a_browser_buttons_to_modify.size() > 0) {
                a_currently_selected_files = file_name(path, true);
                update_browser();
            }
        }
        a_final_path->set_text(final_path_text() + " : " + replace(path, "\\", "/"));
        place_all();
    }

    // Set the file explorer to the user current document directory
    void GUI_File_Explorer::set_current_user_document_directory() { set_path(current_user_document_directory()); }

    // Update the browser of the file explorer
    void GUI_File_Explorer::update_browser() {
        unsigned char current_thread_number = 0;
        bool from_scratch = false;
        unsigned char max_thread_number = 10;
        std::vector<std::string> paths = directory_content(to_utf_8(a_current_path));
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        if(a_browser_buttons_to_modify.size() == 0) {
            // Create the buttons from scratch
            a_browser->reset();
            a_browser_buttons.clear();
            from_scratch = true;
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || !is_file_authorized(paths[i])) continue;

                // Create the button
                paths[i] = file_name(paths[i], true);
                std::shared_ptr<GUI_Text> new_button = *a_browser.get()->new_object_in_scroller<GUI_Text>("browser_button_" + std::to_string(i));
                new_button.get()->set_background_color(scls::Color(255, 255, 255));
                new_button.get()->set_font_color(scls::Color(0, 0, 0));
                new_button.get()->set_font_size(50);
                new_button.get()->set_overflighted_cursor(GLFW_HAND_CURSOR);
                new_button.get()->set_height_in_pixel(30);
                new_button.get()->set_width_in_scale(Fraction(4, 5));
                new_button.get()->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
                new_button.get()->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
                a_browser_buttons.push_back(new_button);

                // Create the thread
                if(max_thread_number > 0) {
                    if(current_thread_number >= max_thread_number) {
                        current_thread_number = 0;
                        for(int j = 0;j<static_cast<int>(threads.size());j++) {
                            std::thread* current_thread = threads[j];
                            current_thread->join();
                            delete current_thread; current_thread = 0;
                            a_browser_buttons[((a_browser_buttons.size() - 1) - threads.size()) + j]->texture()->change_texture();
                        } threads.clear();
                    }
                    std::string& button_text_reference = paths[i];
                    std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button.get(), button_text_reference);
                    threads.push_back(current_thread);
                    current_thread_number++;
                }
                else {
                    new_button.get()->set_text(paths[i]);
                }
            }

            for(int j = 0;j<static_cast<int>(threads.size());j++) {
                std::thread* current_thread = threads[j];
                current_thread->join();
                delete current_thread; current_thread = 0;
                a_browser_buttons[(a_browser_buttons.size() - threads.size()) + j].get()->texture()->change_texture();
            } threads.clear();

            a_browser_y = 0;
        }
        else {
            // Modify some buttons
            std::vector<std::shared_ptr<GUI_Text>> buttons_to_modify = std::vector<std::shared_ptr<GUI_Text>>();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                bool contains = false;
                for(int j = 0;j<static_cast<int>(a_browser_buttons_to_modify.size());j++) {if(a_browser_buttons_to_modify[j] == i){contains = true;break;}}
                if(!std::filesystem::exists(paths[i]) || !contains) continue;

                // Get the button
                std::shared_ptr<GUI_Text> new_button = a_browser_buttons[i];
                std::string button_text = new_button.get()->text();
                buttons_to_modify.push_back(new_button);

                // Change the specification of the button
                if(contains_selected_file(button_text)) {
                    new_button.get()->set_background_color(scls::Color(0, 0, 255));
                    new_button.get()->set_font_color(scls::Color(255, 255, 255));
                }
                else {
                    new_button.get()->set_background_color(scls::Color(255, 255, 255));
                    new_button.get()->set_font_color(scls::Color(0, 0, 0));
                }

                // Create the thread
                std::string& button_text_reference = button_text;
                std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button.get(), button_text_reference);
                threads.push_back(current_thread);
            }
            a_browser_buttons_to_modify.clear();

            // Let each thread work
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                std::thread* current_thread = threads[i];
                current_thread->join();
                delete current_thread; current_thread = 0;
                buttons_to_modify[i].get()->texture()->change_texture();
            }
        }
        threads.clear();

        place_all();
        a_browser.get()->check_scroller(from_scratch);
    }

    // Update the explorer during an event
    void GUI_File_Explorer::update_event() {
        GUI_Object::update_event();

        // Check if a browser button is clicked
        for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
            std::string path = a_current_path + "/" + to_utf_8(a_browser_buttons[i]->plain_text());
            if(a_browser_buttons[i]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                set_path(path);
                break;
            }
        }

        // Check if a top button is clicked
        std::string path = "";
        for(int i = 0;i<static_cast<int>(a_top_bar_buttons.size());i++) {
            if(i > 0) {
                path += a_top_bar_buttons[i]->plain_text();
            }
            if(a_top_bar_buttons[i]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_current_path = "";
                set_path(path);
                break;
            }
        }
    }

    // Update the size of the file explorer
    void GUI_File_Explorer::update_GUI_scale() {
        place_all();
    }

    // Update the current path in the top bar
    void GUI_File_Explorer::update_top_bar() {
        a_top_bar.get()->delete_children(); a_top_bar_buttons.clear();

        // Create the buttons
        std::string first_text = "Ce PC >";
        std::vector<std::string> path_pieces = cut_path(current_path());
        path_pieces.push_back(first_text);
        for(int i = 0;i<static_cast<int>(path_pieces.size());i++) {
            std::string button_text = path_pieces[path_pieces.size() - (1 + i)];
            if(button_text == "") continue;
            if(button_text != first_text) button_text += "/";
            std::shared_ptr<GUI_Text> new_button = *a_top_bar->new_object<GUI_Text>("top_bar_button_" + std::to_string(i), 0, 0);
            new_button.get()->set_font_size(22);
            new_button.get()->set_overflighted_cursor(GLFW_HAND_CURSOR);
            new_button.get()->set_text(button_text);
            new_button.get()->set_height_in_pixel(22);
            new_button.get()->set_texture_alignment(scls::Alignment_Texture::T_Fit_Horizontally);
            new_button.get()->set_texture_alignment_vertical(scls::Alignment_Vertical::V_Center);
            a_top_bar_buttons.push_back(new_button);
        }
        place_all();
    }

    // GUI_File_Explorer destructor
    GUI_File_Explorer::~GUI_File_Explorer() {}

    //*********
    //
    // GUI page
    //
    //*********

    // Balises for a Window loading
    std::shared_ptr<__Balise_Container> gui_loading_balises;

    // GUI_Page most basic constructor
    GUI_Page::GUI_Page(_Window_Advanced_Struct* window_struct, std::string name) : Object(window_struct, name) {
        set_scale(glm::vec3(2, 2, 1));

        a_parent_object = std::make_shared<GUI_Main_Object>((*reinterpret_cast<_Window_Advanced_Struct*>(window_struct)), "main_" + name);
        a_parent_object.get()->set_position_in_pixel(0, 0);
        a_parent_object.get()->set_height_in_scale(scls::Fraction(1));
        a_parent_object.get()->set_width_in_scale(scls::Fraction(1));
    }

    // Render the page
    void GUI_Page::render(){

        // Render the object
        parent_object()->render(absolute_scale());

        // Soft reset the page
        parent_object()->soft_reset();
        soft_reset();

        // using namespace std::chrono_literals;
        // std::this_thread::sleep_for(10ms);
    };

    // Handle an attribute from XML
    void GUI_Page::set_xml_attribute(std::shared_ptr<XML_Text> text, std::shared_ptr<__XML_Loader> loader_shared_ptr, int& i) {
        __XML_Loader& loader = *loader_shared_ptr.get();
        if(text.get()->xml_balise_name() == "content") {
            // Load the content of the page
            std::string src = "";
            bool sub_paged = false;
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                std::string current_attribute_name = text.get()->xml_balise_attributes()[j].name;
                std::string current_attribute_value = text.get()->xml_balise_attributes()[j].value;

                if(current_attribute_name == "src") {
                    // Source of the content
                    src = current_attribute_value;
                }
                else if(current_attribute_name == "paged" || current_attribute_name == "sub_paged") {
                    // Set the page as sub-paged
                    sub_paged = true;
                }
            }
            // Get the content
            std::string final_path = path_parent(loader.window_file_path) + "/" + src;
            if(std::filesystem::exists(final_path)) {
                if(std::filesystem::is_directory(final_path)) {
                    print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The path \"" + final_path + "\" you want to load as the content of this page is a directory.");
                } else {
                    load_objects_from_xml(format_for_xml(remove_comment_out_of(read_file(final_path), "\"")), sub_paged);
                }
            } else {
                print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The path \"" + final_path + "\" you want to load as the content of this page does not exist.");
            }
        }
        else {
            Object::set_xml_attribute(text, loader_shared_ptr, i);
        }
    }

    // Update the event of the page
    void GUI_Page::update_event() {
        Object::update_event();
        parent_object()->update_event();
    }

    //*********
    //
    // Load the page
    //
    //*********

    // Create an object from a type
    std::shared_ptr<GUI_Object> GUI_Page::__create_loaded_object_from_type(std::string object_name, std::string object_type, GUI_Object* parent) {
        if(object_type == "choice") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Scroller_Choice>(object_name);
            return to_return;
        } else if(object_type == "text") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Text>(object_name);
            return to_return;
        }
        else if(object_type == "text_input") {
            std::shared_ptr<GUI_Text_Input> to_return = *parent->new_object<GUI_Text_Input>(object_name);
            return to_return;
        }
        else if(object_type == "scroller") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Scroller>(object_name);
            return to_return;
        }
        else if(object_type == "file_explorer") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_File_Explorer>(object_name);
            return to_return;
        }
        return *parent->new_object<GUI_Object>(object_name);
    };

    // Loads an object in a page from XML and returns it
    std::shared_ptr<GUI_Object> GUI_Page::__load_object_from_xml(std::string object_name, std::string object_type, std::shared_ptr<XML_Text> content) {
        // Search the parent
        GUI_Object* current_parent = parent_object();
        for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
            std::shared_ptr<XML_Text> current_text = content.get()->sub_texts()[i];

            if(current_text.get()->xml_balise_name() == "parent") {
                // Load the parent
                for(int j = 0;j<static_cast<int>(current_text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = current_text.get()->xml_balise_attributes()[j];
                    std::string current_attribute_name = current_attribute.name;

                    if(current_attribute_name == "name") { current_parent = reinterpret_cast<GUI_Object*>(a_loader.get()->created_objects[current_attribute.value].get()); }
                }
                break;
            }
        }
        if(current_parent == 0) current_parent = parent_object();

        // Create the object
        std::shared_ptr<GUI_Object> object = __create_loaded_object_from_type(object_name, object_type, current_parent);
        a_loader.get()->created_objects[object_name] = object;
        object.get()->__load_from_xml(content, a_loader);
        return object;
    }

    // Load objects in a page from XML
    void GUI_Page::load_objects_from_xml(const std::string& content_to_parse, bool sub_paged) {
        // Load the needed balises
        if(gui_loading_balises.get() == 0) {gui_loading_balises = std::make_shared<__Balise_Container>();gui_loading_balises.get()->__load_built_in_balises_gui();}
        std::shared_ptr<XML_Text> content = std::make_shared<XML_Text>(gui_loading_balises, content_to_parse);

        // Check each balises
        a_loader = std::make_shared<__GUI_Page_Loader>();
        for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
            std::shared_ptr<XML_Text> current_text = content.get()->sub_texts()[i];
            std::string current_balise_name = current_text.get()->xml_balise_name();

            // Add a GUI object
            if(current_balise_name == "gui_object") {
                bool must_be_visible = false;
                std::string object_name = "";
                std::string object_type = "";
                for(int j = 0;j<static_cast<int>(current_text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = current_text.get()->xml_balise_attributes()[j];
                    std::string current_attribute_name = current_attribute.name;
                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        object_name = current_attribute.value;
                    }
                    else if(current_attribute_name == "type") {
                        // Get the type of the object
                        object_type = current_attribute.value;
                    }
                    else if(current_attribute_name == "visible") {
                        // Get that the object MUST be visible
                        must_be_visible = true;
                    }
                }

                // Check if the object already exists
                for(std::map<std::string, std::shared_ptr<__GUI_Object_Core>>::iterator it = a_loader.get()->created_objects.begin();it!=a_loader.get()->created_objects.end();it++) {
                    if(it->first == object_name) {
                        set_can_print(true);
                        print("Warning", "SCLS GUI Page \"" + name() + "\"", "The \"" + object_name + "\" name does already exist.");
                        continue;
                    }
                }

                // Create the object
                if(object_name != "") {
                    std::shared_ptr<GUI_Object> object = __load_object_from_xml(object_name, object_type, current_text);
                    if(sub_paged && !must_be_visible) {
                        object.get()->set_visible(object.get()->visible() && object.get()->parent() != parent_object());
                    }
                }
            }
        }
    }
}
