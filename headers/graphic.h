#ifndef MATIX_GRAPHIC
#define MATIX_GRAPHIC

#include "../headers/base_struct.h"
#include "../headers/model.h"

class Graphic_Object
{
	// Class representing a graphic object
public:
    // Graphic_Object contructor
	Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao);
	// Render the graphic object
	void render();
	// Update the graphic object
	void update();
	// Graphic_Object destructor
	~Graphic_Object();

	// Getters and setters (ONLY WITH ATTRIBUTES)
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline Texture* get_texture() { return texture; };
	inline bool is_transparent() { return transparent; };
	inline void set_is_transparent(bool new_transparent) { transparent = new_transparent; };
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
private:
    // If the object use transparency or not
	bool transparent = false;

	// Reference to the attached transform object
	Transform_Object& attached_transform;
	// Pointer to the base struct in the game
	Base_Struct* base_struct = 0;
	// Pointer to the VAO used to render the object
	VAO* vao = 0;
	// Pointer to the Texture used to render the object
	Texture *texture = 0;
};

class HUD_Object
{
	// Class representing a HUD object
public:
    // HUD_Object constructor
	HUD_Object(Base_Struct* a_base_struct, std::string name, HUD_Object* parent, Texture* a_texture, VAO* a_vao);
	// Return the transformation matrix of the object
	glm::mat4 get_model_matrix();
	// Render the graphic HUD
	virtual void render();
	// Soft reset the graphic HUD
	void soft_reset();
	// Update the graphic HUD
	virtual void update();
	// HUD_Object destructor
	~HUD_Object();

	// Hierarchy manipulation
	// Add a children to the object
	void add_children(HUD_Object* object);
	// Returns if the object contains a children
	bool contains_children(HUD_Object* object);
	// Remove a children from the object
	void remove_children(HUD_Object* object);
	// Set the parent of the object
	void set_parent(HUD_Object* new_parent);
	// Sort the children according to their depht
	void sort_children();

	// Getters and setters (ONLY WITHOUT ATTRIBUTES)
	inline glm::vec2 absolute_position() { if(parent() == 0) return position(); return parent()->position() + position() * parent()->absolute_scale(); };
	inline glm::vec2 absolute_scale() {
        if(parent() != 0) return scale() * parent()->absolute_scale();
        return scale();
	};
	inline glm::vec4 get_hud_rect_pos() {
	    glm::vec2 final_scale = scale_for_rendering(absolute_scale()) * glm::vec2(0.5);
		int heigth = final_scale[1] * get_base_struct()->get_window_height();
		int width = final_scale[0] * get_base_struct()->get_window_width();
		int x = (((absolute_position()[0] + 1.0) / 2.0) * get_base_struct()->get_window_width());
		int y = get_base_struct()->get_window_height() - (((absolute_position()[1] + 1.0) / 2.0) * get_base_struct()->get_window_height());

		return glm::vec4(x, y, width, heigth);
	};
    inline glm::vec2 scale_for_rendering(glm::vec2 scale_to_render) {
        double texture_ratio = get_texture()->image_ratio();
        double x_multiplier = 1; double y_multiplier = 1;
        if(texture_ratio < 1) {x_multiplier = texture_ratio;}
        else { y_multiplier /= texture_ratio; }

        double window_ratio = get_base_struct()->window_ratio();
        if(window_ratio < 1) {y_multiplier /= 1.0 / window_ratio;}
        else { x_multiplier /= window_ratio; }

        glm::vec2 final_scale = scale_to_render * glm::vec2(2) * glm::vec2(x_multiplier, y_multiplier);

        return final_scale;
    };
    inline bool is_clicked_during_this_frame() { return a_is_clicked && !a_was_clicked; };
    inline bool is_in(glm::vec2 pos) {
		glm::vec4 rect = get_hud_rect_pos();
		return pos[0] > rect[0] - rect[2] / 2.0 && pos[0] < rect[0] + rect[2] / 2.0 && pos[1] > rect[1] - rect[3] / 2.0 && pos[1] < rect[1] + rect[3] / 2.0;
	};

