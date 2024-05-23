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
        glm::mat4 matrix = glm::mat4(1.0);
        matrix = glm::translate(matrix, glm::vec3(absolute_position_to_apply[0], absolute_position_to_apply[1], 0));
        matrix = glm::scale(matrix, glm::vec3(absolute_scale_to_apply[0], absolute_scale_to_apply[1], 1) * scale_multiplier);

        // Handle the background color
        vao()->get_shader_program()->set_uniform4f_value("background_color", background_color());

        // Handle the border
        vao()->get_shader_program()->set_uniform4f_value("border_color", border_color());
        vao()->get_shader_program()->set_uniform4f_value("border_width", border_width_in_scale());

        // Handle the rect of the texture
        glm::vec4 final_texture_rect = glm::vec4(1);
        if(texture_alignment() == Alignment_Texture::T_User_Defined) final_texture_rect = user_defined_texture_rect();
        else if(texture_alignment() == Alignment_Texture::T_Fit) final_texture_rect = fitted_texture_rect();
        else if(texture_alignment() == Alignment_Texture::T_Fit_Horizontally) final_texture_rect = fitted_horizontally_texture_rect();
        else if(texture_alignment() == Alignment_Texture::T_Fit_Vertically) final_texture_rect = fitted_vertically_texture_rect();
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
        vao()->render();

        for(int i = 0;i<static_cast<int>(children().size());i++) {
            if(children()[i]->visible())children()[i]->render(scale_multiplier);
        }

        soft_reset();
    }

    // GUI_Object destructor
    GUI_Object::~GUI_Object() {
        delete_children();

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

    // Delete the children of an object
    void GUI_Object::delete_children() {
        for(int i = 0;i<static_cast<int>(children().size());i++) {
            delete children()[i];
        }
        children().clear();
    }

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
        double x_texture = 0.5 - width_texture / 2.0;
        double y_texture = 0.5 - height_texture / 2.0;
        return glm::vec4(x_texture, y_texture, width_texture, height_texture);
    }

    // Returns the height adapted for rendering
    double GUI_Object::height_in_adapted_scale() const {
        double to_return = height_in_absolute_scale();

        double divisor = one_pixel_in_absolute_scale()[1];
        unsigned int total = 0;
        while(to_return > divisor) {
            to_return -= divisor;
            total++;
        }
        if(to_return < divisor / 2.0) total++;
        to_return = total * divisor;

        return to_return;
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

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[1];
            unsigned int total = 0;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
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

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[1];
            unsigned int total = 0;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
        }

        return to_return;
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

    // Returns the width adapted for rendering
    double GUI_Object::width_in_adapted_scale() const {
        double to_return = width_in_absolute_scale();

        double divisor = one_pixel_in_absolute_scale()[0];
        unsigned int total = 0;
        while(to_return > divisor) {
            to_return -= divisor;
            total++;
        }
        if(to_return < divisor / 2.0) total++;
        to_return = total * divisor;

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

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[0];
            unsigned int total = 0;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
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

    // Returns the x adapted for rendering
    double GUI_Object::x_in_adapted_scale() const {
        double to_return = x_in_absolute_scale();
        double width_to_apply = width_in_adapted_scale();

        // Handle the pixel perfect system
        double divisor = one_pixel_in_scale()[0];
        unsigned int total = 0;
        to_return++;
        to_return -= width_to_apply;
        while(to_return > divisor) {
            to_return -= divisor;
            total++;
        }
        if(to_return < divisor / 2.0) total++;
        to_return = total * divisor;
        to_return += divisor / 5.0;
        to_return += width_to_apply;
        to_return--;

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

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[0];
            to_return++;
            unsigned int total = 0;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
            to_return--;
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
        double to_return = 0.0;

        double to_add = 0;
        if(a_last_height_definition != _Size_Definition::Absolute_Scale_Size && parent() != 0) to_add += parent()->y_in_absolute_scale();

        if(a_last_y_definition == _Size_Definition::Pixel_Size) {
            to_return = a_y * (one_pixel_in_absolute_scale()[1] * 2.0) + (-1.0 + height_in_scale());
        }
        else {
            to_return = a_y;

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[1];
            to_return++;
            unsigned int total = 0;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
            to_return--;

            if(a_last_height_definition == _Size_Definition::Scale_Size && parent() != 0) to_return *= parent()->height_in_absolute_scale();
        }

        to_return += to_add;

        return to_return;
    }

    // Returns the y adapted for rendering
    double GUI_Object::y_in_adapted_scale() const {
        double to_return = y_in_absolute_scale();
        double height_to_apply = height_in_adapted_scale();

        // Handle the pixel perfect system
        double divisor = one_pixel_in_scale()[1];
        unsigned int total = 0;
        to_return++;
        to_return -= height_to_apply;
        while(to_return > divisor) {
            to_return -= divisor;
            total++;
        }
        if(to_return < divisor / 2.0) total++;
        to_return = total * divisor;
        to_return += divisor / 5.0;
        to_return += height_to_apply;
        to_return--;

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

            // Handle the pixel perfect system
            double divisor = one_pixel_in_absolute_scale()[1];
            unsigned int total = 0;
            to_return++;
            while(to_return > divisor) {
                to_return -= divisor;
                total++;
            }
            if(to_return > divisor / 2.0) total++;
            to_return = total * divisor;
            to_return--;
        }

        return to_return;
    }

    //*********
    //
    // GUI Text main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text::GUI_Text(Window& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) {

    }

    // Move the cursor in the text
    void GUI_Text::move_cursor(int movement) {
        int final_cursor_position = cursor_position() + movement;
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
        else if(final_cursor_position == static_cast<int>(cursor_position())) return;
        set_cursor_position(final_cursor_position);
    }

    // Update the texture of the text
    void GUI_Text::update_text_texture() {
        if(text() != "") {
            // Create the text
            glm::vec2 position_to_apply = position_in_pixel();
            Text_Image* current_text_image = window_struct().text_image_generator()->new_text_image(text());
            current_text_image->global_style().background_color = background_color();
            current_text_image->global_style().color = font_color();
            current_text_image->global_style().font_size = font_size();
            current_text_image->set_cursor_position(cursor_position());
            current_text_image->set_use_cursor(use_cursor());

            // Apply the text
            Image* image_to_paste = current_text_image->image();
            texture()->set_image(image_to_paste);
            delete current_text_image; current_text_image = 0;
        }
        else {
            texture()->set_image(0);
        }
    };

    // GUI_Text destructor
    GUI_Text::~GUI_Text() {

    }

    //*********
    //
    // GUI_Text_Input main functions
    //
    //*********

    // Most basic GUI_Text constructor
    GUI_Text_Input::GUI_Text_Input(Window& window, std::string name, GUI_Object* parent) : GUI_Text(window, name, parent) {

    }

    // GUI_Text_Input destructor
    GUI_Text_Input::~GUI_Text_Input() {

    }

    // Format a char
    std::string GUI_Text_Input::_format(std::string letter, bool apply_capitalisation) {
        std::string result = "";

        for(int i = 0;i<letter.size();i++) {
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
        if(!is_focused()) return;

        std::string final_text = text();
        std::string to_add = "";

        // Handle letters
        bool should_capitalise = (window_struct().key_state("left shift") == Key_State::Pressed || window_struct().key_state("right shift") == Key_State::Pressed);
        if(window_struct().key_state_frame("a") == Key_State::Pressed) { to_add += "a";  }
        if(window_struct().key_state_frame("b") == Key_State::Pressed) { to_add += "b";  }
        if(window_struct().key_state_frame("c") == Key_State::Pressed) { to_add += "c";  }
        if(window_struct().key_state_frame("d") == Key_State::Pressed) { to_add += "d";  }
        if(window_struct().key_state_frame("e") == Key_State::Pressed) { to_add += "e";  }
        if(window_struct().key_state_frame("f") == Key_State::Pressed) { to_add += "f";  }
        if(window_struct().key_state_frame("g") == Key_State::Pressed) { to_add += "g";  }
        if(window_struct().key_state_frame("h") == Key_State::Pressed) { to_add += "h";  }
        if(window_struct().key_state_frame("i") == Key_State::Pressed) { to_add += "i";  }
        if(window_struct().key_state_frame("j") == Key_State::Pressed) { to_add += "j";  }
        if(window_struct().key_state_frame("k") == Key_State::Pressed) { to_add += "k";  }
        if(window_struct().key_state_frame("l") == Key_State::Pressed) { to_add += "l";  }
        if(window_struct().key_state_frame("m") == Key_State::Pressed) { to_add += "m";  }
        if(window_struct().key_state_frame("n") == Key_State::Pressed) { to_add += "n";  }
        if(window_struct().key_state_frame("o") == Key_State::Pressed) { to_add += "o";  }
        if(window_struct().key_state_frame("p") == Key_State::Pressed) { to_add += "p";  }
        if(window_struct().key_state_frame("q") == Key_State::Pressed) { to_add += "q";  }
        if(window_struct().key_state_frame("r") == Key_State::Pressed) { to_add += "r";  }
        if(window_struct().key_state_frame("s") == Key_State::Pressed) { to_add += "s";  }
        if(window_struct().key_state_frame("t") == Key_State::Pressed) { to_add += "t";  }
        if(window_struct().key_state_frame("u") == Key_State::Pressed) { to_add += "u";  }
        if(window_struct().key_state_frame("v") == Key_State::Pressed) { to_add += "v";  }
        if(window_struct().key_state_frame("w") == Key_State::Pressed) { to_add += "w";  }
        if(window_struct().key_state_frame("x") == Key_State::Pressed) { to_add += "x";  }
        if(window_struct().key_state_frame("y") == Key_State::Pressed) { to_add += "y";  }
        if(window_struct().key_state_frame("z") == Key_State::Pressed) { to_add += "z";  }
        // Extended alphabet letter
        if(window_struct().key_state_frame("ù") == Key_State::Pressed) { to_add += "ù";  }

        // Handle numbers
        if(window_struct().key_state_frame("0") == Key_State::Pressed) { to_add += "0";  }
        if(window_struct().key_state_frame("1") == Key_State::Pressed) { to_add += "1";  }
        if(window_struct().key_state_frame("2") == Key_State::Pressed) { to_add += "2";  }
        if(window_struct().key_state_frame("3") == Key_State::Pressed) { to_add += "3";  }
        if(window_struct().key_state_frame("4") == Key_State::Pressed) { to_add += "4";  }
        if(window_struct().key_state_frame("5") == Key_State::Pressed) { to_add += "5";  }
        if(window_struct().key_state_frame("6") == Key_State::Pressed) { to_add += "6";  }
        if(window_struct().key_state_frame("7") == Key_State::Pressed) { to_add += "7";  }
        if(window_struct().key_state_frame("8") == Key_State::Pressed) { to_add += "8";  }
        if(window_struct().key_state_frame("9") == Key_State::Pressed) { to_add += "9";  }
        if(window_struct().key_state_frame("=") == Key_State::Pressed) { to_add += "=";  }

        // Handle numbers / special characters
        if(window_struct().key_state_frame("&") == Key_State::Pressed) { to_add += "&";  }
        if(window_struct().key_state_frame("é") == Key_State::Pressed) { to_add += "é";  }
        if(window_struct().key_state_frame("\"") == Key_State::Pressed) { to_add += "\"";  }
        if(window_struct().key_state_frame("'") == Key_State::Pressed) { to_add += "'";  }
        if(window_struct().key_state_frame("(") == Key_State::Pressed) { to_add += "(";  }
        if(window_struct().key_state_frame("-") == Key_State::Pressed) { to_add += "-";  }
        if(window_struct().key_state_frame("è") == Key_State::Pressed) { to_add += "è";  }
        if(window_struct().key_state_frame("_") == Key_State::Pressed) { to_add += "_";  }
        if(window_struct().key_state_frame("ç") == Key_State::Pressed) { to_add += "ç";  }
        if(window_struct().key_state_frame("à") == Key_State::Pressed) { to_add += "à";  }
        if(window_struct().key_state_frame("^") == Key_State::Pressed) { to_add += "^";  }

        // Handle ponctuation
        if(window_struct().key_state_frame(":") == Key_State::Pressed) { to_add += ":";  }
        if(window_struct().key_state_frame(";") == Key_State::Pressed) { to_add += ";";  }
        if(window_struct().key_state_frame(",") == Key_State::Pressed) { to_add += ",";  }
        if(window_struct().key_state_frame("!") == Key_State::Pressed) { to_add += "!";  }

        // Handle special characters
        if(window_struct().key_state_frame("backspace") == Key_State::Pressed && final_text.size() > 0) {
            unsigned int size_to_delete = 1;
            if(final_text[final_text.size() - size_to_delete] == '>') {
                while(final_text[final_text.size() - size_to_delete] != '<' && size_to_delete < final_text.size() - 1) size_to_delete++;
            }
            set_cursor_position(cursor_position() - window_struct().text_image_generator()->plain_text_size(final_text.substr(final_text.size() - size_to_delete, size_to_delete)));
            final_text = final_text.substr(0, final_text.size() - size_to_delete);
        }
        if(window_struct().key_state_frame("space") == Key_State::Pressed) { to_add += " ";  }
        if(window_struct().key_state_frame(")") == Key_State::Pressed) { to_add += ")";  }
        if(window_struct().key_state_frame("$") == Key_State::Pressed) { to_add += "$";  }
        to_add = _format(to_add, should_capitalise);

        if(window_struct().key_state_frame("enter") == Key_State::Pressed) { to_add += "</br>";  }

        to_add = to_utf_8(to_add);
        final_text += to_add;
        if(final_text != text()) {
            set_cursor_position(cursor_position() + window_struct().text_image_generator()->plain_text_size(to_add));
            set_text(final_text, false);
        }
    }

    // Update the text
    void GUI_Text_Input::update_event() {
        GUI_Text::update_event();
        input_text();
        update_cursor();
    }

    // Update the cursor behavior
    void GUI_Text_Input::update_cursor() {
        if(window_struct().key_state_frame("left arrow") == Key_State::Pressed) move_cursor(-1);
        if(window_struct().key_state_frame("right arrow") == Key_State::Pressed) move_cursor(1);
    }

    //*********
    //
    // GUI_File_Explorer main function
    //
    //*********

    // Most parent GUI_File_Explorer constructor used for displaying
    GUI_File_Explorer::GUI_File_Explorer(Window& window, std::string name, GUI_Object* parent) : GUI_Object(window, name) {
        load();
    }

    // Returns if a file is chosen during this frame
    bool GUI_File_Explorer::file_chosen() {
        if(!a_choose_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) return false;
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
            if(last_button == 0) a_browser_buttons[i]->move_top_of_parent();
            else a_browser_buttons[i]->move_bottom_of_object_in_parent(last_button);
            last_button = a_browser_buttons[i];
        }
    }

    // Place correctly all the buttons in the top bar
    void GUI_File_Explorer::place_top_bar_buttons() {
        GUI_Text* last_button = 0;
        for(int i = 0;i<static_cast<int>(a_top_bar_buttons.size());i++) {
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

        if(std::filesystem::is_directory(path) && (a_current_path == "" || contains_selected_file(file_name(path)))) {
            // Handle a directory
            a_browser_buttons_to_modify.clear();
            a_current_path = path;
            a_currently_selected_files = "";
            update_browser();
            update_top_bar();
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
        a_final_path->set_text(final_path_text() + " : " + replace(path, "/", "\\"));
        place_all();
    }

    // Set the file explorer to the user current document directory
    void GUI_File_Explorer::set_current_user_document_directory() { set_path(current_user_document_directory()); }

    // Update the browser of the file explorer
    void GUI_File_Explorer::update_browser() {
        std::vector<std::string> paths = directory_content(a_current_path);
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        if(a_browser_buttons_to_modify.size() == 0) {
            // Create the buttons from scratch
            a_browser_scroller->delete_children(); a_browser_buttons.clear();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || (a_browser_buttons_to_modify.size() > 0 && contains<unsigned int>(a_browser_buttons_to_modify, i))) continue;

                // Create the button
                std::string button_text = file_name(paths[i], true);
                GUI_Text* new_button = a_browser_scroller->new_object<GUI_Text>("browser_button_" + std::to_string(i));
                new_button->set_background_color(scls::white);
                new_button->set_font_color(scls::black);
                new_button->set_font_size(50);
                new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
                new_button->set_size_in_scale(glm::vec2(1.0, 0.1));
                new_button->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
                a_browser_buttons.push_back(new_button);

                // Create the thread
                std::string& button_text_reference = button_text;
                bool move_cursor = false;
                std::thread* current_thread = new std::thread(&GUI_Text::set_text, new_button, button_text_reference, &move_cursor);
                threads.push_back(current_thread);
            }

            // Let each thread work
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                std::thread* current_thread = threads[i];
                current_thread->join();
                delete current_thread; current_thread = 0;
                a_browser_buttons[i]->texture()->change_texture();
            }
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
            new_button->set_font_size(20);
            new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            new_button->set_text(button_text);
            new_button->set_height_in_pixel(25);
            new_button->set_width_in_scale(0.15);
            new_button->set_position_in_scale(glm::vec2(0, 0));
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

        a_parent_object = new GUI_Object((*reinterpret_cast<Window*>(window_struct)), "main");
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

        // Check the overflighted cursor
        GUI_Object* current_overflighted_object = parent_object();
        for(int i = 0;i<static_cast<int>(current_overflighted_object->children().size());i++) {
            GUI_Object* current_object = current_overflighted_object->children()[i];
            if(current_object->visible() && current_object->is_in_rect_in_pixel(window_struct()->mouse_x(), window_struct()->window_height() - window_struct()->mouse_y())) {
                current_overflighted_object = current_object;
                i = -1;
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

        // Check the focused object
        if(window_struct()->mouse_button_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT)) {
            if(a_overflighted_object != 0) {
                a_focused_object = a_overflighted_object;
            }
            else {
                a_focused_object = 0;
            }
        }
        if(a_focused_object != 0) {
            a_focused_object->set_is_focused(true);
        }

        parent_object()->update_event();
    }

    // GUI_Page destructor
    GUI_Page::~GUI_Page(){
        if(a_parent_object != 0) {
            delete a_parent_object; a_parent_object = 0;
        }
    }
}
