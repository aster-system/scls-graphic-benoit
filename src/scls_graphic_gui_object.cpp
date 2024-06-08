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
    GUI_Object::GUI_Object(Window& window, std::string name, GUI_Object* parent) : a_name(name), a_parent(parent), a_window(window) {
        a_texture = window.new_texture(name, 1, 1, Color(255, 255, 255, 0));
        a_vao = window.vao("hud_default");
    }

    // Function called after that the window is resized
    void GUI_Object::after_resizing(){
        calculate_adapted_scale(true);

        // Apply to children
        for(int i = 0;i<static_cast<int>(a_children.size());i++) a_children[i]->after_resizing();
    }

    // Delete a child of the object
    void GUI_Object::delete_child(GUI_Object* object) {
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i] == object) {
                children().erase(children().begin() + i);
                delete object;
                return;
            }
        }
    }

    // Render the object
    void GUI_Object::render(glm::vec3 scale_multiplier) {
        // Handle the matrix
        glm::vec2 absolute_position_to_apply = absolute_position_in_adapted_scale();
        glm::vec2 absolute_scale_to_apply = absolute_scale_in_adapted_scale();
        absolute_scale_to_apply *= glm::vec2(scale_multiplier[0], scale_multiplier[1]);
        glm::mat4 matrix = glm::mat4(1.0);
        matrix = glm::translate(matrix, glm::vec3(absolute_position_to_apply[0], absolute_position_to_apply[1], 0));
        matrix = glm::scale(matrix, glm::vec3(absolute_scale_to_apply[0], absolute_scale_to_apply[1], 1));

        // Handle the background color
        vao()->get_shader_program()->set_uniform4f_value("background_color", background_color());

        // Handle the border
        vao()->get_shader_program()->set_uniform4f_value("border_color", border_color());
        vao()->get_shader_program()->set_uniform4f_value("border_width", border_width_in_scale());

        // Handle the rect of the texture
        glm::vec4 final_texture_rect = glm::vec4(1);
        if(texture()->get_image() != 0) {
            if(texture_alignment() == Alignment_Texture::T_User_Defined) final_texture_rect = user_defined_texture_rect();
            else if(texture_alignment() == Alignment_Texture::T_Fit) final_texture_rect = fitted_texture_rect();
            else if(texture_alignment() == Alignment_Texture::T_Fit_Horizontally) final_texture_rect = fitted_horizontally_texture_rect();
            else if(texture_alignment() == Alignment_Texture::T_Fit_Vertically) final_texture_rect = fitted_vertically_texture_rect();
        }
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", final_texture_rect);

        // Handle the texture and the VAO
        if(texture() == 0) {
            vao()->get_shader_program()->set_uniformb_value("texture_binded", false);
        }
        else {
            texture()->bind(); // Bind the texture
            vao()->get_shader_program()->set_uniformb_value("texture_binded", true);
        }
        vao()->get_shader_program()->set_uniform4fv_value("model", matrix);
        vao()->get_shader_program()->set_uniform4f_value("object_extremum", object_extremum());
        vao()->get_shader_program()->set_uniform4f_value("object_rect", glm::vec4(0, 0, 1.0, 1.0));
        vao()->render();

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i]->visible())children()[i]->render(scale_multiplier);
        }

        soft_reset();
    }

    // GUI_Object destructor
    GUI_Object::~GUI_Object() {
        delete_children();

        if(parent() != 0) parent()->child_deleted(this);

        window_struct().remove_texture(texture());
    }

    //*********
    //
    // Transform handling
    //
    //*********

    // Returns the absolute scale of the border width
    glm::vec4 GUI_Object::border_width_in_absolute_scale() const {
        if(a_last_border_width_definition_type == _Size_Definition::Pixel_Size) {
            glm::vec2 one_pixel = one_pixel_in_absolute_scale();
            double multiplier = absolute_scale_ratio();
            return a_border_width * glm::vec4(one_pixel[1] * multiplier, one_pixel[0], one_pixel[1] * multiplier, one_pixel[0]);
        }
        return a_border_width;
    };

    // Returns the scale of the border width
    glm::vec4 GUI_Object::border_width_in_scale() const {
        if(a_last_border_width_definition_type == _Size_Definition::Pixel_Size) {
            glm::vec2 one_pixel = one_pixel_in_scale();
            return a_border_width * glm::vec4(one_pixel[1], one_pixel[0], one_pixel[1], one_pixel[0]);
        }
        return a_border_width;
    }

    // Calculate the adapted scale
    void GUI_Object::calculate_adapted_scale(bool force) {
        if(a_adapted_scale_updated && !force) return;

        double divisor_x = one_pixel_in_absolute_scale()[0];
        double divisor_y = one_pixel_in_absolute_scale()[1];

        // Handle the pixel perfect system for the height absolute scale
        a_height_in_adapted_absolute_scale = height_in_absolute_scale();
        unsigned int total = 0;
        while(a_height_in_adapted_absolute_scale > divisor_y) {
            a_height_in_adapted_absolute_scale -= divisor_y;
            total++;
        }
        if(a_height_in_adapted_absolute_scale < divisor_y / 2.0) total++;
        a_height_in_adapted_absolute_scale = total * divisor_y;

        // Handle the pixel perfect system for the width absolute scale
        a_width_in_adapted_absolute_scale = width_in_absolute_scale();
        total = 0;
        while(a_width_in_adapted_absolute_scale > divisor_x) {
            a_width_in_adapted_absolute_scale -= divisor_x;
            total++;
        }
        if(a_width_in_adapted_absolute_scale < divisor_x / 2.0) total++;
        a_width_in_adapted_absolute_scale = total * divisor_x;

        // Handle the pixel perfect system for the x absolute scale
        a_x_in_adapted_absolute_scale = x_in_absolute_scale();
        total = 0;
        a_x_in_adapted_absolute_scale++;
        a_x_in_adapted_absolute_scale -= a_width_in_adapted_absolute_scale;
        while(a_x_in_adapted_absolute_scale > divisor_x) {
            a_x_in_adapted_absolute_scale -= divisor_x;
            total++;
        }
        if(a_x_in_adapted_absolute_scale < divisor_x / 2.0) total++;
        a_x_in_adapted_absolute_scale = total * divisor_x;
        a_x_in_adapted_absolute_scale += divisor_x / 5.0;
        a_x_in_adapted_absolute_scale += a_width_in_adapted_absolute_scale;
        a_x_in_adapted_absolute_scale--;

        // Handle the pixel perfect system for the y absolute scale
        a_y_in_adapted_absolute_scale = y_in_absolute_scale();
        int height_to_apply_in_pixel = a_height_in_adapted_absolute_scale / divisor_y;
        total = 0;
        a_y_in_adapted_absolute_scale++;
        a_y_in_adapted_absolute_scale -= a_height_in_adapted_absolute_scale;
        while(a_y_in_adapted_absolute_scale > divisor_y) {
            a_y_in_adapted_absolute_scale -= divisor_y;
            total++;
        }
        if(a_y_in_adapted_absolute_scale < divisor_y / 2.0) total++;
        a_y_in_adapted_absolute_scale = total * divisor_y;
        a_y_in_adapted_absolute_scale += divisor_y / 5.0;
        a_y_in_adapted_absolute_scale += a_height_in_adapted_absolute_scale;
        a_y_in_adapted_absolute_scale--;
        // Handle the odd height
        if(height_to_apply_in_pixel % 2 == 1) a_y_in_adapted_absolute_scale += divisor_y / 2.0;

        a_adapted_scale_updated = true;
    }

    // Collapse the size of the object with the size of the texture by its height
    void GUI_Object::collapse_size_on_texture_with_height(double height) {
        update_texture();

        double divisor = 1.0;
        if(parent() != 0) divisor = parent()->absolute_scale_ratio_and_window();
        double width = texture()->image_ratio() / (divisor * 1.0);
        set_size_in_scale(glm::vec2(width * height, height));
    }

    // Delete the children of an object
    void GUI_Object::delete_children() { for(int i = 0;i<static_cast<int>(children().size());i++) { delete children()[i]; } children().clear(); }

    // Returns the rect of the fitted texture
    glm::vec4 GUI_Object::fitted_texture_rect() const {
        if(texture()->image_ratio() > (width_in_absolute_scale() / height_in_absolute_scale_and_window())) {
            return fitted_horizontally_texture_rect();
        }
        return fitted_vertically_texture_rect();
    }

    // Returns the rect of the horizontally fitted texture
    glm::vec4 GUI_Object::fitted_horizontally_texture_rect() const {
        double height_texture = (width_in_absolute_scale() / height_in_absolute_scale_and_window()) / texture()->image_ratio();
        double width_texture = 1.0;
        double x_texture = 0.5 - width_texture / 2.0;
        double y_texture = 0.5 - height_texture / 2.0;
        return glm::vec4(x_texture, y_texture, width_texture, height_texture);
    }

    // Returns the rect of the vertically fitted texture
    glm::vec4 GUI_Object::fitted_vertically_texture_rect() const {
        double height_texture = 1.0;
        double width_texture = texture()->image_ratio() / (width_in_absolute_scale() / height_in_absolute_scale_and_window());
        double x_texture = 0;
        double y_texture = 0.5 - height_texture / 2.0;
        if(texture_alignment_horizontal() == Alignment_Horizontal::H_Center) x_texture = 0.5 - width_texture / 2.0;
        else if(texture_alignment_horizontal() == Alignment_Horizontal::H_Right) x_texture = 1.0 - width_texture;
        return glm::vec4(x_texture, y_texture, width_texture, height_texture);
    }

    // Returns the height in pixel of the object
    double GUI_Object::height_in_pixel() const {
        if(a_last_height_definition == _Size_Definition::Pixel_Size) return a_height;
        return height_in_absolute_scale() / one_pixel_in_absolute_scale()[1];
    }

    // Returns the height in absolute scale of the object
    double GUI_Object::height_in_absolute_scale() const {
        double to_return = a_height;

        if(a_last_height_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_in_absolute_scale()[1];
        }
        else {
            if(a_last_height_definition == _Size_Definition::Scale_Size && parent() != 0) {
                to_return *= parent()->height_in_absolute_scale();
            }
        }

        return to_return;
    }

    // Returns the height in absolute scale and by counting the window of the object
    double GUI_Object::height_in_absolute_scale_and_window() const { return height_in_absolute_scale() / window_struct().window_ratio(); }

    // Returns the height in scale of the object
    double GUI_Object::height_in_scale() const {
        double to_return = a_height;

        if(a_last_height_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_in_absolute_scale()[1];
        }
        else {
            if(a_last_height_definition == _Size_Definition::Absolute_Scale_Size && parent() != 0) {
                to_return /= parent()->height_in_absolute_scale();
            }
        }

        return to_return;
    }

    // Returns the extremum of the object
    glm::vec4 GUI_Object::object_extremum() {
        double absolute_y_bottom_to_apply = (y_bottom_in_adapted_absolute_scale() + 1.0) / 2.0;
        double absolute_y_top_to_apply = y_top_in_adapted_absolute_scale();
        double absolute_height_to_apply = height_in_adapted_absolute_scale();
        double absolute_y_bottom_parent_to_apply = -1.0;
        double absolute_y_top_parent_to_apply = 1.0;
        if(parent() != 0) {
            absolute_y_bottom_parent_to_apply = parent()->y_bottom_in_adapted_absolute_scale();
            absolute_y_top_parent_to_apply = parent()->y_top_in_adapted_absolute_scale();
        }
        double y_maximum = ((absolute_y_top_to_apply + 1.0) / 2.0 - (absolute_y_top_parent_to_apply + 1.0) / 2.0);
        double y_minimum = ((absolute_y_bottom_parent_to_apply + 1.0) / 2.0 - absolute_y_bottom_to_apply);

        // Apply the local transformations
        y_maximum /= absolute_height_to_apply; y_maximum = 1.0 - y_maximum;
        y_minimum /= absolute_height_to_apply;
        return glm::vec4(-1.0, y_minimum, 2.0, y_maximum);
    }

    // Returns the rect of user defined texture
    glm::vec4 GUI_Object::user_defined_texture_rect() const {
        double height_texture = texture_height_in_scale();
        double width_texture = texture_width_in_scale();
        double x_texture = 0;
        double y_texture = 0;
        if(texture_alignment_horizontal() == Alignment_Horizontal::H_Center) x_texture = 0.5 - width_texture / 2.0;
        else if(texture_alignment_horizontal() == Alignment_Horizontal::H_Right) x_texture = 1.0 - width_texture;
        if(texture_alignment_vertical() == Alignment_Vertical::V_Center) y_texture = 0.5 - height_texture / 2.0;
        else if(texture_alignment_vertical() == Alignment_Vertical::V_Top) y_texture = 1.0 - height_texture;
        return glm::vec4(x_texture, y_texture, width_texture, height_texture);
    }

    // Returns the width in pixel of the object
    double GUI_Object::width_in_pixel() const {
        if(a_last_width_definition == _Size_Definition::Pixel_Size) return a_width;
        return width_in_absolute_scale() / one_pixel_in_absolute_scale()[0];
    }

    // Returns the width in absolute scale of the object
    double GUI_Object::width_in_absolute_scale() const {
        double to_return = a_width;

        if(a_last_width_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_in_absolute_scale()[0];
        }
        else {
            if(a_last_width_definition == _Size_Definition::Scale_Size && parent() != 0) {
                to_return *= parent()->width_in_absolute_scale();
            }
        }

        return to_return;
    }

    // Returns the width in scale of the object
    double GUI_Object::width_in_scale() const {
        double to_return = a_width;

        if(a_last_width_definition == _Size_Definition::Pixel_Size) {
            to_return *= one_pixel_in_absolute_scale()[0];
        }
        else {
            if(a_last_width_definition == _Size_Definition::Absolute_Scale_Size && parent() != 0) {
                to_return /= parent()->width_in_absolute_scale();
            }
        }

        return to_return;
    }

    // Returns the x position in pixel of the object
    double GUI_Object::x_in_pixel() const {
        double to_return = ((x_in_absolute_scale() + 1.0) / 2.0) * window_struct().window_width() - (width_in_pixel() / 2.0);

        return to_return;
    }

    // Returns the x position in absolute scale of the object
    double GUI_Object::x_in_absolute_scale() const {
        double to_return = 0.0;

        double to_add = 0;
        if(a_last_width_definition != _Size_Definition::Absolute_Scale_Size && parent() != 0) to_add += parent()->x_in_absolute_scale();

        if(a_last_x_definition == _Size_Definition::Pixel_Size) {
            to_return = a_x * (one_pixel_in_absolute_scale()[0] * 2.0) + (-1.0 + width_in_scale());
        }
        else {
            to_return = a_x;

            if(a_last_width_definition == _Size_Definition::Scale_Size && parent() != 0) to_return *= parent()->width_in_absolute_scale();
        }

        to_return += to_add;

        return to_return;
    }

    // Returns the x position in scale of the object
    double GUI_Object::x_in_scale() const {
        double to_return = 0.0;

        if(a_last_x_definition == _Size_Definition::Pixel_Size) {
            to_return = a_x * one_pixel_in_scale()[0];
        }
        else {
            to_return = a_x;
            if(a_last_height_definition == _Size_Definition::Absolute_Scale_Size && parent() != 0) to_return -= parent()->x_in_absolute_scale();
        }

        return to_return;
    }

    // Returns the y position in pixel of the object
    double GUI_Object::y_in_pixel() const {
        double to_return = ((y_in_absolute_scale() + 1.0) / 2.0) * window_struct().window_height() - (height_in_pixel() / 2.0);

        return to_return;
    }

    // Returns the y position in absolute scale of the object
    double GUI_Object::y_in_absolute_scale() const {
        if(a_last_y_definition == _Size_Definition::Absolute_Scale_Size) return a_y;

        double to_return = 0.0;

        double to_add = 0;
        if(a_last_y_definition != _Size_Definition::Absolute_Scale_Size && parent() != 0) to_add += parent()->y_in_absolute_scale();

        if(a_last_y_definition == _Size_Definition::Pixel_Size) {
            to_return = a_y * (one_pixel_in_absolute_scale()[1] * 2.0) + (-1.0 + height_in_scale());
        }
        else {
            to_return = a_y;

            if(a_last_height_definition == _Size_Definition::Scale_Size && parent() != 0) to_return *= parent()->height_in_absolute_scale();
        }

        to_return += to_add;

        return to_return;
    }

    // Returns the y position in scale of the object
    double GUI_Object::y_in_scale() const {
        double to_return = 0.0;

        if(a_last_y_definition == _Size_Definition::Pixel_Size) {
            to_return = a_y * one_pixel_in_scale()[1];
        }
        else {
            to_return = a_y;
            if(a_last_height_definition == _Size_Definition::Absolute_Scale_Size && parent() != 0) to_return -= parent()->y_in_absolute_scale();
        }

        return to_return;
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
            GUI_Object* current_object = current_overflighted_object->children()[i];
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
    GUI_Text::GUI_Text(Window& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {

    }

    // Update the texture of the text
    void GUI_Text::update_text_texture() {
        if(text() != "" || use_cursor()) {
            if(a_text_image == 0) {
                a_text_image = window_struct().text_image_generator()->new_text_image_block("", text_image_type());
            }

            // Create the text
            glm::vec2 position_to_apply = position_in_pixel();
            a_text_image->set_text(text());
            a_text_image->global_style().background_color = background_color();
            a_text_image->global_style().color = font_color();
            a_text_image->global_style().font_size = font_size();
            // a_text_image->set_cursor_position(cursor_position_in_formatted_text());
            // a_text_image->set_use_cursor(use_cursor());

            // Apply the text
            Image* image_to_paste = a_text_image->image();
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
    // GUI_Text_Input main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text_Input::GUI_Text_Input(Window& window, std::string name, GUI_Object* parent) : __GUI_Text_Metadatas(window, name, parent) {
        set_text_image_type(Text_Image_Block::BT_Keep_Block_And_Line_In_Memory);
        // set_use_cursor(true);
    }

    // GUI_Text_Input destructor
    GUI_Text_Input::~GUI_Text_Input() {

    }

    // Add a text to the input at the cursor position
    void GUI_Text_Input::add_text(const std::string& text_to_add) {
        unsigned int cursor_position = cursor_position_in_unformatted_text();
        std::string text_to_modify = text();
        std::string final_text = text_to_modify.substr(0, cursor_position) + text_to_add + text_to_modify.substr(cursor_position, text_to_modify.size() - cursor_position);
        if(final_text != text()) {
            if(a_text_image != 0) {
                unsigned int first_size = a_text_image->lines_datas().size();
                a_text_image->set_text(final_text);
                unsigned int second_size = a_text_image->lines_datas().size();

                // Calculate if lines should be removed or added
                if(first_size == second_size) {
                    Text_Image_Line* current_line = a_text_image->line_at_position_in_plain_text(cursor_position_in_formatted_text());
                    if(current_line != 0) current_line->set_modified(true);
                }
                else {
                    int position = a_text_image->line_number_at_position_in_plain_text(cursor_position_in_formatted_text());
                    if(position != -1) {
                        if(a_text_image->lines()[position] != 0) a_text_image->lines()[position]->set_modified(true);

                        // Add the needed lines
                        for(int i = 0;i<second_size - first_size;i++) {
                            children().insert(children().begin() + position + i, 0);
                            a_text_image->lines().insert(a_text_image->lines().begin() + position + i, 0);
                        }
                    }
                }
            }

            set_cursor_position_in_formatted_text(cursor_position_in_formatted_text() + window_struct().text_image_generator()->plain_text_size(text_to_add));
            __GUI_Text_Metadatas::set_text(final_text, false);
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

        to_add = to_utf_8(to_add);
        add_text(to_add);
    }

    // Move the cursor in the Y axis
    void GUI_Text_Input::move_cursor_y(int movement) {
        if(a_text_image == 0) return;

        Text_Image_Line* cursor_line = a_text_image->cursor_line();
        if(cursor_line != 0) {
            int cursor_number = cursor_line->datas().line_number + movement;
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
            GUI_Object* current_object = children()[i];
            if(current_object != 0) {
                current_object->set_height_in_pixel(current_object->texture()->get_image()->height());
                current_object->move_left_of_parent();
                if(last_object == 0) current_object->move_top_of_parent();
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
            if(a_text_image != 0) {
                unsigned int first_size = a_text_image->lines_datas().size();
                a_text_image->set_text(final_text);
                unsigned int second_size = a_text_image->lines_datas().size();

                // Calculate if lines should be removed or added
                if(first_size == second_size) {
                    a_text_image->set_text(final_text);
                    Text_Image_Line* current_line = a_text_image->line_at_position_in_plain_text(cursor_position_in_formatted_text());
                    if(current_line != 0) current_line->set_modified(true);
                }
                else {
                    int position = a_text_image->line_number_at_position_in_plain_text(cursor_position_in_formatted_text());
                    if(position != -1) {
                        // Remove the needed lines
                        for(int i = 0;i<first_size - second_size;i++) {
                            if(children()[position + i] != 0) {
                                delete children()[position + i];
                            } children().erase(children().begin() + position + i);
                            if(a_text_image->lines()[position + i] != 0) {
                                delete a_text_image->lines()[position + i];
                            } a_text_image->lines().erase(a_text_image->lines().begin() + position + i);
                        }

                        // Update the needed lines (after the line deleted)
                        for(int i = position;i<static_cast<int>(a_text_image->lines().size());i++) {
                            if(a_text_image->lines()[i] != 0) {
                                a_text_image->lines()[i]->set_modified(true);
                            }
                        }
                    }
                }
            }
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
    }

    // Update the texture of the text
    void GUI_Text_Input::update_text_texture() {
        if(attached_text_image() == 0) {
            a_text_image = window_struct().text_image_generator()->new_text_image_block("", text_image_type());
            attached_text_image()->set_use_cursor(true);
            attached_text_image()->global_style().font = font();
            attached_text_image()->set_text(text());
            attached_text_image()->set_cursor_position_in_plain_text(cursor_position_in_formatted_text());
            attached_text_image()->generate_lines();
        }
        else {
            attached_text_image()->global_style().font = font();
            attached_text_image()->set_text(text());
            attached_text_image()->set_cursor_position_in_plain_text(cursor_position_in_formatted_text());
            attached_text_image()->generate_lines(false);
        }

        // Delete the useless children modified
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(i >= static_cast<int>(attached_text_image()->lines().size())) {
                 delete children()[i]; children().erase(children().begin() + i); i--;
            }
            else if(children()[i] != 0 && (attached_text_image()->lines()[i] == 0 || attached_text_image()->lines()[i]->has_been_modified())) {
                delete children()[i]; children()[i] = 0;
            }
        }

        for(int i = 0;i<static_cast<int>(attached_text_image()->lines().size());i++) {
            // Create the configuration for the line
            Text_Image_Line* line_to_apply = attached_text_image()->lines()[i];
            if(line_to_apply != 0 && (line_to_apply->has_been_modified() || (i < static_cast<int>(children().size()) && children()[i] == 0))) {
                Image* image_to_apply = line_to_apply->image();
                if(image_to_apply != 0) {
                    line_to_apply->set_has_been_modified(false);

                    // Generate the object for the line
                    GUI_Object* new_line = new_object<GUI_Object>(name() + "_gen_" + std::to_string(a_generation) + "_line_" + std::to_string(i), 0, i * 60);
                    new_line->set_size_in_scale(glm::vec2(1.0, 0.1)); new_line->set_height_in_pixel(image_to_apply->height());
                    new_line->set_texture_alignment_horizontal(Alignment_Horizontal::H_Left);
                    new_line->texture()->set_image(line_to_apply->shared_image());

                    // Place the children
                    if(children()[i] == 0) { children()[i] = new_line; children().pop_back(); }
                }
            }
        }

        // Delete the useless children in more
        for(int i = 0;i<static_cast<int>(children().size()) - static_cast<int>(attached_text_image()->lines().size());i++) {
            delete children()[children().size() - 1]; children().pop_back();
        }

        place_lines(); a_generation++;
    };

    //*********
    //
    // GUI_File_Explorer main function
    //
    //*********

    // Most parent GUI_File_Explorer constructor used for displaying
    GUI_File_Explorer::GUI_File_Explorer(Window& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) {
        load();
    }

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
        a_browser = new_object<GUI_Object>("browser");
        a_browser->set_background_color(scls::white);
        a_browser->set_border_width_in_pixel(1);
        a_browser->set_size_in_scale(glm::vec2(0.8, 0.8));
        // Scroller of the browser of the file explorer
        a_browser_scroller = a_browser->new_object<GUI_Object>("browser_scroller");
        a_browser_scroller->set_background_color(scls::white);
        a_browser_scroller->set_position_in_scale(glm::vec2(0, 0));
        a_browser_scroller->set_size_in_scale(glm::vec2(0.95, 0.95));
        // Button to chose a file
        a_choose_button = new_object<GUI_Text>("choose_button");
        a_choose_button->set_border_width_in_pixel(1);
        a_choose_button->set_font_size(50);
        a_choose_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
        a_choose_button->set_text(choose_button_text());
        a_choose_button->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_choose_button->set_size_in_scale(glm::vec2(0.15, 0.1));
        // Final path of the file explorer
        a_final_path = new_object<GUI_Text>("final_path");
        a_final_path->set_font_size(50);
        a_final_path->set_text(final_path_text());
        a_final_path->set_texture_alignment(scls::Alignment_Texture::T_Fit);
        a_final_path->set_size_in_scale(glm::vec2(0.8, 0.1));
        // Top bar of the file explorer
        a_top_bar = new_object<GUI_Object>("top_bar");
        a_top_bar->set_background_color(scls::Color(209, 209, 209));
        a_top_bar->set_size_in_scale(glm::vec2(1.0, 0.075));
        a_top_bar->set_x_in_scale(0);

        place_all();
    }

    // Place all the elements in the file explorer
    void GUI_File_Explorer::place_all() {
        GUI_Object::after_resizing();

        // Place each object
        a_choose_button->move_bottom_of_parent();
        a_choose_button->move_left_of_parent();
        a_final_path->set_size_in_scale(glm::vec2(0.8, 0.1));
        a_final_path->move_bottom_of_parent();
        a_final_path->move_right_of_parent();
        a_browser->move_right_of_parent();
        a_browser->move_top_of_object_in_parent(a_final_path);
        a_top_bar->set_x_in_scale(0);
        a_top_bar->move_top_of_parent();

        place_browser_buttons();
        place_top_bar_buttons();
    }

    // Place correctly all the buttons in the browser
    void GUI_File_Explorer::place_browser_buttons() {
        GUI_Text* last_button = 0;
        for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
            a_browser_buttons[i]->move_left_of_parent(0.01);
            if(last_button == 0) a_browser_buttons[i]->move_top_of_parent(a_browser_y);
            else a_browser_buttons[i]->move_bottom_of_object_in_parent(last_button);
            last_button = a_browser_buttons[i];
        }
    }

    // Place correctly all the buttons in the top bar
    void GUI_File_Explorer::place_top_bar_buttons() {
        GUI_Text* last_button = 0;
        for(int i = 0;i<static_cast<int>(a_top_bar_buttons.size());i++) {
            a_top_bar_buttons[i]->collapse_size_on_texture_with_height(1.0);
            a_top_bar_buttons[i]->set_position_in_scale(glm::vec2(0, 0));
            if(last_button == 0) a_top_bar_buttons[i]->move_left_of_parent();
            else a_top_bar_buttons[i]->move_right_of_object_in_parent(last_button);
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
        unsigned char max_thread_number = 10;
        std::vector<std::string> paths = directory_content(a_current_path);
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        if(a_browser_buttons_to_modify.size() == 0) {
            // Create the buttons from scratch
            a_browser_scroller->delete_children(); a_browser_buttons.clear();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i])) continue;

                // Create the button
                paths[i] = file_name(paths[i], true);
                GUI_Text* new_button = a_browser_scroller->new_object<GUI_Text>("browser_button_" + std::to_string(i));
                new_button->set_background_color(scls::white);
                new_button->set_font_color(scls::black);
                new_button->set_font_size(50);
                new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
                new_button->set_size_in_scale(glm::vec2(1.0, 0.1));
                new_button->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
                new_button->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
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
                    std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button, button_text_reference, &move_cursor);
                    threads.push_back(current_thread);
                    current_thread_number++;
                }
                else {
                    new_button->set_text(paths[i], false);
                }
            }

            for(int j = 0;j<static_cast<int>(threads.size());j++) {
                std::thread* current_thread = threads[j];
                current_thread->join();
                delete current_thread; current_thread = 0;
                a_browser_buttons[(a_browser_buttons.size() - threads.size()) + j]->texture()->change_texture();
            } threads.clear();

            a_browser_y = 0;
        }
        else {
            // Modify some buttons
            std::vector<GUI_Text*> buttons_to_modify = std::vector<GUI_Text*>();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || !contains<unsigned int>(a_browser_buttons_to_modify, i)) continue;

                // Get the button
                GUI_Text* new_button = a_browser_buttons[i];
                std::string button_text = new_button->text();
                buttons_to_modify.push_back(new_button);

                // Change the specification of the button
                if(contains_selected_file(button_text)) {
                    new_button->set_background_color(scls::blue);
                    new_button->set_font_color(scls::white);
                }
                else {
                    new_button->set_background_color(scls::white);
                    new_button->set_font_color(scls::black);
                }

                // Create the thread
                std::string& button_text_reference = button_text;
                bool move_cursor = false;
                std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button, button_text_reference, &move_cursor);
                threads.push_back(current_thread);
            }
            a_browser_buttons_to_modify.clear();

            // Let each thread work
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                std::thread* current_thread = threads[i];
                current_thread->join();
                delete current_thread; current_thread = 0;
                buttons_to_modify[i]->texture()->change_texture();
            }
        }
        threads.clear();

        place_all();
    }

    // Update the explorer during an event
    void GUI_File_Explorer::update_event() {
        GUI_Object::update_event();

        // Check if a browser button is clicked
        for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
            std::string path = a_current_path + "/" + a_browser_buttons[i]->plain_text();
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

        // Handle wheel scrolling
        if(has_child_overflighted() ||is_overflighted()) {
            double sensibility = 0.1;

            double final_movement = sensibility * window_struct().wheel_movement_y_during_this_frame();
            if(a_browser_y + final_movement > 0) final_movement = -a_browser_y;
            if(final_movement != 0) {
                a_browser_y += final_movement;
                place_browser_buttons();
            }
        }
    }

    // Update the size of the file explorer
    void GUI_File_Explorer::update_GUI_scale() {
        place_all();
    }

    // Update the current path in the top bar
    void GUI_File_Explorer::update_top_bar() {
        a_top_bar->delete_children(); a_top_bar_buttons.clear();

        // Create the buttons
        std::string first_text = "Ce PC >";
        std::vector<std::string> path_pieces = cut_path(current_path());
        path_pieces.push_back(first_text);
        for(int i = 0;i<static_cast<int>(path_pieces.size());i++) {
            std::string button_text = path_pieces[path_pieces.size() - (1 + i)];
            if(button_text == "") continue;
            if(button_text != first_text) button_text += "/";
            GUI_Text* new_button = a_top_bar->new_object<GUI_Text>("top_bar_button_" + std::to_string(i), 0, 0);
            new_button->set_font_size(50);
            new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            new_button->set_text(button_text);
            new_button->set_height_in_pixel(25);
            new_button->set_width_in_scale(0.15);
            new_button->set_position_in_scale(glm::vec2(0, 0));
            new_button->set_texture_alignment(scls::Alignment_Texture::T_Fit_Vertically);
            new_button->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
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
        set_vao("hud_default");

        a_parent_object = new GUI_Main_Object((*reinterpret_cast<Window*>(window_struct)), "main");
    }

    // Render the page
    void GUI_Page::render(){

        // Render the object
        parent_object()->render(absolute_scale());

        // Soft reset the page
        parent_object()->soft_reset();
        soft_reset();
    };

    // Update the event of the page
    void GUI_Page::update_event() {
        Object::update_event();
        parent_object()->update_event();
    }

    // GUI_Page destructor
    GUI_Page::~GUI_Page(){
        if(a_parent_object != 0) {
            delete a_parent_object; a_parent_object = 0;
        }
    }
}