	// Getters and setters (ONLY WITH ATTRIBUTES)
	inline std::vector<HUD_Object*>& children() {return a_children;};
	inline unsigned long cursor_overflighted() {return a_cursor_overflighted;};
	inline unsigned short depht() {return a_depht;};
	inline Base_Struct* get_base_struct() { return base_struct; };
	inline glm::vec4 get_border_color() { return a_border_color; };
	inline glm::vec4 get_border_width() { return a_border_width; };
	inline glm::vec3 get_rotation() { return rotation; };
	inline Texture* get_texture() { return texture; };
	inline bool is_clicked() { return a_is_clicked; };
	inline bool is_overflighted() { return a_is_overflighted; };
	inline std::string name() {return a_name;};
	inline HUD_Object* parent() {return a_parent;};
	inline glm::vec2 position() { return a_position; };
	inline glm::vec2 scale() { return a_scale; };
	inline void set_border_color(glm::vec4 new_border_color) { a_border_color = new_border_color; };
	inline void set_border_width(glm::vec4 new_border_width) { a_border_width = new_border_width; };
	inline void set_border_width(double new_border_width) { a_border_width = glm::vec4(new_border_width); };
	inline void set_cursor_overflighted(unsigned long new_cursor_overflighted) {a_cursor_overflighted = new_cursor_overflighted;};
	inline void set_depht(unsigned short new_depht) {a_depht = new_depht;};
	inline void set_is_clicked(bool is_clicked) { a_is_clicked = is_clicked; };
	inline void set_is_overflighted(bool is_overflighted) { a_is_overflighted = is_overflighted; };
	inline void set_position(glm::vec2 new_position) { a_position = new_position; };
	inline void set_rotation(glm::vec3 a_rotation) { rotation = a_rotation; };
	inline void set_scale(double new_scale) {a_scale = glm::vec2(new_scale, new_scale);};
	inline void set_scale(glm::vec2 new_scale) { a_scale = new_scale; };
	inline void set_sized_according_to_ratio(bool new_sized_according_to_ratio) {a_sized_according_to_ratio = new_sized_according_to_ratio;};
	inline void set_texture(Texture* a_texture) { texture = a_texture; };
	inline void set_visible(bool new_visible) {a_visible = new_visible;};
	inline bool sized_according_to_ratio() {return a_sized_according_to_ratio;};
	inline std::vector<HUD_Object*>& sorted_children() {return a_sorted_children;};
	inline bool visible() {return a_visible;};

protected:
	Base_Struct* base_struct = 0; // Pointer to the base struct in the game
	VAO* vao = 0; // Pointer to the VAO used to render the object
	Texture* texture = 0; // Pointer to the Texture used to render the object
private:
	std::string a_name = ""; // Name of the object

	// Color of the border of the HUD
	glm::vec4 a_border_color = glm::vec4(0, 0, 0, 1);
	// Width of the border of the HUD (top, left, bottom, right)
	glm::vec4 a_border_width = glm::vec4(0, 0, 0, 0);
	// Children of the object
	std::vector<HUD_Object*> a_children = std::vector<HUD_Object*>();
	// Cursor to apply when the object is overflighted
	unsigned long a_cursor_overflighted = GLFW_ARROW_CURSOR;
	// Depht of the object in his parent
	unsigned short a_depht = 0;
	// If the object is clicked or not
	bool a_is_clicked = false;
	// If the object is overflighted or not
	bool a_is_overflighted = false;
	// Parent of this object
	HUD_Object* a_parent = 0;
	// Position of the HUD on the screen
	glm::vec2 a_position = glm::vec2(0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0); // Rotation of the HUD on the screen
	// Size of the HUD on the screen
	glm::vec2 a_scale = glm::vec2(1, 1);
	// If the object use a relative or an absolute size
	bool a_sized_according_to_ratio = true;
	// Children sorted according to their depht
	std::vector<HUD_Object*> a_sorted_children = std::vector<HUD_Object*>();
	// If the object is visible or not
	bool a_visible = true;
	// If the object was clicked before or not
	bool a_was_clicked = false;
};

