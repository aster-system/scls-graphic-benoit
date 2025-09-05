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

    // Get datas about a border from an XML loading system
    void border_from_xml(std::shared_ptr<__XML_Text_Base> text, scls::Color& border_color, scls::Fraction& border_bottom, scls::Fraction& border_left, scls::Fraction& border_right, scls::Fraction& border_top) {
        for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
            XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
            std::string current_attribute_name = current_attribute.name;
            std::string current_attribute_value = current_attribute.value;

            if(current_attribute_name == "red") {border_color.set_red(std::stoi(current_attribute_value));}
            else if(current_attribute_name == "green") {border_color.set_green(std::stoi(current_attribute_value));}
            else if(current_attribute_name == "blue") {border_color.set_blue(std::stoi(current_attribute_value));}
            else if(current_attribute_name == "bottom") {border_bottom = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "left") {border_left = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "right") {border_right = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "top") {border_top = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "width") {
                border_bottom = Fraction::from_std_string(current_attribute_value);
                border_left = Fraction::from_std_string(current_attribute_value);
                border_right = Fraction::from_std_string(current_attribute_value);
                border_top = Fraction::from_std_string(current_attribute_value);
            }
        }
    }
    void border_from_xml(std::shared_ptr<__XML_Text_Base> text, scls::Color& border_color, glm::vec4& border){
        scls::Fraction bottom = 0;scls::Fraction left = 0;scls::Fraction right = 0;scls::Fraction top = 0;
        border_from_xml(text, border_color, bottom, left, right, top);
        border.x = bottom.to_double();border.y = left.to_double();border.z = right.to_double();border.w = top.to_double();
    }
    void border_from_xml(std::shared_ptr<__XML_Text_Base> text, std::shared_ptr<__Image_Base> img) {
        scls::Color border_color;scls::Fraction bottom; scls::Fraction left; scls::Fraction right; scls::Fraction top;
        border_from_xml(text, border_color, bottom, left, right, top);
        img.get()->draw_border(bottom.to_double(), left.to_double(), right.to_double(), top.to_double(), border_color);
    }
    void border_from_xml(std::shared_ptr<__XML_Text_Base> text, scls::Text_Style style){
        scls::Fraction bottom = 0;
        scls::Color border_color;
        scls::Fraction left = 0;
        scls::Fraction right = 0;
        scls::Fraction top = 0;
        border_from_xml(text, border_color, bottom, left, right, top);
        style.set_border_bottom_width(bottom.to_double());
        style.set_border_color(border_color);
        style.set_border_left_width(left.to_double());
        style.set_border_right_width(right.to_double());
        style.set_border_top_width(top.to_double());
    }

    // Get datas about a padding from an XML loading system
    void padding_from_xml(std::shared_ptr<__XML_Text_Base> text, scls::Fraction& border_bottom, scls::Fraction& border_left, scls::Fraction& border_right, scls::Fraction& border_top) {
        for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
            XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
            std::string current_attribute_name = current_attribute.name;
            std::string current_attribute_value = current_attribute.value;

            if(current_attribute_name == "bottom") {border_bottom = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "left") {border_left = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "right") {border_right = Fraction::from_std_string(current_attribute_value);}
            else if(current_attribute_name == "top") {border_top = Fraction::from_std_string(current_attribute_value);}
        }
    }
    void padding_from_xml(std::shared_ptr<__XML_Text_Base> text, scls::Text_Style style){
        scls::Fraction bottom = 0; scls::Fraction left = 0; scls::Fraction right = 0; scls::Fraction top = 0;
        padding_from_xml(text, bottom, left, right, top);
        style.set_padding_bottom(bottom.to_double());style.set_padding_left(left.to_double());style.set_padding_right(right.to_double());style.set_padding_top(top.to_double());
    }

    //*********
    //
    // GUI Object main functions
    //
    //*********

    // Most basic GUI_Object constructor
    GUI_Object::GUI_Object(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent) : __GUI_Object_Core(window, name, parent.lock().get()), a_parent(parent) {
        a_texture = (*window.new_texture_shared_ptr(name, 1, 1, Color(255, 255, 255, 0)));
        a_vao = (*window.vao_shared_ptr("gui_default"));
    }

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

    // Merges the current object with a specific style
    void GUI_Object::merge_style(GUI_Style* new_style) {set_background_color(new_style->a_background_color);current_style()->global_text_style.merge_style(new_style->global_text_style);}

    // Render the object
    void GUI_Object::render(bool render_children, glm::vec3 scale_multiplier) {
        if(a_transformation_updated){calculate_transformation(true);}

        // Handle the extremum
        if(use_extremums()){if(!apply_extremum(vao())){return;}}
        else{vao()->get_shader_program()->set_uniform4f_value("object_extremum", glm::vec4(0, 0, 1, 1));}

        // Debug mode
        if(window_struct().debug_mode() & 2){print(std::string("SCLS GUI Object \"") + name() + std::string("\""), std::string("Start rendering..."));}

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
        glm::vec4 needed_texture_rect = texture_rect();
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", needed_texture_rect);

        // Handle the texture and the VAO
        if(texture() == 0){vao()->get_shader_program()->set_uniformb_value("texture_binded", false);}
        else{texture()->bind();vao()->get_shader_program()->set_uniformb_value("texture_binded", true);}
        vao()->get_shader_program()->set_uniform4fv_value("model", matrix);
        vao()->get_shader_program()->set_uniform4f_value("object_rect", glm::vec4(0, 0, width_in_pixel(), height_in_pixel()));
        vao()->get_shader_program()->set_uniform2f_value("scale", texture_scale_x(), texture_scale_y());
        vao()->render();
        set_should_render_during_this_frame(false);

        // Debug mode
        if(window_struct().debug_mode() >> 1){print(std::string("SCLS GUI Object \"") + name() + std::string("\""), std::string("Successful rendering."));}

        if(render_children){for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] != 0 && children()[i]->visible()) children()[i]->render(true, scale_multiplier);}}
    }

    // Update the object for the events
    void GUI_Object::update_event() {
        for(int i = 0;i<static_cast<int>(children().size());i++){
            if(children()[i] != 0 && children()[i]->visible()){
                // Debug
                if(window_struct().debug_mode() & 3){scls::print(std::string("SCLS GUI Page \"") + name() + std::string("\""), std::string("Start updating GUI object \"") + children()[i]->name() + std::string("\" events..."));}

                children()[i]->update_event();

                // Debug
                if(window_struct().debug_mode() & 3){scls::print(std::string("SCLS GUI Page \"") + name() + std::string("\""), std::string("Successfully updated GUI object \"") + children()[i]->name() + std::string("\" events."));}
            }
        }
    }

    // GUI_Object destructor
    GUI_Object::~GUI_Object() {delete_children();if(parent() != 0) {parent()->child_deleted(this);}window_struct().remove_texture(texture());}

    //*********
    //
    // Loading handler
    //
    //*********

    // Loads the object from XML
    void GUI_Object::__load_from_xml_apply(std::string event) {
        // Apply the changes
        for(int i = 0;i<static_cast<int>(a_loading_parts.size());i++){if(a_loading_parts[i].content.get()->xml_balise_name() != std::string("content")){set_xml_attribute(a_loading_parts[i].content, a_loading_parts[i].event);a_loading_parts.erase(a_loading_parts.begin() + i);i--;}}
        for(int i = 0;i<static_cast<int>(a_loading_parts.size());i++){set_xml_attribute(a_loading_parts[i].content, a_loading_parts[i].event);a_loading_parts.erase(a_loading_parts.begin() + i);i--;}
    }
    void GUI_Object::load_from_xml(std::shared_ptr<__XML_Text_Base> text, std::string event) {
        if(text.get() == 0){return;}
        set_loaded(true);

        // Parse the content
        std::vector<__Loading_Part>& needed_part = a_loading_parts;
        for(int i = 0;i<static_cast<int>(text.get()->sub_texts().size());i++) {
            std::shared_ptr<__XML_Text_Base> current_text = text.get()->sub_texts()[i];
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
                    load_from_xml(current_text, current_event);
                }
                else {
                    print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't load an event inside an another event with XML.");
                }
            }
            else if(current_balise_name != "parent") {__Loading_Part to_add;to_add.content=current_text;to_add.event=event;needed_part.push_back(to_add);}
        }

        // Apply the change
        if(event == std::string()){__load_from_xml_apply(event);}
    }

    // Handle an attribute from XML
    void GUI_Object::set_xml_attribute(std::shared_ptr<__XML_Text_Base> text, std::string event) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "cursor") {
            if(event == "overflighted") {
                // Load the cursor of the object when overflighted
                unsigned long cursor = GLFW_ARROW_CURSOR;
                for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                    std::string current_attribute_name = current_attribute.name;
                    std::string current_attribute_value = current_attribute.value;

                    if(current_attribute_name == "type") {
                        // Value of the cursor
                        if(current_attribute_value == "pointing_hand_cursor" || current_attribute_value == "pointing_hand") { cursor = GLFW_HAND_CURSOR; }
                        else {print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Unknown value of cursor \"" + current_attribute_value + "\".");}
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
            Alignment_Texture texture_alignment = Alignment_Texture::T_User_Defined;
            Alignment_Horizontal texture_alignment_horizontal = Alignment_Horizontal::H_Center; bool texture_alignment_horizontal_used = false;
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
                    if(current_attribute_value == "left") {texture_alignment_horizontal = Alignment_Horizontal::H_Left;texture_alignment_horizontal_used = true;}
                    else if(current_attribute_value == "right"){texture_alignment_horizontal = Alignment_Horizontal::H_Right;texture_alignment_horizontal_used = true;}
                    else if(current_attribute_value == "center"){texture_alignment_horizontal = Alignment_Horizontal::H_Center;texture_alignment_horizontal_used = true;}
                    else{print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't set the value of the texture vertical alignment to \"" + current_attribute_value + "\".");}
                }
                else{print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't find the attribute \"" + current_attribute_value + "\" of the texture.");}
            }
            // Set the goot texture
            if(texture_name != "") set_texture(texture_name);
            set_texture_alignment(texture_alignment);
            if(texture_alignment_horizontal_used){set_texture_alignment_horizontal(texture_alignment_horizontal);}
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

            // Set the final X
            if(attachment_horizontal == 0) {
                // Set the x
                if(x_type == _Size_Definition::Object_Scale_Size) set_x_in_object_scale(x);
                else if(x_type == _Size_Definition::Pixel_Size) set_x_in_pixel(x.to_double());
                else set_x_in_scale(x);
            }
            else {
                // Attach the object
                std::shared_ptr<__GUI_Object_Core> other_object;
                if(attached_object_vertical != ""){
                    other_object = a_loader.get()->created_objects[attached_object_vertical];
                    if(other_object.get() == 0) {
                        print("Warning", "SCLS GUI Object \"" + name() + "\"", "The object \"" + attached_object_vertical + "\" you want to attach at the X axis does not exist.");
                    }
                    else if(!other_object.get()->loaded()){
                        print("Warning", "SCLS GUI Object \"" + name() + "\"", "The object \"" + attached_object_vertical + "\" you want to attach at the X axis is not loaded.");
                    }
                }
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
                    catch(const std::invalid_argument&) {
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

            // Set the final Y
            if(attachment_vertical == 0) {
                // Set the y
                if(y_type == _Size_Definition::Object_Scale_Size) set_y_in_object_scale(y);
                else if(y_type == _Size_Definition::Pixel_Size) set_y_in_pixel(y.to_double());
                else set_y_in_scale(y);
            }
            else {
                // Attach the object
                std::shared_ptr<__GUI_Object_Core> other_object;
                if(attached_object_vertical != "") {
                    other_object = a_loader.get()->created_objects[attached_object_vertical];
                    if(other_object.get() == 0){
                        print("Warning", "SCLS GUI Object \"" + name() + "\"", "The object \"" + attached_object_vertical + "\" you want to attach at the Y axis does not exist.");
                    }
                    else if(!other_object.get()->loaded()){
                        print("Warning", "SCLS GUI Object \"" + name() + "\"", "The object \"" + attached_object_vertical + "\" you want to attach at the Y axis is not loaded.");
                    }
                }
                if(attachment_vertical == 1) {
                    if(other_object.get() == 0) attach_top_in_parent(attachment_vertical_offset);
                }
                else if(attachment_vertical == 2) {
                    if(other_object.get() == 0) attach_bottom_in_parent(attachment_vertical_offset);
                    else attach_bottom_of_object_in_parent(other_object, attachment_vertical_offset);
                }
            }
        }
        else{set_xml_attribute_style(text, current_style());}
    }

    // Set the XML attributs for a style
    void GUI_Object::set_xml_attribute_style(std::shared_ptr<__XML_Text_Base> text, GUI_Style* needed_style) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "background_color") {needed_style->a_background_color = Color::from_xml(text);}
        else if(xml_attribute_name == "border") {border_from_xml(text, needed_style->global_text_style);}
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
            needed_style->global_text_style.set_font_size(final_font_size.to_double());
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
            a_transform_attachment.attachment_horizontal_type = 1;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_horizontal_type == 2) {
            __move_right_in_parent(a_transform_attachment.attachment_horizontal_offset.to_double());
            a_transform_attachment.attachment_horizontal_type = 2;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_horizontal_type == 4) {
            __move_right_of_object_in_parent(a_transform_attachment.attached_object_horizontal, a_transform_attachment.attachment_horizontal_offset.to_double());
            a_transform_attachment.attachment_horizontal_type = 4;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }

        // Check for the attachment vertical
        if(a_transform_attachment.attachment_vertical_type == 1) {
            __move_top_in_parent(a_transform_attachment.attachment_vertical_offset.to_double());
            a_transform_attachment.attachment_vertical_type = 1;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 2) {
            __move_bottom_in_parent(a_transform_attachment.attachment_vertical_offset.to_double());
            a_transform_attachment.attachment_vertical_type = 2;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 3) {
            __move_top_of_object_in_parent(a_transform_attachment.attached_object_vertical.lock().get(), a_transform_attachment.attachment_vertical_offset.to_double());
            a_transform_attachment.attachment_vertical_type = 3;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }
        else if(a_transform_attachment.attachment_vertical_type == 4) {
            __move_bottom_of_object_in_parent(a_transform_attachment.attached_object_vertical.lock().get(), a_transform_attachment.attachment_vertical_offset.to_double());
            a_transform_attachment.attachment_vertical_type = 4;
            _apply_calculate_transformation(transformation_shared_ptr());
            transformation()->calculate_position();
        }

        // Apply to children
        if(with_children) {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i].get() != 0) {a_children[i].get()->calculate_transformation();}}}

        // Call the virtual function
        __GUI_Object_Core::calculate_transformation();
    }

    // Create the new transformation
    void GUI_Object::__create_new_transformation(std::shared_ptr<__GUI_Transformation> last_transformation_to_use) {
        std::shared_ptr<__GUI_Transformation> parent_transformation;
        if(parent() != 0) {parent()->calculate_transformation(false, false);parent_transformation = parent()->transformation_shared_ptr();}
        if(last_transformation_to_use.get() == 0) transformation_shared_ptr() = std::make_shared<__GUI_Transformation>(0, 0, parent_transformation);
        else { transformation_shared_ptr() = std::make_shared<__GUI_Transformation>(*last_transformation_to_use.get()); }
        transformation()->parent_shared_ptr() = parent_transformation;
        transformation()->set_window_height(window_struct().window_height());
        transformation()->set_window_width(window_struct().window_width());
    };

    // Delete the children of an object
    void GUI_Object::delete_children() { children().clear(); }

    // Returns the rect of the fitted texture
    glm::vec4 GUI_Object::fitted_texture_rect() {if(texture()->image_ratio() > (width_in_absolute_scale() / height_in_absolute_scale_and_window())) {return fitted_horizontally_texture_rect();}return fitted_vertically_texture_rect();}

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
        if(texture_alignment_horizontal() == Alignment_Horizontal::H_Center){x_texture = Fraction(1, 2) - width_texture / Fraction(2);}
        else if(texture_alignment_horizontal() == Alignment_Horizontal::H_Right){x_texture = Fraction(1) - width_texture;}
        if(texture_alignment_vertical() == Alignment_Vertical::V_Center){y_texture = Fraction(1, 2) - height_texture /Fraction(2);}
        else if(texture_alignment_vertical() == Alignment_Vertical::V_Top){y_texture = Fraction(1) - height_texture;}
        return glm::vec4(x_texture.to_double(), y_texture.to_double(), width_texture.to_double(), height_texture.to_double());
    }

    //*********
    //
    // GUI Main Object main functions
    //
    //*********

    // Function called after that the window is resized
    void GUI_Main_Object::after_resizing(){if(parent() != 0){calculate_transformation(true, true);}}

    // Update the object for the events
    void GUI_Main_Object::update_event() {
        // Debug
        if(window_struct().debug_mode() & 2){scls::print(std::string("SCLS GUI Page \"") + name() + std::string("\""), std::string("Start updating GUI page events..."));}

        // Check the overflighted cursor
        std::weak_ptr<GUI_Object> current_overflighted_object = a_this_object;
        for(int i = 0;i<static_cast<int>(current_overflighted_object.lock().get()->children().size());i++) {
            GUI_Object* current_object = current_overflighted_object.lock().get()->children()[i].get();
            std::weak_ptr<GUI_Object> current_object_weak_ptr = current_overflighted_object.lock().get()->children()[i];
            if(current_object->visible() && !current_object->ignore_click() && current_object->is_in_rect_in_pixel(window_struct().mouse_x(), window_struct().window_height() - window_struct().mouse_y())) {
                current_overflighted_object = current_object_weak_ptr;
                i = -1;
            }
        }

        // Update the cursor texture
        if(a_overflighted_object.lock().get() != current_overflighted_object.lock().get()) {
            if(current_overflighted_object.lock().get() == 0) {window_struct().set_cursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));}
            else {window_struct().set_cursor(glfwCreateStandardCursor(current_overflighted_object.lock().get()->overflighted_cursor()));}
        }
        a_overflighted_object = current_overflighted_object;
        std::weak_ptr<GUI_Object> current_parent = a_overflighted_object;
        short state = 0;
        while(current_parent.lock().get() != 0) {
            current_parent.lock().get()->set_overflighting_state(state);
            current_parent = current_parent.lock().get()->parent_weak_ptr();
            state++;
        }

        // Check the focused object
        if(window_struct().mouse_button_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(a_overflighted_object.lock().get() != 0) {a_focused_object = a_overflighted_object;}
            else {a_focused_object.reset();}
        }
        current_parent = a_focused_object;
        state = 0;
        while(current_parent.lock().get() != 0) {
            current_parent.lock().get()->set_focusing_state(state);
            current_parent = current_parent.lock().get()->parent_weak_ptr();
            state++;
        }

        GUI_Object::update_event();

        // Debug
        if(window_struct().debug_mode() & 2){scls::print(std::string("SCLS GUI Page \"") + name() + std::string("\""), std::string("Successfully updated GUI page events."));}
    }

    //*********
    //
    // GUI_Scroller main functions
    //
    //*********

    // Function called after the creation of the object
    void GUI_Scroller::after_creation(){clear_scroller_children();}

    // Function called after that the window is resized
    void GUI_Scroller::after_resizing() {check_scroller();}

    // Function called after an XML loading
    void GUI_Scroller::after_xml_loading() {check_scroller();GUI_Object::after_xml_loading();}

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
        if(a_scroller_children.get() == 0){return;}

        // Handle the Y of the scroller
        if(a_scroller_children->height_in_scale() <= 1 || static_cast<int>(height_in_pixel()) - (a_scroller_children->y_in_pixel() + static_cast<int>(a_scroller_children->height_in_pixel())) > 0){
            a_scroller_children->attach_top_in_parent(1);
        }

        // Resize the children
        int border_offset = (border_width_in_pixel()[1] + border_width_in_pixel()[3]);
        if(a_scroller_children.get()->width_in_pixel() != width_in_pixel() - border_offset){
            if(static_cast<int>(width_in_pixel()) - border_offset > 0){a_scroller_children->set_width_in_pixel(width_in_pixel() - border_offset);}
            else{a_scroller_children->set_width_in_pixel(0);}
            a_scroller_children->set_x_in_pixel(border_width_in_pixel()[1]);
        }

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
        set_should_render_during_this_frame(true);
    }

    // Clears the scroller
    void GUI_Scroller::clear_scroller() {if(a_scroller_children.get() != 0){a_scroller_children.get()->delete_children();}check_scroller(false);};
    void GUI_Scroller::clear_scroller_children(){delete_child(a_scroller_children.get());a_scroller_children.reset();a_scroller_children=*_create_scroller_children();};

    // Private function to create the children scroller
    std::shared_ptr<GUI_Object>* GUI_Scroller::_create_scroller_children() {
        std::shared_ptr<GUI_Object>* to_return = GUI_Object::new_object<GUI_Object>(name() + "_children_scroller");
        to_return->get()->set_height_in_scale(Fraction(1));
        to_return->get()->set_width_in_scale(Fraction(1));
        to_return->get()->move_left_in_parent(1);
        to_return->get()->move_top_in_parent(1);
        a_created_objects_parent = *to_return;
        return to_return;
    }

    // Scroll the scroller
    void GUI_Scroller::scroll_y(Fraction movement) {
        if(scroller_vertical_alignment() == Alignment_Vertical::V_Top || true) {
            if(a_scroller_children->height_in_scale() > 1) {
                movement *= Fraction(15);
                Fraction final_pos = (a_scroller_children->y_in_pixel()) - movement;
                a_scroller_children->set_y_in_pixel(final_pos);
                check_scroller();
            }
            else {a_scroller_children->move_top_in_parent(1);}
        }
        set_should_render_during_this_frame(true);
    }

    //*********
    //
    // GUI_Scroller_Choice main functions
    //
    //*********

    // Most basic GUI_Scroller_Choice constructor
    GUI_Scroller_Choice::GUI_Scroller_Choice(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent) : GUI_Scroller(window, name, parent) {a_unselected_objects_style.cursor=GLFW_HAND_CURSOR;}

    // GUI_Scroller_Choice destructor
    GUI_Scroller_Choice::~GUI_Scroller_Choice() {}

    // Loads the choice from an XML test
    void GUI_Scroller_Choice::load_choices_from_xml(std::shared_ptr<__XML_Text_Base> text) {}

    // Loads the object from XML
    void GUI_Scroller_Choice::__load_from_xml_apply(std::string event) {
        // Apply the changes
        for(int i = 0;i<static_cast<int>(a_loading_parts.size());i++){
            if(a_loading_parts[i].event == std::string() && a_loading_parts[i].content.get()->xml_balise_name() != std::string("sub_choice") && a_loading_parts[i].content.get()->xml_balise_name() != std::string("choice")){
                set_xml_attribute(a_loading_parts[i].content, a_loading_parts[i].event);a_loading_parts.erase(a_loading_parts.begin() + i);i--;
            }
        }
        clear_scroller_children();
        for(int i = 0;i<static_cast<int>(a_loading_parts.size());i++){set_xml_attribute(a_loading_parts[i].content, a_loading_parts[i].event);a_loading_parts.erase(a_loading_parts.begin() + i);i--;}
        place_objects();
    }

    // Place the objects
    void GUI_Scroller_Choice::place_objects() {
        // Place the objects
        std::shared_ptr<scls::GUI_Object> last_object;
        if(a_displayed) {
            for(int i = 0;i<static_cast<int>(a_objects.size());i++) {
                GUI_Scroller_Single_Choice current_object = a_objects[(static_cast<int>(a_objects.size()) - 1) - i];
                if(last_object.get() != 0) {current_object.object()->attach_top_of_object_in_parent(last_object);}
                else {current_object.object()->attach_bottom_in_parent();}
                current_object.object()->set_visible(true);
                last_object = current_object.object_shared_ptr();
            }
        }

        // Displayer object
        if(a_displayer_object.get() != 0){
            if(last_object.get() != 0) {a_displayer_object.get()->attach_top_of_object_in_parent(last_object);}
            else{a_displayer_object.get()->attach_bottom_in_parent();}
        }

        // Check the scroller
        if(scroller_children() != 0){scroller_children()->set_height_in_pixel(needed_height());}
        GUI_Scroller::check_scroller(false);
    }

    // Select an object
    void GUI_Scroller_Choice::select_object(GUI_Scroller_Single_Choice needed_object) {
        // Get the good GUI object
        GUI_Object* object = 0;
        if(needed_object.is_sub_section()){object = needed_object.sub_section()->a_displayer_object.get();}
        else{object = needed_object.object();}

        if(object != 0) {
            // Add the contained object
            a_choice_modified = true;
            if(contains_selected_object(needed_object.name())) {
                // Confirm the object
                if(a_unselect_object_on_confirmation){unselect_object(needed_object);}
                a_currently_confirmed_objects.push_back(needed_object.name());
            }
            else {
                needed_object.__choice.get()->good = true;
                a_currently_selected_objects.push_back(needed_object);
                a_currently_selected_objects_during_this_frame.push_back(needed_object);
                a_selection_modified = true;

                // Set the style for the object
                object->merge_style(&selected_objects_style());

                // Remove the too much objects
                while(static_cast<int>(a_currently_selected_objects.size()) > max_number_selected_object()) {unselect_object(a_currently_selected_objects[0]);}
            }
        }
    };

    // Handles styles
    void GUI_Scroller_Choice::set_selected_objects_style_color(Color new_color){a_selected_objects_style.a_background_color = new_color;};

    // Unselect an object
    void GUI_Scroller_Choice::unselect_object(std::string object_name) {
        for(int i = 0;i<static_cast<int>(currently_selected_objects().size());i++) {
            if(currently_selected_objects()[i].name()==object_name) {
                // Get the good GUI object
                GUI_Object* object = 0;
                if(currently_selected_objects().at(i).is_sub_section()){object = currently_selected_objects().at(i).sub_section()->a_displayer_object.get();}
                else{object = currently_selected_objects().at(i).object();}

                currently_selected_objects()[i].__choice.get()->good = false;
                currently_selected_objects().erase(currently_selected_objects().begin()+i,currently_selected_objects().begin()+i+1);

                // Set the style for the object
                if(object != 0){object->merge_style(&unselected_objects_style());}
                return;
            }
        }
    }

    // Update the even in the scroller
    void GUI_Scroller_Choice::update_event() {
        GUI_Scroller::update_event();

        // Check each buttons
        for(int i = 0;i<static_cast<int>(a_objects.size());i++) {
            if(a_objects[i].is_sub_section()) {
                // Check each sub-choices
                if(a_objects.at(i).sub_section()->a_displayer_object.get() && a_objects.at(i).sub_section()->a_displayer_object.get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)){
                    select_object(a_objects.at(i));
                }
                else {
                    for(int j = 0;j<static_cast<int>(a_objects[i].sub_section()->currently_selected_objects_during_this_frame().size());j++) {
                        if(a_objects[i].__choice.get()->good){select_object(a_objects[i].sub_section()->currently_selected_objects_during_this_frame()[j]);}
                    }
                }
            }
            else {if(a_objects[i].object()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {select_object(a_objects[i]);}}
        }
        check_number_selected_object();

        // Check the displayer
        if(a_displayer_object.get() != 0) {
            if(a_displayer_object.get()->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                if(a_displayed) {hide_objects();} else {show_objects();}
            }
        }
    }

    // Handle an attribute from XML
    void GUI_Scroller_Choice::set_xml_attribute(std::shared_ptr<__XML_Text_Base> text, std::string event) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "choice") {
            // Get each attributes
            std::string needed_name = std::string("");
            std::string needed_text = text.get()->text();
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                // Load the font size of the text
                if(current_attribute_name == "name") {
                    // Value of the font size
                    needed_name = current_attribute_value;
                    if(needed_name.size() > 0 && needed_name[0] == '\"'){needed_name = needed_name.substr(1, needed_name.size() - 1);}
                    if(needed_name.size() > 0 && needed_name[needed_name.size() - 1] == '\"'){needed_name = needed_name.substr(0, needed_name.size() - 1);}
                }
            }
            // Get the content of the choice
            add_object(needed_name, needed_text);
        }
        else if(xml_attribute_name == "sub_choice" || xml_attribute_name == "sub_section") {
            // Get each attributes
            std::string needed_displayer = std::string("");
            std::string needed_name = std::string("");
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;

                // Load the font size of the text
                if(current_attribute_name == "displayer") {
                    // Value of the font size
                    needed_displayer = current_attribute_value;
                    if(needed_displayer.size() > 0 && needed_displayer[0] == '\"'){needed_displayer = needed_displayer.substr(1, needed_displayer.size() - 1);}
                    if(needed_displayer.size() > 0 && needed_displayer[needed_displayer.size() - 1] == '\"'){needed_displayer = needed_displayer.substr(0, needed_displayer.size() - 1);}
                }
                else if(current_attribute_name == "name") {
                    // Value of the font size
                    needed_name = current_attribute_value;
                    if(needed_name.size() > 0 && needed_name[0] == '\"'){needed_name = needed_name.substr(1, needed_name.size() - 1);}
                    if(needed_name.size() > 0 && needed_name[needed_name.size() - 1] == '\"'){needed_name = needed_name.substr(0, needed_name.size() - 1);}
                }
            }

            // Create the sub section
            std::shared_ptr<scls::GUI_Scroller_Choice> current_section = *add_sub_section(needed_name, needed_displayer);
            for(int i = 0;i<static_cast<int>(text.get()->sub_texts().size());i++) {
                // Loads the sub-choices
                current_section.get()->set_xml_attribute(text.get()->sub_texts()[i], event);
            }
            current_section.get()->check_objects();
        }
        else if(event == "selected"){GUI_Object::set_xml_attribute_style(text, &selected_objects_style());}
        else if(event == "unselected"){GUI_Object::set_xml_attribute_style(text, &unselected_objects_style());}
        else {GUI_Object::set_xml_attribute(text, event);}
    }

    //*********
    //
    // __GUI_Text_Metadatas main functions
    //
    //*********

    // Add a text to the input at the cursor position returns if the text has correctly been added
    bool __GUI_Text_Metadatas::add_text(std::string text_to_add) {
        // Prepare the needed datas to add (the text is treated as unformatted here)
        if(text_to_add != "") {
            if(attached_text_image() == 0) {set_text(text_to_add);set_cursor_position_in_formatted_text(plain_text_size());}
            else {
                // Add the lines if needed
                unsigned int cursor_position = attached_text_image()->cursor_position_in_full_text();
                int new_line = count_string(text_to_add, "</br>");
                for(int i = 0;i<new_line;i++) {
                    if((attached_text_image()->line_number_at_position(cursor_position) - line_offset()) < static_cast<int>(children().size())) {
                        children().insert(children().begin() + (attached_text_image()->line_number_at_position(cursor_position) - line_offset()) + 1, 0);
                    }
                    else {children().push_back(0);}
                }

                // Update the text
                reset();
                attached_text_image()->add_text(text_to_add);
                update_text_texture();
                a_text_modified_during_this_frame = true;
            }
            return true;
        }
        return false;
    };

    // Function called after that the window is resized
    void __GUI_Text_Metadatas::after_resizing(){GUI_Object::after_resizing();if(max_width() != -1) {set_max_width(width_in_pixel());update_text_texture(scls::Image_Generation_Type::IGT_Size);}}

    // Creates a text block from a block of text
    std::shared_ptr<__GUI_Text_Metadatas::__GUI_Text_Block> __GUI_Text_Metadatas::__create_text_block_object(Text_Image_Block* block_to_apply) {
        std::shared_ptr<__GUI_Text_Metadatas::__GUI_Text_Block> temp = __create_text_block_object();
        temp.get()->object()->set_ignore_click(true);
        return temp;
    }
    std::shared_ptr<__GUI_Text_Metadatas::__GUI_Text_Block> __GUI_Text_Metadatas::__create_text_block_object(Text_Image_Line* line_to_apply) {
        std::shared_ptr<__GUI_Text_Metadatas::__GUI_Text_Block> temp = __create_text_block_object();
        temp.get()->object()->set_ignore_click(true);
        return temp;
    }
    int __generation = 0;std::string __GUI_Text_Metadatas::__create_text_block_object_name(){return name() + std::string("_gen_") + std::to_string(__generation++);};

    // Updates the texture of the block
    void __GUI_Text_Metadatas::__GUI_Text_Block::update_texture(Text_Image_Block* block_to_apply, scls::Image_Generation_Type generation_type) {
        // Generate the image
        std::shared_ptr<__Image_Base> image_to_apply = block_to_apply->image_shared_pointer(generation_type);

        // Update the object
        if(image_to_apply.get() != 0) {
            object()->set_height_in_pixel(image_to_apply.get()->height());
            object()->set_width_in_pixel(image_to_apply.get()->width());
            object()->texture()->set_image(image_to_apply);
        }
    }
    void __GUI_Text_Metadatas::__GUI_Text_Block::update_texture(Text_Image_Line* line_to_apply, scls::Image_Generation_Type generation_type) {
        // Generate the image
        std::shared_ptr<__Image_Base> image_to_apply = line_to_apply->image_shared_ptr(generation_type);

        // Update the object
        if(image_to_apply.get() != 0) {
            object()->set_height_in_pixel(image_to_apply.get()->height());
            object()->set_width_in_pixel(image_to_apply.get()->width());
            object()->texture()->set_image(image_to_apply);
        }
    }

    // Generates a text block from a block of text
    void __GUI_Text_Metadatas::__generate_text_block_object(Text_Image_Line* line_to_apply, scls::Image_Generation_Type generation_type, unsigned int& total_height) {
        // Generate the object for the line
        std::shared_ptr<GUI_Text::__GUI_Text_Block> new_block = __create_text_block_object(line_to_apply);
        new_block.get()->set_style(line_to_apply->global_style());
        new_block.get()->update_texture(line_to_apply, generation_type);

        // Place the children
        a_blocks_children.push_back(new_block);
        total_height += new_block.get()->height();
    }
    void __GUI_Text_Metadatas::__generate_text_block_object(Text_Image_Block* block_to_apply, scls::Image_Generation_Type generation_type, unsigned int& total_height) {
        // Generate the object for the line
        std::shared_ptr<GUI_Text::__GUI_Text_Block> new_block = __create_text_block_object(block_to_apply);
        new_block.get()->set_style(block_to_apply->global_style());
        new_block.get()->update_texture(block_to_apply, generation_type);

        // Place the children
        a_blocks_children.push_back(new_block);
        total_height += new_block.get()->height();
    }

    // Merges the current object with a specific style
    void __GUI_Text_Metadatas::merge_style(GUI_Style* new_style) {global_style().merge_style(new_style->global_text_style);GUI_Object::merge_style(new_style);update_text_image_block();};

    // Move the cursor in the text
    void __GUI_Text_Metadatas::move_cursor(int movement) {
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
    void __GUI_Text_Metadatas::move_cursor_at_position_in_scale(glm::vec2 position) {
        if(a_blocks_children.size() <= 0) return;

        GUI_Object* current_object = a_blocks_children.at(0).get()->object(); unsigned int i = 0;
        while(current_object->y_in_scale() > position[1] && i < a_blocks_children.size()) {i++;if(i < a_blocks_children.size()) {current_object = a_blocks_children.at(i).get()->object();}}
        if(i >= a_blocks_children.size()) {
            i = a_blocks_children.size() - 1;
            current_object = a_blocks_children.at(a_blocks_children.size() - 1).get()->object();
        }

        move_cursor(attached_text_image()->cursor_position_in_plain_text_at_line_and_x(i + line_offset(), position[0] * width_in_pixel()) - cursor_position_in_formatted_text());
    }

    // Moves the cursor in the Y axis
    void __GUI_Text_Metadatas::move_cursor_y(int movement) {
        if(attached_text_image() == 0) return;

        Text_Image_Line* cursor_line = attached_text_image()->cursor_line();
        if(cursor_line != 0) {
            int cursor_number = (cursor_line->datas().line_number) + movement;
            if(cursor_number < 0) {move_cursor(-cursor_position_in_formatted_text());}
            else if(cursor_number >= static_cast<int>(attached_text_image()->lines().size())) {move_cursor(plain_text_size() - cursor_position_in_formatted_text());}
            else {move_cursor(attached_text_image()->cursor_position_in_plain_text_at_line_and_x(cursor_number, cursor_line->cursor_x()) - cursor_position_in_formatted_text());}
        }
    }

    // Places the blocks in the text
    void __GUI_Text_Metadatas::place_blocks() {
        // Place each children
        unsigned int total_height = 0;
        for(int i = 0;i<static_cast<int>(a_blocks_children.size());i++) {
            if(a_blocks_children[i].get() != 0) {
                // Place the X
                if(a_blocks_children[i].get()->style().alignment_horizontal() == scls::Alignment_Horizontal::H_Left){a_blocks_children[i].get()->object()->set_x_in_pixel(0);}
                else if(a_blocks_children[i].get()->style().alignment_horizontal() == scls::Alignment_Horizontal::H_Right){
                    a_blocks_children[i].get()->object()->set_x_in_pixel(width_in_pixel() - a_blocks_children[i].get()->object()->width_in_pixel());
                }
                else{a_blocks_children[i].get()->object()->set_x_in_object_scale(scls::Fraction(1, 2));}

                // Place the Y
                if(texture_alignment_vertical() == scls::Alignment_Vertical::V_Top){
                    int needed_y = height_in_pixel() - ((a_blocks_children[i].get()->height() + total_height) - a_offset_y);
                    a_blocks_children[i].get()->object()->set_y_in_pixel(needed_y + a_blocks_children[i].get()->style().margin_top());
                }
                else{a_blocks_children[i].get()->object()->set_y_in_object_scale(scls::Fraction(1, 2));}
                total_height += a_blocks_children[i].get()->height();
            }
        }

        // Update the view
        set_should_render_during_this_frame(true);
    }

    // Return the plain text in the object
    std::string __GUI_Text_Metadatas::plain_text(){return window_struct().text_image_generator()->plain_text(text());};
    unsigned int __GUI_Text_Metadatas::plain_text_size() {return plain_text().size();};

    // Remove a text to the input at the cursor position
    void __GUI_Text_Metadatas::remove_text(unsigned int size_to_delete) {
        if(size_to_delete == 0){return;}

        // Preparate the needed datas
        // Remove the text
        unsigned int line_to_delete = attached_text_image()->line_number_at_position(cursor_position_in_unformatted_text());
        unsigned int removed_lines = attached_text_image()->remove_text(size_to_delete);
        // Remove the needed children
        int final_size = static_cast<int>(children().size()) + line_offset();
        if(removed_lines > 0) {for(int i = line_to_delete;i<final_size;i++) { children().erase(children().begin() + line_to_delete);}}
        update_text_texture();
    }

    // Sets the new position of the cursor
    void __GUI_Text_Metadatas::set_cursor_position_in_formatted_text(unsigned int new_cursor_position_in_formatted_text) {
        if(new_cursor_position_in_formatted_text == cursor_position_in_formatted_text()) return;

        if(attached_text_image() != 0) {
            Text_Image_Line* line_to_modify = attached_text_image()->line_at_position_in_plain_text(cursor_position_in_formatted_text());
            if(line_to_modify != 0) {
                line_to_modify->set_modified(true);
            }
        }
        if(attached_text_image() != 0) {
            Text_Image_Line* line_to_modify = attached_text_image()->line_at_position_in_plain_text(new_cursor_position_in_formatted_text);
            if(line_to_modify != 0) {
                line_to_modify->set_modified(true);
            }
        }

        // Set the position
        attached_text_image()->set_cursor_position_in_plain_text(new_cursor_position_in_formatted_text);
    }

    // Sets the text from plain text
    void __GUI_Text_Metadatas::set_plain_text(std::string new_text) {if(new_text == plain_text()){return;}new_text=scls::format_string_break_line(scls::format_string_from_plain_text(new_text), std::string("</br>"));update_text_image_block_style();attached_text_image_block()->set_text(new_text);update_texture();};

    // Handle an attribute from XML
    void __GUI_Text_Metadatas::set_xml_attribute(std::shared_ptr<__XML_Text_Base> text, std::string event) {
        std::string xml_attribute_name = text.get()->xml_balise_name();
        if(xml_attribute_name == "content") {
            // Get each attributes
            std::string needed_text = text.get()->text();
            std::string needed_text_src = std::string("");
            for(int j = 0;j<static_cast<int>(text.get()->xml_balise_attributes().size());j++) {
                XML_Attribute& current_attribute = text.get()->xml_balise_attributes()[j];
                std::string current_attribute_name = current_attribute.name;
                std::string current_attribute_value = current_attribute.value;
                if(current_attribute_value[0] == '\"'){current_attribute_value = current_attribute_value.substr(1, current_attribute_value.size() - 1);}
                if(current_attribute_value[current_attribute_value.size()-1] == '\"'){current_attribute_value = current_attribute_value.substr(0, current_attribute_value.size() - 1);}

                // Load the font size of the text
                if(current_attribute_name == "alignment_horizontal" || current_attribute_name == "horizontal_alignment") {
                    // Loads the content of a balise from a file
                    if(current_attribute_value == std::string("center")){set_text_alignment_horizontal(Alignment_Horizontal::H_Center);}
                    else {print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The text horizontal alignment \"" + current_attribute_value + "\" is not a valid attribute.");}
                }
                else if(current_attribute_name == "font_size") {set_font_size(Fraction::from_std_string(current_attribute_value).to_double());}
                else if(current_attribute_name == "max_width") {set_max_width(Fraction::from_std_string(current_attribute_value).to_double());}
                else if(current_attribute_name == "src") {
                    // Loads the content of a balise from a file
                    if(current_attribute_value[0] == '\"'){current_attribute_value = current_attribute_value.substr(1, current_attribute_value.size() - 1);}
                    if(current_attribute_value[current_attribute_value.size()-1] == '\"'){current_attribute_value = current_attribute_value.substr(0, current_attribute_value.size() - 1);}
                    std::string base_attribute_value = current_attribute_value;
                    if(!std::filesystem::exists(current_attribute_value)){current_attribute_value = path_parent(a_loader.get()->path) + "/" + current_attribute_value;}
                    if(std::filesystem::exists(current_attribute_value)) {
                        needed_text = read_file(current_attribute_value);
                        needed_text_src = current_attribute_value;
                    }
                    else {print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The path \"" + base_attribute_value + "\" you want to load as the content of this text does not exist.");}
                }
            }
            // Get the content of the text
            needed_text = scls::format_string_tabulations(scls::format_string_break_line(needed_text, " "), "");
            if(max_width() != -1) {set_max_width(width_in_pixel());}
            std::shared_ptr<__XML_Text_Base> final_text = xml(window_struct().balises_shared_ptr(), needed_text);
            final_text.get()->check_include(needed_text_src);
            set_xml_text(final_text);
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
        else {GUI_Object::set_xml_attribute(text, event);}
    }

    // Returns the word clicked at a certain position in the text
    std::shared_ptr<__XML_Text_Base> __GUI_Text_Metadatas::text_clicked_at_position(int x, int y) {
        if(a_blocks_children.size() <= 0){return std::shared_ptr<__XML_Text_Base>();}

        // Get the good children
        __GUI_Text_Block* current_object = a_blocks_children[0].get(); unsigned int i = 0;
        while(current_object->object()->y_in_pixel() > y && i < a_blocks_children.size()) {
            i++;
            if(i < children().size()) {current_object = a_blocks_children[i].get();}
        }
        // If the children needed is the last children
        if(i >= a_blocks_children.size()) {
            i = a_blocks_children.size() - 1;
            current_object = a_blocks_children[i].get();
        }
        y -= current_object->object()->y_in_pixel();

        // Get the needed block
        int needed_height = current_object->object()->texture()->get_image()->height();
        Text_Image_Block* needed_block = attached_text_image_block()->blocks()[i].get(); int needed_y = 0;
        Text_Image_Line* needed_line = needed_block->line_at_position_in_pixel(x, needed_height - y, needed_y);
        y = (needed_height - y) - needed_y;
        Text_Image_Word* needed_word = needed_line->word_at_position_in_pixel(x, y).get();
        if(needed_word == 0){return std::shared_ptr<__XML_Text_Base>();}
        return needed_word->datas().balise_parent();
    }

    // Updates text image block
    void __GUI_Text_Metadatas::update_text_image_block(){String temp=text();attached_text_image_block()->free_memory();set_text(temp);};
    void __GUI_Text_Metadatas::update_text_image_block_style(){attached_text_image_block()->global_style().merge_style(a_global_style);};

    // Updates the event
    void __GUI_Text_Metadatas::update_event(){
        GUI_Object::update_event();

        // Check the scroller
        if(is_focused() || has_child_focused()) {
            int wheel_move=window_struct().wheel_movement_y_during_this_frame()*4;
            if(wheel_move!=0){if(wheel_move - a_offset_y < 0){a_offset_y -= wheel_move;}else{a_offset_y=0;} place_blocks();}
        }
    }

    // Update the texture of the text
    void __GUI_Text_Metadatas::update_text_texture(scls::Image_Generation_Type generation_type) {
        if(text() != "") {
            // Update the text
            update_text_image_block_style();

            // Add lines if needed
            int block_offset = 0;
            delete_children();
            a_blocks_children.clear();

            int cursor_pos = 0;if(attached_text_image() != 0){cursor_pos = attached_text_image()->cursor_position_in_plain_text();}
            unsigned int total_height = 0;int total_lines = 0;attached_text_image_block()->generate_blocks();
            if(attached_text_image() != 0){attached_text_image()->set_cursor_position_in_plain_text(cursor_pos);}
            for(int i = 0;i + block_offset<static_cast<int>(attached_text_image_block()->blocks().size());i++) {
                attached_text_image_block()->generate_next_block(i);attached_text_image_block()->blocks().at(i).get()->generate_lines();
                for(int j = 0;j<static_cast<int>(attached_text_image_block()->blocks().at(i).get()->lines().size());j++) {
                    // Create the configuration for the line
                    Text_Image_Line* line_to_apply = attached_text_image_block()->blocks().at(i).get()->lines().at(j);
                    if(line_to_apply != 0) {__generate_text_block_object(line_to_apply, generation_type, total_height);}
                    total_lines++;
                }
            }

            // Delete the useless children in more
            for(int i = 0;i<static_cast<int>(children().size()) - total_lines;i++) {children().pop_back();}

            // Delete empty children
            for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i].get() == 0) {children().erase(children().begin() + i);i--;}}

            place_blocks();
            a_generation++;
        }
        else {delete_children();texture()->set_image(0);}
        set_should_render_during_this_frame(true);
    }

    //*********
    //
    // GUI_Text_Input_Base main functions
    //
    //*********

    // Format a letter
    std::string __format_one_letter(std::string letter, bool apply_alt, bool apply_capitalisation, bool apply_control, std::string& last_descriptive_character) {
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
        if(apply_control){letter = std::string();}

        // Set the last descriptive character
        if(last_descriptive_character == ""){
            if(letter == "^" || letter == "¨") {
                last_descriptive_character = letter;
                letter = "";
            }
        }
        else {
            if(last_descriptive_character == "^") {
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
                    if(letter == " ") letter = last_descriptive_character;
                    else letter = last_descriptive_character + letter;
                }
            }
            else if(last_descriptive_character == "¨") {
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
                    if(letter == " ") letter = last_descriptive_character;
                    else letter = last_descriptive_character + letter;
                }
            }
            last_descriptive_character = "";
        }

        return letter;
    }
    std::string __format(std::string letter, bool apply_alt, bool apply_capitalisation, bool apply_control, std::string& last_descriptive_character) {
        std::string result = "";
        std::string to_analyse = "";

        for(int i = 0;i<static_cast<int>(letter.size());i++) {
            to_analyse += letter[i];
            if(!is_character_utf_8(letter[i])) {
                result += __format_one_letter(to_analyse, apply_alt, apply_capitalisation, apply_control, last_descriptive_character);
                to_analyse = "";
            }
        }
        // Last analyse
        if(to_analyse != "") result += __format_one_letter(to_analyse, apply_alt, apply_capitalisation, apply_control, last_descriptive_character);

        return result;
    }

    // Returns the inputed text
    std::string __input_text(scls::_Window_Advanced_Struct& window_struct, std::string final_text, int cursor_position_in_unformatted_text, std::string& last_descriptive_character, int& to_remove) {
        std::string to_add = "";

        // Handle letters
        bool should_control = (window_struct.key_state("left control") == scls::Key_State::Pressed || window_struct.key_state("right control") == scls::Key_State::Pressed);
        bool should_alt = (window_struct.key_state("alt gr") == scls::Key_State::Pressed);
        bool should_capitalise = (window_struct.key_state("left shift") == scls::Key_State::Pressed || window_struct.key_state("right shift") == scls::Key_State::Pressed);
        if(window_struct.key_pressed_or_repeated_pressed("a")) { to_add += "a";  }
        if(window_struct.key_pressed_or_repeated_pressed("b")) { to_add += "b";  }
        if(window_struct.key_pressed_or_repeated_pressed("c")) { to_add += "c";  }
        if(window_struct.key_pressed_or_repeated_pressed("d")) { to_add += "d";  }
        if(window_struct.key_pressed_or_repeated_pressed("e")) { to_add += "e";  }
        if(window_struct.key_pressed_or_repeated_pressed("f")) { to_add += "f";  }
        if(window_struct.key_pressed_or_repeated_pressed("g")) { to_add += "g";  }
        if(window_struct.key_pressed_or_repeated_pressed("h")) { to_add += "h";  }
        if(window_struct.key_pressed_or_repeated_pressed("i")) { to_add += "i";  }
        if(window_struct.key_pressed_or_repeated_pressed("j")) { to_add += "j";  }
        if(window_struct.key_pressed_or_repeated_pressed("k")) { to_add += "k";  }
        if(window_struct.key_pressed_or_repeated_pressed("l")) { to_add += "l";  }
        if(window_struct.key_pressed_or_repeated_pressed("m")) { to_add += "m";  }
        if(window_struct.key_pressed_or_repeated_pressed("n")) { to_add += "n";  }
        if(window_struct.key_pressed_or_repeated_pressed("o")) { to_add += "o";  }
        if(window_struct.key_pressed_or_repeated_pressed("p")) { to_add += "p";  }
        if(window_struct.key_pressed_or_repeated_pressed("q")) { to_add += "q";  }
        if(window_struct.key_pressed_or_repeated_pressed("r")) { to_add += "r";  }
        if(window_struct.key_pressed_or_repeated_pressed("s")) { to_add += "s";  }
        if(window_struct.key_pressed_or_repeated_pressed("t")) { to_add += "t";  }
        if(window_struct.key_pressed_or_repeated_pressed("u")) { to_add += "u";  }
        if(window_struct.key_pressed_or_repeated_pressed("v")) {
            if(should_control) {
                to_add += clipboard_datas();
            }
            else {
                to_add += "v";
            }
        }
        if(window_struct.key_pressed_or_repeated_pressed("w")) { to_add += "w";  }
        if(window_struct.key_pressed_or_repeated_pressed("x")) { to_add += "x";  }
        if(window_struct.key_pressed_or_repeated_pressed("y")) { to_add += "y";  }
        if(window_struct.key_pressed_or_repeated_pressed("z")) { to_add += "z";  }
        // Extended alphabet letter
        if(window_struct.key_pressed_or_repeated_pressed("ù")) { to_add += "ù";  }

        // Handle numbers
        if(window_struct.key_pressed_or_repeated_pressed("0")) { to_add += "0";  }
        if(window_struct.key_pressed_or_repeated_pressed("1")) { to_add += "1";  }
        if(window_struct.key_pressed_or_repeated_pressed("2")) { to_add += "2";  }
        if(window_struct.key_pressed_or_repeated_pressed("3")) { to_add += "3";  }
        if(window_struct.key_pressed_or_repeated_pressed("4")) { to_add += "4";  }
        if(window_struct.key_pressed_or_repeated_pressed("5")) { to_add += "5";  }
        if(window_struct.key_pressed_or_repeated_pressed("6")) { to_add += "6";  }
        if(window_struct.key_pressed_or_repeated_pressed("7")) { to_add += "7";  }
        if(window_struct.key_pressed_or_repeated_pressed("8")) { to_add += "8";  }
        if(window_struct.key_pressed_or_repeated_pressed("9")) { to_add += "9";  }
        if(window_struct.key_pressed_or_repeated_pressed("=")) { to_add += "=";  }
        if(window_struct.key_pressed_or_repeated_pressed("+")) { to_add += "+";  }
        if(window_struct.key_pressed_or_repeated_pressed("/")) { to_add += "/";  }
        if(window_struct.key_pressed_or_repeated_pressed(".")) { to_add += ".";  }

        // Handle numbers / special characters
        if(window_struct.key_pressed_or_repeated_pressed("&")) { to_add += "&";  }
        if(window_struct.key_pressed_or_repeated_pressed("é")) { to_add += "é";  }
        if(window_struct.key_pressed_or_repeated_pressed("\"")) { to_add += "\"";  }
        if(window_struct.key_pressed_or_repeated_pressed("'")) { to_add += "'";  }
        if(window_struct.key_pressed_or_repeated_pressed("(")) { to_add += "(";  }
        if(window_struct.key_pressed_or_repeated_pressed("-")) { to_add += "-";  }
        if(window_struct.key_pressed_or_repeated_pressed("è")) { to_add += "è";  }
        if(window_struct.key_pressed_or_repeated_pressed("_")) { to_add += "_";  }
        if(window_struct.key_pressed_or_repeated_pressed("ç")) { to_add += "ç";  }
        if(window_struct.key_pressed_or_repeated_pressed("à")) { to_add += "à";  }
        if(window_struct.key_pressed_or_repeated_pressed("^")) { to_add += "^";  }
        if(window_struct.key_pressed_or_repeated_pressed("*")) { to_add += "*";  }

        // Handle ponctuation
        if(window_struct.key_pressed_or_repeated_pressed(":")) { to_add += ":";  }
        if(window_struct.key_pressed_or_repeated_pressed(";")) { to_add += ";";  }
        if(window_struct.key_pressed_or_repeated_pressed(",")) { to_add += ",";  }
        if(window_struct.key_pressed_or_repeated_pressed("!")) { to_add += "!";  }
        if(window_struct.key_pressed_or_repeated_pressed("<")) { to_add += "<";  }

        // Handle special characters
        if(window_struct.key_pressed_or_repeated_pressed("backspace") == scls::Key_State::Pressed && final_text.size() > 0 && cursor_position_in_unformatted_text > 0) {to_remove=1;}
        if(window_struct.key_pressed_or_repeated_pressed("space")) { to_add += " ";  }
        if(window_struct.key_pressed_or_repeated_pressed("tab")) { to_add += "    ";  }
        if(window_struct.key_pressed_or_repeated_pressed(")")) { to_add += ")";  }
        if(window_struct.key_pressed_or_repeated_pressed("$")) { to_add += "$";  }
        to_add = __format(to_add, should_alt, should_capitalise, should_control, last_descriptive_character);
        to_add = format_string_from_plain_text(to_add);

        if(window_struct.key_pressed_or_repeated_pressed("-k")) { to_add += "-";  }
        if(window_struct.key_pressed_or_repeated_pressed("*k")) { to_add += "*";  }
        if(window_struct.key_pressed_or_repeated_pressed("enter") == scls::Key_State::Pressed) { to_add += "</br>";  }

        return to_add;
    }

    //*********
    //
    // GUI_File_Explorer main function
    //
    //*********

    // Most parent GUI_File_Explorer constructor used for displaying
    GUI_File_Explorer::GUI_File_Explorer(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent) : GUI_Object(window, name, parent) {}

    // Function called after that the window is resized
    void GUI_File_Explorer::after_resizing() {place_all();}

    // Returns if a file is chosen during this frame
    bool GUI_File_Explorer::file_chosen() {return choose_clicked() || a_file_chose;}

    // Load the explorer
    void GUI_File_Explorer::load() {
        // Browser of the file explorer
        a_browser = *new_object<GUI_Scroller_Choice>("browser");
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
        a_choose_button->attach_bottom_in_parent();
        a_choose_button->attach_left_in_parent();
        a_final_path->set_height_in_scale(Fraction(1, 10));
        a_final_path->set_width_in_scale(Fraction(4, 5));
        a_final_path->move_bottom_in_parent();
        a_final_path->move_right_in_parent();
        a_browser->move_right_in_parent();
        a_browser->move_top_of_object_in_parent(a_final_path);
        a_top_bar->set_height_in_pixel(30);
        a_top_bar->set_x_in_scale(Fraction(0));
        a_top_bar->move_top_in_parent();

        place_top_bar_buttons();

        set_should_render_during_this_frame(true);
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
            else {a_file_chose = true;}
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
        std::vector<std::string> paths = directory_content(to_utf_8(a_current_path));
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        if(a_browser_buttons_to_modify.size() == 0) {
            // Create the buttons from scratch
            a_browser->reset();
            a_browser_buttons.clear();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || !is_file_authorized(paths[i])) continue;

                // Create the button
                paths[i] = file_name(paths[i], true);
                std::shared_ptr<GUI_Text>* result = a_browser.get()->add_object(paths[i], paths[i]);
                if(result != 0) {a_browser_buttons.push_back(*result);result->get()->set_height_in_pixel(20);result->get()->set_width_in_scale(1);}
            }

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
        a_browser.get()->check_objects();
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
            if(i > 0) {path += a_top_bar_buttons[i]->plain_text();}
            if(a_top_bar_buttons[i]->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
                a_current_path = "";
                set_path(path);
                break;
            }
        }
    }

    // Update the size of the file explorer
    void GUI_File_Explorer::update_GUI_scale() {place_all();}

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

        a_parent_object = std::make_shared<GUI_Main_Object>(*window_struct, "main_" + name);
        a_parent_object.get()->set_position_in_pixel(0, 0);
        a_parent_object.get()->set_height_in_scale(scls::Fraction(1));
        a_parent_object.get()->set_this_object(a_parent_object);
        a_parent_object.get()->set_width_in_scale(scls::Fraction(1));
    }

    // Render the page
    void GUI_Page::render(){parent_object()->render(true, absolute_scale());};

    // Handle an attribute from XML
    void GUI_Page::set_xml_attribute(std::shared_ptr<__XML_Text_Base> text, std::shared_ptr<__XML_Loader> loader_shared_ptr, int& i) {
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
                    load_objects_from_xml(format_for_xml(remove_comment_out_of(read_file(final_path), "\"")), final_path, sub_paged);
                }
            } else {
                print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The path \"" + final_path + "\" you want to load as the content of this page does not exist.");
            }
        }
        else {Object::set_xml_attribute(text, loader_shared_ptr, i);}
    }

    // Update the event of the page
    void GUI_Page::update_event() {Object::update_event();parent_object()->update_event();}

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
        else if(object_type == "scroller_choice") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Scroller_Choice>(object_name);
            return to_return;
        }
        else if(object_type == "file_explorer") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_File_Explorer>(object_name);
            return to_return;
        }
        return *parent->new_object<GUI_Object>(object_name);
    };

    // Loads an object in a page from XML and returns it
    std::shared_ptr<GUI_Object> GUI_Page::__load_object_from_xml(std::string object_name, std::string object_type, std::shared_ptr<__XML_Text_Base> content, bool load_content) {
        // Search the parent
        GUI_Object* current_parent = parent_object();
        for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
            std::shared_ptr<__XML_Text_Base> current_text = content.get()->sub_texts()[i];

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
        if(current_parent == 0){current_parent = parent_object();}

        // Create the object
        std::shared_ptr<GUI_Object> object = __create_loaded_object_from_type(object_name, object_type, current_parent);
        // Handle a possible error
        a_loader.get()->created_objects[object_name] = object;
        if(object.get() == 0) {
            print("Warning", "SCLS Graphic Benoit page \"" + name() + "\"", "The object \"" + object_name + "\" you want to load with XML is not recognised by the GUI.");
        }
        else {
            object.get()->set_visible(true);
            object.get()->set_xml_loading_datas(content, a_loader);
            if(load_content && current_parent->loaded()) {object.get()->load_from_xml(std::string(""));}
            else {object.get()->set_loaded(false);}
        }
        return object;
    }

    // Load objects in a page from XML
    void GUI_Page::__load_objects_from_xml(std::shared_ptr<__XML_Text_Base> content, bool sub_paged) {
        // Analyse each balises
        for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
            std::shared_ptr<__XML_Text_Base> current_text = content.get()->sub_texts()[i];
            std::string current_balise_name = current_text.get()->xml_balise_name();

            // Add a GUI object
            if(current_balise_name == "gui_object") {
                bool load_content = true;
                bool must_be_visible = false;
                std::string object_name = "";
                std::string object_type = "";
                int sub_page = -1;
                for(int j = 0;j<static_cast<int>(current_text.get()->xml_balise_attributes().size());j++) {
                    XML_Attribute& current_attribute = current_text.get()->xml_balise_attributes()[j];
                    std::string current_attribute_name = current_attribute.name;
                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        object_name = current_attribute.value;
                    }
                    else if(current_attribute_name == "no_load") {
                        // Do not load directly the object
                        load_content = false;
                    }
                    else if(current_attribute_name == "sub_page") {
                        // Get the number of the sub-page of the object
                        if(current_attribute.value == "") {sub_page = 0;}
                        else {sub_page = std::stoi(current_attribute.value);}
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
                        print("Warning", "SCLS GUI Page \"" + name() + "\"", "The \"" + object_name + "\" name does already exist.");
                        continue;
                    }
                }

                // Create the object
                if(object_name != "") {
                    std::shared_ptr<GUI_Object> object = __load_object_from_xml(object_name, object_type, current_text, load_content);

                    // Apply the sub-page system
                    if(sub_page >= 0 && object.get()->parent() != 0){
                        //object.get()->parent()->sub_pages().push_back(object);
                        parent_object()->sub_pages().push_back(object);
                        if(!must_be_visible){object.get()->set_visible(false);}
                    }
                }
            }
            else if(current_balise_name == "included"){__load_objects_from_xml(current_text, sub_paged);}
        }
    }
    void GUI_Page::load_objects_from_xml(const std::string& content_to_parse, std::string path, bool sub_paged) {
        // Load the needed balises
        if(gui_loading_balises.get() == 0) {gui_loading_balises = std::make_shared<__Balise_Container>();gui_loading_balises.get()->__load_built_in_balises_gui();}

        // Check each balises
        a_loader = std::make_shared<__GUI_Page_Loader>(); a_loader.get()->path = path;

        // Load the balises
        std::shared_ptr<__XML_Text_Base> content = scls::xml(gui_loading_balises, content_to_parse);
        content.get()->check_include(path);
        __load_objects_from_xml(content, sub_paged);
    }
}
