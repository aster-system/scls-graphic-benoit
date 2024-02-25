#include "../headers/turtle.h"

namespace turtle
{
	// Turtle constructor
	Turtle::Turtle(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao) : HUD_Object(a_base_struct, a_name, a_texture, a_vao)
	{
		reset();
	}

	// Draw the turtle on the object
	void Turtle::draw_turtle()
	{
		if (get_turtle_position() == get_last_turtle_position()) return;

		// Get the position of the turtle on the image
		unsigned int x = (get_texture()->get_image()->get_width() / 2.0) + get_turtle_position()[0];
		unsigned int y = (get_texture()->get_image()->get_height() / 2.0) + get_turtle_position()[1];

		// Draw the turtle on the image
		glm::vec4 color = get_turtle_color();
		unsigned int width = get_turtle_width();
		get_texture()->get_image()->set_pixel(x, y, color.x, color.y, color.z, color[3], width);
		get_texture()->change_texture();

		// Avoid infinite useless recall of the function
		a_last_turtle_position = a_turtle_position;
	}

	// Reset the texture of the turtle
	void Turtle::reset()
	{
		a_last_turtle_position = a_turtle_position + glm::vec2(0, 1);
		get_texture()->get_image()->fill(255, 255, 255);
		get_texture()->change_texture();
	}

	// Turtle destructor
	Turtle::~Turtle()
	{
		
	}

	// Turtle_HUD constructor
	Turtle_HUD::Turtle_HUD(Advanced_Struct* a_advanced_struct, std::string a_name) : HUD(a_advanced_struct, a_name), a_game((Game*)a_advanced_struct)
	{

	}

	// Create the HUD
	void Turtle_HUD::create()
	{
		// Define each variable for the creation
		// Textures
		std::string background_texture = a_game->get_assets_directory_path() + "textures/welcome_hud/background.png";

		// Create each parts
		a_turtle = new_hud_object<Turtle>("background", background_texture);

		// Configure each parts
		a_turtle->set_border_color(glm::vec4(0.0, 0, 0, 1));
		a_turtle->set_border_width(glm::vec4(0.05, 0.05, 0.05, 0.05));
		a_turtle->set_position(glm::vec3(0, 0, -1.0));
		a_turtle->set_scale(glm::vec3(0.75, 0.75, 1.0));
	}

	// Update the HUD
	void Turtle_HUD::update()
	{
		// Handle the movemment of the turtle
		float speed = a_turtle->get_turtle_speed() * a_game->get_delta_time();
		if (a_game->get_key_state("z") == Key_State::Pressed)
		{
			a_turtle->set_turtle_position(a_turtle->get_turtle_position() + glm::vec2(0, speed));
		}
		if (a_game->get_key_state("s") == Key_State::Pressed)
		{
			a_turtle->set_turtle_position(a_turtle->get_turtle_position() + glm::vec2(0, -speed));
		}
		if (a_game->get_key_state("q") == Key_State::Pressed)
		{
			a_turtle->set_turtle_position(a_turtle->get_turtle_position() + glm::vec2(-speed, 0));
		}
		if (a_game->get_key_state("d") == Key_State::Pressed)
		{
			a_turtle->set_turtle_position(a_turtle->get_turtle_position() + glm::vec2(speed, 0));
		}

		if (a_game->get_key_state("a") == Key_State::Pressed)
		{
			a_turtle->reset();
		}

		a_turtle->draw_turtle();
	}

	// Base_Welcome_HUD destructor
	Turtle_HUD::~Turtle_HUD()
	{
		unload();
	}

	// Load the Base_Welcome_HUD
	void load_turtle_hud(Game* game)
	{
		Turtle_HUD* hud = game->new_hud<Turtle_HUD>("turtle_hud");
		hud->create();
	}

	// Set the Base_Welcome_HUD
	void set_turtle_hud(Game* game)
	{
		game->enable_cursor();
		game->set_background_color(glm::vec4(0, 0, 0, 1));
		game->set_current_hud("turtle_hud");
	}
}