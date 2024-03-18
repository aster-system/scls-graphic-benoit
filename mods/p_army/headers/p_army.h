/*
*
* This file represents the "P_Army" mod.
* It represents a test to some Matix things.
* It contains a lot of military things.
*
*/

#pragma once

#include "../../../mods_hidden/headers/mob_maker.h"
#include "p_army_core.h"
#include "p_army_hud.h"
#include "p_tank.h"

namespace p_army
{
	class P_Army
	{
		/*
		* Class representing the main Handler of the P_Army mod.
		*/
	public:
		// P_Army constructor
		P_Army(std::string exec_path);
		// P_Army destructor
		~P_Army();

		// Add things into the game
		// Add a tank into the game
		void add_tank(std::string name, Tank_Types type);

		// Change the P_Army state
		// Set the 3D simulation
		void set_simulation();
		// Set the welcome HUD
		void set_welcome_hud();

		// Run the P_Army game
		void run();

		// Getters and setters (ONLY FOR ATTRIBUTES)
		inline _P_Army_Core* core() { return a_core; };
		inline Game* game() { return a_core->game(); };
		inline _P_Army_HUD* hud() {return a_hud;};
	private:
		// Pointer to the P_Army_Core
		_P_Army_Core* a_core = 0;

		// Pointer to the P_Army HUD
		_P_Army_HUD* a_hud = 0;
	};
}
