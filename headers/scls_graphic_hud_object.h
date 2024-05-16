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
//   In this referencial, the height of the window is always 1, but the width of the window is the ratio width / height of the window.
//   In this referencial, (0, 0) represent the middle of the window.
// - The pixel window referencial :
//   The simplest window referencial, where the height of the window is its width in pixel, and the height of the window is its height in pixel.
//   In this referencial, (0, 0) represent the bottom left of the window.
//
// For an object in the window, it's a little more complicated.
// Here the used referencials :
// - The direct referencial (scale())
//   The lowest tier transformation way, where the coordonate system always start at the bottom left at (-1;-1) and end at the top right at (1;1) in the object.
//   For using it, the scale (1, 1) correspond to its full parent and its position (0;0) is the middle of its parent.
// - The absolute direct referencial (absolute_scale())
//   The same referencial as the direct referencial, but not limited at its parent scale, but to the window.
//   For using it, the scale (1, 1) correspond to its the full window and its position (0;0) is the middle of the window.
//   This is the referencial used by OpenGL at the lowest tier of the drawing.
// - The square referencial (one_square_absolute_scale())
//   In this referencial, the height of the object is always 1, but the width of the object is the ratio width / height of the object, defined by the user.
//   For using it, its position (0;0) is the middle of its parent.
// - The object referencial (one_object_absolute_scale())
//   In this referencial, the height of the object is always 1, but the width of the object is the ratio width / height of the texture of the object.
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

    // Type of border width definition
    enum _Border_Width_Definition {
        Direct_Border_Width,
        Pixel_Border_Width
    };

    // Type of scale definition
    enum _Scale_Definition {
        Direct_Scale,
        Object_Scale,
        Object_Scale_Pixel,
        Object_Scale_Width
    };

    class HUD_Object : public Object {
        // Class representing an HUD object displayed into the window
    public:

        //*********
        //
        // HUD Object main functions
        //
        //*********

        // Most basic HUD_Object constructor
        HUD_Object(_Window_Advanced_Struct* window_struct, std::string name);
        // Most parent HUD_Object constructor used for displaying
        HUD_Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object constructor used for displaying
        HUD_Object(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object destructor
        virtual ~HUD_Object();

        // Function called after that the window is resized
        virtual void after_window_resizing(glm::vec2 last_scale){update_hud_scale();Object::after_window_resizing(last_scale);};
        // Convert a proportion of this object height to pixels
        inline double proportion_height_to_pixel(double proportion) {double absolute_height = transform()->absolute_scale()[1]/2.0;return (absolute_height * static_cast<double>(window_struct()->window_height())) * proportion;};
        // Convert a proportion of this object width to pixels
        inline double proportion_width_to_pixel(double proportion) {double absolute_width = (transform()->absolute_scale()[0]/2.0)*texture_ratio();return (absolute_width * (static_cast<double>(window_struct()->window_width()) / window_struct()->window_ratio())) * proportion;};
        // Render the HUD object on the window
        virtual void render();
        // Reset the object without changing it
        virtual void soft_reset() {set_is_overflighted(false);set_is_focused(false);Object::soft_reset();};
        // Update the size of the HUD elements
        virtual void update_hud_scale();

        // Returns if the ratio of the texture ratio is bigger than the square ratio
        inline bool is_texture_ratio_bigger_than_square_ratio() {
            if(texture() == 0) return false;
            return texture_ratio() > one_square_absolute_scale();
        };

        // Getters and setters (ONLY WITHOUT ATRIBUTES)
        inline glm::vec4 absolute_border_width() {
            glm::vec3 absolute_scale_for_calcul = absolute_scale();
            return (border_width() * glm::vec4(0.5)) / glm::vec4(absolute_scale_for_calcul[1], absolute_scale_for_calcul[0], absolute_scale_for_calcul[1], absolute_scale_for_calcul[0]);
        };
        inline glm::vec2 absolute_object_scale() {double absolute_width = static_cast<double>(window_struct()->window_ratio());if(parent_hud() != 0)absolute_width *= parent_hud()->absolute_scale_ratio();double new_width = absolute_scale()[1] * (texture_ratio() / absolute_width);return glm::vec2(new_width, absolute_scale()[1]);};
        inline double absolute_scale_ratio() {return absolute_scale()[0] / absolute_scale()[1];};
        inline HUD_Object* parent_hud() {if(parent() == 0 || parent()->type(1) != SCLS_GRAPHIC_HUD_OBJECT_TYPE_NAME)return 0;return reinterpret_cast<HUD_Object*>(parent());};
        inline double scale_ratio() {return scale()[0] / scale()[1];};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline glm::vec2 absolute_position() {return glm::vec2(transform()->get_absolute_position()[0], transform()->get_absolute_position()[1]);};
        inline glm::vec4 absolute_rect() {
            return glm::vec4(absolute_position()[0], absolute_position()[1], absolute_scale()[0], absolute_scale()[1]);
        };
        inline glm::vec4 absolute_rect_for_render() {
            return glm::vec4(absolute_position()[0], absolute_position()[1], absolute_scale()[0], absolute_scale()[1]);
        };
        inline glm::vec4 absolute_rect_parent_for_render() {
            if(parent_hud() == 0)return glm::vec4(0,0,2,2);
            glm::vec4 border = parent_hud()->absolute_border_width();
            glm::vec4 to_return = parent_hud()->absolute_rect() + glm::vec4(border[1], border[0], -border[1] - border[3], -border[0] - border[2]);
            return to_return;
        };
        inline Color background_color() {return a_background_color;};
        inline glm::vec4 border_width() {return a_border_width;};
        inline bool is_clicked(unsigned int button) { return is_overflighted() && window_struct()->mouse_button_clicked(button); };
        inline bool is_clicked_during_this_frame(unsigned int button) { return is_overflighted() && window_struct()->mouse_button_clicked_during_this_frame(button); };
        inline bool is_focused() {return a_is_focused;};
        inline bool is_overflighted() const {return a_is_overflighted;};
        inline unsigned long overflighted_cursor() {return a_overflighted_cursor;};
        inline glm::vec2 position() {return glm::vec2(transform()->get_position()[0], transform()->get_position()[1]);};
        virtual void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_border_width(double new_border_width) {set_border_width(glm::vec4(new_border_width));};
        inline void set_border_width(glm::vec4 new_border_width) {a_border_width = new_border_width;a_last_border_width=new_border_width;a_last_border_width_definition_type = _Border_Width_Definition::Direct_Border_Width;};
        inline void set_is_focused(bool new_is_focused) {a_is_focused = new_is_focused;};
        inline void set_is_overflighted(bool new_is_overflighted) {a_is_overflighted = new_is_overflighted;};
        virtual void set_object_scale(double new_scale) {set_object_scale(glm::vec2(new_scale));};
        virtual void set_object_scale(glm::vec2 new_scale, bool register_scaling = true) {
            glm::vec2 new_scale_unmodified = new_scale;
            new_scale[0] = new_scale[0] * one_object_absolute_scale();
            set_scale(new_scale);
            a_last_scale = new_scale_unmodified;
            a_last_scale_definition_type = _Scale_Definition::Object_Scale;
        };
        virtual void set_object_scale_width(double new_width) {
            double new_scale = new_width / one_object_absolute_scale();
            HUD_Object::set_object_scale(new_scale);
            a_last_scale = glm::vec2(new_width);
            a_last_scale_definition_type = _Scale_Definition::Object_Scale_Width;
        };
        inline void set_overflighted_cursor(unsigned long new_overflighted_cursor) {a_overflighted_cursor = new_overflighted_cursor;};
        inline void set_position(glm::vec2 new_position) {Object::set_position(glm::vec3(new_position[0], new_position[1], 0));};
        inline void set_resize_texture_with_scale(bool new_resize_texture_with_scale) {a_resize_texture_with_scale = new_resize_texture_with_scale;};
        void set_scale(glm::vec2 new_scale) {
            Object::set_scale(glm::vec3(new_scale[0], new_scale[1], 1));
            a_last_scale = new_scale;
            a_last_scale_definition_type = _Scale_Definition::Direct_Scale;
        };

        //*********
        //
        // Easy position system
        //
        //*********

        // Move the object at the bottom of an object in their parent
        inline void move_bottom_of_object_in_parent(HUD_Object* object, double y_offset = 0) {
            double new_y = object->position()[1] - (object->scale()[1] / 2.0 + scale()[1] / 2.0 + y_offset);
            set_position(glm::vec2(position()[0], new_y));
        };
        // Move the object at the bottom of its parent
        inline void move_bottom_of_parent() {double new_y = -0.5 + scale()[1] / 2.0;set_position(glm::vec2(position()[0], new_y));};
        // Move the object at the left of its parent
        inline void move_left_of_parent(double offset = 0.0) {double new_x = -0.5 + scale()[0] / 2.0 + offset;if(parent_hud()!=0){new_x -= parent_hud()->border_width()[1];}set_position(glm::vec2(new_x, position()[1]));};
        // Move the object at the right of an object in their parent
        inline void move_right_of_object_in_parent(HUD_Object* object, double x_offset = 0) {
            double new_x = object->position()[0] + (object->scale()[0] / 2.0 + scale()[0] / 2.0 + x_offset);
            set_position(glm::vec2(new_x, position()[1]));
        };
        // Move the object at the right of its parent
        inline void move_right_of_parent() {double new_x = 0.5 - scale()[0] / 2.0;if(parent_hud()!=0){new_x -= parent_hud()->border_width()[3];}set_position(glm::vec2(new_x, position()[1]));};
        // Move the object at the top of its parent
        inline void move_top_of_parent() {double new_y = 0.5 - scale()[1] / 2.0;if(parent_hud()!=0){new_y -= parent_hud()->border_width()[0];}set_position(glm::vec2(position()[0], new_y));};
        // Move the object at the top of an object in their parent
        inline void move_top_of_object_in_parent(HUD_Object* object, double y_offset = 0) {
            double new_y = object->position()[1] + (object->scale()[1] / 2.0 + scale()[1] / 2.0 + y_offset);
            set_position(glm::vec2(position()[0], new_y));
        };

        // Returns the abolute size of one object scale
        inline double one_object_absolute_scale() {
            double absolute_width = static_cast<double>(window_struct()->window_ratio());
            if(transform_parent() != 0) absolute_width *= transform_parent()->absolute_scale()[0] / transform_parent()->absolute_scale()[1];
            return texture_ratio() / absolute_width;
        };
        // Returns the abolute size of one object scale
        inline double one_square_absolute_scale() { return static_cast<double>(window_struct()->window_ratio()) * absolute_scale_ratio(); };

        // Returns the absolute HUD x position of the object in pixel
        inline unsigned int absolute_hud_x_in_pixel() {double absolute_x = (1.0 + transform()->get_absolute_position()[0]) / 2.0; return static_cast<double>(window_struct()->window_width()) * absolute_x - static_cast<double>(width_in_pixel()) / 2.0;};
        // Returns the absolute x position of the object in pixel
        inline unsigned int absolute_x_in_pixel() {double absolute_x = (1.0 + transform()->get_absolute_position()[0]) / 2.0; return static_cast<double>(window_struct()->window_width()) * absolute_x;};
        // Returns the absolute HUD y position of the obejct in pixel
        inline unsigned int absolute_hud_y_in_pixel() {double absolute_y = (1.0 + transform()->get_absolute_position()[1]) / 2.0; return (static_cast<double>(window_struct()->window_height()) * absolute_y) - static_cast<double>(height_in_pixel()) / 2.0;};
        // Returns the absolute y position of the obejct in pixel
        inline unsigned int absolute_y_in_pixel() {double absolute_y = (1.0 + transform()->get_absolute_position()[1]) / 2.0; return static_cast<double>(window_struct()->window_height()) * absolute_y;};
        // Returns the height of the object in pixel
        inline unsigned int height_in_pixel() {double absolute_height = absolute_object_scale()[1]/2.0;return absolute_height * static_cast<double>(window_struct()->window_height());};
        // Returns if a x;y position is in the object
        inline bool is_in_pixel(unsigned int x, unsigned int y) {return x > absolute_hud_x_in_pixel() && y > absolute_hud_y_in_pixel() && x < absolute_hud_x_in_pixel() + width_in_pixel() && y < absolute_hud_y_in_pixel() + height_in_pixel();};
        // Set the object scale of the object in pixel
        inline void set_object_scale_pixel(unsigned short pixel) {
            set_object_scale(one_pixel_scale_parent()[1] * static_cast<double>(pixel));
            a_last_scale = glm::vec2(pixel, pixel);

            a_last_scale_definition_type = _Scale_Definition::Object_Scale_Pixel;
        };
        // Returns the size of one pixel in the object, in absolute scale
        inline glm::vec2 one_pixel_scale() {
            return glm::vec2(static_cast<double>(2.0/(static_cast<double>(window_struct()->window_width()) * (absolute_scale()[0]))),
                             static_cast<double>(2.0/(static_cast<double>(window_struct()->window_height()) * (absolute_scale()[1]))));
        };
        // Returns the size of one pixel in the parent, in absolute scale
        inline glm::vec2 one_pixel_scale_parent() {
            if(parent_hud() == 0) {
                return glm::vec2(static_cast<double>(1.0/(static_cast<double>(window_struct()->window_width()))),
                             static_cast<double>(1.0/(static_cast<double>(window_struct()->window_height()))));
            }
            return glm::vec2(static_cast<double>(2.0/(static_cast<double>(window_struct()->window_width()) * (parent_hud()->absolute_scale()[0]))),
                             static_cast<double>(2.0/(static_cast<double>(window_struct()->window_height()) * (parent_hud()->absolute_scale()[1]))));
        };
        inline glm::vec2 pixel_scale() {
            return glm::vec2(static_cast<double>(window_struct()->window_width()) * (absolute_scale()[0] / 2.0),
                             static_cast<double>(window_struct()->window_height()) * (absolute_scale()[1] / 2.0));
        };
        inline void set_pixel_border_width(unsigned short pixel) {set_pixel_border_width(glm::vec4(pixel));};
        inline void set_pixel_border_width(glm::vec4 pixels) {
            glm::vec2 border_scale = one_pixel_scale();
            set_border_width(glm::vec4(border_scale[1] * static_cast<double>(pixels[0]), border_scale[0] * static_cast<double>(pixels[1]), border_scale[1] * static_cast<double>(pixels[2]), border_scale[0] * static_cast<double>(pixels[3])));
            a_last_border_width = pixels;
            a_last_border_width_definition_type = _Border_Width_Definition::Pixel_Border_Width;
        };
        inline glm::vec2 size_in_pixel() {return glm::vec2(width_in_pixel(), height_in_pixel());};
        // Returns the width of the object in pixel
        inline unsigned int width_in_pixel() {double absolute_width = (absolute_scale()[0]/2.0);return absolute_width * static_cast<double>(window_struct()->window_width());};

        //*********
        //
        // Texture handling
        //
        //*********

        // Final rect of the text
        glm::vec4 final_texture_rect();
        // Final size of the texture
        inline glm::vec2 final_texture_size() {
            if(texture() == 0) return glm::vec2(1, 1);
            else if(resize_texture_with_scale()) return glm::vec2(texture_rect()[2], texture_rect()[3]);
            return glm::vec2(texture()->get_texture_size()[0] / width_in_pixel(), texture()->get_texture_size()[1] / height_in_pixel());
        };
        // Change the object scale of the texture
        virtual void set_texture_object_scale(double new_texture_scale) {
            glm::vec4 new_scale = texture_rect();
            // Calculate the new scale of the texture
            new_scale[2] = (texture_ratio() * new_texture_scale) / one_square_absolute_scale();
            new_scale[3] = new_texture_scale;
            set_texture_rect(new_scale);
            a_last_texture_scale = glm::vec2(new_scale[2], new_scale[3]);
            a_last_texture_scale_definition_type = _Scale_Definition::Object_Scale;
        };
        // Change the object scale of the texture with the width
        virtual void set_texture_object_scale_width(double new_texture_scale) {
            double object_scale = (new_texture_scale * one_square_absolute_scale()) / texture_ratio();
            set_texture_object_scale(object_scale);
            a_last_texture_scale = glm::vec2(new_texture_scale, object_scale);
            a_last_texture_scale_definition_type = _Scale_Definition::Object_Scale_Width;
        };
        // Set the texture y to its max value
        inline void set_texture_y_max() { set_texture_y(texture_y_max()); };
        // Returns the max texture y value
        inline double texture_y_max() { double texture_height = final_texture_size()[1]; double final_y = 1.0 - texture_height; if(texture_height <= 1)return final_y; return final_y+=texture_height-1.0; };

        // Getters and setters
        inline bool resize_texture_with_scale() {return a_resize_texture_with_scale;};
        inline void set_texture_aligmnent_horizontal(Alignment_Horizontal new_texture_aligmnent_horizontal) {a_texture_aligmnent_horizontal = new_texture_aligmnent_horizontal;};
        inline void set_texture_alignment_vertical(Alignment_Vertical new_texture_alignment_vertical) {a_texture_aligmnent_vertical = new_texture_alignment_vertical;};
        inline void set_texture_rect(glm::vec4 new_texture_rect) {
            a_texture_rect = new_texture_rect;
            a_last_texture_scale = glm::vec2(new_texture_rect[2], new_texture_rect[3]);
            a_last_texture_scale_definition_type = _Scale_Definition::Direct_Scale;
        };
        inline void set_texture_y(double new_texture_y) {a_texture_y = new_texture_y;};
        inline Alignment_Horizontal texture_aligmnent_horizontal() {return a_texture_aligmnent_horizontal;};
        inline Alignment_Vertical texture_aligmnent_vertical() {return a_texture_aligmnent_vertical;};
        inline glm::vec4 texture_rect() {return a_texture_rect;};
        inline double texture_y() {return a_texture_y;};

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
        // If the object is focused or not
        bool a_is_focused = false;
        // If the object is overfighted or not
        bool a_is_overflighted = false;
        // Id of the overflighted cursor
        unsigned long a_overflighted_cursor = GLFW_ARROW_CURSOR;

        //*********
        //
        // Last modified attributes
        //
        //*********

        // Contains the last user defined border width
        glm::vec4 a_last_border_width = glm::vec4(0);
        // Type of the last border width definition
        _Border_Width_Definition a_last_border_width_definition_type = _Border_Width_Definition::Direct_Border_Width;
        // Contains the last user defined scale
        glm::vec2 a_last_scale = glm::vec2(1);
        // Type of the last scale definition
        _Scale_Definition a_last_scale_definition_type = _Scale_Definition::Direct_Scale;
        // Contains the last user defined texture scale
        glm::vec2 a_last_texture_scale = glm::vec2(1);
        // Type of the last scale definition
        _Scale_Definition a_last_texture_scale_definition_type = _Scale_Definition::Direct_Scale;

        //*********
        //
        // Texture handling
        //
        //*********

        // If the texture can be resized or not
        bool a_resize_texture_with_scale = true;
        // Horizontal alignment of the texture if the texture can not be resized
        Alignment_Horizontal a_texture_aligmnent_horizontal = Alignment_Horizontal::H_Center;
        // Vertical alignment of the texture if the texture can not be resized
        Alignment_Vertical a_texture_aligmnent_vertical = Alignment_Vertical::V_Top;
        // Rect of the texture
        glm::vec4 a_texture_rect = glm::vec4(0, 0, 1, 1);
        // Y position of the texture, if the texture vertical alignment is V_User_Defined
        double a_texture_y = 0;
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
        virtual ~HUD_Text();

        // Move the cursor in the text
        void move_cursor(int movement);

        // Soft reset the text
        virtual void soft_reset() {HUD_Object::soft_reset();a_text_modified_during_this_frame = false;};
        // Update the text texture
        void update_text_texture();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline std::string plain_text(){return window_struct()->text_image_generator()->plain_text(text());};
        inline unsigned int plain_text_size() {return plain_text().size();};
        virtual void set_object_scale(double new_scale) {update_text_texture();HUD_Object::set_object_scale(new_scale);};
        virtual void set_object_scale(glm::vec2 new_scale) {update_text_texture();HUD_Object::set_object_scale(new_scale);};
        virtual void set_object_scale_width(double new_width) {
            update_text_texture();
            HUD_Object::set_object_scale_width(new_width);
        };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned int cursor_position() {return a_cursor_position;};
        inline Color font_color() {return a_font_color;};
        inline std::string font_family() {return a_font_family;};
        inline unsigned short font_size() {return a_font_size;};
        virtual void set_background_color(Color new_background_color) {HUD_Object::set_background_color(new_background_color);a_modified = true;};
        inline void set_cursor_position(unsigned int new_cursor_position) {a_cursor_position = new_cursor_position;};
        inline void set_font_color(Color new_font_color) {a_font_color = new_font_color;a_modified = true;};
        inline void set_font_family(std::string new_font_family) {a_font_family = new_font_family;a_modified = true;update_text_texture();};
        inline void set_font_size(unsigned short new_font_size) {a_font_size = new_font_size;a_modified = true;update_text_texture();};
        void set_text(std::string new_text, bool move_cursor = true) {if(new_text == a_text)return;a_text = new_text;if(move_cursor)set_cursor_position(window_struct()->text_image_generator()->plain_text_size(a_text));a_modified = true;update_text_texture();a_text_modified_during_this_frame = true;};
        inline void set_text_alignment_horizontal(Alignment_Horizontal new_text_alignment_horizontal) {a_text_alignment_horizontal = new_text_alignment_horizontal;a_modified = true;update_text_texture();};
        inline void set_text_offset(double new_text_offset) {a_text_offset = glm::vec4(new_text_offset);set_texture_rect(glm::vec4(new_text_offset, new_text_offset, 1.0 - new_text_offset * 2.0, 1.0 - new_text_offset * 2.0));};
        inline void set_text_offset(glm::vec4 new_text_offset) {a_text_offset = new_text_offset;set_texture_rect(glm::vec4(new_text_offset[1], new_text_offset[0], 1.0 - (new_text_offset[1] + new_text_offset[3]), 1.0 - (new_text_offset[0] + new_text_offset[2])));};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline std::string text() {return a_text;update_text_texture();};
        inline Alignment_Horizontal text_alignment_horizontal() {return a_text_alignment_horizontal;};
        inline bool text_modified_during_this_frame() {return a_text_modified_during_this_frame;};
        inline glm::vec4 text_offset() {return a_text_offset;};
        inline bool use_cursor() {return a_use_cursor;};
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
        // Alignment of the text
        Alignment_Horizontal a_text_alignment_horizontal = Alignment_Horizontal::H_Left;
        // If the text has been modified during this frame
        bool a_text_modified_during_this_frame = false;
        // Offset of the text
        glm::vec4 a_text_offset = glm::vec4(0, 0, 0, 0);

        //*********
        //
        // Cursor handling
        //
        //*********

        // Position of the cursor in the text
        unsigned int a_cursor_position = 0;
        // If the text use a cursor or not
        bool a_use_cursor = false;
    };

    class HUD_Text_Input : public HUD_Text {
        // Class representing an HUD object displaying and getting a text into the window
    public:

        //*********
        //
        // HUD_Text_Input main functions
        //
        //*********

        // Most parent HUD_Text_Input constructor used for displaying
        HUD_Text_Input(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Text_Input constructor used for displaying
        HUD_Text_Input(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Text_Input destructor
        virtual ~HUD_Text_Input();

        // Format a std::string
        std::string _format(std::string letter, bool apply_capitalisation = true);
        // Format an only letter
        std::string _format_one_letter(std::string letter, bool apply_capitalisation = true);
        // Input the inputed text
        void input_text();
        // Update the text
        virtual void update_event();
        // Update the cursor behavior
        void update_cursor();
    private:
        //*********
        //
        // HUD_Text_Input main atrtibutes
        //
        //*********

        // Last outputted descriptive character (^, ¨...)
        std::string a_last_descriptive_character = "";
    };

    class HUD_File_Explorer : public HUD_Object {
        // Class representing an HUD object to explore files
    public :
        //*********
        //
        // HUD_File_Explorer main function
        //
        //*********

        // HUD_File_Explorer most basic constructor
        HUD_File_Explorer(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_File_Explorer most basic constructor used for displaying
        HUD_File_Explorer(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_File_Explorer destructor
        virtual ~HUD_File_Explorer();

        // Returns if the explorer contains a selected file
        inline bool contains_selected_file(std::string file) {
            std::vector<std::string> selected_files = cut_string(a_currently_selected_files, ";");
            return contains<std::string>(selected_files, file);
        };
        // Returns if a file is chosen during this frame
        bool file_chosen();
        // Load the explorer
        void load();
        // Place all the elements in the file explorer
        void place_all();
        // Place correctly all the buttons in the browser
        void place_browser_buttons();
        // Place correctly all the buttons in the top bar
        void place_top_bar_buttons();
        // Set the current path to a new path
        void set_path(std::string path);
        // Set the file explorer to the user current document directory
        void set_current_user_document_directory();
        // Update the browser of the file explorer
        void update_browser();
        // Update the explorer during an event
        virtual void update_event();
        // Update the size of the file explorer
        virtual void update_hud_scale();
        // Update the current path in the top bar
        void update_top_bar();

        // Getters and setters
        inline std::string choose_button_text(){return a_choose_button_text;};
        inline std::string current_path() {return a_current_path;};
        inline std::string final_path_text() {return a_final_path_text;};
        inline void set_choose_button_text(std::string new_choose_button_text) {a_choose_button_text = new_choose_button_text;};
        inline void set_final_path_text(std::string new_final_path_text) {a_final_path_text = new_final_path_text;};
    private:
        // Current path of the file explorer
        std::string a_current_path = "";
        // Currently selected file in the current path
        std::string a_currently_selected_files = "";

        //*********
        //
        // Annoying GUI stuff
        //
        //*********

        // Browser of the explorer
        scls::HUD_Object* a_browser = 0;
        // List of every buttons in the browser
        std::vector<scls::HUD_Text*> a_browser_buttons = std::vector<scls::HUD_Text*>();
        // List of every buttons in the browser to modify
        std::vector<unsigned int> a_browser_buttons_to_modify = std::vector<unsigned int>();
        // Button to chose a file
        scls::HUD_Text* a_choose_button = 0;
        // Text in the chose button
        std::string a_choose_button_text = "Choose";
        // Final path selected
        scls::HUD_Text* a_final_path = 0;
        // Text in the final path
        std::string a_final_path_text = "Final path";
        // Top bar of the explorer
        scls::HUD_Object* a_top_bar = 0;
        // List of every buttons in the top bar
        std::vector<scls::HUD_Text*> a_top_bar_buttons = std::vector<scls::HUD_Text*>();
    };

    class HUD_Page : public HUD_Object {
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

        // Render the page
        virtual void render();
        // Function called when the page is hidden
        virtual void after_hiding() {
            HUD_Object::after_hiding();
            // Soft reset the page
            soft_reset();

            // Update the cursor if necessary
            if(a_overflighted_object == 0) {
                window_struct()->set_cursor(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
            }

            // Check the focused object
            a_focused_object = 0;
            // Check the overflighted cursor
            a_overflighted_object = 0;
        };
        // Update the event of the page
        virtual void update_event();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline HUD_Object* focused_object() {return a_focused_object;};
        inline HUD_Object* overflighted_object() {return a_overflighted_object;};
    private:
        // Handle the focused object
        // Pointer to the focused object
        HUD_Object* a_focused_object = 0;

        // Handle the overflighted object
        // Pointer to the overflighted object
        HUD_Object* a_overflighted_object = 0;
    };
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
