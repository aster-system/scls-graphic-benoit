#include "../headers/base_struct.h"
#include "../headers/graphic.h"
#include "../headers/model.h"

// Compare the 2 objects with their depths
bool compare_depht_hud_object(HUD_Object* a, HUD_Object* b) {
    return a->depht() < b->depht();
}

// Graphic_Object contructor
Graphic_Object::Graphic_Object(Base_Struct* a_base_struct, Transform_Object& a_attached_transform, Texture *a_texture, VAO* a_vao) : base_struct(a_base_struct), attached_transform(a_attached_transform), texture(a_texture), vao(a_vao) {

}

// Render the graphic object
void Graphic_Object::render() {
	texture->bind(); // Bind the texture
	vao->get_shader_program()->set_uniform4fv_value("model", attached_transform.get_model_matrix()); // Write some uniform variables into the shader
	vao->get_shader_program()->set_uniform4fv_value("projection", get_base_struct()->get_projection());
	vao->get_shader_program()->set_uniform4fv_value("view", get_base_struct()->get_camera()->get_view());
	if (texture->use_resize())
	{
		vao->render(attached_transform.get_scale()); // Render the object with scaling
	}
	else
	{
		vao->render(glm::vec3(1.0, 1.0, 1.0)); // Render the object without scaling
	}
}

// Update the graphic object
void Graphic_Object::update() {}

// Graphic_Object destructor
Graphic_Object::~Graphic_Object() {

}

// HUD_Object constructor
HUD_Object::HUD_Object(Base_Struct* a_base_struct, std::string name, HUD_Object* parent, Texture* a_texture, VAO* a_vao) : base_struct(a_base_struct), a_name(name), texture(a_texture), vao(a_vao) {
    set_parent(parent);
}

// Add a children to the object
void HUD_Object::add_children(HUD_Object* object) {
    if(!contains_children(object))
    {
        children().push_back(object);

        sort_children();
    }
}

// Returns if the object contains a children
bool HUD_Object::contains_children(HUD_Object* object) {
    for(int i = 0;i<static_cast<int>(children().size());i++)
    {
        if(children()[i] == object) return true;
    }
    return false;
}

// Return the transformation matrix of the HUD
glm::mat4 HUD_Object::get_model_matrix() {
	glm::mat4 matrix = glm::mat4(1.0f);

	// Move matrix
	matrix = glm::translate(matrix, glm::vec3(absolute_position()[0], absolute_position()[1], 0));

	// Rotate matrix
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[1]), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[0]), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(get_rotation()[2]), glm::vec3(0, 0, 1));

	// Scale matrix
	glm::vec2 final_scale = glm::vec2(1);
	if(sized_according_to_ratio())
    {
        final_scale = scale_for_rendering(absolute_scale());
    }
    else
    {
        final_scale = absolute_scale() * glm::vec2(2);
    }
    matrix = glm::scale(matrix, glm::vec3(final_scale[0], final_scale[1], 1));

	return matrix;
}

// Remove a children from the object
void HUD_Object::remove_children(HUD_Object* object) {
    for(int i = 0;i<static_cast<int>(children().size());i++)
    {
        if(children()[i] == object)
        {
            children().erase(children().begin() + i, children().begin() + i + 1);
            return;
        }
    }
}

// Render the HUD object
void HUD_Object::render() {
    texture->bind(); // Bind the texture
	vao->get_shader_program()->set_uniform4f_value("border_color", get_border_color()); // Write the border color of the HUD in the shader
	vao->get_shader_program()->set_uniform4f_value("border_width", get_border_width()); // Write the border width of the HUD in the shader
	vao->get_shader_program()->set_uniform4fv_value("model", get_model_matrix()); // Write some uniform variables into the shader

	// Get the scale for the VAO
	glm::vec2 final_scale = glm::vec2(1);
	if(sized_according_to_ratio())
    {
        if (texture->use_resize())
        {
            final_scale = scale_for_rendering(absolute_scale()); // Render the object with scaling
        }
        else
        {
            final_scale = scale_for_rendering(glm::vec2(1)); // Render the object without scaling
        }
    }
    else if (texture->use_resize())
    {
        final_scale = absolute_scale(); // Render the object with scaling
    }
    vao->render(glm::vec3(final_scale[0], final_scale[1], 1)); // Render the object

    // Render all the childrens
    for(int i = 0;i<static_cast<int>(sorted_children().size());i++)
    {
        if(sorted_children()[i]->visible())
        {
            sorted_children()[i]->render();
        }
    }
}

