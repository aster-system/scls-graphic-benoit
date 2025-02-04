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

#include "scls_graphic_gui_base.h"

#ifndef SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME
#define SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME "GUI_object"
#endif // SCLS_GRAPHIC_GUI_OBJECT_TYPE_NAME

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // Mains GUI classes
    //
    //*********

    // Balises for a GUI loading
    extern std::shared_ptr<__Balise_Container> gui_loading_balises;

    struct __GUI_Page_Loader {
        // Class containing datas about a loaded GUI page

        // Created objects by name
        std::map<std::string, std::shared_ptr<__GUI_Object_Core>> created_objects = std::map<std::string, std::shared_ptr<__GUI_Object_Core>>();
        // Path of the loaded file
        std::string path = std::string("");
    };

    struct GUI_Style : public __GUI_Style_Transformation {
        // Struct representating the complete style of a GUI

        // Background color of the object
        Color a_background_color = Color(0, 0, 0, 0);
        // Color of the border
        Color a_border_color = Color(0, 0, 0, 255);
    };

    class GUI_Object : public __GUI_Object_Core {
        // Class representing an GUI object displayed into the window
    public:

        //*********
        //
        // GUI Object main functions
        //
        //*********

        // Most basic GUI_Object constructor
        GUI_Object(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent);
        // GUI_Object destructor
        virtual ~GUI_Object();

        // Returns a child by its name
        template <typename T = GUI_Object>
        inline T* child_by_name(std::string child_name) const {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i] != 0 && a_children[i].get()->name() == child_name) return reinterpret_cast<T*>(a_children[i].get());}return 0;};
        // Returns a child shared pointer by its name
        inline std::shared_ptr<GUI_Object>* child_by_name_shared_ptr(std::string child_name) {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i] != 0 && a_children[i].get()->name() == child_name) return &a_children[i];}return 0;};
        // Function called when a child is deleted
        virtual void child_deleted(GUI_Object* child) { if(parent() != 0) parent()->child_deleted(child); };
        // Deletes a child of the object and returns if the child has been correctly deleted
        bool delete_child(GUI_Object* object);
        // Delete the children of an object
        void delete_children();
        // Hide all the children in the object
        inline void hide_children() {for(int i = 0;i<static_cast<int>(a_children.size());i++) {if(a_children[i] != 0) a_children[i]->set_visible(false);}};
        // Add a child object to the object
        template<typename O>
        std::shared_ptr<O>* new_object(std::string name);
        // Add a child object to the object with its position
        template<typename O>
        std::shared_ptr<O>* new_object(std::string name, unsigned int x, unsigned int y);
        // Render the object
        virtual void render(glm::vec3 scale_multiplier = glm::vec3(1, 1, 1));

        // Hierarchy functions
        // Function called after the creation of the object
        virtual void after_creation(){};
        // Function called after that the window is resized
        virtual void after_resizing();
        // Function called after an XML loading
        virtual void after_xml_loading() {for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i].get()!=0)children()[i].get()->after_xml_loading();}};
        // Reset the object without changing it
        virtual void soft_reset() {set_focusing_state(-1);set_overflighting_state(-1);for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0){children()[i]->soft_reset();}}};
        // Update the object
        virtual void update(){ for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0 && children()[i]->visible())children()[i]->update();}};
        // Update the object for the events
        virtual void update_event() {for(int i = 0;i<static_cast<int>(children().size());i++){if(children()[i] != 0 && children()[i]->visible())children()[i]->update_event();}};
        // Update the texture when needed
        virtual void update_texture() {for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] != 0 && children()[i]->visible())children()[i]->update_texture();}};

        // Getters and setters (ONLY WITH ATRIBUTES)
        inline Color background_color() {return current_style()->a_background_color;};
        inline Color border_color() {return current_style()->a_border_color;};
        inline std::vector<std::shared_ptr<GUI_Object>>& children() {return a_children;};
        inline bool ignore_click() const {return a_ignore_click;};
        inline std::string name() const {return a_name;};
        inline void set_background_color(Color new_background_color) {current_style()->a_background_color = new_background_color;};
        inline void set_border_color(Color new_color) {current_style()->a_border_color = new_color;};
        inline void set_ignore_click(bool new_ignore_click) {a_ignore_click = new_ignore_click;};
        inline void set_visible(bool new_visible) {a_visible = new_visible;};
        inline bool visible() {return a_visible;};

        //*********
        //
        // Style handling
        //
        //*********

        // Returns the current style transformation of the GUI
        GUI_Style* current_style() const {return a_current_style.get();};
        // Returns the current style transformation of the GUI
        virtual __GUI_Style_Transformation* __current_style_transformation() const {return current_style(); };

        // Returns the current style transformation of the GUI
        GUI_Style* overflighted_style() const {return a_overflighted_style.get();};
        // Returns the overflighted style transformation of the GUI
        virtual __GUI_Style_Transformation* __overflighted_style_transformation() const {return overflighted_style(); };

        //*********
        //
        // Loading handler
        //
        //*********

        // Loads the object from XML
        void __load_from_xml(std::shared_ptr<XML_Text> text, std::shared_ptr<__GUI_Page_Loader> loader, std::string event = "");
        // Handle an attribute from XML
        virtual void set_xml_attribute(std::shared_ptr<XML_Text> text, std::string event, std::shared_ptr<__GUI_Page_Loader> loader, int& i);

        //*********
        //
        // Sub-page handling
        //
        //*********

        // Hides all a sub-pages
        inline void hide_sub_pages(bool child){std::vector<std::shared_ptr<GUI_Object>>& needed_sub_pages = sub_pages();for(int i = 0;i<static_cast<int>(needed_sub_pages.size());i++){needed_sub_pages[i].get()->set_visible(false);if(child){needed_sub_pages[i].get()->hide_sub_pages(true);}}};
        inline void hide_sub_pages(){hide_sub_pages(false);};
        // Shows all a page from a sub-pages
        inline void show_page(std::string page){std::vector<std::shared_ptr<GUI_Object>>& needed_sub_pages = sub_pages();for(int i = 0;i<static_cast<int>(needed_sub_pages.size());i++){if(needed_sub_pages[i].get()->name()==page){needed_sub_pages[i].get()->set_visible(true);}}};
        // Returns a precise sub-page
        inline GUI_Object* sub_page(std::string page){std::vector<std::shared_ptr<GUI_Object>>& needed_sub_pages = sub_pages();for(int i = 0;i<static_cast<int>(needed_sub_pages.size());i++){if(needed_sub_pages[i].get()->name()==page){return needed_sub_pages[i].get();}}return 0;};

        //*********
        //
        // Transform handling
        //
        //*********

        // Calculate the transformations
        void calculate_transformation(bool force = true, bool with_children = true);
        // Create the new transformation
        void __create_new_transformation(std::shared_ptr<__GUI_Transformation> last_transformation_to_use);

        // Returns the position of the mouse in scale
        glm::vec2 mouse_position_in_scale();

        // Getters
        inline GUI_Object* parent() const {return a_parent.lock().get();};

        //*********
        //
        // Texture handling
        //
        //*********

        // Returns the rect of the fitted texture
        glm::vec4 fitted_texture_rect();
        // Returns the rect of the horizontally fitted texture
        glm::vec4 fitted_horizontally_texture_rect();
        // Returns the rect of the vertically fitted texture
        glm::vec4 fitted_vertically_texture_rect();
        // Returns the height of the texture in scale of the object
        inline Fraction texture_height_in_scale() { return Fraction(image()->height(), height_in_pixel()); };
        // Returns the rect of the texture
        inline glm::vec4 texture_rect() {
            glm::vec4 final_texture_rect = glm::vec4(1);
            if(texture()->get_image() != 0) {
                if(texture_alignment() == Alignment_Texture::T_User_Defined) final_texture_rect = user_defined_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit) final_texture_rect = fitted_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit_Horizontally) final_texture_rect = fitted_horizontally_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fit_Vertically) final_texture_rect = fitted_vertically_texture_rect();
                else if(texture_alignment() == Alignment_Texture::T_Fill) final_texture_rect = glm::vec4(0, 0, 1, 1);
            }
            return final_texture_rect;
        };
        // Returns the width of the texture in scale of the object
        inline Fraction texture_width_in_scale() {return Fraction(image()->width(), width_in_pixel()); };
        // Returns the rect of user defined texture
        glm::vec4 user_defined_texture_rect();
        // Unload the texture
        inline void unload_texture() {a_texture.reset();};

        // Getters and setters
        inline Image* image() const {return texture()->get_image();};
        inline void set_texture(std::shared_ptr<Image> new_texture, bool remove_last_texture = true) {std::shared_ptr<Texture> needed_texture = std::make_shared<Texture>();needed_texture.get()->set_image(new_texture);set_texture(needed_texture, remove_last_texture);};
        inline void set_texture(std::shared_ptr<Texture> new_texture, bool remove_last_texture = true) {if(remove_last_texture) {window_struct().remove_texture(texture());}a_texture = new_texture;};
        inline void set_texture(std::string texture_name, bool remove_last_texture = true) {
            std::shared_ptr<Texture>* new_texture = window_struct().texture_shared_ptr(texture_name);
            if(new_texture != 0) set_texture(*new_texture, remove_last_texture);
            else print("Warning", "SCLS GUI Object \"" + name() + "\"", "The texture \"" + texture_name + "\" you want to set as the object texture does not exist.");
        };
        inline void set_texture_alignment(Alignment_Texture new_texture_alignment) {a_texture_alignment = new_texture_alignment;};
        virtual void set_texture_alignment_horizontal(Alignment_Horizontal new_texture_alignment_horizontal) {a_texture_alignment_horizontal = new_texture_alignment_horizontal;};
        virtual void set_texture_alignment_vertical(Alignment_Vertical new_texture_alignment_vertical) {a_texture_alignment_vertical = new_texture_alignment_vertical;};
        inline void set_vao(std::shared_ptr<VAO> new_vao){a_vao = new_vao;};
        inline std::vector<std::shared_ptr<GUI_Object>>& sub_pages() {if(a_sub_page.size() <= 0){a_sub_page.push_back(std::vector<std::shared_ptr<GUI_Object>>());}return a_sub_page[0];};
        inline Texture* texture() const {return a_texture.get();};
        inline Alignment_Texture texture_alignment() const {return a_texture_alignment;};
        inline Alignment_Horizontal texture_alignment_horizontal() const {return a_texture_alignment_horizontal;};
        inline Alignment_Vertical texture_alignment_vertical() const {return a_texture_alignment_vertical;};
        inline bool texture_fill_object() const {return a_texture_fill_object;};
        inline std::shared_ptr<Texture>& texture_shared_ptr() {return a_texture;};
        inline VAO* vao() const {return a_vao.get();};

    protected:
        //*********
        //
        // GUI Object base attributes
        //
        //*********

        // Parent of the futures created objects
        std::shared_ptr<GUI_Object> a_created_objects_parent;
        // Parent of the object
        std::weak_ptr<GUI_Object> a_parent;
        // Weak pointer to this object
        std::weak_ptr<GUI_Object> a_this_object;

    private:
        //*********
        //
        // GUI Object base attributes
        //
        //*********

        // Children of this object
        std::vector<std::shared_ptr<GUI_Object>> a_children = std::vector<std::shared_ptr<GUI_Object>>();
        // If the object ignore focus or not
        bool a_ignore_click = false;
        // Name of this object
        std::string a_name = "";
        // If the object is visible
        bool a_visible = true;

        //*********
        //
        // Style handling
        //
        //*********

        // Main style of the object
        std::shared_ptr<GUI_Style> a_main_style = std::make_shared<GUI_Style>();
        // Overflighted style of the object
        std::shared_ptr<GUI_Style> a_overflighted_style = std::make_shared<GUI_Style>();

        // Current applied style
        std::shared_ptr<GUI_Style> a_current_style = a_main_style;

        //*********
        //
        // Sub-page handling
        //
        //*********

        // Each sub-page
        std::vector<std::vector<std::shared_ptr<GUI_Object>>> a_sub_page;

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
        GUI_Main_Object(_Window_Advanced_Struct& window, std::string name) : GUI_Object(window, name, std::shared_ptr<GUI_Object>()) {};
        // GUI_Main_Object destructor
        virtual ~GUI_Main_Object() {};

        // Function called when a child is deleted
        virtual void child_deleted(GUI_Object* child) { if(a_focused_object == child) a_focused_object = 0; GUI_Object::child_deleted(child); };
        // Sets this object
        inline void set_this_object(std::weak_ptr<scls::GUI_Object> this_object){a_this_object = this_object;};
        // Updates the object for the events
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
        GUI_Scroller(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent):GUI_Object(window, name, parent) {};
        // GUI_Scroller destructor
        virtual ~GUI_Scroller(){a_scroller_children.reset();};

        // Function called after the creation of the object
        virtual void after_creation(){a_scroller_children=*_create_scroller_children();};
        // Function called after that the window is resized
        virtual void after_resizing() {check_scroller();};
        // Function called after an XML loading
        virtual void after_xml_loading() {check_scroller();GUI_Object::after_xml_loading();};
        // Check if a scroll should occurs
        void check_scroll();
        // Check the scroller object
        void check_scroller(bool reset = false);
        // Resets the scroller
        virtual void reset() {a_scroller_children.get()->delete_children();};
        // Scroll the scroller on the Y axis
        void scroll_y(Fraction movement);

        // Add a child object to the scroller with its position
        template<typename O>
        std::shared_ptr<O>* new_object_in_scroller(std::string name, unsigned int x, unsigned int y);
        // Add a child object to the scroller
        template<typename O>
        std::shared_ptr<O>* new_object_in_scroller(std::string name);
        // Update the object for the events
        virtual void update_event() {GUI_Object::update_event();check_scroll();};

        // Getters and setters
        inline Alignment_Vertical scroller_vertical_alignment() const {return a_scroller_vertical_alignment;};

    protected:

        //*********
        //
        // GUI_Scroller main attributes
        //
        //*********

        // Returns the children scroller of the object
        inline GUI_Object* scroller_children() const {return a_scroller_children.get();};

    private:

        //*********
        //
        // GUI_Scroller main attributes
        //
        //*********

        // Private function to create the children scroller
        std::shared_ptr<GUI_Object>* _create_scroller_children();
        // Scroller children which contains each elements
        std::shared_ptr<GUI_Object> a_scroller_children;

        // Vertical alignment of the scroller
        Alignment_Vertical a_scroller_vertical_alignment = Alignment_Vertical::V_Top;
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
        __GUI_Text_Metadatas(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent) : GUI_Object(window, name, parent) {a_text_image.reset(window_struct().text_image_generator()->new_text_image_multi_block(""));};

        // Return the plain text in the object
        inline std::string plain_text(){return window_struct().text_image_generator()->plain_text(text());};
        // Return the size of the plain text in the object
        inline unsigned int plain_text_size() {return plain_text().size();};
        // Updates text image block
        inline void update_text_image_block(){String temp=text();attached_text_image_block()->free_memory();set_text(temp);};
        inline void update_text_image_block_style(){attached_text_image_block()->global_style().background_color = background_color();attached_text_image_block()->global_style().color = font_color();attached_text_image_block()->global_style().font_size = font_size();attached_text_image_block()->global_style().max_width = max_width();};

        // Getters and setters
        inline Text_Image_Multi_Block* attached_text_image_block() const {return a_text_image.get();};
        inline Font font() const {return a_global_style.font;};
        inline Color font_color() const {return a_global_style.color;};
        inline std::string font_family() const {return a_global_style.font.font_family;};
        inline unsigned short font_size() const {return a_global_style.font_size;};
        inline int max_width() const {return a_global_style.max_width;};
        inline void set_font_color(Color new_font_color) {a_global_style.color = new_font_color;};
        inline void set_font_family(std::string new_font_family) {a_global_style.font = get_system_font(new_font_family);};
        inline void set_font_size(unsigned short new_font_size) {a_global_style.font_size = new_font_size;update_text_image_block();};
        inline void set_max_width(int new_max_width) {a_global_style.max_width = new_max_width;};
        virtual void set_text(std::string new_text) {if(new_text == text()){return;}update_text_image_block_style();attached_text_image_block()->set_text(new_text);update_texture();};
        inline void set_text_alignment_horizontal(Alignment_Horizontal new_text_alignment_horizontal) {a_global_style.alignment_horizontal = new_text_alignment_horizontal;};
        virtual void set_text_image_type(Block_Type new_text_image_type) {a_text_image_type = new_text_image_type;};
        inline void set_text_offset(double new_text_offset) {a_global_style.text_offset_x = new_text_offset;a_global_style.text_offset_y = new_text_offset;a_global_style.text_offset_width = new_text_offset;a_global_style.text_offset_height = new_text_offset;};
        inline String text() const {if(attached_text_image_block()==0)return String();return attached_text_image_block()->text();};
        inline Block_Type text_image_type() const {return a_text_image_type;};
        inline glm::vec4 text_offset() const {return glm::vec4(a_global_style.text_offset_x, a_global_style.text_offset_y, a_global_style.text_offset_width, a_global_style.text_offset_height);};

        //*********
        //
        // Loading handler
        //
        //*********

        // Handle an attribute from XML
        virtual void set_xml_attribute(std::shared_ptr<XML_Text> text, std::string event, std::shared_ptr<__GUI_Page_Loader> loader, int& i);

    protected:

        //*********
        //
        // Text mains attributes
        //
        //*********

        // Text image of the object
        std::shared_ptr<Text_Image_Multi_Block> a_text_image;

    private:

        //*********
        //
        // Text mains attributes
        //
        //*********

        // Global style of the text
        Text_Style a_global_style;
        // Alignment of the text
        Alignment_Horizontal a_text_alignment_horizontal = Alignment_Horizontal::H_Left;
        // Type of the text image
        Block_Type a_text_image_type = Block_Type::BT_Always_Free_Memory;
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
        GUI_Text(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent);

        // Places the blocks in the text
        void place_blocks();

        // Function called after that the window is resized
        virtual void after_resizing();
        // Soft reset the text
        virtual void soft_reset() {GUI_Object::soft_reset();a_text_modified_during_this_frame = false;};
        // Update the texture when needed
        virtual void update_texture() {GUI_Object::update_texture();update_text_texture();};
        // Update the texture of the text
        void update_text_texture(scls::Image_Generation_Type generation_type);
        void update_text_texture(){update_text_texture(scls::Image_Generation_Type::IGT_Full);};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        virtual void set_texture_alignment_horizontal(Alignment_Horizontal new_texture_alignment_horizontal){if(new_texture_alignment_horizontal != texture_alignment_horizontal()){GUI_Object::set_texture_alignment_horizontal(new_texture_alignment_horizontal);place_blocks();}};
        virtual void set_texture_alignment_vertical(Alignment_Vertical new_texture_alignment_vertical){if(new_texture_alignment_vertical != texture_alignment_vertical()){GUI_Object::set_texture_alignment_vertical(new_texture_alignment_vertical);place_blocks();}}
        inline bool text_modified_during_this_frame() {return a_text_modified_during_this_frame;};
    private:
        //*********
        //
        // GUI Text base attributes
        //
        //*********

        // Number of the generation
        unsigned int a_generation = 0;
        // If the text has been modified or not
        bool a_modified = false;
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
        GUI_Text_Input(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent);

        // Return the plain text in the object
        inline std::string plain_text(){return window_struct().text_image_generator()->plain_text(text());};
        // Return the size of the plain text in the object
        inline unsigned int plain_text_size() {return plain_text().size();};

        // Function called after that the window is resized
        virtual void after_resizing();
        // Soft reset the input
        virtual void soft_reset() {GUI_Object::soft_reset();a_input_during_this_frame = false;};
        // Update the texture when needed
        virtual void update_texture() {GUI_Object::update_texture();attached_text_image_block()->generate_blocks();update_text_texture();};
        // Update the texture of the text
        void update_text_texture();

        // Add a text to the input at the cursor position
        void add_text(std::string text_to_add);
        // Format a std::string
        std::string _format(std::string letter, bool apply_alt = false, bool apply_capitalisation = true);
        // Format an only letter
        std::string _format_one_letter(std::string letter, bool apply_alt = false, bool apply_capitalisation = true);
        // Input the inputed text
        void input_text();
        // Places the lines in the text
        void place_lines();
        // Remove a text to the input at the cursor position
        void remove_text(unsigned int size_to_delete_in_plain_text);
        // Reset the text and the text image
        void reset() {delete_children();a_line_offset = 0;};
        // Update the text
        virtual void update_event();
        // Update the cursor behavior
        void update_cursor();

        // Getters and setters
        inline Text_Image_Block* attached_text_image() const {if(attached_text_image_block()->blocks().size()<=0)return 0; return attached_text_image_block()->blocks()[0].get();};
        inline bool input_during_this_frame() const {return a_input_during_this_frame;};
        inline unsigned short line_offset() const {return a_line_offset;};
        virtual void set_text(std::string new_text, bool should_move_cursor = true) {if(new_text == text())return;reset();a_text_modified = true;__GUI_Text_Metadatas::set_text(new_text);};
        virtual void set_text_image_type(Block_Type new_text_image_type) {__GUI_Text_Metadatas::set_text_image_type(new_text_image_type);reset();};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in unformatted text
        inline unsigned int cursor_position_in_unformatted_text() const {return window_struct().text_image_generator()->plain_text_position_to_unformatted_text_position(text(), cursor_position_in_formatted_text());};
        // Moves the cursor in the text
        void move_cursor(int movement);
        // Moves the cursor at a scale position
        void move_cursor_at_position_in_scale(glm::vec2 position);
        // Moves the cursor in the Y axis
        void move_cursor_y(int movement);

        // Getters and setters
        inline unsigned int cursor_position_in_formatted_text() const {return attached_text_image()->cursor_position_in_plain_text();};
        inline void set_cursor_position_in_formatted_text(unsigned int new_cursor_position_in_formatted_text) {
            if(new_cursor_position_in_formatted_text == cursor_position_in_formatted_text()) return;

            if(attached_text_image() != 0) {
                Text_Image_Line* line_to_modify = attached_text_image()->line_at_position_in_plain_text(cursor_position_in_formatted_text());
                if(line_to_modify != 0) {
                    line_to_modify->set_modified(true);
                }
            }
            if(attached_text_image() != 0) {
                Text_Image_Line* line_to_modify = attached_text_image()->line_at_position_in_plain_text(new_cursor_position_in_formatted_text);
                if(line_to_modify != 0) {
                    line_to_modify->set_modified(true);
                }
            } attached_text_image()->set_cursor_position_in_plain_text(new_cursor_position_in_formatted_text);
        };
        inline void set_use_cursor(bool new_use_cursor) {attached_text_image()->set_use_cursor(new_use_cursor);};
        inline bool use_cursor() const {return attached_text_image()->use_cursor();};
    private:
        //*********
        //
        // GUI_Text_Input main atrtibutes
        //
        //*********

        // Number of the generation
        unsigned int a_generation = 0;
        // If an input occurred during this frame
        bool a_input_during_this_frame = false;
        // Last outputted descriptive character (^, ¨...)
        std::string a_last_descriptive_character = "";
        // Offset of the first line
        unsigned int a_line_offset = 0;
        // If the text is modified or not
        bool a_text_modified = false;
    };

    class GUI_Scroller_Choice : public GUI_Scroller {
        // Class representing a simple GUI scroller displayed into the window, which propose choices to use
    public:

        //*********
        //
        // __GUI_Scroller_Single_Choice struct
        //
        //*********

        struct __GUI_Scroller_Single_Choice {bool good=true;bool is_sub_section = false;int index = 0;std::shared_ptr<GUI_Object> object;std::string name;inline GUI_Scroller_Choice* sub_section()const{return reinterpret_cast<GUI_Scroller_Choice*>(object.get());};};
        struct GUI_Scroller_Single_Choice {std::shared_ptr<__GUI_Scroller_Single_Choice> __choice;inline bool is_sub_section()const{return __choice.get()->is_sub_section;};inline std::string& name(){return __choice.get()->name;};inline GUI_Object* object()const{return __choice.get()->object.get();};inline std::shared_ptr<GUI_Object>object_shared_ptr()const{return __choice.get()->object;};inline GUI_Scroller_Choice* sub_section()const{return __choice.get()->sub_section();}};

        //*********
        //
        // GUI_Scroller_Choice main functions
        //
        //*********

        // Most basic GUI_Scroller_Choice constructor
        GUI_Scroller_Choice(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent);
        // GUI_Scroller_Choice destructor
        virtual ~GUI_Scroller_Choice();

        // Adds an object
        template <typename T = GUI_Text>
        std::shared_ptr<T>* __add_object(std::string object_name, std::string object_text){
            // Create the object
            std::shared_ptr<T>* current_object = new_object<T>(name() + "-" + object_name);
            current_object->get()->set_text(object_text);
            current_object->get()->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
            if(a_displayer_object.get() != 0) {current_object->get()->attach_left_in_parent(20);}
            else {current_object->get()->attach_left_in_parent(0);}
            current_object->get()->set_height_in_pixel(current_object->get()->font_size() + 4);
            current_object->get()->set_width_in_scale(1);

            // Create the choice
            __GUI_Scroller_Single_Choice choice;
            choice.name = object_name;
            choice.object = *current_object;
            GUI_Scroller_Single_Choice to_add; to_add.__choice = std::make_shared<__GUI_Scroller_Single_Choice>(choice);
            a_objects.push_back(to_add);

            // Apply the needed style
            current_object->get()->set_overflighted_cursor(a_unselected_objects_style.cursor);

            // Place the object
            place_objects();

            // Returns the object
            return current_object;
        };
        template <typename T = GUI_Text>
        std::shared_ptr<T>* add_object(std::string object_name, std::string object_text, std::string sub_section){
            // Asserts
            GUI_Scroller_Choice* needed_parent = this;
            if(contains_object(object_name)){return 0;}
            if(sub_section != ""){for(int i=0;i<static_cast<int>(a_objects.size());i++){if(a_objects[i].name()==sub_section){needed_parent = reinterpret_cast<GUI_Scroller_Choice*>(a_objects[i].object());break;}}}

            // Returns the object
            return needed_parent->__add_object(object_name, object_text);
        };
        template <typename T = GUI_Text> inline std::shared_ptr<T>* add_object(std::string object_name, std::string object_text){return add_object(object_name, object_text, "");};
        // Adds a sub-section for the object
        template <typename T = GUI_Scroller_Choice>
        std::shared_ptr<T>* add_sub_section(std::string object_name, std::string object_text){
            // Asserts
            if(contains_object(object_name)){return 0;}

            // Create the object
            std::shared_ptr<T>* current_object = new_object<T>(object_name);
            current_object->get()->a_scroller_parent = a_this_object;
            current_object->get()->set_x_in_object_scale(scls::Fraction(1, 2));
            current_object->get()->set_height_in_pixel(50);
            current_object->get()->set_width_in_scale(1);

            // Create the choice
            __GUI_Scroller_Single_Choice choice;
            choice.is_sub_section = true;
            choice.object = *current_object;
            GUI_Scroller_Single_Choice to_add; to_add.__choice = std::make_shared<__GUI_Scroller_Single_Choice>(choice);
            a_objects.push_back(to_add);

            // Add the object
            current_object->get()->set_displayer_object(object_name + std::string("-displayer"), object_text);

            // Place the object
            place_objects();

            // Returns the object
            return current_object;
        };

        // Adds an object (without text)
        template <typename T = GUI_Object> std::shared_ptr<T>* __add_object(std::string object_name){
            // Create the object
            std::shared_ptr<T>* current_object = new_object<T>(name() + "-" + object_name);
            current_object->get()->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
            if(a_displayer_object.get() != 0) {current_object->get()->attach_left_in_parent(20);}
            else {current_object->get()->attach_left_in_parent(0);}
            current_object->get()->set_height_in_pixel(20);
            current_object->get()->set_width_in_scale(1);

            // Create the choice
            __GUI_Scroller_Single_Choice choice;
            choice.name = object_name;
            choice.object = *current_object;
            GUI_Scroller_Single_Choice to_add; to_add.__choice = std::make_shared<__GUI_Scroller_Single_Choice>(choice);
            a_objects.push_back(to_add);

            // Apply the needed style
            current_object->get()->set_overflighted_cursor(a_unselected_objects_style.cursor);

            // Place the object
            place_objects();

            // Returns the object
            return current_object;
        };
        template <typename T = GUI_Object> std::shared_ptr<T>* add_object(std::string object_name){
            // Asserts
            GUI_Scroller_Choice* needed_parent = this;
            if(contains_object(object_name)){return 0;}
            //if(sub_section != ""){for(int i=0;i<static_cast<int>(a_objects.size());i++){if(a_objects[i].name()==sub_section){needed_parent = reinterpret_cast<GUI_Scroller_Choice*>(a_objects[i].object());break;}}}

            // Returns the object
            return needed_parent->__add_object(object_name);
        };

        // Checks the number of selected objects
        inline void check_number_selected_object() {while(static_cast<int>(a_currently_selected_objects.size())>max_number_selected_object())a_currently_selected_objects.erase(a_currently_selected_objects.begin(),a_currently_selected_objects.begin()+1);};
        // Returns if an object is confirmed
        inline bool contains_confirmed_object(std::string object_name) {for(int i = 0;i<static_cast<int>(a_currently_confirmed_objects.size());i++){if(a_currently_confirmed_objects[i]==object_name)return true; } return false;};
        // Returns if an object is in the scroller
        inline bool contains_object(std::string object_name) {for(int i = 0;i<static_cast<int>(a_objects.size());i++){if(a_objects[i].name()==object_name)return true; } return false;};
        // Returns the number of similar object of a name, with the cutting part
        inline int count_object_similar(std::string object_name, std::string cut) {int to_return=0;for(int i = 0;i<static_cast<int>(a_objects.size());i++){std::vector<std::string>splitted=cut_string(a_objects[i].name(), cut);if(splitted.size()>0&&splitted[0]==object_name){to_return++;}}return to_return;};
        // Returns if an object is selected
        inline bool contains_selected_object(std::string object_name) {for(int i = 0;i<static_cast<int>(a_currently_selected_objects.size());i++){if(a_currently_selected_objects[i].name()==object_name)return true; } return false;};
        inline bool contains_selected_object_during_this_frame(std::string object_name) {for(int i = 0;i<static_cast<int>(a_currently_selected_objects_during_this_frame.size());i++){if(a_currently_selected_objects_during_this_frame[i].name()==object_name)return true; } return false;};
        // Returns an object by its name
        inline GUI_Scroller_Single_Choice object_by_name(std::string object_name) {for(int i = 0;i<static_cast<int>(a_objects.size());i++){if(a_objects[i].name()==object_name)return a_objects[i]; } return GUI_Scroller_Single_Choice();};
        // Resets the scroller
        virtual void reset() {a_displayer_object.reset();a_objects.clear();GUI_Scroller::reset();};
        // Select an object
        void select_object(GUI_Scroller_Single_Choice needed_object);
        void select_object(std::string object_name){select_object(object_by_name(object_name));};
        // Unselect an object
        void unselect_object(std::string object_name);
        void unselect_object(GUI_Scroller_Single_Choice needed_object){unselect_object(needed_object.name());};

        // Function called after an XML loading
        virtual void after_xml_loading() {if(a_displayed){show_objects();}else{hide_objects();}GUI_Object::after_xml_loading();};
        // Check the objects (place + size)
        inline void check_objects() {place_objects();if(scroller_parent()!=0){if(a_displayed){set_height_in_pixel(needed_height());}else if(a_displayer_object.get()!=0){set_height_in_pixel(a_displayer_object.get()->height_in_pixel());}scroller_parent()->check_objects();}};
        // Hides the object
        void hide_objects(){a_displayed=false;for(int i = 0;i<static_cast<int>(a_objects.size());i++){a_objects[i].object()->set_visible(false);}check_objects();};
        // Returns the parent scroller of the object
        inline GUI_Scroller_Choice* scroller_parent() const {return reinterpret_cast<GUI_Scroller_Choice*>(a_scroller_parent.lock().get());};
        // Places the objects
        void place_objects();
        // Returns the needed height
        inline int needed_height() const {int to_return = 0;if(a_displayer_object.get()!=0){to_return+=a_displayer_object.get()->height_in_pixel();}for(int i = 0;i<static_cast<int>(a_objects.size());i++){to_return += a_objects[i].object()->height_in_pixel();}return to_return;};
        // Resets the object
        void reset_objects() {a_currently_confirmed_objects.clear();while(static_cast<int>(a_currently_selected_objects.size() > 0)){unselect_object(a_currently_selected_objects[0]);}};
        // Shows the object
        void show_objects(){a_displayed=true;for(int i = 0;i<static_cast<int>(a_objects.size());i++){a_objects[i].object()->set_visible(true);}check_objects();};
        // Soft resets the object
        virtual void soft_reset() {GUI_Scroller::soft_reset();a_choice_modified=false;a_selection_modified=false;a_currently_confirmed_objects.clear();a_currently_selected_objects_during_this_frame.clear();for(int i=0;i<static_cast<int>(a_currently_selected_objects.size());i++){if(!a_currently_selected_objects[i].__choice.get()->good){a_currently_selected_objects.erase(a_currently_selected_objects.begin() + i, a_currently_selected_objects.begin() + i + 1);i--;}}};
        // Updates the even in the scroller
        virtual void update_event();

        // Getters and setters
        inline bool choice_modified() const {return a_choice_modified;};
        inline bool choice_modified_during_this_frame() const {return a_choice_modified;};
        inline std::vector<GUI_Scroller_Single_Choice>& currently_selected_objects() {return a_currently_selected_objects;};
        inline std::vector<GUI_Scroller_Single_Choice>& currently_selected_objects_during_this_frame() {return a_currently_selected_objects_during_this_frame;};
        inline unsigned int max_number_selected_object() const {return a_max_number_selected_object;};
        inline std::vector<GUI_Scroller_Single_Choice>& objects() {return a_objects;};
        inline GUI_Style& selected_objects_style() {return a_selected_objects_style;};
        inline bool selection_modified() const {return a_selection_modified;};
        inline void set_unselect_object_on_confirmation(bool new_unselect_object_on_confirmation) {a_unselect_object_on_confirmation=new_unselect_object_on_confirmation;};
        inline GUI_Style& unselected_objects_style() {return a_unselected_objects_style;};

        //*********
        //
        // Loading handler
        //
        //*********

        // Loads the choice from an XML test
        void load_choices_from_xml(std::shared_ptr<XML_Text> text);
        // Handle an attribute from XML
        virtual void set_xml_attribute(std::shared_ptr<XML_Text> text, std::string event, std::shared_ptr<__GUI_Page_Loader> loader, int& i);

    private:

        //*********
        //
        // GUI_Scroller_Choice main attributes
        //
        //*********

        // If the coice as been modified during this frame
        bool a_choice_modified = false;
        // Currently confirmed objects
        std::vector<std::string> a_currently_confirmed_objects = std::vector<std::string>();
        // Currently selected objects
        std::vector<GUI_Scroller_Single_Choice> a_currently_selected_objects = std::vector<GUI_Scroller_Single_Choice>();
        std::vector<GUI_Scroller_Single_Choice> a_currently_selected_objects_during_this_frame = std::vector<GUI_Scroller_Single_Choice>();
        // Maximum number of selected object
        unsigned int a_max_number_selected_object = 1;
        // Scroller parent of the object
        std::weak_ptr<GUI_Object> a_scroller_parent;
        // If the selection as been modified during this frame
        bool a_selection_modified = false;
        // If the object muse be unselected after confirmation
        bool a_unselect_object_on_confirmation = false;

        //*********
        //
        // GUI_Scroller_Choice elements handling
        //
        //*********

        // Set the good displayer object
        template <typename T = GUI_Text>
        void set_displayer_object(std::string object_name, std::string object_text) {
            // Create the object
            std::shared_ptr<T>* current_object = new_object<T>(object_name);
            current_object->get()->set_text(object_text);
            current_object->get()->set_texture_alignment_horizontal(scls::Alignment_Horizontal::H_Left);
            current_object->get()->set_x_in_object_scale(scls::Fraction(1, 2));
            current_object->get()->set_height_in_pixel(current_object->get()->font_size() + 4);
            current_object->get()->set_width_in_scale(1);
            a_displayer_object = *current_object;

            // Apply the needed style
            current_object->get()->set_overflighted_cursor(a_unselected_objects_style.cursor);
        };

        // If the object is displayed or not
        bool a_displayed = true;
        // Displayer object
        std::shared_ptr<GUI_Object> a_displayer_object;
        // Objects
        std::vector<GUI_Scroller_Single_Choice> a_objects = std::vector<GUI_Scroller_Single_Choice>();
        // Selected objects style
        GUI_Style a_selected_objects_style;
        // Unelected objects style
        GUI_Style a_unselected_objects_style;
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
        GUI_File_Explorer(_Window_Advanced_Struct& window, std::string name, std::weak_ptr<GUI_Object> parent);
        // GUI_File_Explorer destructor
        virtual ~GUI_File_Explorer();

        // Add an extension to the authorized extensions
        inline void add_authorized_extension(std::string extension){if(a_authorized_file_extension.size() <= 0 || !is_file_authorized(extension))a_authorized_file_extension.push_back(extension);};
        // Function called after that the window is resized
        virtual void after_resizing();
        // Function called after an XML loading
        virtual void after_xml_loading() {place_all();GUI_Object::after_xml_loading();};
        // Returns if the choose button is clicked during this frame
        bool choose_clicked() const {return a_choose_button->is_clicked_during_this_frame(GLFW_MOUSE_BUTTON_LEFT);};
        // Returns if the explorer contains a selected file
        inline bool contains_selected_file(std::string file) {std::vector<std::string> selected_files = cut_string(a_currently_selected_files, ";");for(int i = 0;i<static_cast<int>(selected_files.size());i++) {if(selected_files[i] == file) return true;} return false;};
        // Returns if a file is chosen during this frame
        bool file_chosen();
        // Returns if a file is authorized or not
        inline bool is_file_authorized(std::string tested_file) const {
            if(a_authorized_file_extension.size() <= 0 || std::filesystem::is_directory(tested_file)) return true;
            std::string extension=file_extension(tested_file); std::string extension_wp=file_extension(tested_file,true);
            for(int i=0;i<static_cast<int>(a_authorized_file_extension.size());i++){if(a_authorized_file_extension.at(i)==extension||a_authorized_file_extension.at(i)==extension_wp){return true;}}
            return false;
        };
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
        // List of each authorised files extension
        std::vector<std::string> a_authorized_file_extension;
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
        std::shared_ptr<scls::GUI_Scroller> a_browser = 0;
        // Y position of the buttons in the browser
        double a_browser_y = 0;
        // List of every buttons in the browser
        std::vector<std::shared_ptr<scls::GUI_Text>> a_browser_buttons = std::vector<std::shared_ptr<scls::GUI_Text>>();
        // List of every buttons in the browser to modify
        std::vector<unsigned int> a_browser_buttons_to_modify = std::vector<unsigned int>();
        // Button to chose a file
        std::shared_ptr<scls::GUI_Text> a_choose_button = 0;
        // Text in the chose button
        std::string a_choose_button_text = "Choose";
        // If tbe file has been chose or not
        bool a_file_chose = false;
        // Final path selected
        std::shared_ptr<scls::GUI_Text> a_final_path = 0;
        // Text in the final path
        std::string a_final_path_text = "Final path";
        // Top bar of the explorer
        std::shared_ptr<scls::GUI_Object> a_top_bar = 0;
        // List of every buttons in the top bar
        std::vector<std::shared_ptr<scls::GUI_Text>> a_top_bar_buttons = std::vector<std::shared_ptr<scls::GUI_Text>>();
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
        virtual ~GUI_Page() {a_loader.reset();};

        // Function called after that the window is resized
        virtual void after_window_resizing(glm::vec2 last_scale){Object::after_window_resizing(last_scale);parent_object()->after_resizing();};
        // Function called after an XML loading
        virtual void after_xml_loading() {a_parent_object.get()->after_xml_loading();Object::after_xml_loading();};
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
        inline std::shared_ptr<GUI_Main_Object>& parent_object_shared_ptr() {return a_parent_object;};

        //*********
        //
        // Load the page
        //
        //*********

        // Create an object from a type
        virtual std::shared_ptr<GUI_Object> __create_loaded_object_from_type(std::string object_name, std::string object_type, GUI_Object* parent);
        // Load the page from XML
        void load_from_xml(const std::string& content_to_parse, bool sup_paged = false) {load_objects_from_xml(content_to_parse, "", sup_paged);};
        // Load an object in a page from XML
        std::shared_ptr<GUI_Object> __load_object_from_xml(std::string object_name, std::string object_type, std::shared_ptr<XML_Text> content);
        // Load objects in a page from XML
        void __load_objects_from_xml(std::shared_ptr<XML_Text> content, bool sub_paged);
        void load_objects_from_xml(const std::string& content_to_parse, std::string path, bool sub_paged);
        // Handle an attribute from XML
        virtual void set_xml_attribute(std::shared_ptr<XML_Text> text, std::shared_ptr<__XML_Loader> loader_shared_ptr, int& i);

        //*********
        //
        // Sub-page handling
        //
        //*********

        // Hides all a sub-pages
        inline void hide_sub_pages(bool child){parent_object()->hide_sub_pages(child);};
        // Shows all a page from a sub-pages
        inline void show_page(std::string page){parent_object()->show_page(page);};
    private:
        // Loader of the page
        std::shared_ptr<__GUI_Page_Loader> a_loader;

        // Handle the focused object
        // Pointer to the focused object
        GUI_Object* a_focused_object = 0;

        // Handle the overflighted object
        // Pointer to the overflighted object
        GUI_Object* a_overflighted_object = 0;

        // Parent object of the page
        std::shared_ptr<GUI_Main_Object> a_parent_object;
    };

    // Add a child object to the object
    template<typename O>
    std::shared_ptr<O>* GUI_Object::new_object(std::string name) { return new_object<O>(name, 0, 0); }

    // Add a child object to the object
    template<typename O>
    std::shared_ptr<O>* GUI_Object::new_object(std::string name, unsigned int x, unsigned int y) {
        // Create the child
        std::shared_ptr<O> new_ptr;
        std::shared_ptr<O>* to_return = 0;
        if(a_created_objects_parent.get() == 0) {
             children().push_back(new_ptr);
             to_return = reinterpret_cast<std::shared_ptr<O>*>(&children()[children().size() - 1]);
             *to_return = std::make_shared<O>(window_struct(), name, a_this_object);
        }
        else {
            a_created_objects_parent.get()->children().push_back(new_ptr);
            to_return = reinterpret_cast<std::shared_ptr<O>*>(&a_created_objects_parent.get()->children()[a_created_objects_parent.get()->children().size() - 1]);
            *to_return = std::make_shared<O>(window_struct(), name, a_created_objects_parent);
        }
        O* new_object = to_return->get();

        // Configurate the child
        new_object->set_position_in_pixel(x, y);
        new_object->a_this_object = *to_return;
        new_object->after_creation();

        return to_return;
    }

    // Add a child object to the scroller with its position
    template<typename O>
    std::shared_ptr<O>* GUI_Scroller::new_object_in_scroller(std::string name, unsigned int x, unsigned int y) {
        // Create the new object
        std::shared_ptr<O>* new_object = a_scroller_children->new_object<O>(name, x, y);

        // Configurate the scroller
        check_scroller();
        return new_object;
    }

    // Add a child object to the scroller
    template<typename O>
    std::shared_ptr<O>* GUI_Scroller::new_object_in_scroller(std::string name) { return new_object_in_scroller<O>(name, 0, 0); }
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
