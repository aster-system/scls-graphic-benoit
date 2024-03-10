#include "../headers/p_army.h"

namespace p_army
{
	// P_Army constructor
	P_Army::P_Army(std::string exec_path) : a_core(new _P_Army_Core(900, 900, exec_path))
	{

	}

	// Add a tank into the game
	void P_Army::add_tank(std::string name, Tank_Types type)
	{
		_Tank_Core* tank = new _Tank_Core(name, core(), type);
		core()->objects()[name] = tank;
	}

	// Run the P_Army game
	void P_Army::run()
	{
		while (game()->run())
		{
			// Update the game
			game()->update_event();
			game()->update();

			// Render the game
			game()->render();
		}
	}

	// P_Army destructor
	P_Army::~P_Army()
	{
		// Delete every pointers
		delete a_core; a_core = 0;
	}
}