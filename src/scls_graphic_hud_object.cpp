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


        // Resize texture if necessary
        glm::vec4 final_texture_rect = texture_rect();
        if(!resize_texture_with_scale()) {
            double hud_pixel_height = height_in_pixel();
            double hud_pixel_width = width_in_pixel();

            // Set the X position
            double texture_width = texture()->get_texture_size()[0] / hud_pixel_width;
            double texture_x = 0;
            if(texture_aligmnent_horizontal() == Alignment_Horizontal::H_Center) {
                texture_x = 0.5 - texture_width / 2.0;
            }
            else if(texture_aligmnent_horizontal() == Alignment_Horizontal::H_Right) {
                texture_x = 1.0 - texture_width;
            }

            // Set the Y position
            double texture_height = texture()->get_texture_size()[1] / hud_pixel_height;
            double texture_y = 0;
            if(texture_aligmnent_vertical() == Alignment_Vertical::V_Center) {
                texture_y = 0.5 - texture_height / 2.0;
            }
            else if(texture_aligmnent_vertical() == Alignment_Vertical::V_Top) {
                texture_y = 1.0 - texture_height;
            }

            final_texture_rect = glm::vec4(texture_x, texture_y, texture_width, texture_height);
        }
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", final_texture_rect);

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
        HUD_Object::update();

        // Update the texture of the text
        update_text_texture();
    }

    // Update the text texture
    void HUD_Text::update_text_texture() {
        if(!a_modified) return;

        if(text() != "") {
            // Create the texture
            Text_Image* current_text_image = window_struct()->text_image_generator()->new_text_image(text());
            current_text_image->global_style().font_size = font_size();
            current_text_image->global_style().alignment_horizontal = text_alignment_horizontal();
            current_text_image->set_cursor_position(cursor_position());
            current_text_image->set_use_cursor(use_cursor());
            Image* new_image_texture = current_text_image->image();
            delete current_text_image; current_text_image = 0;

            // Update the texture
            texture()->set_image(new_image_texture);
        }
        else {
            texture()->set_image(new Image(1, 1, background_color(), 0));
        }
        a_modified = false;
    }

    // HUD_Text destructor
    HUD_Text::~HUD_Text() {

    }

    //*********
    //
    // HUD_Text_Input main functions
    //
    //*********

    // Most parent HUD_Text_Input constructor used for displaying
    HUD_Text_Input::HUD_Text_Input(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Text(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), transform_parent, name, texture_name, vao_name) {
        set_use_cursor(true);
    }

    // HUD_Text_Input constructor used for displaying
    HUD_Text_Input::HUD_Text_Input(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Text(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), parent, name, texture_name, vao_name) {
        set_use_cursor(true);
    }

    // HUD_Text_Input destructor
    HUD_Text_Input::~HUD_Text_Input() {

    }

    // Capitalize a std::string
    std::string HUD_Text_Input::_capitalize(std::string letter, bool apply) {
        if(!apply) return letter;

        // Alphabet letter
        if(letter == "a") return "A";
        if(letter == "b") return "B";
        if(letter == "c") return "C";
        if(letter == "d") return "D";
        if(letter == "e") return "E";
        if(letter == "f") return "F";
        if(letter == "g") return "G";
        if(letter == "h") return "H";
        if(letter == "i") return "I";
        if(letter == "j") return "J";
        if(letter == "k") return "K";
        if(letter == "l") return "L";
        if(letter == "m") return "M";
        if(letter == "n") return "N";
        if(letter == "o") return "O";
        if(letter == "p") return "P";
        if(letter == "q") return "Q";
        if(letter == "r") return "R";
        if(letter == "s") return "S";
        if(letter == "t") return "T";
        if(letter == "u") return "U";
        if(letter == "v") return "V";
        if(letter == "w") return "W";
        if(letter == "x") return "X";
        if(letter == "y") return "Y";
        if(letter == "z") return "Z";
        return letter;
    }

    // Input the inputed text
    void HUD_Text_Input::input_text() {
        if(!is_focused()) return;

        std::string final_text = text();
        std::string to_add = "";

        // Handle letters
        bool should_capitalize = (window_struct()->key_state("left shift") == Key_State::Pressed || window_struct()->key_state("right shift") == Key_State::Pressed);
        if(window_struct()->key_state_frame("a") == Key_State::Pressed) { to_add += _capitalize("a", should_capitalize);  }
        if(window_struct()->key_state_frame("b") == Key_State::Pressed) { to_add += _capitalize("b", should_capitalize);  }
        if(window_struct()->key_state_frame("c") == Key_State::Pressed) { to_add += _capitalize("c", should_capitalize);  }
        if(window_struct()->key_state_frame("d") == Key_State::Pressed) { to_add += _capitalize("d", should_capitalize);  }
        if(window_struct()->key_state_frame("e") == Key_State::Pressed) { to_add += _capitalize("e", should_capitalize);  }
        if(window_struct()->key_state_frame("f") == Key_State::Pressed) { to_add += _capitalize("f", should_capitalize);  }
        if(window_struct()->key_state_frame("g") == Key_State::Pressed) { to_add += _capitalize("g", should_capitalize);  }
        if(window_struct()->key_state_frame("h") == Key_State::Pressed) { to_add += _capitalize("h", should_capitalize);  }
        if(window_struct()->key_state_frame("i") == Key_State::Pressed) { to_add += _capitalize("i", should_capitalize);  }
        if(window_struct()->key_state_frame("j") == Key_State::Pressed) { to_add += _capitalize("j", should_capitalize);  }
        if(window_struct()->key_state_frame("k") == Key_State::Pressed) { to_add += _capitalize("k", should_capitalize);  }
        if(window_struct()->key_state_frame("l") == Key_State::Pressed) { to_add += _capitalize("l", should_capitalize);  }
        if(window_struct()->key_state_frame("m") == Key_State::Pressed) { to_add += _capitalize("m", should_capitalize);  }
        if(window_struct()->key_state_frame("n") == Key_State::Pressed) { to_add += _capitalize("n", should_capitalize);  }
        if(window_struct()->key_state_frame("o") == Key_State::Pressed) { to_add += _capitalize("o", should_capitalize);  }
        if(window_struct()->key_state_frame("p") == Key_State::Pressed) { to_add += _capitalize("p", should_capitalize);  }
        if(window_struct()->key_state_frame("q") == Key_State::Pressed) { to_add += _capitalize("q", should_capitalize);  }
        if(window_struct()->key_state_frame("r") == Key_State::Pressed) { to_add += _capitalize("r", should_capitalize);  }
        if(window_struct()->key_state_frame("s") == Key_State::Pressed) { to_add += _capitalize("s", should_capitalize);  }
        if(window_struct()->key_state_frame("t") == Key_State::Pressed) { to_add += _capitalize("t", should_capitalize);  }
        if(window_struct()->key_state_frame("u") == Key_State::Pressed) { to_add += _capitalize("u", should_capitalize);  }
        if(window_struct()->key_state_frame("v") == Key_State::Pressed) { to_add += _capitalize("v", should_capitalize);  }
        if(window_struct()->key_state_frame("w") == Key_State::Pressed) { to_add += _capitalize("w", should_capitalize);  }
        if(window_struct()->key_state_frame("x") == Key_State::Pressed) { to_add += _capitalize("x", should_capitalize);  }
        if(window_struct()->key_state_frame("y") == Key_State::Pressed) { to_add += _capitalize("y", should_capitalize);  }
        if(window_struct()->key_state_frame("z") == Key_State::Pressed) { to_add += _capitalize("z", should_capitalize);  }

        // Handle numbers
        if(window_struct()->key_state_frame("0") == Key_State::Pressed) { to_add += "0";  }
        if(window_struct()->key_state_frame("1") == Key_State::Pressed) { to_add += "1";  }
        if(window_struct()->key_state_frame("2") == Key_State::Pressed) { to_add += "2";  }
        if(window_struct()->key_state_frame("3") == Key_State::Pressed) { to_add += "3";  }
        if(window_struct()->key_state_frame("4") == Key_State::Pressed) { to_add += "4";  }
        if(window_struct()->key_state_frame("5") == Key_State::Pressed) { to_add += "5";  }
        if(window_struct()->key_state_frame("6") == Key_State::Pressed) { to_add += "6";  }
        if(window_struct()->key_state_frame("7") == Key_State::Pressed) { to_add += "7";  }
        if(window_struct()->key_state_frame("8") == Key_State::Pressed) { to_add += "8";  }
        if(window_struct()->key_state_frame("9") == Key_State::Pressed) { to_add += "9";  }

        // Handle numbers / special characters
        if(window_struct()->key_state_frame("&") == Key_State::Pressed) { to_add += _capitalize("&", should_capitalize);  }
        if(window_struct()->key_state_frame("é") == Key_State::Pressed) { to_add += _capitalize("é", should_capitalize);  }
        if(window_struct()->key_state_frame("\"") == Key_State::Pressed) { to_add += _capitalize("\"", should_capitalize);  }
        if(window_struct()->key_state_frame("'") == Key_State::Pressed) { to_add += _capitalize("'", should_capitalize);  }
        if(window_struct()->key_state_frame("(") == Key_State::Pressed) { to_add += _capitalize("(", should_capitalize);  }
        if(window_struct()->key_state_frame("-") == Key_State::Pressed) { to_add += _capitalize("-", should_capitalize);  }
        if(window_struct()->key_state_frame("è") == Key_State::Pressed) { to_add += _capitalize("è", should_capitalize);  }
        if(window_struct()->key_state_frame("_") == Key_State::Pressed) { to_add += _capitalize("_", should_capitalize);  }
        if(window_struct()->key_state_frame("ç") == Key_State::Pressed) { to_add += _capitalize("ç", should_capitalize);  }
        if(window_struct()->key_state_frame("à") == Key_State::Pressed) { to_add += _capitalize("à", should_capitalize);  }

        // Handle special characters
        if(window_struct()->key_state_frame("backspace") == Key_State::Pressed && final_text.size() > 0) {
            unsigned int size_to_delete = 1;
            if(final_text[final_text.size() - size_to_delete] == '>') {
                while(final_text[final_text.size() - size_to_delete] != '<' && size_to_delete < final_text.size() - 1) size_to_delete++;
            }
            set_cursor_position(cursor_position() - window_struct()->text_image_generator()->defined_balises()->plain_text_size(final_text.substr(final_text.size() - size_to_delete, size_to_delete)));
            final_text = final_text.substr(0, final_text.size() - size_to_delete);
        }
        if(window_struct()->key_state_frame("enter") == Key_State::Pressed) { to_add += "</br>";  }
        if(window_struct()->key_state_frame("space") == Key_State::Pressed) { to_add += " ";  }

        to_add = to_utf_8(to_add);
        set_cursor_position(cursor_position() + window_struct()->text_image_generator()->defined_balises()->plain_text_size(to_add));
        final_text += to_add;
        if(final_text != text()) set_text(final_text, false);
    }

    // Update the text
    void HUD_Text_Input::update() {
        HUD_Text::update();
        input_text();
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
    }

    // HUD_Page destructor
    HUD_Page::~HUD_Page(){

    }
}
