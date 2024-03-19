#include "../headers/p_army.h"

namespace p_army
{
	// P_Army constructor
	P_Army::P_Army(std::string exec_path) : a_core(new _P_Army_Core(900, 900, exec_path))
	{
	    mob_maker::create_p_army_mob(game());

	    a_hud = game()->new_hud<_P_Army_HUD>(P_ARMY_HUD_NAME);
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
	    set_simulation();
	    // set_welcome_hud();
		while (game()->run())
		{
			// Update the game
			game()->update_event();
			game()->update();

			if(hud() != 0 && hud()->launch_clicked()) {
                set_simulation();
			}

			// Render the game
			game()->render();
		}
	}

	// Set the 3D simulation
    void P_Army::set_simulation() {
        game()->disable_cursor();
        game()->set_current_hud("");
        game()->set_current_scene(P_ARMY_SCENE_NAME);
    }

	// Change the P_Army state
	void P_Army::set_welcome_hud() {
	    hud()->set_welcome_hud();
	    game()->enable_cursor();
	    game()->set_current_scene("");
        game()->set_current_hud(hud()->get_name());
	}

	// P_Army destructor
	P_Army::~P_Army()
	{
		// Delete every pointers
		delete a_core; a_core = 0;
	}
}
