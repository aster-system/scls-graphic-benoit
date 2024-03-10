/*
*
* This file represents the "P_Army" mod.
* It represents a test to some Matix things.
* It contains a lot of military things.
*
*/

#pragma once

#include "p_army_core.h"
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

		// Run the P_Army game
		void run();

		// Getters and setters (ONLY FOR ATTRIBUTES)
		inline _P_Army_Core* core() { return a_core; };
		inline Game* game() { return a_core->game(); };
	private:
		// Pointer to the P_Army_Core
		_P_Army_Core* a_core = 0;
	};
}