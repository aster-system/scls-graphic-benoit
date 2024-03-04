/*
*
* This file represents the "MOB_Loader" mod.
* It adds a the ability to handle MOB.
* A MOB is an object stored in a file.
* It can handle texturing, VBOs and transformations.
*
*/

#pragma once

#include "../../headers/game.h"

namespace mob_loader
{
	class MOB_Loader
	{
		/*
		* Class representing all the necessary stuff to load a MOB.
		*/
	public:
		// MOB_Loader constructor
		MOB_Loader();
		// MOB_Loader destructor
		~MOB_Loader();

		// Include a MOB path
		void include_mob(std::string path);
		// Load a MOB from a path
		Object* load_from_mob(std::string path, Scene* scene);
	private:

	};
}