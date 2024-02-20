#include "../headers/welcome_hud.h"

namespace welcome_hud
{
	// Base_Welcome_HUD constructor
	Base_Welcome_HUD::Base_Welcome_HUD(Advanced_Struct* a_advanced_struct, std::string a_name) : HUD(a_advanced_struct, a_name), a_game((Game*)a_advanced_struct)
	{

	}

	// Create the HUD
	void Base_Welcome_HUD::create()
	{
		// Define each variable for the creation
		// Textures
		std::string background_texture = a_game->get_assets_directory_path() + "textures/welcome_hud/background.png";
		std::string title_texture = a_game->get_assets_directory_path() + "textures/welcome_hud/title.png";

		// Create each parts
		a_background = new_hud_object("background", background_texture);
		a_title = new_hud_object("title", title_texture);

		// Configure each parts
		a_title->set_border_color(glm::vec4(0.5, 0, 0, 1));
		a_title->set_border_width(glm::vec4(0.1, 0.1, 0.1, 0.1));
		a_title->set_position(glm::vec3(0.0, 0.5, -1.0));
		a_title->set_scale(glm::vec3(0.4, 0.4, 1.0));
	}

	// Base_Welcome_HUD destructor
	Base_Welcome_HUD::~Base_Welcome_HUD()
	{
		unload();
	}

	// Load the Base_Welcome_HUD
	void load_base_welcome_hud(Game* game)
	{
		Base_Welcome_HUD* hud = game->new_hud<Base_Welcome_HUD>("base_welcome_hud");
		hud->create();
	}

	// Set the Base_Welcome_HUD
	void set_base_welcome_hud(Game* game)
	{
		game->enable_cursor();
		game->set_background_color(glm::vec4(0, 0, 0, 1));
		game->set_current_hud("base_welcome_hud");
	}
}