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
        vao()->get_shader_program()->set_uniform4f_value("texture_rect", final_texture_rect());

        // Call the hidden render part
        _render(matrix);
    }

    // Returns the final rect of the texture
    glm::vec4 HUD_Object::final_texture_rect() {
        glm::vec2 texture_size = final_texture_size();

        // Set the X position
        double texture_x = 0;
        if(texture_aligmnent_horizontal() == Alignment_Horizontal::H_Center) {
            texture_x = 0.5 - texture_size[0] / 2.0;
        }
        else if(texture_aligmnent_horizontal() == Alignment_Horizontal::H_Right) {
            texture_x = 1.0 - texture_size[0];
        }

        // Set the Y position
        double final_texture_y = 0;
        if(texture_aligmnent_vertical() == Alignment_Vertical::V_Center) {
            final_texture_y = 0.5 - texture_size[1] / 2.0;
        }
        else if(texture_aligmnent_vertical() == Alignment_Vertical::V_Top) {
            final_texture_y = 1.0 - texture_size[1];
        }
        else if(texture_aligmnent_vertical() == Alignment_Vertical::V_User_Defined) {
            final_texture_y = texture_y();
        }

        return glm::vec4(texture_x, final_texture_y, texture_size[0], texture_size[1]);
    };

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
        else if(a_last_scale_definition_type == _Scale_Definition::Object_Scale_Width) {
            set_object_scale_width(a_last_scale[0]);
        }
        else if(a_last_scale_definition_type == _Scale_Definition::Object_Scale_Pixel) {
            set_object_scale_pixel(a_last_scale[1]);
        }

        // Check texture size
        if(a_last_texture_scale_definition_type == _Scale_Definition::Object_Scale) {
            set_texture_object_scale(a_last_texture_scale[1]);
        }
        else if(a_last_texture_scale_definition_type == _Scale_Definition::Object_Scale_Width) {
            set_texture_object_scale_width(a_last_texture_scale[0]);
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
        a_only_texture_user = true;
    }

    // HUD_Text constructor used for displaying
    HUD_Text::HUD_Text(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), parent, name, texture_name, vao_name) {
        a_only_texture_user = true;
    }

    // Move the cursor in the text
    void HUD_Text::move_cursor(int movement) {
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
        a_modified = true; update_text_texture();
    }

    // Update the text texture
    void HUD_Text::update_text_texture() {
        if(!a_modified) return;

        if(text() != "") {
            // Create the texture
            Text_Image* current_text_image = window_struct()->text_image_generator()->new_text_image(text());
            current_text_image->global_style().background_color = background_color();
            current_text_image->global_style().color = font_color();
            current_text_image->global_style().font_size = font_size();
            current_text_image->global_style().alignment_horizontal = text_alignment_horizontal();
            current_text_image->set_cursor_position(cursor_position());
            current_text_image->set_use_cursor(use_cursor());
            Image* new_image_texture = current_text_image->image();
            texture()->set_image(new_image_texture);
            delete current_text_image; current_text_image = 0;
        }
        else {
            texture()->set_image(new Image(1, 1, background_color(), 0));
        }
        a_modified = false;
        set_texture_y_max();
        update_hud_scale();
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

    // Format a char
    std::string HUD_Text_Input::_format(std::string letter, bool apply_capitalisation) {
        std::string result = "";

        for(int i = 0;i<letter.size();i++) {
            std::string to_analyse = ""; to_analyse += letter[i];
            result += _format_one_letter(to_analyse, apply_capitalisation);
        }

        return result;
    }

    // Capitalize a std::string
    std::string HUD_Text_Input::_format_one_letter(std::string letter, bool apply_capitalisation) {
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
    void HUD_Text_Input::input_text() {
        if(!is_focused()) return;

        std::string final_text = text();
        std::string to_add = "";

        // Handle letters
        bool should_capitalise = (window_struct()->key_state("left shift") == Key_State::Pressed || window_struct()->key_state("right shift") == Key_State::Pressed);
        if(window_struct()->key_state_frame("a") == Key_State::Pressed) { to_add += "a";  }
        if(window_struct()->key_state_frame("b") == Key_State::Pressed) { to_add += "b";  }
        if(window_struct()->key_state_frame("c") == Key_State::Pressed) { to_add += "c";  }
        if(window_struct()->key_state_frame("d") == Key_State::Pressed) { to_add += "d";  }
        if(window_struct()->key_state_frame("e") == Key_State::Pressed) { to_add += "e";  }
        if(window_struct()->key_state_frame("f") == Key_State::Pressed) { to_add += "f";  }
        if(window_struct()->key_state_frame("g") == Key_State::Pressed) { to_add += "g";  }
        if(window_struct()->key_state_frame("h") == Key_State::Pressed) { to_add += "h";  }
        if(window_struct()->key_state_frame("i") == Key_State::Pressed) { to_add += "i";  }
        if(window_struct()->key_state_frame("j") == Key_State::Pressed) { to_add += "j";  }
        if(window_struct()->key_state_frame("k") == Key_State::Pressed) { to_add += "k";  }
        if(window_struct()->key_state_frame("l") == Key_State::Pressed) { to_add += "l";  }
        if(window_struct()->key_state_frame("m") == Key_State::Pressed) { to_add += "m";  }
        if(window_struct()->key_state_frame("n") == Key_State::Pressed) { to_add += "n";  }
        if(window_struct()->key_state_frame("o") == Key_State::Pressed) { to_add += "o";  }
        if(window_struct()->key_state_frame("p") == Key_State::Pressed) { to_add += "p";  }
        if(window_struct()->key_state_frame("q") == Key_State::Pressed) { to_add += "q";  }
        if(window_struct()->key_state_frame("r") == Key_State::Pressed) { to_add += "r";  }
        if(window_struct()->key_state_frame("s") == Key_State::Pressed) { to_add += "s";  }
        if(window_struct()->key_state_frame("t") == Key_State::Pressed) { to_add += "t";  }
        if(window_struct()->key_state_frame("u") == Key_State::Pressed) { to_add += "u";  }
        if(window_struct()->key_state_frame("v") == Key_State::Pressed) { to_add += "v";  }
        if(window_struct()->key_state_frame("w") == Key_State::Pressed) { to_add += "w";  }
        if(window_struct()->key_state_frame("x") == Key_State::Pressed) { to_add += "x";  }
        if(window_struct()->key_state_frame("y") == Key_State::Pressed) { to_add += "y";  }
        if(window_struct()->key_state_frame("z") == Key_State::Pressed) { to_add += "z";  }
        // Extended alphabet letter
        if(window_struct()->key_state_frame("ù") == Key_State::Pressed) { to_add += "ù";  }

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
        if(window_struct()->key_state_frame("=") == Key_State::Pressed) { to_add += "=";  }

        // Handle numbers / special characters
        if(window_struct()->key_state_frame("&") == Key_State::Pressed) { to_add += "&";  }
        if(window_struct()->key_state_frame("é") == Key_State::Pressed) { to_add += "é";  }
        if(window_struct()->key_state_frame("\"") == Key_State::Pressed) { to_add += "\"";  }
        if(window_struct()->key_state_frame("'") == Key_State::Pressed) { to_add += "'";  }
        if(window_struct()->key_state_frame("(") == Key_State::Pressed) { to_add += "(";  }
        if(window_struct()->key_state_frame("-") == Key_State::Pressed) { to_add += "-";  }
        if(window_struct()->key_state_frame("è") == Key_State::Pressed) { to_add += "è";  }
        if(window_struct()->key_state_frame("_") == Key_State::Pressed) { to_add += "_";  }
        if(window_struct()->key_state_frame("ç") == Key_State::Pressed) { to_add += "ç";  }
        if(window_struct()->key_state_frame("à") == Key_State::Pressed) { to_add += "à";  }
        if(window_struct()->key_state_frame("^") == Key_State::Pressed) { to_add += "^";  }

        // Handle ponctuation
        if(window_struct()->key_state_frame(":") == Key_State::Pressed) { to_add += ":";  }
        if(window_struct()->key_state_frame(";") == Key_State::Pressed) { to_add += ";";  }
        if(window_struct()->key_state_frame(",") == Key_State::Pressed) { to_add += ",";  }
        if(window_struct()->key_state_frame("!") == Key_State::Pressed) { to_add += "!";  }

        // Handle special characters
        if(window_struct()->key_state_frame("backspace") == Key_State::Pressed && final_text.size() > 0) {
            unsigned int size_to_delete = 1;
            if(final_text[final_text.size() - size_to_delete] == '>') {
                while(final_text[final_text.size() - size_to_delete] != '<' && size_to_delete < final_text.size() - 1) size_to_delete++;
            }
            set_cursor_position(cursor_position() - window_struct()->text_image_generator()->plain_text_size(final_text.substr(final_text.size() - size_to_delete, size_to_delete)));
            final_text = final_text.substr(0, final_text.size() - size_to_delete);
        }
        if(window_struct()->key_state_frame("space") == Key_State::Pressed) { to_add += " ";  }
        if(window_struct()->key_state_frame(")") == Key_State::Pressed) { to_add += ")";  }
        if(window_struct()->key_state_frame("$") == Key_State::Pressed) { to_add += "$";  }
        to_add = _format(to_add, should_capitalise);

        if(window_struct()->key_state_frame("enter") == Key_State::Pressed) { to_add += "</br>";  }

        to_add = to_utf_8(to_add);
        set_cursor_position(cursor_position() + window_struct()->text_image_generator()->plain_text_size(to_add));
        final_text += to_add;
        if(final_text != text()) set_text(final_text, false);
    }

    // Update the text
    void HUD_Text_Input::update_event() {
        HUD_Text::update_event();
        input_text();
        update_cursor();
    }

    // Update the cursor behavior
    void HUD_Text_Input::update_cursor() {
        if(window_struct()->key_state_frame("left arrow") == Key_State::Pressed) move_cursor(-1);
        if(window_struct()->key_state_frame("right arrow") == Key_State::Pressed) move_cursor(1);
    }

    //*********
    //
    // HUD_File_Explorer main function
    //
    //*********

    // Most parent HUD_File_Explorer constructor used for displaying
    HUD_File_Explorer::HUD_File_Explorer(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), transform_parent, name, texture_name, vao_name) {
        load();
    }

    // HUD_File_Explorer constructor used for displaying
    HUD_File_Explorer::HUD_File_Explorer(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name) : HUD_Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), parent, name, texture_name, vao_name) {
        load();
    }

    // Load the explorer
    void HUD_File_Explorer::load() {
        // Browser of the file explorer
        a_browser = new_object<HUD_Object>("browser", SCLS_GRAPHIC_NO_TEXTURE);
        a_browser->set_background_color(scls::white);
        a_browser->set_pixel_border_width(1);
        a_browser->set_scale(glm::vec2(0.8, 0.8));
        // Final path of the file explorer
        a_final_path = new_object<HUD_Text>("final_path");
        a_final_path->set_font_size(50);
        a_final_path->set_text(final_path_text());
        a_final_path->set_scale(glm::vec2(0.8, 0.1));
        a_final_path->set_texture_object_scale(0.9);
        // Top bar of the file explorer
        a_top_bar = new_object<HUD_Object>("top_bar", SCLS_GRAPHIC_NO_TEXTURE);
        a_top_bar->set_background_color(scls::Color(209, 209, 209));
        a_top_bar->set_scale(glm::vec2(1.0, 0.075));

        // Place each object
        a_final_path->move_bottom_of_parent();
        a_final_path->move_right_of_parent();
        a_browser->move_right_of_parent();
        a_browser->move_top_of_object_in_parent(a_final_path);
        a_top_bar->move_top_of_parent();
    }

    // Place correctly all the buttons in the browser
    void HUD_File_Explorer::place_browser_buttons() {
        HUD_Text* last_button = 0;
        for(int i = 0;i<static_cast<int>(a_browser_buttons.size());i++) {
            a_browser_buttons[i]->set_object_scale(0.1);
            a_browser_buttons[i]->move_left_of_parent(0.01);
            if(last_button == 0) a_browser_buttons[i]->move_top_of_parent();
            else a_browser_buttons[i]->move_bottom_of_object_in_parent(last_button);
            last_button = a_browser_buttons[i];
        }
    }

    // Place correctly all the buttons in the top bar
    void HUD_File_Explorer::place_tob_bar_buttons() {
        HUD_Text* last_button = 0;
        for(int i = 0;i<static_cast<int>(a_top_bar_buttons.size());i++) {
            a_top_bar_buttons[i]->set_object_scale_pixel(25);
            if(last_button == 0) a_top_bar_buttons[i]->move_left_of_parent();
            else a_top_bar_buttons[i]->move_right_of_object_in_parent(last_button);
            last_button = a_top_bar_buttons[i];
        }
    }

    // Set the current path to a new path
    void HUD_File_Explorer::set_path(std::string path) {
        if(path[path.size() - 1] == '/' && path[path.size() - 2] != ':') path = path.substr(0, path.size() - 1);

        if(std::filesystem::is_directory(path)) {
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
    }

    // Set the file explorer to the user current document directory
    void HUD_File_Explorer::set_current_user_document_directory() {
        set_path(current_user_home_directory() + "/documents");
    }

    // Update the browser of the file explorer
    void HUD_File_Explorer::update_browser() {
        std::vector<std::string> paths = directory_content(a_current_path);
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        if(a_browser_buttons_to_modify.size() == 0) {
            // Create the buttons from scratch
            a_browser->delete_children(); a_browser_buttons.clear();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || (a_browser_buttons_to_modify.size() > 0 && contains<unsigned int>(a_browser_buttons_to_modify, i))) continue;

                // Create the button
                std::string button_text = file_name(paths[i], true);
                HUD_Text* new_button = a_browser->new_object<HUD_Text>("browser_button_" + std::to_string(i));
                new_button->set_background_color(scls::white);
                new_button->set_font_color(scls::black);
                new_button->set_font_size(50);
                new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
                a_browser_buttons.push_back(new_button);

                // Create the thread
                std::string& button_text_reference = button_text;
                bool move_cursor = false;
                std::thread* current_thread = new std::thread(&HUD_Text::set_text, new_button, button_text_reference, &move_cursor);
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
            std::vector<HUD_Text*> buttons_to_modify = std::vector<HUD_Text*>();
            for(unsigned int i = 0;i<static_cast<unsigned int>(paths.size());i++) {
                if(!std::filesystem::exists(paths[i]) || !contains<unsigned int>(a_browser_buttons_to_modify, i)) continue;

                // Get the button
                HUD_Text* new_button = a_browser_buttons[i];
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
                std::thread* current_thread = new std::thread(&HUD_Text::set_text, new_button, button_text_reference, &move_cursor);
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
        place_browser_buttons();
    }

    // Update the explorer during an event
    void HUD_File_Explorer::update_event() {
        HUD_Object::update_event();

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
                set_path(path);
                break;
            }
        }
    }

    // Update the size of the file explorer
    void HUD_File_Explorer::update_hud_scale() {
        HUD_Object::update_hud_scale();
        place_browser_buttons();
        place_tob_bar_buttons();
    }

    // Update the current path in the top bar
    void HUD_File_Explorer::update_top_bar() {
        a_top_bar->delete_children(); a_top_bar_buttons.clear();

        // Create the buttons
        std::string first_text = "Ce PC >";
        std::vector<std::string> path_pieces = cut_path(current_path());
        path_pieces.push_back(first_text);
        for(int i = 0;i<static_cast<int>(path_pieces.size());i++) {
            std::string button_text = path_pieces[path_pieces.size() - (1 + i)];
            if(button_text == "") continue;
            if(button_text != first_text) button_text += "/";
            HUD_Text* new_button = a_top_bar->new_object<HUD_Text>("top_bar_button_" + std::to_string(i));
            new_button->set_resize_texture_with_scale(false);
            new_button->set_font_size(20);
            new_button->set_overflighted_cursor(GLFW_HAND_CURSOR);
            new_button->set_text(button_text);
            new_button->set_object_scale_pixel(25);
            new_button->set_position(glm::vec2(0, 0));
            a_top_bar_buttons.push_back(new_button);
        }
        place_tob_bar_buttons();
    }

    // HUD_File_Explorer destructor
    HUD_File_Explorer::~HUD_File_Explorer() {

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
                if(current_object->visible() && current_object->is_in_pixel(window_struct()->mouse_x(), window_struct()->window_height() - window_struct()->mouse_y())) {
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

        HUD_Object::update_event();
    }

    // HUD_Page destructor
    HUD_Page::~HUD_Page(){

    }
}