class HUD_Text: public HUD_Object
{
	// Class representing an HUD text object
public:
    // HUD_Text constructor
	HUD_Text(Base_Struct* a_base_struct, std::string a_name, HUD_Object* parent, Texture* a_texture, VAO* a_vao);
	// Update the text texture
	void update_text();
	// HUD_Object destructor
	~HUD_Text();

	// Getters and setters (ONLY WITHOUT ATTRIBUTES)

	// Getters and setters (ONLY WITH ATTRIBUTES)
	inline glm::vec4 background_color() { return a_background_color; };
	inline bool can_take_input() { return input; };
	inline std::string font_family() {return a_font_family;};
	inline std::string get_cursor_character() { return cursor_character; };
	inline float get_cursor_display_time() { return cursor_display_time; };
	inline glm::vec4 get_font_color() { return font_color; };
	inline unsigned short font_size() { return a_font_size; };
	inline std::string get_input_text() { return input_text; };
	inline std::string get_text(bool with_cursor = false) {
		std::string cursor = "";
		if (with_cursor && is_using_cursor() && display_cursor) cursor = get_cursor_character();
		return text + cursor;
	};
	inline bool is_focused() { return focused; };
	inline bool is_using_cursor() { return use_cursor; };
	inline glm::vec4 out_offset() {return a_out_offset;};
	inline void set_background_color(glm::vec4 new_background_color) { a_background_color = new_background_color; };
	inline void set_cursor_character(std::string new_cursor_character) { cursor_character = new_cursor_character; };
	inline void set_focused(bool a_focused) { focused = a_focused; };
	inline void set_font_color(glm::vec4 a_font_color) { font_color = a_font_color; };
	inline void set_font_family(std::string new_font_family) {a_font_family = new_font_family;};
	inline void set_font_size(unsigned short new_size) { a_font_size = new_size; };
	inline void set_input(bool a_input) { input = a_input; };
	inline void set_input_text(std::string a_input_text) { input_text = a_input_text; };
	inline void set_ouf_offset(glm::vec4 new_out_offset) {a_out_offset = new_out_offset;};
	inline void set_ouf_offset(double new_out_offset) {set_ouf_offset(glm::vec4(new_out_offset));};
	inline void set_text(std::string a_text) { text = a_text; update_text(); };
	inline void set_use_cursor(bool a_use_cursor) { use_cursor = a_use_cursor; };
private:
    // Background color of the object
	glm::vec4 a_background_color = glm::vec4(0, 0, 0, 0);
    // Return the character representing the cursor
	std::string cursor_character = "_";
	// Number of second the cursor is displayed and hide
	float cursor_display_time = 0.5;
	// If the cursor should be add to the text at this frame or not
	bool display_cursor = true;
	// If the object is focused or not
	bool focused = false;
	// If text can be inputed or not
	bool input = false;
	// Text that can be took in input
	std::string input_text = "";
	// Ouf offset of the text
	glm::vec4 a_out_offset = glm::vec4(0, 0, 0, 0);
	// Time passed since the last cursor displaying
	float time_since_last_cursor_display = 0;
	// If the text use a cursor
	bool use_cursor = false;

	// Datas about the text
	// Color of the font of the text
	glm::vec4 font_color = glm::vec4(0, 0, 0, 1);
	// Font family of the text
	std::string a_font_family = "arial";
	// Size of the font of the text
	unsigned short a_font_size = 50;
	// Text in the HUD
	std::string text = "Matix";
};

class HUD_Button : public HUD_Text {
    // Class representing an HUD button object
public:
    // HUD_Button constructor
	HUD_Button(Base_Struct* a_base_struct, std::string a_name, HUD_Object* parent, Texture* a_texture, VAO* a_vao);
	// HUD_Button destructor
	~HUD_Button();
private:

};

#endif // MATIX_GRAPHIC