// Set the parent of the object
void HUD_Object::set_parent(HUD_Object* new_parent) {
    // Reset the old parent
	if(parent() != 0)
    {
        parent()->remove_children(this);
    }

    a_parent = new_parent;

    if(parent() != 0)
    {
        parent()->add_children(this);
    }
}

// Sort the children according to their depht
void HUD_Object::sort_children() {
    a_sorted_children = a_children;
    std::sort(sorted_children().begin(), sorted_children().end(), compare_depht_hud_object);
}

// Soft reset the graphic HUD
void HUD_Object::soft_reset() {
	if (is_clicked())
	{
		a_was_clicked = true;
	}
	else
	{
		a_was_clicked = false;
	}

	set_is_clicked(false);
	set_is_overflighted(false);
}

// Update the graphic object
void HUD_Object::update() {}

// HUD_Object destructor
HUD_Object::~HUD_Object() {

}

// HUD_Text constructor
HUD_Text::HUD_Text(Base_Struct* a_base_struct, std::string a_name, HUD_Object* parent, Texture* a_texture, VAO* a_vao) : HUD_Object(a_base_struct, a_name, parent, a_texture, a_vao) {

}

// Update the text texture
void HUD_Text::update_text() {
    basix::Text_Image_Data datas; datas.font = basix::get_system_font(font_family()); datas.font_size = font_size();
    datas.background_alpha = background_color()[3]; datas.background_blue = background_color()[2]; datas.background_green = background_color()[1]; datas.background_red = background_color()[0];
    datas.out_offset_top_width = out_offset()[0]; datas.out_offset_left_width = out_offset()[1]; datas.out_offset_bottom_width = out_offset()[2]; datas.out_offset_right_width = out_offset()[3];
    datas.alignment = basix::Left;

    basix::Image* new_texture = basix::text_image(get_text(), datas);
    texture->set_image(new_texture);
}

/*// Update the cursor of the text
void HUD_Text::update_cursor() {
	if (is_using_cursor())
	{
		time_since_last_cursor_display += get_base_struct()->get_delta_time();
	}
	else
	{
		display_cursor = true;
		time_since_last_cursor_display = 0;
	}

	if (time_since_last_cursor_display > get_cursor_display_time())
	{
		display_cursor = !display_cursor;
		time_since_last_cursor_display = 0;
	}
}

// Apply the text input to the text
void HUD_Text::update_text_input() {
	if (is_focused() && can_take_input())
	{
		std::string authorized_text = get_input_text();
		std::vector<std::string>* pressed_keys = get_base_struct()->get_pressed_keys_frame();
		for (int i = 0; i < pressed_keys->size(); i++) // Check each pressed keys
		{
			std::string key = (*pressed_keys)[i];

			if (key == "left shift" || key == "right shift" || key == "backspace") continue; // Ignored keys

			if (key == "space") { key = " "; }
			else if (key == "enter") { key = "\n"; }

			if (get_base_struct()->get_key_state("left shift") || get_base_struct()->get_key_state("right shift")) // Uppercase the character
			{
				key = to_uppercase(key);
			}

			if (authorized_text == "all" || authorized_text.find(key) != std::string::npos) // If the key is authorized
			{
				set_text(get_text() + key);
				display_cursor = true;
				time_since_last_cursor_display = 0;
			}
		}

		if (get_base_struct()->get_key_state_frame("backspace") == Key_State::Pressed) // Delete a character
		{
			if (get_text().size() > 0)
			{
				set_text(get_text().substr(0, get_text().size() - 1));
				display_cursor = true;
				time_since_last_cursor_display = 0;
			}
		}
	}
}//*/

// HUD_Text destructor
HUD_Text::~HUD_Text() {
    HUD_Object::~HUD_Object();
}

// HUD_Button constructor
HUD_Button::HUD_Button(Base_Struct* a_base_struct, std::string a_name, HUD_Object* parent, Texture* a_texture, VAO* a_vao) : HUD_Text(a_base_struct, a_name, parent, a_texture, a_vao) {
    set_border_width(0.2);
}

// HUD_Button destructor
HUD_Button::~HUD_Button() {
    HUD_Text::~HUD_Text();
}
