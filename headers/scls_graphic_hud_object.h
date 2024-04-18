//******************
//
// scls_graphic_hud_object.h
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
//******************
// The HUD Transformation system :
//
// To do a good transformation with the HUD system, there is a precise method to do it.
// The idea is to consider a real transformation system used in the lower tier of the library, and add a lot of various way to handle it.
// Each ways to handle it take a different differencial.
//
// For the main window, here the used referencials :
// - The direct window referencial :
//   The lowest tier transformation way, where the coordonate system always start at the bottom left at (-1;-1) and end at the top right at (1;1) in the main graphic window.
//   In this referencial, (0, 0) represent the middle of the window.
//   This is the referencial used by OpenGL at the lowest tier of the drawing.
// - The window referencial :
//   The second lowest tier transformation way, where the height of the window is always 1, but the width of the window is the ratio width / height of the window.
//   In this referencial, (0, 0) represent the middle of the window.
// - The pixel window referencial :
//   The simplest window referencial, where the height of the window is its width in pixel, and the height of the window is its height in pixel.
//   In this referencial, (0, 0) represent the bottom left of the window.
//
// For an object in the window, it's a little more complicated.
// Here the used referencials :
// - The direct referencial
//   The lowest tier transformation way, where the coordonate system always start at the bottom left at (-1;-1) and end at the top right at (1;1) in the object.
//   For using it, the scale (1, 1) correspond to its full parent and its position (0;0) is the middle of its parent.
// - The object referencial
//   The second lowest tier transformation way, where the height of the object is always 1, but the width of the object is the ratio width / height of the texture of the object.
//   For using it, its position (0;0) is the middle of its parent.
//

#ifndef SCLS_GRAPHIC_GUI_OBJECT
#define SCLS_GRAPHIC_GUI_OBJECT

#include "scls_graphic_window.h"

#ifndef SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME
#define SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME "hud_object"
#endif // SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    class HUD_Object : public Object {
        // Class representing an HUD object displayed into the window
    public:

        //*********
        //
        // HUD Object main functions
        //
        //*********

        // Most parent HUD_Object constructor used for displaying
        HUD_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object constructor used for displaying
        HUD_Object(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object destructor
        ~HUD_Object();

        // Convert a proportion of this object height to pixels
        inline double proportion_height_to_pixel(double proportion) {double absolute_height = transform()->absolute_scale()[1]/2.0;return (absolute_height * static_cast<double>(window_struct()->window_height())) * proportion;};
        // Convert a proportion of this object width to pixels
        inline double proportion_width_to_pixel(double proportion) {double absolute_width = (transform()->absolute_scale()[0]/2.0)*texture_ratio();return (absolute_width * (static_cast<double>(window_struct()->window_width()) / window_struct()->window_ratio())) * proportion;};
        // Render the HUD object on the window
        virtual void render();
        // Reset the object without changing it
        virtual void soft_reset() {Object::soft_reset();set_is_overflighted(false);};

        // Pixel size handler
        inline unsigned int absolute_hud_x_in_pixel() {double absolute_x = (1.0 + transform()->get_absolute_position()[0]) / 2.0; return static_cast<double>(window_struct()->window_width() / window_struct()->window_ratio()) * absolute_x - static_cast<double>(width_in_pixel()) / 2.0;};
        inline unsigned int absolute_x_in_pixel() {double absolute_x = (1.0 + transform()->get_absolute_position()[0]) / 2.0; return static_cast<double>(window_struct()->window_width() / window_struct()->window_ratio()) * absolute_x;};
        inline unsigned int absolute_hud_y_in_pixel() {double absolute_y = (1.0 + transform()->get_absolute_position()[1]) / 2.0; return (static_cast<double>(window_struct()->window_height()) * absolute_y) - static_cast<double>(height_in_pixel()) / 2.0;};
        inline unsigned int absolute_y_in_pixel() {double absolute_y = (1.0 + transform()->get_absolute_position()[1]) / 2.0; return static_cast<double>(window_struct()->window_height()) * absolute_y;};
        inline unsigned int height_in_pixel() {double absolute_height = transform()->absolute_scale()[1]/2.0;return absolute_height * static_cast<double>(window_struct()->window_height());};
        inline bool is_in_pixel(unsigned int x, unsigned int y) {return x > absolute_hud_x_in_pixel() && y > absolute_hud_y_in_pixel() && x < absolute_hud_x_in_pixel() + width_in_pixel() && y < absolute_hud_y_in_pixel() + height_in_pixel();};
        inline void set_width_in_pixel(unsigned int width) {
            double absolute_width = width / static_cast<double>(window_struct()->window_width());
            double final_width = (absolute_width * 2) * window_struct()->window_ratio();
            if(parent() != 0) final_width = parent_hud()->absolute_scale()[1] / final_width;
            set_scale(final_width / texture_ratio());
        };
        inline glm::vec2 size_in_pixel() {return glm::vec2(width_in_pixel(), height_in_pixel());};
        inline unsigned int width_in_pixel() {double absolute_width = (transform()->absolute_scale()[0]/2.0)*texture_ratio();return absolute_width * (static_cast<double>(window_struct()->window_width()) / window_struct()->window_ratio());};

        // Getters and setters (ONLY WITHOUT ATRIBUTES)
        inline glm::vec2 absolute_scale() {return transform()->absolute_scale();};
        inline HUD_Object* parent_hud() {if(parent() == 0 || parent()->type(1) != SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME)return 0;return reinterpret_cast<HUD_Object*>(parent());};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline Color background_color() {return a_background_color;};
        inline glm::vec4 border_width() {return a_border_width;};
        inline bool is_clicked(unsigned int button) { return is_overflighted() && window_struct()->mouse_button_clicked(button); };
        inline bool is_clicked_during_this_frame(unsigned int button) { return is_overflighted() && window_struct()->mouse_button_clicked_during_this_frame(button); };
        inline bool is_overflighted() const {return a_is_overflighted;};
        inline unsigned long overflighted_cursor() {return a_overflighted_cursor;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_border_width(double new_border_width) {a_border_width = glm::vec4(new_border_width, new_border_width, new_border_width, new_border_width);};
        virtual void set_normalized_scale(glm::vec2 new_scale) {new_scale[0] = new_scale[0] / (texture_ratio() / window_struct()->window_ratio());set_scale(glm::vec3(new_scale[0], new_scale[1], 1));};
        inline void set_is_overflighted(bool new_is_overflighted) {a_is_overflighted = new_is_overflighted;};
        inline void set_overflighted_cursor(unsigned long new_overflighted_cursor) {a_overflighted_cursor = new_overflighted_cursor;};
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
        // If the object is overfighted or not
        bool a_is_overflighted = false;
        // Id of the overflighted cursor
        unsigned long a_overflighted_cursor = GLFW_ARROW_CURSOR;
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

        // Most parent HUD_Object constructor used for displaying
        HUD_Text(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Text constructor used for displaying
        HUD_Text(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
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

        // Update the event of the page
        virtual void update_event();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline HUD_Object* overflighted_object(){return a_overflighted_object;};
    private:
        // Handle the overflighted object
        // Pointer to the overflighted object
        HUD_Object* a_overflighted_object = 0;
    };
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
