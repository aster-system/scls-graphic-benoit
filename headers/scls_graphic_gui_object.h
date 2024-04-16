//******************
//
// scls_graphic_gui_object.h
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
// This file contains some basics GUI built-in elements.
//

#ifndef SCLS_GRAPHIC_GUI_OBJECT
#define SCLS_GRAPHIC_GUI_OBJECT

#include "scls_graphic_window.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    class HUD_Page : public _Page {
        // Class representing an HUD page to display on the window
    public:

        //*********
        //
        // HUD page
        //
        //*********

        // HUD_Page most basic constructor
        HUD_Page(_Window_Advanced_Struct* window_struct, std::string name);
        // HUD_Page destructor
        virtual ~HUD_Page();
    };

    class HUD_Object : public Object {
        // Class representing an HUD object displayed into the window
    public:

        //*********
        //
        // HUD Object main functions
        //
        //*********

        // HUD_Object most basic constructor
        HUD_Object(_Window_Advanced_Struct* window_struct);
        // HUD_Object constructor used for displaying
        HUD_Object(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object destructor
        ~HUD_Object();

        // Convert a proportion of this object height to pixels
        inline double proportion_height_to_pixel(double proportion) {double absolute_height = transform()->absolute_scale()[1]/2.0;return (absolute_height * static_cast<double>(window_struct()->window_height())) * proportion;};
        // Convert a proportion of this object width to pixels
        inline double proportion_width_to_pixel(double proportion) {double absolute_width = (transform()->absolute_scale()[0]/2.0)*texture_ratio();return (absolute_width * (static_cast<double>(window_struct()->window_width()) / window_struct()->window_ratio())) * proportion;};
        // Render the HUD object on the window
        virtual void render();

        // Getters and setters (ONLY WITHOUT ATRIBUTES)
        inline unsigned int height_in_pixel() {double absolute_height = transform()->absolute_scale()[1]/2.0;return absolute_height * static_cast<double>(window_struct()->window_height());};
        inline glm::vec2 size_in_pixel() {return glm::vec2(width_in_pixel(), height_in_pixel());};
        inline unsigned int width_in_pixel() {double absolute_width = (transform()->absolute_scale()[0]/2.0)*texture_ratio();return absolute_width * (static_cast<double>(window_struct()->window_width()) / window_struct()->window_ratio());};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline Color background_color() {return a_background_color;};
        inline glm::vec4 border_width() {return a_border_width;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_border_width(double new_border_width) {a_border_width = glm::vec4(new_border_width, new_border_width, new_border_width, new_border_width);};
        inline void set_texture_rect(glm::vec4 new_texture_rect) {a_texture_rect = new_texture_rect;};
        inline glm::vec4 texture_rect() {return a_texture_rect;};
    private:
        //*********
        //
        // HUD Object base attributes
        //
        //*********

        // Background color of the object
        Color a_background_color = Color(0, 0, 0, 0);
        // Width of the border
        glm::vec4 a_border_width = glm::vec4(0, 0, 0, 0);
        // Rect of the texture
        glm::vec4 a_texture_rect = glm::vec4(0, 0, 1, 1);
    };

    class HUD_Text : public HUD_Object {
        // Class representing an HUD object displaying a text into the window
    public:

        //*********
        //
        // HUD Text main functions
        //
        //*********

        // HUD_Text most basic constructor
        HUD_Text(_Window_Advanced_Struct* window_struct);
        // HUD_Text constructor used for displaying
        HUD_Text(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Text destructor
        ~HUD_Text();

        // Update the text
        virtual void update();
        // Update the text texture
        void update_text_texture();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Color font_color() {return a_font_color;};
        inline std::string font_family() {return a_font_family;};
        inline unsigned short font_size() {return a_font_size;};
        inline void set_font_color(Color new_font_color) {a_font_color = new_font_color;};
        inline void set_font_family(std::string new_font_family) {a_font_family = new_font_family;a_modified = true;};
        inline void set_font_size(unsigned short new_font_size) {a_font_size = new_font_size;a_modified = true;};
        inline void set_text(std::string new_text) {if(new_text == a_text)return;a_text = new_text;a_modified = true;};
        inline void set_text_offset(double new_text_offset) {a_text_offset = glm::vec4(new_text_offset);set_texture_rect(glm::vec4(new_text_offset, new_text_offset, 1.0 - new_text_offset * 2.0, 1.0 - new_text_offset * 2.0));};
        inline void set_text_offset(glm::vec4 new_text_offset) {a_text_offset = new_text_offset;set_texture_rect(glm::vec4(new_text_offset[1], new_text_offset[0], 1.0 - (new_text_offset[1] + new_text_offset[3]), 1.0 - (new_text_offset[0] + new_text_offset[2])));};
        inline std::string text() {return a_text;};
        inline glm::vec4 text_offset() {return a_text_offset;};
    private:
        //*********
        //
        // HUD Text base attributes
        //
        //*********

        // Color of the text
        Color a_font_color = black;
        // Family of the font of the text in the object
        std::string a_font_family = "arial";
        // Size of the font of the text in the object
        unsigned short a_font_size = 50;
        // If the text has been modified or not
        bool a_modified = false;
        // Text in the object
        std::string a_text = "";
        // Offset of the text
        glm::vec4 a_text_offset = glm::vec4(0, 0, 0, 0);
    };
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
