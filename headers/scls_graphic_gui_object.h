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

#ifndef SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME
#define SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME "GUI_object"
#endif // SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    // Possibles alignment of a texture
    enum Alignment_Texture {
        T_Fit,
        T_Fit_Horizontally,
        T_Fit_Vertically,
        T_User_Defined
    };

    // Type of size definition
    enum _Size_Definition {
        Absolute_Scale_Size,
        Object_Scale_Size,
        Pixel_Size,
        Scale_Size
    };

    //*********
    //
    // Mains GUI classes
    //
    //*********

    class GUI_Object {
        // Class representing an GUI object displayed into the window
    public:

        //*********
        //
        // GUI Object main functions
        //
        //*********

        // Most basic GUI_Object constructor
        GUI_Object(Window& window, std::string name, GUI_Object* parent);
        // GUI_Object destructor
        virtual ~GUI_Object();

        // Delete a child of the object
        void delete_child(GUI_Object* object);
        // Delete the children of an object
        void delete_children();
        // Hide all the children in the object
        inline void hide_children() {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i] != 0) a_children[i]->set_visible(false);}};
        // Add a child object to the object
        template<typename O>
        O* new_object(std::string name);
        // Add a child object to the object with its position
        template<typename O>
        O* new_object(std::string name, unsigned int x, unsigned int y);
        // Render the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));

        // Hierarchy functions
        // Function called after that the window is resized
        virtual void after_resizing();
        // Function called when a child is deleted
        virtual void child_deleted(GUI_Object* child) { if(parent() != 0) parent()->child_deleted(child); };
        // Reset the object without changing it
        virtual void soft_reset() {set_focusing_state(-1);set_overflighting_state(-1);for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0){children()[i]->soft_reset();}}};
        // Update the object
        virtual void update(){ for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0 && children()[i]->visible())children()[i]->update();}};
        // Update the object for the events
        virtual void update_event() {for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0 && children()[i]->visible())children()[i]->update_event();}};
        // Update the texture when needed
        virtual void update_texture() {for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] != 0 && children()[i]->visible())children()[i]->update_texture();}};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline Color background_color() {return a_background_color;};
        inline Color border_color() {return a_border_color;};
        inline std::vector<std::shared_ptr<GUI_Object>>& children() {return a_children;};
        inline short focused_state() const {return a_focusing_state;};
        inline bool has_child_clicked_during_this_frame(unsigned int button) { return has_child_overflighted() && window_struct().mouse_button_clicked_during_this_frame(button); };
        inline bool has_child_focused() const {return a_focusing_state > 0;};
        inline bool has_child_overflighted() const {return a_overflighting_state > 0;};
        inline bool is_clicked(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked(button); };
        inline bool is_clicked_during_this_frame(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked_during_this_frame(button); };
        inline bool is_focused() const {return a_focusing_state == 0;};
        inline bool is_overflighted() const {return a_overflighting_state == 0;};
        inline std::string name() const {return a_name;};
        inline unsigned long overflighted_cursor() const {return a_overflighted_cursor;};
        inline short overflighting_state() const {return a_overflighting_state;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_border_color(Color new_color) {a_border_color = new_color;};
        inline void set_focusing_state(bool new_focusing_state) {a_focusing_state = new_focusing_state;};
        inline void set_overflighted_cursor(unsigned long new_overflighted_cursor) {a_overflighted_cursor = new_overflighted_cursor;};
        inline void set_overflighting_state(short new_overflighting_state) {a_overflighting_state = new_overflighting_state;};
        inline void set_visible(bool new_visible) {a_visible = new_visible;};
        inline bool visible() {return a_visible;};
        inline Window& window_struct() const {return a_window;};

        //*********
        //
        // Transform handling
        //
        //*********

        // Calculate the scale
        void calculate_adapted_scale(bool force = true);

        // Absolute scale plan
        // Returns the height of the object in absolute scale
        Fraction height_in_absolute_scale() const;
        // Returns the height of the object in absolute scale from the main window
        Fraction height_in_absolute_scale_and_window() const;
        // Returns the width of the object in absolute scale
        Fraction width_in_absolute_scale() const;
        // One pixel in absolute height
        inline Fraction one_pixel_height_in_absolute_scale() const {return Fraction(1, window_struct().window_height());};
        // One pixel in absolute width
        inline Fraction one_pixel_width_in_absolute_scale() const {return Fraction(1, window_struct().window_width());};
        // Returns the x of the object in absolute scale
        Fraction x_in_absolute_scale() const;
        // Returns the y of the object in absolute scale
        Fraction y_in_absolute_scale() const;
        // Setters
        inline void set_border_width_in_pixel(int new_width) {a_border_width = glm::vec4(new_width);};
        inline void set_position_in_pixel(int new_x, int new_y) {set_x_in_pixel(new_x);set_y_in_pixel(new_y);};
        inline void set_x_in_pixel(Fraction new_x) {if(a_x == new_x && a_last_x_definition == _Size_Definition::Pixel_Size)return;a_x=new_x;a_last_x_definition=_Size_Definition::Pixel_Size;a_adapted_scale_updated=true;};
        inline void set_x_in_pixel(int x) {set_x_in_pixel(Fraction(x));};
        inline void set_y_in_pixel(Fraction new_y) {if(a_y == new_y && a_last_y_definition == _Size_Definition::Pixel_Size)return;a_y=new_y;a_last_y_definition=_Size_Definition::Pixel_Size;a_adapted_scale_updated=true;};
        inline void set_y_in_pixel(int new_y) {set_y_in_pixel(Fraction(new_y));};

        // Adapted absolute scale plan
        // Returns the height in adapted absolute scale
        inline Fraction height_in_adapted_absolute_scale() const {return a_height_in_adapted_absolute_scale;};
        // Returns the width in adapted absolute scale
        inline Fraction width_in_adapted_absolute_scale() const {return a_width_in_adapted_absolute_scale;};
        // Returns the x in adapted absolute scale
        inline Fraction x_in_adapted_absolute_scale() const {return a_x_in_adapted_absolute_scale;};
        // Returns the y in adapted absolute scale
        inline Fraction y_in_adapted_absolute_scale() const {return a_y_in_adapted_absolute_scale;};

        // Object scale plan
        // Setters
        inline void set_x_in_object_scale(Fraction new_x) {if(a_x == new_x && a_last_x_definition == _Size_Definition::Object_Scale_Size)return;a_x=new_x;a_last_x_definition=_Size_Definition::Object_Scale_Size;a_adapted_scale_updated=true;};
        inline void set_y_in_object_scale(Fraction new_y) {if(a_y == new_y && a_last_y_definition == _Size_Definition::Object_Scale_Size)return;a_y=new_y;a_last_y_definition=_Size_Definition::Object_Scale_Size;a_adapted_scale_updated=true;};

        // Pixel plan
        // Returns the border in the pixel plan
        glm::vec4 border_width_in_pixel() const {return a_border_width;};
        // Returns the height of the object in pixel plan
        unsigned int height_in_pixel() const;
        // Returns if a pixel is in the object
        inline bool is_in_rect_in_pixel(int x_position, int y_position) const { return x_position >= x_in_absolute_pixel() && y_position >= y_in_absolute_pixel() && x_position < x_in_absolute_pixel() + width_in_pixel() && y_position < y_in_absolute_pixel() + height_in_pixel(); };
        // Returns the width of the object in pixel plan
        unsigned int width_in_pixel() const;
        // Returns the x of the object in absolute pixel plan
        int x_in_absolute_pixel() const {if(parent()==0)return x_in_pixel();return x_in_pixel() + parent()->x_in_absolute_pixel();};;
        // Returns the x of the object in pixel plan
        int x_in_pixel() const;
        // Returns the y of the object in absolute pixel plan
        inline int y_in_absolute_pixel() const {if(parent()==0)return y_in_pixel();return y_in_pixel() + parent()->y_in_absolute_pixel();};
        // Returns the y of the object in pixel plan
        int y_in_pixel() const;
        // Setters
        inline void set_height_in_pixel(unsigned int new_height) {if(a_height == new_height && a_last_height_definition == _Size_Definition::Pixel_Size)return;a_height=new_height;a_last_height_definition=_Size_Definition::Pixel_Size;a_adapted_scale_updated=true;};
        inline void set_width_in_pixel(unsigned int new_width) {if(a_width == new_width && a_last_width_definition == _Size_Definition::Pixel_Size)return;a_width=new_width;a_last_width_definition=_Size_Definition::Pixel_Size;a_adapted_scale_updated=true;};

        // Scale plan
        // Returns the width of the border in scale
        glm::vec4 border_width_in_scale();
        // Returns the height of the object in scale
        Fraction height_in_scale() const;
        // Returns the position of the mouse in scale
        glm::vec2 mouse_position_in_scale();
        // One pixel in height
        inline Fraction one_pixel_height_in_scale() const {if(parent()==0)return one_pixel_height_in_absolute_scale();return Fraction(1, height_in_pixel());};
        // One pixel in width
        inline Fraction one_pixel_width_in_scale() const {if(parent()==0)return one_pixel_width_in_absolute_scale();return Fraction(1, width_in_pixel());};
        // Returns the width of the object in scale
        Fraction width_in_scale() const;
        // Returns the x of the object in scale
        Fraction x_in_scale() const;
        // Returns the y of the object in scale
        Fraction y_in_scale() const;
        // Getters and setter
        inline GUI_Object* parent() const {return a_parent;};
        // Setters
        inline void set_height_in_scale(Fraction new_height) {if(a_height == new_height && a_last_height_definition == _Size_Definition::Scale_Size)return;a_height=new_height;a_last_height_definition=_Size_Definition::Scale_Size;a_adapted_scale_updated=true;};
        inline void set_position_in_scale(Fraction new_x, Fraction new_y) {set_x_in_scale(new_x);set_y_in_scale(new_y);};
        inline void set_width_in_scale(Fraction new_width) {if(a_width == new_width && a_last_width_definition == _Size_Definition::Scale_Size)return;a_width=new_width;a_last_width_definition=_Size_Definition::Scale_Size;a_adapted_scale_updated=true;};
        inline void set_x_in_scale(Fraction new_x) {if(a_x == new_x && a_last_x_definition == _Size_Definition::Scale_Size)return;a_x=new_x;a_last_x_definition=_Size_Definition::Scale_Size;a_adapted_scale_updated=true;};
        inline void set_y_in_scale(Fraction new_y) {if(a_y == new_y && a_last_y_definition == _Size_Definition::Scale_Size)return;a_y=new_y;a_last_y_definition=_Size_Definition::Scale_Size;a_adapted_scale_updated=true;};

        // Move precisely the object
        // Move the object at the bottom of its parent
        inline void move_bottom_in_parent(int offset = 0) { set_y_in_pixel(Fraction(offset)); };
        // Move the object bottom of another in the parent
        inline void move_bottom_of_object_in_parent(GUI_Object* another_object, Fraction offset = Fraction(0)) { set_y_in_pixel(Fraction(another_object->y_in_pixel()) - (Fraction(height_in_pixel()) + offset)); };
        // Move the object at the left of its parent
        inline void move_left_in_parent(int offset = 0) { set_x_in_pixel(Fraction(offset)); };
        // Move the object at the right of its parent
        inline void move_right_in_parent(int offset = 0) {
            Fraction width = window_struct().window_width();
            if(parent()!= 0) width = parent()->width_in_pixel();
            set_x_in_pixel(width - (Fraction(width_in_pixel()) + Fraction(offset)));
        };
        // Move the object at the right of another of its parent
        inline void move_right_of_object_in_parent(GUI_Object* another_object, Fraction offset = Fraction(0)) { set_x_in_pixel(Fraction(another_object->x_in_pixel() + another_object->width_in_pixel()) + offset); };
        // Move the object at the top of its parent
        inline void move_top_in_parent(int offset = 0) {
            Fraction height = window_struct().window_height();
            if(parent()!= 0) height = parent()->height_in_pixel();
            set_y_in_pixel(height - (Fraction(height_in_pixel()) + Fraction(offset)));
        };
        // Move the object top of another in the parent
        inline void move_top_of_object_in_parent(GUI_Object* another_object, Fraction offset = Fraction(0)) {
            set_y_in_pixel(Fraction(another_object->y_in_pixel() + another_object->height_in_pixel()) + offset);
        };

        //*********
        //
        // Texture handling
        //
        //*********

        // Returns the rect of the fitted texture
        glm::vec4 fitted_texture_rect() const;
        // Returns the rect of the horizontally fitted texture
        glm::vec4 fitted_horizontally_texture_rect() const;
        // Returns the rect of the vertically fitted texture
        glm::vec4 fitted_vertically_texture_rect() const;
        // Returns the first absolute extremum of the object in the Y axis
        Fraction object_absolute_y_first_extremum(bool remove_border = false);
        // Returns the last absolute extremum of the object in the Y axis
        Fraction object_absolute_y_last_extremum(bool remove_border = false);
        // Returns the extremum of the object
        glm::vec4 object_extremum();
        // Returns the height of the texture in scale of the object
        inline Fraction texture_height_in_scale() const { return Fraction(image()->height(), height_in_pixel()); };
        // Returns the rect of the texture
        inline glm::vec4 texture_rect() const {
            glm::vec4 final_texture_rect = glm::vec4(1);
            if(texture()->get_image() != 0) {
                if(texture_alignment() == Alignment_Texture::T_User_Defined) final_texture_rect = user_defined_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit) final_texture_rect = fitted_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit_Horizontally) final_texture_rect = fitted_horizontally_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit_Vertically) final_texture_rect = fitted_vertically_texture_rect();
            }
            return final_texture_rect;
        };
        // Returns the width of the texture in scale of the object
        inline Fraction texture_width_in_scale() const {return Fraction(image()->width(), width_in_pixel()); };
        // Returns the rect of user defined texture
        glm::vec4 user_defined_texture_rect() const;
        // Unload the texture
        inline void unload_texture() {a_texture.reset();};

        // Getters and setters
        inline Image* image() const {return texture()->get_image();};
        inline void set_texture_alignment(Alignment_Texture new_texture_alignment) {a_texture_alignment = new_texture_alignment;};
        inline void set_texture_alignment_horizontal(Alignment_Horizontal new_texture_alignment_horizontal) {a_texture_alignment_horizontal = new_texture_alignment_horizontal;};
        inline void set_texture_alignment_vertical(Alignment_Vertical new_texture_alignment_vertical) {a_texture_alignment_vertical = new_texture_alignment_vertical;};;
        inline Texture* texture() const {return a_texture.get();};
        inline Alignment_Texture texture_alignment() const {return a_texture_alignment;};
        inline Alignment_Horizontal texture_alignment_horizontal() const {return a_texture_alignment_horizontal;};
        inline Alignment_Vertical texture_alignment_vertical() const {return a_texture_alignment_vertical;};
        inline bool texture_fill_object() const {return a_texture_fill_object;};
        inline std::shared_ptr<Texture>& texture_shared_ptr() {return a_texture;};
        inline VAO* vao() const {return a_vao.get();};

    private:
        //*********
        //
        // GUI Object base attributes
        //
        //*********

        // Background color of the object
        Color a_background_color = Color(0, 0, 0, 0);
        // Children of this object
        std::vector<std::shared_ptr<GUI_Object>> a_children = std::vector<std::shared_ptr<GUI_Object>>();
        // The state of focusing of the object (0 == focused, > 0 == child focused)
        short a_focusing_state = -1;
        // Name of this object
        std::string a_name = "";
        // Id of the overflighted cursor
        unsigned long a_overflighted_cursor = GLFW_ARROW_CURSOR;
        // The state of overflighting of the object (0 == overflighted, > 0 == child overflighted)
        short a_overflighting_state = -1;
        // If the object is visible
        bool a_visible = true;
        // Pointer to the main window
        Window& a_window;

        //*********
        //
        // Border handling
        //
        //*********

        // Color of the border
        Color a_border_color = Color(0, 0, 0, 255);
        // Width of the border
        glm::vec4 a_border_width = glm::vec4(0, 0, 0, 0);

        //*********
        //
        // Texture handling
        //
        //*********

        // Texture of this object
        std::shared_ptr<Texture> a_texture;
        // Type of alignment of the texture
        Alignment_Texture a_texture_alignment = Alignment_Texture::T_User_Defined;
        // Horizontal alignment of the texture, if the texture does not fill the object
        Alignment_Horizontal a_texture_alignment_horizontal = Alignment_Horizontal::H_Center;
        // Vertical alignment of the texture, if the texture does not fill the object
        Alignment_Vertical a_texture_alignment_vertical = Alignment_Vertical::V_Center;
        // If the texture fill the object or not
        bool a_texture_fill_object = false;
        // VAO of this object (GUI)
        std::shared_ptr<VAO> a_vao = 0;

        //*********
        //
        // Transform handling
        //
        //*********

        // If the adapted scale is updated or not
        bool a_adapted_scale_updated = true;
        // Height of the object
        Fraction a_height = Fraction(1, 1);
        // Height of the object in absolute adapted scale
        Fraction a_height_in_adapted_absolute_scale = Fraction(1, 1);
        // Height of the object in adapted scale
        Fraction a_height_in_adapted_scale = Fraction(1, 1);
        // Last type of definition of the border width
        _Size_Definition a_last_border_width_definition_type = _Size_Definition::Pixel_Size;
        // Last type of definition of the height
        _Size_Definition a_last_height_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the width
        _Size_Definition a_last_width_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the x
        _Size_Definition a_last_x_definition = _Size_Definition::Scale_Size;
        // Last type of definition of the y
        _Size_Definition a_last_y_definition = _Size_Definition::Scale_Size;
        // Maximum y position of the object
        Fraction a_object_y_maximum = Fraction(0);
        // Minimum y position of the object
        Fraction a_object_y_minimum = Fraction(0);
        // Parent of this object
        GUI_Object* a_parent = 0;
        // Width of the object
        Fraction a_width = Fraction(1, 1);
        // Width of the object in absolute adapted scale
        Fraction a_width_in_adapted_absolute_scale = Fraction(1, 1);
        // Width of the object in adapted scale
        Fraction a_width_in_adapted_scale = Fraction(1, 1);
        // X position of the object
        Fraction a_x = Fraction(0, 1);
        // X position of the object in absolute adapted scale
        Fraction a_x_in_adapted_absolute_scale = Fraction(0, 1);
        // X position of the object in adapted scale
        Fraction a_x_in_adapted_scale = Fraction(0, 1);
        // X position of the object in real pixel
        int a_x_in_real_pixel = 0;
        // Y position of the object
        Fraction a_y = Fraction(0, 1);
        // Y position of the object in absolute adapted scale
        Fraction a_y_in_adapted_absolute_scale = Fraction(0, 1);
        // Y position of the object in adapted scale
        Fraction a_y_in_adapted_scale = Fraction(0, 1);
        // X position of the object in real pixel
        int a_y_in_real_pixel = 0;
    };

    class GUI_Main_Object : public GUI_Object {
        // Class representing the main GUI object displayed into the window
    public:

        //*********
        //
        // GUI Main Object main functions
        //
        //*********

        // Most basic GUI_Main_Object constructor
        GUI_Main_Object(Window& window, std::string name) : GUI_Object(window, name, 0) {};
        // GUI_Main_Object destructor
        virtual ~GUI_Main_Object() {};

        // Function called when a child is deleted
        virtual void child_deleted(GUI_Object* child) { if(a_focused_object == child) a_focused_object = 0; GUI_Object::child_deleted(child); };
        // Update the object for the events
        virtual void update_event();

    private:
        // Handle the focused object
        // Pointer to the focused object
        GUI_Object* a_focused_object = 0;

        // Handle the overflighted object
        // Pointer to the overflighted object
        GUI_Object* a_overflighted_object = 0;
    };

    class GUI_Scroller : public GUI_Object {
        // Class representing a simple GUI scroller displayed into the window
    public:

        //*********
        //
        // GUI_Scroller main functions
        //
        //*********

        // Most basic GUI_Scroller constructor
        GUI_Scroller(Window& window, std::string name, GUI_Object* parent);
        // GUI_Scroller destructor
        virtual ~GUI_Scroller();

        // Function called after that the window is resized
        virtual void after_resizing() {check_scroller();};
        // Check if a scroll should occurs
        void check_scroll();
        // Check the scroller object
        void check_scroller();
        // Reset the scroller
        void reset() {a_scroller_children->delete_children();};
        // Scroll the scroller on the Y axis
        void scroll_y(Fraction movement);

        // Add a child object to the scroller with its position
        template<typename O>
        O* new_object_in_scroller(std::string name, unsigned int x, unsigned int y);
        // Add a child object to the scroller
        template<typename O>
        O* new_object_in_scroller(std::string name);
        // Update the object for the events
        virtual void update_event() {GUI_Object::update_event();check_scroll();};

    private:

        //*********
        //
        // GUI_Scroller main attributes
        //
        //*********

        // Private function to create the children scroller
        GUI_Object* _create_scroller_children();
        // Scroller children which contains each elements
        GUI_Object* a_scroller_children = 0;
    };

    class __GUI_Text_Metadatas : public GUI_Object {
        // Class representing the metadatas of a graphic text
    public:

        //*********
        //
        // __GUI_Text_Metadatas main functions
        //
        //*********

        // __GUI_Text_Metadatas constructor
        __GUI_Text_Metadatas(Window& window, std::string name, GUI_Object* parent) : GUI_Object(window, name, parent) {};
        // __GUI_Text_Metadatas destructor
        virtual ~__GUI_Text_Metadatas() {};

        // Return the plain text in the object
        inline std::string plain_text(){return window_struct().text_image_generator()->plain_text(text());};
        // Return the size of the plain text in the object
        inline unsigned int plain_text_size() {return plain_text().size();};

        // Getters and setters
        inline Font font() const {return a_global_style.font;};
        inline Color font_color() const {return a_global_style.color;};
        inline std::string font_family() const {return a_global_style.font.font_family;};
        inline unsigned short font_size() const {return a_global_style.font_size;};
        inline void set_font_color(Color new_font_color) {a_global_style.color = new_font_color;};
        inline void set_font_family(std::string new_font_family) {a_global_style.font = get_system_font(new_font_family);};
        inline void set_font_size(unsigned short new_font_size) {a_global_style.font_size = new_font_size;};
        virtual void set_text(std::string new_text, bool should_move_cursor = true) {if(new_text == a_text)return;a_text = new_text;if(should_move_cursor)set_cursor_position_in_formatted_text(plain_text_size());update_texture();};
        inline void set_text_alignment_horizontal(Alignment_Horizontal new_text_alignment_horizontal) {a_global_style.alignment_horizontal = new_text_alignment_horizontal;};
        virtual void set_text_image_type(Block_Type new_text_image_type) {a_text_image_type = new_text_image_type;};
        inline void set_text_offset(double new_text_offset) {a_global_style.text_offset_x = new_text_offset;a_global_style.text_offset_y = new_text_offset;a_global_style.text_offset_width = new_text_offset;a_global_style.text_offset_height = new_text_offset;};
        inline std::string text() const {return a_text;};
        inline Block_Type text_image_type() const {return a_text_image_type;};
        inline glm::vec4 text_offset() const {return glm::vec4(a_global_style.text_offset_x, a_global_style.text_offset_y, a_global_style.text_offset_width, a_global_style.text_offset_height);};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in unformatted text
        inline unsigned int cursor_position_in_unformatted_text() const { return window_struct().text_image_generator()->plain_text_position_to_unformatted_text_position(text(), cursor_position_in_formatted_text()); };
        // Moves the cursor in the text
        void move_cursor(int movement);

        // Getters and setters
        inline unsigned int cursor_position_in_formatted_text() const {return a_cursor_position_in_formatted_text;};
        virtual void set_cursor_position_in_formatted_text(unsigned int new_cursor_position_in_formatted_text) {a_cursor_position_in_formatted_text = new_cursor_position_in_formatted_text;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

    private:

        //*********
        //
        // Text mains attributes
        //
        //*********

        // Global style of the text
        Text_Style a_global_style;
        // Text in the object
        std::string a_text = "";
        // Alignment of the text
        Alignment_Horizontal a_text_alignment_horizontal = Alignment_Horizontal::H_Left;
        // Type of the text image
        Block_Type a_text_image_type = Block_Type::BT_Always_Free_Memory;

        //*********
        //
        // Cursor handling
        //
        //*********

        // Position of the cursor in the text
        unsigned int a_cursor_position_in_formatted_text = 0;
        // If the text use a cursor or not
        bool a_use_cursor = false;
    };

    class GUI_Text : public __GUI_Text_Metadatas {
        // Class representing an GUI object displaying a text into the window
    public:

        //*********
        //
        // GUI Text main functions
        //
        //*********

        // Most basic GUI_Object constructor
        GUI_Text(Window& window, std::string name, GUI_Object* parent);
        // GUI_Object destructor
        virtual ~GUI_Text();

        // Soft reset the text
        virtual void soft_reset() {GUI_Object::soft_reset();a_text_modified_during_this_frame = false;};
        // Update the texture when needed
        virtual void update_texture() {GUI_Object::update_texture();update_text_texture();};
        // Update the texture of the text
        void update_text_texture();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Text_Image_Multi_Block* attached_text_image() {return a_text_image;};
        inline bool text_modified_during_this_frame() {return a_text_modified_during_this_frame;};
    private:
        //*********
        //
        // GUI Text base attributes
        //
        //*********

        // If the text has been modified or not
        bool a_modified = false;
        // Text image of the object
        Text_Image_Multi_Block* a_text_image = 0;
        // If the text has been modified during this frame
        bool a_text_modified_during_this_frame = false;
    };

    class GUI_Text_Input : public __GUI_Text_Metadatas {
        // Class representing an GUI object displaying and getting a text into the window
    public:

        //*********
        //
        // GUI_Text_Input main functions
        //
        //*********

        // Most basic GUI_Text_Input constructor
        GUI_Text_Input(Window& window, std::string name, GUI_Object* parent);
        // GUI_Text_Input destructor
        virtual ~GUI_Text_Input();

        // Return the plain text in the object
        inline std::string plain_text(){return window_struct().text_image_generator()->plain_text(text());};
        // Return the size of the plain text in the object
        inline unsigned int plain_text_size() {return plain_text().size();};

        // Function called after that the window is resized
        virtual void after_resizing();
        // Update the texture when needed
        virtual void update_texture() {GUI_Object::update_texture();update_text_texture();};
        // Update the texture of the text
        void update_text_texture();

        // Add a text to the input at the cursor position
        void add_text(const std::string& text_to_add);
        // Format a std::string
        std::string _format(std::string letter, bool apply_capitalisation = true);
        // Format an only letter
        std::string _format_one_letter(std::string letter, bool apply_capitalisation = true);
        // Input the inputed text
        void input_text();
        // Place the lines in the text
        void place_lines();
        // Remove a text to the input at the cursor position
        void remove_text(unsigned int size_to_delete_in_plain_text);
        // Reset the text and the text image
        void reset() {a_text_image.reset(0);a_text_image=0;a_line_offset = 0;};
        // Update the text
        virtual void update_event();
        // Update the cursor behavior
        void update_cursor();

        // Getters and setters
        inline Text_Image_Block* attached_text_image() {return a_text_image.get();};
        inline unsigned short line_offset() const {return a_line_offset;};
        virtual void set_text(std::string new_text, bool should_move_cursor = true) {if(new_text == text())return;reset();a_text_modified = true;__GUI_Text_Metadatas::set_text(new_text, should_move_cursor);};
        virtual void set_text_image_type(Block_Type new_text_image_type) {__GUI_Text_Metadatas::set_text_image_type(new_text_image_type);reset();};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Moves the cursor at a scale position
        void move_cursor_at_position_in_scale(glm::vec2 position);
        // Moves the cursor in the Y axis
        void move_cursor_y(int movement);

        // Getters and setters
        virtual void set_cursor_position_in_formatted_text(unsigned int new_cursor_position_in_formatted_text) {
            if(new_cursor_position_in_formatted_text == cursor_position_in_formatted_text()) return;

            if(a_text_image != 0) {
                Text_Image_Line* line_to_modify = a_text_image->line_at_position_in_plain_text(cursor_position_in_formatted_text());
                if(line_to_modify != 0) {
                    line_to_modify->set_modified(true);
                }
            }
            if(a_text_image != 0) {
                Text_Image_Line* line_to_modify = a_text_image->line_at_position_in_plain_text(new_cursor_position_in_formatted_text);
                if(line_to_modify != 0) {
                    line_to_modify->set_modified(true);
                }
            }
            __GUI_Text_Metadatas::set_cursor_position_in_formatted_text(new_cursor_position_in_formatted_text);
        };
    private:
        //*********
        //
        // GUI_Text_Input main atrtibutes
        //
        //*********

        // Number of the generation
        unsigned int a_generation = 0;
        // Last outputted descriptive character (^, ¨...)
        std::string a_last_descriptive_character = "";
        // Offset of the first line
        unsigned int a_line_offset = 0;
        // Text image of the object
        std::unique_ptr<Text_Image_Block> a_text_image;
        // If the text is modified or not
        bool a_text_modified = false;
    };

    class GUI_File_Explorer : public GUI_Object {
        // Class representing an GUI object to explore files
    public :
        //*********
        //
        // GUI_File_Explorer main function
        //
        //*********

        // GUI_File_Explorer most basic constructor
        GUI_File_Explorer(Window& window, std::string name, GUI_Object* parent);
        // GUI_File_Explorer destructor
        virtual ~GUI_File_Explorer();

        // Function called after that the window is resized
        virtual void after_resizing();
        // Returns if the choose button is clicked during this frame
        bool choose_clicked() const {return a_choose_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT);};
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
        // Soft reset the object
        virtual void soft_reset() {a_file_chose = false;GUI_Object::soft_reset();};
        // Update the browser of the file explorer
        void update_browser();
        // Update the explorer during an event
        virtual void update_event();
        // Update the size of the file explorer
        virtual void update_GUI_scale();
        // Update the current path in the top bar
        void update_top_bar();

        // Getters and setters
        inline std::string choose_button_text(){return a_choose_button_text;};
        inline std::string current_path() {return a_current_path;};
        inline std::string currently_selected_path() {return current_path() + "/" + a_currently_selected_files;};
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
        scls::GUI_Scroller* a_browser = 0;
        // Y position of the buttons in the browser
        double a_browser_y = 0;
        // List of every buttons in the browser
        std::vector<scls::GUI_Text*> a_browser_buttons = std::vector<scls::GUI_Text*>();
        // List of every buttons in the browser to modify
        std::vector<unsigned int> a_browser_buttons_to_modify = std::vector<unsigned int>();
        // Button to chose a file
        scls::GUI_Text* a_choose_button = 0;
        // Text in the chose button
        std::string a_choose_button_text = "Choose";
        // If tbe file has been chose or not
        bool a_file_chose = false;
        // Final path selected
        scls::GUI_Text* a_final_path = 0;
        // Text in the final path
        std::string a_final_path_text = "Final path";
        // Top bar of the explorer
        scls::GUI_Object* a_top_bar = 0;
        // List of every buttons in the top bar
        std::vector<scls::GUI_Text*> a_top_bar_buttons = std::vector<scls::GUI_Text*>();
    };

    class GUI_Page : public Object {
        // Class representing an GUI page to display on the window
    public:

        //*********
        //
        // GUI page
        //
        //*********

        // GUI_Page most basic constructor
        GUI_Page(_Window_Advanced_Struct* window_struct, std::string name);
        // GUI_Page destructor
        virtual ~GUI_Page() {};

        // Function called after that the window is resized
        virtual void after_window_resizing(glm::vec2 last_scale){
            Object::after_window_resizing(last_scale);
            parent_object()->after_resizing();
        };
        // Render the page
        virtual void render();
        // Update the event of the page
        virtual void update_event();
        // Update the page
        virtual void update(){Object::update();parent_object()->update();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline GUI_Object* focused_object() {return a_focused_object;};
        inline GUI_Object* overflighted_object() {return a_overflighted_object;};
        inline GUI_Main_Object* parent_object() {return a_parent_object.get();};
    private:
        // Handle the focused object
        // Pointer to the focused object
        GUI_Object* a_focused_object = 0;

        // Handle the overflighted object
        // Pointer to the overflighted object
        GUI_Object* a_overflighted_object = 0;

        // Parent object of the page
        std::unique_ptr<GUI_Main_Object> a_parent_object;
    };

    // Add a child object to the object
    template<typename O>
    O* GUI_Object::new_object(std::string name) {
        return new_object<O>(name, 0, 0);
    }

    // Add a child object to the object
    template<typename O>
    O* GUI_Object::new_object(std::string name, unsigned int x, unsigned int y) {
        // Create the child
        std::shared_ptr<O> new_ptr = std::make_shared<O>(window_struct(), name, this);
        children().push_back(new_ptr);
        O* new_object = new_ptr.get();

        // Configurate the child
        new_object->set_position_in_pixel(x, y);

        return new_object;
    }

    // Add a child object to the scroller with its position
    template<typename O>
    O* GUI_Scroller::new_object_in_scroller(std::string name, unsigned int x, unsigned int y) {
        // Create the new object
        O* new_object = a_scroller_children->new_object<O>(name, x, y);

        // Configurate the scroller
        check_scroller();
        return new_object;
    }

    // Add a child object to the scroller
    template<typename O>
    O* GUI_Scroller::new_object_in_scroller(std::string name) { return new_object_in_scroller<O>(name, 0, 0); }
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
