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
    // __GUI_Transformation main functions
    //
    //*********

    // Most basic __GUI_Transformation constructor
    __GUI_Transformation::__GUI_Transformation(unsigned int window_height, unsigned int window_width, std::shared_ptr<__GUI_Transformation> parent) : a_window_height(window_height), a_window_width(window_width), a_parent(parent) {}

    // Calculate the transformation
    void __GUI_Transformation::calculate_transformation() {
        Fraction divisor_x = Fraction(1, window_width());
        Fraction divisor_y = Fraction(1, window_height());

        // Calculate the real height with the pixel perfect system
        a_height_in_adapted_absolute_scale = divisor_y * height_in_pixel();
        a_height_in_adapted_scale = a_height_in_adapted_absolute_scale;
        if(parent() != 0) a_height_in_adapted_scale /= parent()->height_in_adapted_absolute_scale();
        a_height_in_adapted_scale -= divisor_y / 2;

        // Calculate the real width with the pixel perfect system
        a_width_in_adapted_absolute_scale = divisor_x * width_in_pixel();
        a_width_in_adapted_scale = a_width_in_adapted_absolute_scale;
        if(parent() != 0) a_width_in_adapted_scale /= parent()->width_in_adapted_absolute_scale();
        a_width_in_adapted_scale -= divisor_x / 2;

        a_x_in_adapted_absolute_scale = divisor_x * x_in_absolute_pixel() * 2 + divisor_x / 4;
        a_y_in_adapted_absolute_scale = divisor_y * y_in_absolute_pixel() * 2 + divisor_y / 4;

        Fraction absolute_height_to_apply = height_in_adapted_absolute_scale();

        // Calculate the y minimum and maximum scale
        a_object_y_maximum = object_absolute_y_last_extremum();
        a_object_y_minimum = object_absolute_y_first_extremum();
        if(parent() != 0) {
            a_object_y_maximum -= y_in_absolute_pixel();
            a_object_y_minimum -= y_in_absolute_pixel();
        }
        // Apply the local transformations
        a_object_y_maximum *= one_pixel_height_in_scale();
        a_object_y_minimum *= one_pixel_height_in_scale();
    }

    //*********
    //
    // Border handling
    //
    //*********

    // Returns the first absolute extremum of the object in the Y axis
    Fraction __GUI_Transformation::object_absolute_y_first_extremum(bool remove_border) {
        Fraction to_return = y_in_absolute_pixel();
        if(remove_border) to_return += border_width_in_pixel()[2];
        Fraction parent_extremum = Fraction(-1);
        if(parent() != 0) { parent_extremum = parent()->object_absolute_y_first_extremum(true); }
        if(parent_extremum > to_return) to_return = parent_extremum;
        return to_return;
    }

    // Returns the last absolute extremum of the object in the Y axis
    Fraction __GUI_Transformation::object_absolute_y_last_extremum(bool remove_border) {
        Fraction to_return = (y_in_absolute_pixel() + height_in_pixel());
        if(remove_border) to_return -= border_width_in_pixel()[0];
        Fraction parent_extremum = Fraction(window_height());
        if(parent() != 0) { parent_extremum = parent()->object_absolute_y_last_extremum(true); }
        if(parent_extremum < to_return) to_return = parent_extremum;
        return to_return;
    }

    // Returns the extremum of the object
    glm::vec4 __GUI_Transformation::object_extremum() { return glm::vec4(-1.0, a_object_y_minimum.to_double(), 2.0, a_object_y_maximum.to_double()); }

    //*********
    //
    // Absolute scale plan
    //
    //*********

    // Returns the height in absolute scale of the object
    Fraction __GUI_Transformation::height_in_absolute_scale() const {
        if(a_last_height_definition == _Size_Definition::Absolute_Scale_Size || (a_last_height_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_height;
        Fraction to_return = a_height;

        if(a_last_height_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_height_in_absolute_scale();
        }
        else {
            if(parent() != 0) { to_return *= parent()->height_in_absolute_scale(); }
        }

        return to_return;
    }

    // Returns the height in absolute scale and by counting the window of the object
    Fraction __GUI_Transformation::height_in_absolute_scale_and_window() const { return height_in_absolute_scale() / Fraction(window_width(), window_height()); }

    // Returns the width in absolute scale of the object
    Fraction __GUI_Transformation::width_in_absolute_scale() const {
        if(a_last_width_definition == _Size_Definition::Absolute_Scale_Size || (a_last_width_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_width;
        Fraction to_return = a_width;

        if(a_last_width_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_width_in_absolute_scale();
        }
        else if(a_last_width_definition == _Size_Definition::Scale_Size) {
            to_return *= parent()->width_in_absolute_scale();
        }

        return to_return;
    }

    // Returns the x position in absolute scale of the object
    Fraction __GUI_Transformation::x_in_absolute_scale() const {
        if(a_last_x_definition == _Size_Definition::Absolute_Scale_Size || (a_last_x_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_x;
        Fraction to_return = Fraction(0);
        Fraction to_add = Fraction(0);
        if(parent() != 0) to_add += parent()->x_in_absolute_scale();

        // Check for the needed convertions
        if(a_last_x_definition == _Size_Definition::Pixel_Size) {
            to_return = a_x * one_pixel_width_in_absolute_scale();
        }
        else if(a_last_x_definition == _Size_Definition::Object_Scale_Size) {
            to_return = (width_in_absolute_scale() * Fraction(-1, 2));
            if(parent() == 0) to_return += a_x;
            else to_return += a_x * parent()->width_in_absolute_scale();
        }
        else {
            to_return = a_x;
            if(parent() != 0) to_return *= parent()->width_in_absolute_scale();
        }

        to_return += to_add;
        return to_return;
    }

    // Returns the y position in absolute scale of the object
    Fraction __GUI_Transformation::y_in_absolute_scale() const {
        if(a_last_y_definition == _Size_Definition::Absolute_Scale_Size || (a_last_y_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_y;
        Fraction to_return = Fraction(0);
        Fraction to_add = Fraction(0);
        if(parent() != 0) to_add += parent()->y_in_absolute_scale();

        // Check for the needed convertions
        if(a_last_y_definition == _Size_Definition::Pixel_Size) {
            to_return = a_y * one_pixel_height_in_absolute_scale();
        }
        else if(a_last_y_definition == _Size_Definition::Object_Scale_Size) {
            to_return = (height_in_absolute_scale() * Fraction(-1, 2));
            if(parent() == 0) to_return += a_y;
            else to_return += a_y * parent()->height_in_absolute_scale();
        }
        else {
            to_return = a_y;
            if(parent() != 0) to_return *= parent()->height_in_absolute_scale();
        }

        to_return += to_add;
        return to_return;
    }

    //*********
    //
    // Pixel plan scale
    //
    //*********

    // Returns the height in pixel of the object
    unsigned int __GUI_Transformation::height_in_pixel() const {
        if(a_last_height_definition == _Size_Definition::Pixel_Size) return static_cast<unsigned int>(a_height.to_double());

        // Calculate the real height with the pixel perfect system
        Fraction divisor_y = Fraction(1, window_height());
        Fraction start_height = height_in_absolute_scale();
        unsigned int real_height_in_pixel = 0;
        while(start_height >= divisor_y) {
            start_height -= divisor_y;
            real_height_in_pixel++;
        }
        if(start_height > 0 && start_height < divisor_y / 2.0) real_height_in_pixel++;
        return real_height_in_pixel;
    }

    // Returns the width in pixel of the object
    unsigned int __GUI_Transformation::width_in_pixel() const {
        if(a_last_width_definition == _Size_Definition::Pixel_Size) return a_width.to_double();

        // Calculate the real width with the pixel perfect system
        Fraction divisor_x = Fraction(1, window_width());
        Fraction start_width = width_in_absolute_scale();
        unsigned int real_width_in_pixel = 0;
        while(start_width >= divisor_x) {
            start_width -= divisor_x;
            real_width_in_pixel++;
        }
        if(start_width > 0 && start_width < divisor_x / 2.0) real_width_in_pixel++;
        return real_width_in_pixel;
    }

    // Returns the x position in pixel of the object
    int __GUI_Transformation::x_in_pixel() const {
        if(a_last_x_definition == _Size_Definition::Pixel_Size) return static_cast<int>(a_x.to_double());
        Fraction to_return = (x_in_absolute_scale() / one_pixel_width_in_absolute_scale());
        if(parent() != 0) to_return -= parent()->x_in_absolute_pixel();
        return static_cast<int>(to_return.to_double());
    }

    // Returns the y position in pixel of the object
    int __GUI_Transformation::y_in_pixel() const {
        if(a_last_y_definition == _Size_Definition::Pixel_Size) return static_cast<int>(a_y.to_double());
        Fraction to_return = (y_in_absolute_scale() / one_pixel_height_in_absolute_scale());
        if(parent() != 0) to_return -= parent()->y_in_absolute_pixel();
        return static_cast<int>(to_return.to_double());
    }

    //*********
    //
    // Scale plan
    //
    //*********

    // Returns the scale of the border width
    glm::vec4 __GUI_Transformation::border_width_in_scale() const {
        double one_pixel_height = one_pixel_height_in_scale().to_double();
        double one_pixel_width = one_pixel_width_in_scale().to_double();
        return a_border_width * glm::vec4(one_pixel_height, one_pixel_width, one_pixel_height, one_pixel_width);
    }

    // Returns the height in scale of the object
    Fraction __GUI_Transformation::height_in_scale() const {
        if(a_last_height_definition == _Size_Definition::Scale_Size || (a_last_height_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_height;
        Fraction to_return = height_in_absolute_scale();
        if(parent() != 0) to_return /= parent()->height_in_absolute_scale();

        return to_return;
    }

    // Returns the width in scale of the object
    Fraction __GUI_Transformation::width_in_scale() const {
        if(a_last_width_definition == _Size_Definition::Scale_Size || (a_last_width_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_width;
        if(parent() != 0) return width_in_absolute_scale() / parent()->width_in_absolute_scale();
        return width_in_absolute_scale();
    }

    // Returns the x position in scale of the object
    Fraction __GUI_Transformation::x_in_scale() const {
        if(a_last_x_definition == _Size_Definition::Scale_Size || (a_last_width_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_x;
        if(parent() != 0) return x_in_absolute_scale() / parent()->x_in_absolute_scale();
        return x_in_absolute_scale();
    }

    // Returns the y position in scale of the object
    Fraction __GUI_Transformation::y_in_scale() const {
        if(a_last_y_definition == _Size_Definition::Scale_Size) { return a_y; }

        Fraction to_return = y_in_absolute_scale();
        if(parent() != 0) { to_return = (to_return - parent()->y_in_absolute_scale()) / parent()->height_in_absolute_scale(); }

        return to_return;
    }

    //*********
    //
    // __GUI_Object_Core main functions
    //
    //*********

    // Most basic __GUI_Object_Core constructor
    __GUI_Object_Core::__GUI_Object_Core(_Window_Advanced_Struct& window, __GUI_Object_Core* parent) : a_window(window) {
         if(parent != 0) a_transformation = std::make_shared<__GUI_Transformation>(window_struct().window_height(), window_struct().window_width(), parent->transformation_shared_ptr());
         else {
            std::shared_ptr<__GUI_Transformation> empty_ptr;
            a_transformation = std::make_shared<__GUI_Transformation>(window_struct().window_height(), window_struct().window_width(), empty_ptr);
         }
    }

    // Calculate the transformation of the object
    void __GUI_Object_Core::_apply_calculate_transformation(std::shared_ptr<__GUI_Transformation> current_transformation) {
        // Calculate the transformation
        // Pass the border
        current_transformation.get()->set_border_width_in_pixel(current_style().border_width_to_apply());
        // Pass the height
        if(current_style().height_definition_to_apply() == _Size_Definition::Scale_Size || current_style().height_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_height_in_scale(current_style().height_to_apply());
        else if(current_style().height_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_height_in_pixel(current_style().height_to_apply());
        // Pass the width
        if(current_style().width_definition_to_apply() == _Size_Definition::Scale_Size || current_style().width_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_width_in_scale(current_style().width_to_apply());
        else if(current_style().width_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_width_in_pixel(current_style().width_to_apply());
        // Pass the X
        if(current_style().x_definition_to_apply() == _Size_Definition::Scale_Size) current_transformation.get()->set_x_in_scale(current_style().x_to_apply());
        else if(current_style().x_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_x_in_object_scale(current_style().x_to_apply());
        else if(current_style().x_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_x_in_pixel(current_style().x_to_apply());
        // Pass the Y
        if(current_style().y_definition_to_apply() == _Size_Definition::Scale_Size) current_transformation.get()->set_y_in_scale(current_style().y_to_apply());
        else if(current_style().y_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_y_in_object_scale(current_style().y_to_apply());
        else if(current_style().y_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_y_in_pixel(current_style().y_to_apply());
    }

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

    // Delete a child of the object
    void GUI_Object::delete_child(GUI_Object* object) { for(int i = 0;i<static_cast<int>(children().size());i++) { if(children()[i].get() == object) { children().erase(children().begin() + i); return; } } }

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
    void GUI_Object::__load_from_xml(std::vector<_Text_Balise_Part> &cutted, std::string event) {
        // Parse the content
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::string content = formatted_balise(cutted[i].content); if(content == "") continue;
            std::string current_balise_name = balise_name(content);

            // Apply the XML balise
            std::vector<std::string> attributes = cut_balise_by_attributes_out_of(content, "\"");
            if(current_balise_name == "when") {
                // Load the object look at a certain even
                std::vector<_Text_Balise_Part> final_content = std::vector<_Text_Balise_Part>();
                std::string current_event = "";
                // Get each attributes
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string current_attribute_name = attribute_name(attributes[j]);
                    std::string value = attribute_value(attributes[j]);
                    if(value.size() > 0) {
                        // Remove the "
                        if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                        if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                    }

                    if(current_attribute_name == "event") {
                        // Value of the event
                        current_event = value;
                    }
                }
                // Get the content of the text
                i++;
                while(i < cutted.size()) {
                    std::string current_content = cutted[i].content;
                    if(current_content[0] == '<') {
                        std::string next_balise = formatted_balise(current_content);
                        std::string next_balise_name = balise_name(next_balise);

                        if(next_balise_name == current_balise_name) break;
                        else final_content.push_back(cutted[i]);
                    }
                    else {
                        final_content.push_back(cutted[i]);
                    }

                    i++;
                }

                if(event == "") {
                    // Apply the attribute for the event
                    __load_from_xml(final_content, current_event);
                    std::cout << "When " << current_event << " " << final_content.size() << std::endl;
                }
                else {
                    print("Warning", "SCLS Graphic \"Benoit\" object \"" + name() + "\"", "Can't load an event inside an another event with XML.");
                }
            }
            else if(current_balise_name != "parent") {
                set_xml_attribute(current_balise_name, attributes, event, cutted, i);
            }
        }
    }

    // Handle an attribute from XML
    void GUI_Object::set_xml_attribute(std::string xml_attribute_name, std::vector<std::string> xml_attribute_value, std::string event, const std::vector<_Text_Balise_Part>& cutted, int& i) {
        if(xml_attribute_name == "background_color") {
            // Load the background color
            Color background_color(0, 0, 0, 255);
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "red") {
                    // Red part of the color
                    background_color.set_red(std::stoi(value));
                }
                else if(current_attribute_name == "green") {
                    // Red part of the color
                    background_color.set_green(std::stoi(value));
                }
                else if(current_attribute_name == "blue") {
                    // Red part of the color
                    background_color.set_blue(std::stoi(value));
                }
                else if(current_attribute_name == "alpha") {
                    // Alpha part of the color
                    background_color.set_alpha(std::stoi(value));
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
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "red") {
                    // Red part of the color
                    border_color.set_red(std::stoi(value));
                }
                else if(current_attribute_name == "green") {
                    // Red part of the color
                    border_color.set_green(std::stoi(value));
                }
                else if(current_attribute_name == "blue") {
                    // Red part of the color
                    border_color.set_blue(std::stoi(value));
                }
                else if(current_attribute_name == "bottom") {
                    // Bottom width of the border
                    border_bottom = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "left") {
                    // Left width of the border
                    border_left = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "right") {
                    // Right width of the border
                    border_right = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "top") {
                    // Top width of the border
                    border_top = Fraction::from_std_string(value);
                }
            }
            set_border_color(border_color);
            set_border_width_in_pixel(border_top.to_double());
        }
        else if(xml_attribute_name == "cursor") {
            if(event == "overflighted") {
                // Load the cursor of the object when overflighted
                unsigned long cursor = GLFW_ARROW_CURSOR;
                for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                    std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                    std::string value = attribute_value(xml_attribute_value[j]);
                    if(value.size() > 0) {
                        // Remove the "
                        if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                        if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                    }

                    if(current_attribute_name == "type") {
                        // Value of the cursor
                        if(value == "pointing_hand_cursor") { cursor = GLFW_HAND_CURSOR; }
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
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "value") {
                    // Value of the height
                    height = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the height
                    if(value == "scale") height_type = _Size_Definition::Scale_Size;
                    else if(value == "pixel") height_type = _Size_Definition::Pixel_Size;
                }
            }
            // Set the height
            if(height_type == _Size_Definition::Pixel_Size) set_height_in_pixel(height.to_double());
            else set_height_in_scale(height);
        }
        else if(xml_attribute_name == "texture") {
            // Load the texture position
            Alignment_Texture texture_alignment = Alignment_Texture::T_Fill;
            std::string texture_name = "";
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "name") {
                    // Value of the name
                    texture_name = value;
                }
                else if(current_attribute_name == "alignment") {
                    // Alignment of the texture
                    if(value == "fill") texture_alignment = Alignment_Texture::T_Fill;
                    else if(value == "fit") texture_alignment = Alignment_Texture::T_Fit;
                    else if(value == "fit_horizontally") texture_alignment = Alignment_Texture::T_Fit_Horizontally;
                    else if(value == "fit_vertically") texture_alignment = Alignment_Texture::T_Fit_Vertically;
                    else if(value == "user_defined") texture_alignment = Alignment_Texture::T_User_Defined;
                }
            }
            // Set the goot texture
            if(texture_name != "") set_texture(texture_name);
            set_texture_alignment(texture_alignment);
        }
        else if(xml_attribute_name == "width") {
            // Load the width of the object
            Fraction width = Fraction(1);
            _Size_Definition width_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "value") {
                    // Value of the width
                    width = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the width
                    if(value == "scale") width_type = _Size_Definition::Scale_Size;
                    else if(value == "pixel") width_type = _Size_Definition::Pixel_Size;
                }
            }
            // Set the width
            if(width_type == _Size_Definition::Pixel_Size) set_width_in_pixel(width.to_double());
            else set_width_in_scale(width);
        }
        else if(xml_attribute_name == "x") {
            // Load the X position
            Fraction x = Fraction(1);
            _Size_Definition x_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "value") {
                    // Value of the width
                    x = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the width
                    if(value == "scale") x_type = _Size_Definition::Scale_Size;
                    else if(value == "pixel") x_type = _Size_Definition::Pixel_Size;
                    else if(value == "object_scale") x_type = _Size_Definition::Object_Scale_Size;
                }
            }
            // Set the x
            if(x_type == _Size_Definition::Object_Scale_Size) set_x_in_object_scale(x);
            else if(x_type == _Size_Definition::Pixel_Size) set_x_in_pixel(x.to_double());
            else set_x_in_scale(x);
        }
        else if(xml_attribute_name == "y") {
            // Load the Y position
            Fraction y = Fraction(1);
            _Size_Definition y_type = _Size_Definition::Scale_Size;
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "value") {
                    // Value of the width
                    y = Fraction::from_std_string(value);
                }
                else if(current_attribute_name == "type") {
                    //Type of the width
                    if(value == "scale") y_type = _Size_Definition::Scale_Size;
                    else if(value == "pixel") y_type = _Size_Definition::Pixel_Size;
                    else if(value == "object_scale") y_type = _Size_Definition::Object_Scale_Size;
                }
            }
            // Set the y
            if(y_type == _Size_Definition::Object_Scale_Size) set_y_in_object_scale(y);
            else if(y_type == _Size_Definition::Pixel_Size) set_y_in_pixel(y.to_double());
            else set_y_in_scale(y);
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

        // Calculate the transformation
        _apply_calculate_transformation(transformation_shared_ptr());

        a_transformation_updated = false;
        transformation()->calculate_transformation();

        // Apply to children
        if(with_children) {for(int i = 0;i<static_cast<int>(a_children.size());i++) a_children[i].get()->after_resizing();}
    }

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
    glm::vec4 GUI_Object::fitted_horizontally_texture_rect() {
        Fraction height_texture = (width_in_absolute_scale() / height_in_absolute_scale_and_window()) / texture()->image_ratio();
        Fraction width_texture = Fraction(1);
        Fraction x_texture = Fraction(1, 2) - width_texture / Fraction(2);
        Fraction y_texture = Fraction(1, 2) - height_texture / Fraction(2);
        return glm::vec4(x_texture.to_double(), y_texture.to_double(), width_texture.to_double(), height_texture.to_double());
    }

    // Returns the rect of the vertically fitted texture
    glm::vec4 GUI_Object::fitted_vertically_texture_rect() {
        Fraction height_texture = Fraction(1);
        Fraction width_texture = texture()->image_ratio() / (width_in_absolute_scale() / height_in_absolute_scale_and_window());
        Fraction x_texture = Fraction(0);
        Fraction y_texture = Fraction(1, 2) - height_texture / 2.0;
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
            if(current_object->visible() && current_object->is_in_rect_in_pixel(window_struct().mouse_x(), window_struct().window_height() - window_struct().mouse_y())) {
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
        int last_y_position = (a_scroller_children->y_in_pixel() + a_scroller_children->height_in_pixel()) - a_scroller_children->transformation()->parent()->height_in_pixel();
        GUI_Object::after_resizing();
        Fraction tallest_point = Fraction(0);
        for(int i = 0;i<static_cast<int>(a_scroller_children->children().size());i++) {
            GUI_Object* current_object = a_scroller_children->children()[i].get();
            if(Fraction(current_object->y_in_pixel()) + Fraction(current_object->height_in_pixel()) > tallest_point) {
                tallest_point = Fraction(current_object->y_in_pixel()) + current_object->height_in_pixel();
            }
        }
        a_scroller_children->set_height_in_pixel(static_cast<unsigned int>(tallest_point.to_double()));
        a_scroller_children->move_top_in_parent(1);

        // Calculate according to the last position
        if(!reset) {
            if(last_y_position >= 0) {
                if(last_y_position < static_cast<int>(a_scroller_children->height_in_pixel()) - static_cast<int>(height_in_pixel())) {
                    a_scroller_children->move_top_in_parent(-last_y_position);
                }
                else {
                    a_scroller_children->move_bottom_in_parent(1);
                }
            }
        }
        GUI_Object::after_resizing();
    }

    // Private function to create the children scroller
    std::shared_ptr<GUI_Object>* GUI_Scroller::_create_scroller_children() {
        std::shared_ptr<GUI_Object>* to_return = GUI_Object::new_object<GUI_Object>(name() + "_children_scroller");
        return to_return;
    }

    // Scroll the scroller
    void GUI_Scroller::scroll_y(Fraction movement) {
        if(a_scroller_children->height_in_scale() > 1) {
            movement *= Fraction(1, 30);
            Fraction final_pos = a_scroller_children->y_in_scale() - movement;
            if(final_pos > 0) final_pos = Fraction(0);
            if(final_pos < a_scroller_children->height_in_scale() * Fraction(-1) + Fraction(1)) final_pos = a_scroller_children->height_in_scale() * Fraction(-1) + Fraction(1);
            a_scroller_children->set_y_in_scale(final_pos);
        }
        else {
            a_scroller_children->move_top_in_parent();
        }
        GUI_Object::after_resizing();
    }

    //*********
    //
    // __GUI_Text_Metadatas main functions
    //
    //*********

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

        update_texture();
    }

    //*********
    //
    // GUI Text main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text::GUI_Text(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {}

    // Update the texture of the text
    void GUI_Text::update_text_texture() {
        if(text() != "" || use_cursor()) {
            if(a_text_image == 0) {
                a_text_image = window_struct().text_image_generator()->new_text_image_multi_block("");
            }

            // Create the text
            glm::vec2 position_to_apply = glm::vec2(x_in_pixel(), y_in_pixel());
            a_text_image->set_text(text());
            a_text_image->global_style().background_color = background_color();
            a_text_image->global_style().color = font_color();
            a_text_image->global_style().font_size = font_size();
            // a_text_image->set_cursor_position(cursor_position_in_formatted_text());
            // a_text_image->set_use_cursor(use_cursor());

            // Apply the text
            std::shared_ptr<Image> image_to_paste = a_text_image->image_shared_pointer();
            texture()->set_image(image_to_paste);
        }
        else {
            texture()->set_image(0);
        }
    };

    // GUI_Text destructor
    GUI_Text::~GUI_Text() {
        if(a_text_image != 0) {
            delete a_text_image;
            a_text_image = 0;
        }
    }

    //*********
    //
    // Loading handler
    //
    //*********

    // Handle an attribute from XML
    void GUI_Text::set_xml_attribute(std::string xml_attribute_name, std::vector<std::string> xml_attribute_value, std::string event, const std::vector<_Text_Balise_Part>& cutted, int& i) {
        if(xml_attribute_name == "content") {
            // Load the background color
            std::string final_content = "";
            // Get each attributes
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                // Load the font size of the text
                if(current_attribute_name == "font_size") {
                    // Value of the font size
                    set_font_size(Fraction::from_std_string(value).to_double());
                }
            }
            // Get the content of the text
            i++;
            while(i < cutted.size()) {
                std::string current_content = cutted[i].content;
                if(current_content[0] == '<') {
                    std::string current_balise = formatted_balise(current_content);
                    std::string current_balise_name = balise_name(current_balise);

                    if(current_balise_name == "content") break;
                    else final_content += current_balise;
                }
                else {
                    final_content += current_content;
                }

                i++;
            }
            set_text(final_content);
        }
        else if(xml_attribute_name == "font_size") {
            // Load the font size of the text
            Fraction final_font_size = Fraction(1);
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "value") {
                    // Value of the height
                    final_font_size = Fraction::from_std_string(value);
                }
            }
            // Set the font size
            set_font_size(final_font_size.to_double());
        }
        else {
            // Load a lowest level attribute
            __GUI_Text_Metadatas::set_xml_attribute(xml_attribute_name, xml_attribute_value, event, cutted, i);
        }
    }

    //*********
    //
    // GUI_Text_Input main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text_Input::GUI_Text_Input(_Window_Advanced_Struct& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {
        set_text_image_type(Block_Type::BT_Keep_Block_And_Line_In_Memory);
        // set_use_cursor(true);
    }

    // GUI_Text_Input destructor
    GUI_Text_Input::~GUI_Text_Input() {}

    // Add a text to the input at the cursor position
    void GUI_Text_Input::add_text(const std::string& text_to_add) {
        unsigned int cursor_position = cursor_position_in_unformatted_text();
        std::string text_to_modify = text();
        std::string final_text = text_to_modify.substr(0, cursor_position) + text_to_add + text_to_modify.substr(cursor_position, text_to_modify.size() - cursor_position);
        if(final_text != text()) {
            std::vector<std::string> cutted = cut_string(text_to_add, "</br>", false, false);
            if(a_text_image != 0 && cutted.size() > 1) {
                for(int i = 0;i<cutted.size() - 1;i++) {
                    if((a_text_image->line_number_at_position(cursor_position) - line_offset()) + i < children().size()) {
                        children().insert(children().begin() + (a_text_image->line_number_at_position(cursor_position) - line_offset()) + i, 0);
                    }
                    else {
                        children().push_back(0);
                    }
                }
            }

            if(a_text_image != 0) {
                a_text_image->add_text(cutted, final_text, text_to_add, cursor_position);
            }
            set_cursor_position_in_formatted_text(cursor_position_in_formatted_text() + window_struct().text_image_generator()->plain_text_size(text_to_add));
            __GUI_Text_Metadatas::set_text(final_text, false);
            if(a_text_image != 0) a_text_image->set_cursor_position_in_plain_text(cursor_position_in_formatted_text());
        }
    };

    // Function called after that the window is resized
    void GUI_Text_Input::after_resizing(){
        GUI_Object::after_resizing();
        place_lines();
    }

    // Format a char
    std::string GUI_Text_Input::_format(std::string letter, bool apply_capitalisation) {
        std::string result = "";

        for(int i = 0;i<static_cast<int>(letter.size());i++) {
            std::string to_analyse = ""; to_analyse += letter[i];
            result += _format_one_letter(to_analyse, apply_capitalisation);
        }

        return result;
    }

    // Capitalize a std::string
    std::string GUI_Text_Input::_format_one_letter(std::string letter, bool apply_capitalisation) {
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
            if(letter == "<") letter = "&gt;";
        }
        else {
            if(letter == "<") letter = "&lt;";
        }

        // Check the letter after capitalization (copy pasted)
        if(letter == ">") letter = "&gt;";

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
        if(!(visible() && (is_focused() || has_child_focused()))) return;

        std::string final_text = text();
        std::string to_add = "";

        // Handle letters
        bool should_control = (window_struct().key_state("left control") == Key_State::Pressed || window_struct().key_state("right control") == Key_State::Pressed);
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
        to_add = _format(to_add, should_capitalise);
        to_add = format_string_from_plain_text(to_add);

        if(window_struct().key_pressed_or_repeated_pressed("enter") == Key_State::Pressed) { to_add += "</br>";  }

        to_add = to_utf_8_code_point(to_add);
        add_text(to_add);
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

        move_cursor(a_text_image->cursor_position_in_plain_text_at_line_and_x(i + line_offset(), position[0] * width_in_pixel()) - cursor_position_in_formatted_text());
    }

    // Moves the cursor in the Y axis
    void GUI_Text_Input::move_cursor_y(int movement) {
        if(a_text_image == 0) return;

        Text_Image_Line* cursor_line = a_text_image->cursor_line();
        if(cursor_line != 0) {
            int cursor_number = (cursor_line->datas().line_number) + movement;
            if(cursor_number < 0) {
                move_cursor(-cursor_position_in_formatted_text());
            }
            else if(cursor_number >= a_text_image->lines().size()) {
                move_cursor(plain_text_size() - cursor_position_in_formatted_text());
            }
            else {
                move_cursor(a_text_image->cursor_position_in_plain_text_at_line_and_x(cursor_number, cursor_line->cursor_x()) - cursor_position_in_formatted_text());
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
    void GUI_Text_Input::remove_text(unsigned int size_to_delete_in_plain_text) {
        unsigned int cursor_position = cursor_position_in_unformatted_text() - 1;
        unsigned int position_to_delete = window_struct().text_image_generator()->first_plain_text_character_before_position_in_informatted_text(text(), cursor_position);
        unsigned int size_to_delete = (cursor_position - position_to_delete) + 1;
        set_cursor_position_in_formatted_text(cursor_position_in_formatted_text() - 1);

        std::string text_to_modify = text();
        std::string final_text = text_to_modify.substr(0, position_to_delete) + text_to_modify.substr(position_to_delete + size_to_delete, text_to_modify.size() - (position_to_delete + size_to_delete));
        if(a_text_image != 0) {
            unsigned int line_to_delete = a_text_image->line_number_at_position(cursor_position + 1) - line_offset();
            unsigned int lines_deleted = a_text_image->remove_text(final_text, size_to_delete, size_to_delete_in_plain_text, cursor_position + 1);

            // Delete the children to delete
            for(int i = 0;i<lines_deleted;i++) { children().erase(children().begin() + line_to_delete - i); }
        }
        __GUI_Text_Metadatas::set_text(final_text, false);
    }

    // Update the text
    void GUI_Text_Input::update_event() {
        GUI_Object::update_event();
        input_text();
        update_cursor();
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
                update_texture();
            }
        }
    }

    // Update the texture of the text
    void GUI_Text_Input::update_text_texture() {
        // Configure the text image
        if(attached_text_image() == 0) {
            a_text_image.reset(window_struct().text_image_generator()->new_text_image_block("", text_image_type()));
            attached_text_image()->set_use_cursor(true);
            attached_text_image()->free_memory();
            delete_children();
        }
        attached_text_image()->global_style().font = font();
        if(a_text_modified) attached_text_image()->set_text(text()); a_text_modified = false;
        attached_text_image()->set_cursor_position_in_plain_text(cursor_position_in_formatted_text());

        // Configure the needed creation settings
        attached_text_image()->reset_line_generation();

        // Delete the useless children modified
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(i >= static_cast<int>(attached_text_image()->lines_datas().size())) {
                 children().erase(children().begin() + i); i--;
            }
            else if(children()[i] != 0 && (i + line_offset() < attached_text_image()->lines().size() && attached_text_image()->lines()[i + line_offset()] == 0 || attached_text_image()->lines()[i + line_offset()]->has_been_modified())) {
                children()[i].reset();
            }
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
        for(int i = 0;i<static_cast<int>(children().size()) - static_cast<int>(attached_text_image()->lines_datas().size());i++) {
            children().pop_back();
        }

        // Delete empty children
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i].get() == 0) {children().erase(children().begin() + i); i--;}
        }

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
        a_browser.get()->set_background_color(scls::white);
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
                if(!std::filesystem::exists(paths[i])) continue;

                // Create the button
                paths[i] = to_utf_8_code_point(file_name(paths[i], true));
                std::shared_ptr<GUI_Text> new_button = *a_browser.get()->new_object_in_scroller<GUI_Text>("browser_button_" + std::to_string(i));
                new_button.get()->set_background_color(scls::white);
                new_button.get()->set_font_color(scls::black);
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
                    bool move_cursor = false;
                    std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button.get(), button_text_reference, &move_cursor);
                    threads.push_back(current_thread);
                    current_thread_number++;
                }
                else {
                    new_button.get()->set_text(paths[i], false);
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
                if(!std::filesystem::exists(paths[i]) || !contains<unsigned int>(a_browser_buttons_to_modify, i)) continue;

                // Get the button
                std::shared_ptr<GUI_Text> new_button = a_browser_buttons[i];
                std::string button_text = new_button.get()->text();
                buttons_to_modify.push_back(new_button);

                // Change the specification of the button
                if(contains_selected_file(button_text)) {
                    new_button.get()->set_background_color(scls::blue);
                    new_button.get()->set_font_color(scls::white);
                }
                else {
                    new_button.get()->set_background_color(scls::white);
                    new_button.get()->set_font_color(scls::black);
                }

                // Create the thread
                std::string& button_text_reference = button_text;
                bool move_cursor = false;
                std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button.get(), button_text_reference, &move_cursor);
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
    GUI_File_Explorer::~GUI_File_Explorer() {

    }

    //*********
    //
    // GUI page
    //
    //*********

    // GUI_Page most basic constructor
    GUI_Page::GUI_Page(_Window_Advanced_Struct* window_struct, std::string name) : Object(window_struct, name) {
        set_scale(glm::vec3(2, 2, 1));

        a_parent_object = std::make_shared<GUI_Main_Object>((*reinterpret_cast<_Window_Advanced_Struct*>(window_struct)), "main");
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
    void GUI_Page::set_xml_attribute(std::string xml_attribute_name, std::vector<std::string> xml_attribute_value, __XML_Loader& loader, const std::vector<_Text_Balise_Part>& cutted, int& i) {
        if(xml_attribute_name == "content") {
            // Load the content of the page
            std::string src = "";
            for(int j = 0;j<static_cast<int>(xml_attribute_value.size());j++) {
                std::string current_attribute_name = attribute_name(xml_attribute_value[j]);
                std::string value = attribute_value(xml_attribute_value[j]);
                if(value.size() > 0) {
                    // Remove the "
                    if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                    if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                }

                if(current_attribute_name == "src") {
                    // Source of the content
                    src = value;
                }
            }
            // Get the content
            std::string final_path = path_parent(loader.window_file_path) + "/" + src;
            load_objects_from_xml(read_file(final_path));
        }
        else {
            Object::set_xml_attribute(xml_attribute_name, xml_attribute_value, loader, cutted, i);
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
        if(object_type == "text") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Text>(object_name);
            return to_return;
        }
        else if(object_type == "scroller") {
            std::shared_ptr<GUI_Object> to_return = *parent->new_object<GUI_Scroller>(object_name);
            return to_return;
        }
        return *parent->new_object<GUI_Object>(object_name);
    };

    // Load an object in a page from XML
    void GUI_Page::__load_object_from_xml(std::string object_name, std::string object_type, const std::string& content, std::map<std::string, std::shared_ptr<GUI_Object>>& created_objects) {
        // Parse the content
        std::vector<_Text_Balise_Part> cutted = cut_string_by_balise_out_of(content, "\"", true);
        // Search the parent
        GUI_Object* current_parent = parent_object();
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::string content = formatted_balise(cutted[i].content); if(content == "") continue;
            std::string current_balise_name = balise_name(content);

            if(current_balise_name == "parent") {
                // Load the parent
                std::vector<std::string> attributes = cut_balise_by_attributes_out_of(content, "\"");
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string current_attribute_name = attribute_name(attributes[j]);
                    std::string value = attribute_value(attributes[j]);
                    if(value.size() > 0) {
                        // Remove the "
                        if(value[0] == '\"') value = value.substr(1, value.size() - 1);
                        if(value.size() > 0 && value[value.size() - 1] == '\"') value = value.substr(0, value.size() - 1);
                    }

                    if(current_attribute_name == "name") { current_parent = created_objects[value].get(); }
                }
                break;
            }
        }
        if(current_parent == 0) current_parent = parent_object();

        // Create the object
        std::shared_ptr<GUI_Object> object = __create_loaded_object_from_type(object_name, object_type, current_parent);
        created_objects[object_name] = object;
        object.get()->__load_from_xml(cutted);
    }

    // Load objects in a page from XML
    void GUI_Page::load_objects_from_xml(const std::string& content_to_parse) {
        const std::string content = format_for_xml(content_to_parse);
        std::vector<_Text_Balise_Part> cutted = cut_string_by_balise(content, true);

        // Check each balises
        std::map<std::string, std::shared_ptr<GUI_Object>> created_objects = std::map<std::string, std::shared_ptr<GUI_Object>>();
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::string content = formatted_balise(cutted[i].content); if(content == "") continue;
            std::string current_balise_name = balise_name(content);

            // Get the content of the object
            std::string object_content = "";
            if(current_balise_name == "gui_object") {
                i++;
                while(i < static_cast<int>(cutted.size())) {
                    std::string current_content = cutted[i].content;
                    if(current_content[0] == '<') {
                        std::string parsed_content = formatted_balise(current_content); if(parsed_content == "") { i++;continue; }
                        std::string parsed_balise_name = balise_name(parsed_content);
                        if(parsed_balise_name == current_balise_name) break;
                        object_content += parsed_content;
                    }
                    else {
                        object_content += current_content;
                    }
                    i++;
                }
            }

            // Add a 3D object
            if(current_balise_name == "gui_object") {
                std::string object_name = "";
                std::string object_type = "";
                std::vector<std::string> attributes = cut_balise_by_attributes(content);
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string current_attribute_name = attribute_name(attributes[j]);
                    if(current_attribute_name == "name") {
                        // Get the name of the object
                        object_name = attribute_value(attributes[j]);
                        if(object_name.size() > 0) {
                            // Remove the "
                            if(object_name[0] == '\"') object_name = object_name.substr(1, object_name.size() - 1);
                            if(object_name.size() > 0 && object_name[object_name.size() - 1] == '\"') object_name = object_name.substr(0, object_name.size() - 1);
                        }
                    }
                    else if(current_attribute_name == "type") {
                        // Get the type of the object
                        object_type = attribute_value(attributes[j]);
                        if(object_type.size() > 0) {
                            // Remove the "
                            if(object_type[0] == '\"') object_type = object_type.substr(1, object_type.size() - 1);
                            if(object_type.size() > 0 && object_type[object_type.size() - 1] == '\"') object_type = object_type.substr(0, object_type.size() - 1);
                        }
                    }
                }

                if(object_name != "") {
                    __load_object_from_xml(object_name, object_type, object_content, created_objects);
                }
            }
        }
    }
}
