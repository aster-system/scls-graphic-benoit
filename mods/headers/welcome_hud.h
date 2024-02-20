/*
*
* This class represents the "Welcome HUD" mod.
* It adds an HUD welcoming the user.
*
*/

#pragma once

#include "../../headers/game.h"

namespace welcome_hud
{
	class Base_Welcome_HUD : public HUD
	{
		/*
		* Class representing the base welcome HUD.
		*/
	public:
		// Base_Welcome_HUD constructor
		Base_Welcome_HUD(Advanced_Struct* a_advanced_struct, std::string a_name);
		// Base_Welcome_HUD destructor
		~Base_Welcome_HUD();

		// Create the HUD
		void create();
	private:
		// Pointer to the game
		Game* a_game = 0;

		// Each parts of the HUD
		// Background
		HUD_Object* a_background = 0;
		// Title
		HUD_Object* a_title = 0;
	};

	// Load the Base_Welcome_HUD
	void load_base_welcome_hud(Game* game);

	// Set the Base_Welcome_HUD
	void set_base_welcome_hud(Game* game);
}