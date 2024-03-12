/*
*
* This file represents the "P_Tank" mod.
* It represents a test to some Matix things, and should be used with P_Army mod.
* It adds the possibility to use tanks in the P_Army mod.
*
*/

#pragma once

#include "p_army_core.h"

#define PTANK_CANON_NAME "canon"
#define PTANK_COMMANDER_VIEW_NAME "commander_view"
#define PTANK_MOB_PATH (game()->file_formatted("./mods/p_army/mobs/p_tank.mob"))
#define PTANK_TURRET_SUPPORT_NAME "turret_support"

namespace p_army
{
	class _Tank_Core : public _P_Army_Object
	{
		/*
		* Class representing a core to create a tank
		*/
	public:
		// _Tank_Core constructor
		_Tank_Core(std::string name, void* core, Tank_Types type);
		// _Tank_Core destructor
		~_Tank_Core();

		// Returns if the tank contains a parts
		bool contains_part(std::string name);

		// Getters and setter (ONLY WITH ATTRIBUTES)
		inline Object* part(std::string name) { if (contains_part(name)) return a_parts[name]; return 0; };
		inline std::map<std::string, Object*>& parts() { return a_parts; };
	private:
		// Type of the tank
		Tank_Types a_type;

		// Parts of the tank, with the name of the part as a key
		std::map<std::string, Object*> a_parts = std::map<std::string, Object*>();
	};
}
