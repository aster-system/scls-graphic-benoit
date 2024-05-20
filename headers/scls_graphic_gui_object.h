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
//******************
// The GUI Transformation system :
//
// To do a good transformation with the GUI system, there is a precise method to do it.
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

#ifndef SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME
#define SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME "GUI_object"
#endif // SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    // Type of size definition
    enum _Size_Definition {
        Absolute_Scale_Size,
        Pixel_Size,
        Scale_Size
    };

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
        // GUI_Object constructor made to the parent object of the page
        GUI_Object(Window& window, std::string name) : GUI_Object(window, name, 0) {a_is_main_object = true;};
        // GUI_Object destructor
        virtual ~GUI_Object();

        // Function called after that the window is resized
        virtual void after_window_resizing(glm::vec2 last_scale){ for(int i = 0;i<static_cast<int>(a_children.size());i++) a_children[i]->after_window_resizing(last_scale); };
        // Delete a child of the object
        void delete_child(GUI_Object* object);
        // Add a child object to the object
        template<typename O>
        O* new_object(std::string name);
        // Add a child object to the object with its position
        template<typename O>
        O* new_object(std::string name, unsigned int x, unsigned int y);
        // Render the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));
        // Reset the object without changing it
        virtual void soft_reset() {set_is_overflighted(false);set_is_focused(false);for(int i = 0;i<static_cast<int>(children().size());i++)children()[i]->soft_reset();};
        // Update the object
        virtual void update(){ for(int i = 0;i<static_cast<int>(children().size());i++)children()[i]->update(); };
        // Update the object for the events
        virtual void update_event() {for(int i = 0;i<static_cast<int>(children().size());i++)children()[i]->update_event();};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline Color background_color() {return a_background_color;};
        inline Color border_color() {return a_border_color;};
        inline std::vector<GUI_Object*>& children() {return a_children;};
        inline bool is_clicked(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked(button); };
        inline bool is_clicked_during_this_frame(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked_during_this_frame(button); };
        inline bool is_focused() const {return a_is_focused;};
        inline bool is_main_object() const {return a_is_main_object;};
        inline bool is_overflighted() const {return a_is_overflighted;};
        inline std::string name() const {return a_name;};
        inline unsigned long overflighted_cursor() {return a_overflighted_cursor;};
        inline void set_background_color(Color new_background_color) {a_background_color = new_background_color;};
        inline void set_border_color(Color new_color) {a_border_color = new_color;};
        inline void set_is_focused(bool new_is_focused) {a_is_focused = new_is_focused;};
        inline void set_is_overflighted(bool new_is_overflighted) {a_is_overflighted = new_is_overflighted;};
        inline void set_overflighted_cursor(unsigned long new_overflighted_cursor) {a_overflighted_cursor = new_overflighted_cursor;};
        inline void set_visible(bool new_visible) {a_visible = new_visible;};
        inline bool visible() {return a_visible;};
        inline Window& window_struct() const {return a_window;};

        //*********
        //
        // Transform handling
        //
        //*********

        // Absolute scale handling
        // Returns the absolute scale of the object
        inline glm::vec2 absolute_scale() const { return glm::vec2(width_in_absolute_scale(), height_in_absolute_scale()); }
        // Returns the ratio w / h of the absolute scale of the object
        inline double absolute_scale_ratio() const { return width_in_absolute_scale() / height_in_absolute_scale(); };
        // Returns the absolute scale of the border width
        glm::vec4 border_width_in_absolute_scale() const;
        // Returns the height in absolute scale of the object
        double height_in_absolute_scale() const;
        // Returns the absolute position of the object
        inline glm::vec2 position_in_absolute_scale() const {return glm::vec2(x_in_absolute_scale(), y_in_absolute_scale());};
        // Set the border width of all the side in absolute scale
        inline void set_border_width_in_absolute_scale(double new_border_width) {set_border_width_in_absolute_scale(glm::vec4(new_border_width));};
        // Set the border with in absolute scale
        inline void set_border_width_in_absolute_scale(glm::vec4 new_border_width) {a_border_width = new_border_width;a_last_border_width_definition_type = _Size_Definition::Absolute_Scale_Size;};
        // Returns the width in absolute scale of the object
        double width_in_absolute_scale() const;
        // Returns the x position in absolute scale of the object
        double x_in_absolute_scale() const;
        // Returns the y position in absolute scale of the object
        double y_in_absolute_scale() const;

        // Scale handling
        // Returns the scale of the border width
        glm::vec4 border_width_in_scale() const;
        // Returns the height in scale of the object
        double height_in_scale() const;
        // Set the height in scale
        inline void set_height_in_scale(double new_height) { a_height = new_height; a_last_height_definition = _Size_Definition::Scale_Size; };
        // Set the position in scale
        inline void set_position_in_scale(glm::vec2 new_position) { set_x_in_scale(new_position[0]); set_y_in_scale(new_position[1]); };
        // Set the size in scale
        inline void set_size_in_scale(glm::vec2 new_size) { set_height_in_scale(new_size[1]); set_width_in_scale(new_size[0]); };
        // Set the width in scale
        inline void set_width_in_scale(double new_width) { a_width = new_width; a_last_width_definition = _Size_Definition::Scale_Size; };
        // Set the x position in scale
        inline void set_x_in_scale(double new_x) { a_x = new_x; a_last_x_definition = _Size_Definition::Scale_Size; };
        // Set the y position in scale
        inline void set_y_in_scale(double new_y) { a_y = new_y; a_last_y_definition = _Size_Definition::Scale_Size; };
        // Returns the width in scale of the object
        double width_in_scale() const;
        // Returns the x position in scale of the object
        double x_in_scale() const;
        // Returns the y position in scale of the object
        double y_in_scale() const;

        // Pixel handling
        // Returns the absolute position of the object in pixel
        inline glm::vec2 absolute_position_in_pixel() {if(parent() == 0)return glm::vec2(0, 0);return position_in_pixel() + parent()->absolute_position_in_pixel();};
        // Returns the rect of the object in pixels
        inline glm::vec4 absolute_rect_in_pixel() { return glm::vec4(absolute_position_in_pixel()[0], absolute_position_in_pixel()[1], size_in_pixel()[0], size_in_pixel()[1]); };
        // Returns the height in pixel of the object
        double height_in_pixel() const;
        // Returns if a pixel is in the object
        inline bool is_in_rect_in_pixel(unsigned short x_position, unsigned short y_position) const { return x_position >= x_in_pixel() && y_position >= y_in_pixel() && x_position < x_in_pixel() + width_in_pixel(); };
        // Returns the scale of a pixel in the absolute window
        inline glm::vec2 one_pixel_in_absolute_scale() const { return glm::vec2(1.0 / static_cast<double>(window_struct().window_width()), 1.0 / static_cast<double>(window_struct().window_height())); };
        // Returns the scale of a pixel in the object
        inline glm::vec2 one_pixel_in_scale() const { return one_pixel_in_absolute_scale() / absolute_scale(); };
        // Returns the absolute position of the object in pixel
        inline glm::vec2 position_in_pixel() const {return glm::vec2(x_in_pixel(), y_in_pixel());};
        // Set the border width of all the side in pixel
        inline void set_border_width_in_pixel(double new_border_width) {set_border_width_in_pixel(glm::vec4(new_border_width));};
        // Set the border with in absolute scale
        inline void set_border_width_in_pixel(glm::vec4 new_border_width) {a_border_width = new_border_width;a_last_border_width_definition_type = _Size_Definition::Pixel_Size;};
        // Returns the size in pixel of the object
        inline glm::vec2 size_in_pixel() const {return glm::vec2(width_in_pixel(), height_in_pixel());};
        // Returns the width in pixel of the object
        double width_in_pixel() const;
        // Returns the x position in pixel of the object
        double x_in_pixel() const;
        // Returns the y position in pixel of the object
        double y_in_pixel() const;

        // Movement handling
        // Move at the bottom of an object in its parent
        inline void move_bottom_of_object_in_parent(GUI_Object* object, double offset = 0.0) { set_y_in_scale(object->y_in_absolute_scale() - (object->height_in_absolute_scale() + height_in_absolute_scale() + offset)); };
        // Move at the left the object in its parent
        inline void move_left_of_parent(double offset = 0.0) { set_x_in_scale(-1.0 + (width_in_scale() + offset)); };
        // Move at the right the object in its parent
        inline void move_right_of_parent(double offset = 0.0) { set_x_in_scale(1.0 - (width_in_scale() + offset)); };
        // Move at the top the object in its parent
        inline void move_top_of_parent(double offset = 0.0) { set_y_in_scale(1.0 - (height_in_scale() + offset)); };

        // Getters and setters
        inline GUI_Object* parent() const {return a_parent;};
        inline void set_position_in_pixel(glm::vec2 new_position) { set_x_in_pixel(new_position[0]); set_y_in_pixel(new_position[1]); };
        inline void set_height_in_pixel(double new_height) { a_height = new_height; a_last_height_definition = _Size_Definition::Pixel_Size; };
        inline void set_size_in_pixel(glm::vec2 new_size) { set_height_in_pixel(new_size[1]); set_width_in_pixel(new_size[0]); };
        inline void set_width_in_pixel(double new_width) { a_width = new_width; a_last_width_definition = _Size_Definition::Pixel_Size; };
        inline void set_x_in_pixel(double new_x) { a_x = new_x; a_last_x_definition = _Size_Definition::Pixel_Size; };
        inline void set_y_in_pixel(double new_y) { a_y = new_y; a_last_y_definition = _Size_Definition::Pixel_Size; };

        //*********
        //
        // Texture handling
        //
        //*********

        // Unload the texture
        inline void unload_texture() {if(a_texture != 0){delete a_texture; a_texture = 0;}};

        // Getters and setters
        inline Image* image() {return texture()->get_image();};
        inline void set_texture_alignment_horizontal(Alignment_Horizontal new_texture_alignment_horizontal) {a_texture_alignment_horizontal = new_texture_alignment_horizontal;};
        inline void set_texture_alignment_vertical(Alignment_Vertical new_texture_alignment_vertical) {a_texture_alignment_vertical = new_texture_alignment_vertical;};;
        inline Texture* texture() {return a_texture;};
        inline Alignment_Horizontal texture_alignment_horizontal() const {return a_texture_alignment_horizontal;};
        inline Alignment_Vertical texture_alignment_vertical() const {return a_texture_alignment_vertical;};
        inline bool texture_fill_object() {return a_texture_fill_object;};
        inline VAO* vao() {return a_vao;};

    private:
        //*********
        //
        // GUI Object base attributes
        //
        //*********

        // Background color of the object
        Color a_background_color = Color(0, 0, 0, 0);
        // Children of this object
        std::vector<GUI_Object*> a_children = std::vector<GUI_Object*>();
        // If the object is focused or not
        bool a_is_focused = false;
        // If the object is overfighted or not
        bool a_is_overflighted = false;
        // If the object is the main object of the page or not
        bool a_is_main_object = false;
        // Name of this object
        std::string a_name = "";
        // Id of the overflighted cursor
        unsigned long a_overflighted_cursor = GLFW_ARROW_CURSOR;
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
        Texture* a_texture = 0;
        // Horizontal alignment of the texture, if the texture does not fill the object
        Alignment_Horizontal a_texture_alignment_horizontal = Alignment_Horizontal::H_Center;
        // Vertical alignment of the texture, if the texture does not fill the object
        Alignment_Vertical a_texture_alignment_vertical = Alignment_Vertical::V_Center;
        // If the texture fill the object or not
        bool a_texture_fill_object = false;
        // VAO of this object (GUI)
        VAO* a_vao = 0;

        //*********
        //
        // Transform handling
        //
        //*********

        // Height of the object
        double a_height = 1.0;
        // Last type of definition of the border width
        _Size_Definition a_last_border_width_definition_type = _Size_Definition::Pixel_Size;
        // Last type of definition of the height
        _Size_Definition a_last_height_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the width
        _Size_Definition a_last_width_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the x
        _Size_Definition a_last_x_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the y
        _Size_Definition a_last_y_definition = _Size_Definition::Pixel_Size;
        // Parent of this object
        GUI_Object* a_parent = 0;
        // Position of the object in pixels
        glm::vec2 a_position = glm::vec2(0, 0);
        // Height of the object
        double a_width = 1.0;
        // X position of th eobject
        double a_x = 0.0;
        // Y position of th eobject
        double a_y = 0.0;
    };

    class GUI_Text : public GUI_Object {
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

        // Return the plain text in the object
        inline std::string plain_text(){return window_struct().text_image_generator()->plain_text(text());};
        // Return the size of the plain text in the object
        inline unsigned int plain_text_size() {return plain_text().size();};

        // Move the cursor in the text
        void move_cursor(int movement);

        // Soft reset the text
        virtual void soft_reset() {GUI_Object::soft_reset();a_text_modified_during_this_frame = false;};
        // Update the texture of the text
        void update_text_texture();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned int cursor_position() {return a_cursor_position;};
        inline Color font_color() {return a_font_color;};
        inline std::string font_family() {return a_font_family;};
        inline unsigned short font_size() {return a_font_size;};
        inline void set_cursor_position(unsigned int new_cursor_position) {a_cursor_position = new_cursor_position;};
        inline void set_font_color(Color new_font_color) {a_font_color = new_font_color;};
        inline void set_font_family(std::string new_font_family) {a_font_family = new_font_family;};
        inline void set_font_size(unsigned short new_font_size) {a_font_size = new_font_size;};
        void set_text(std::string new_text, bool move_cursor = true) {if(new_text == a_text)return;a_text = new_text;if(move_cursor)set_cursor_position(window_struct().text_image_generator()->plain_text_size(a_text));a_text_modified_during_this_frame = true;update_text_texture();};
        inline void set_text_alignment_horizontal(Alignment_Horizontal new_text_alignment_horizontal) {a_text_alignment_horizontal = new_text_alignment_horizontal;};
        inline void set_text_offset(double new_text_offset) {set_text_offset(glm::vec4(new_text_offset));};
        inline void set_text_offset(glm::vec4 new_text_offset) {a_text_offset = new_text_offset;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline std::string text() {return a_text;};
        inline Alignment_Horizontal text_alignment_horizontal() {return a_text_alignment_horizontal;};
        inline bool text_modified_during_this_frame() {return a_text_modified_during_this_frame;};
        inline glm::vec4 text_offset() {return a_text_offset;};
        inline bool use_cursor() {return a_use_cursor;};
    private:
        //*********
        //
        // GUI Text base attributes
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

    class GUI_Text_Input : public GUI_Text {
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
        // GUI_Text_Input main atrtibutes
        //
        //*********

        // Last outputted descriptive character (^, ¨...)
        std::string a_last_descriptive_character = "";
    };

    /*
    class GUI_File_Explorer : public GUI_Object {
        // Class representing an GUI object to explore files
    public :
        //*********
        //
        // GUI_File_Explorer main function
        //
        //*********

        // GUI_File_Explorer most basic constructor
        GUI_File_Explorer(_Window_Advanced_Struct* window_struct, Transform_Object* transform_object, std::string name, std::string texture_name, std::string vao_name = "GUI_default");
        // GUI_File_Explorer most basic constructor used for displaying
        GUI_File_Explorer(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "GUI_default");
        // GUI_File_Explorer destructor
        virtual ~GUI_File_Explorer();

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
        virtual void update_GUI_scale();
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
        scls::GUI_Object* a_browser = 0;
        // Scroller in the browser of the explorer
        scls::GUI_Object* a_browser_scroller = 0;
        // List of every buttons in the browser
        std::vector<scls::GUI_Text*> a_browser_buttons = std::vector<scls::GUI_Text*>();
        // List of every buttons in the browser to modify
        std::vector<unsigned int> a_browser_buttons_to_modify = std::vector<unsigned int>();
        // Button to chose a file
        scls::GUI_Text* a_choose_button = 0;
        // Text in the chose button
        std::string a_choose_button_text = "Choose";
        // Final path selected
        scls::GUI_Text* a_final_path = 0;
        // Text in the final path
        std::string a_final_path_text = "Final path";
        // Top bar of the explorer
        scls::GUI_Object* a_top_bar = 0;
        // List of every buttons in the top bar
        std::vector<scls::GUI_Text*> a_top_bar_buttons = std::vector<scls::GUI_Text*>();
    };

    // */

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
        virtual ~GUI_Page();

        // Function called after that the window is resized
        virtual void after_window_resizing(glm::vec2 last_scale){
            Object::after_window_resizing(last_scale);
            // parent_object()->set_size_in_pixel(glm::vec2(window_struct()->window_width(), window_struct()->window_height()));
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
        inline GUI_Object* parent_object() {return a_parent_object;};
    private:
        // Handle the focused object
        // Pointer to the focused object
        GUI_Object* a_focused_object = 0;

        // Handle the overflighted object
        // Pointer to the overflighted object
        GUI_Object* a_overflighted_object = 0;

        // Parent object of the page
        GUI_Object* a_parent_object = 0;
    };

    // Add a child object to the object
    template<typename O>
    O* GUI_Object::new_object(std::string name) {
        return new_object<O>(name, 0, 0);
    }

    // Add a child object to the object
    template<typename O>
    O* GUI_Object::new_object(std::string name, unsigned int x, unsigned int y) {
        O* new_object = new O(window_struct(), name, this);
        new_object->set_position_in_pixel(glm::vec2(x, y));

        children().push_back(new_object);

        return new_object;
    }
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
