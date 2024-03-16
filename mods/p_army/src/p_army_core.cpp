#include "../headers/p_army_core.h"

namespace p_army
{
	// _P_Army_Core_Object constructor
	_P_Army_Core_Object::_P_Army_Core_Object(std::string name, void* core) : a_core(core), a_name(name)
	{

	}

	// _P_Army_Core_Object destructor
	_P_Army_Core_Object::~_P_Army_Core_Object()
	{

	}

	// _P_Army_Core constructor
	_P_Army_Core::_P_Army_Core(unsigned short window_width, unsigned short window_height, std::string exec_path)
	{
	    // Create and configurate the game
		a_game = new Game(window_width, window_height, exec_path, false);
		// a_game->load_from_config_file("matix_config.cfg");
		a_game->load_VAOs();

		// Create a scene
		a_scene = game()->new_scene(P_ARMY_SCENE_NAME);

		// Create the camera
		a_camera = scene()->new_object<Player::Camera_Handler>("viewer", "", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "", false, false, false);
	}

	// _P_Army_Core destructor
	_P_Army_Core::~_P_Army_Core()
	{
		// Delete every _P_Army_Core
		for (std::map<std::string, _P_Army_Core_Object*>::iterator it = objects().begin(); it != objects().end(); it++)
		{
			delete it->second; it->second = 0;
		}

		// Delete every pointers
		delete a_game; a_game = 0;
	}

	// _P_Army_Object constructor
	_P_Army_Object::_P_Army_Object(std::string name, void* core) : _P_Army_Core_Object(name, core)
	{

	}

	// _P_Army_Object destructor
	_P_Army_Object::~_P_Army_Object()
	{
		_P_Army_Core_Object::~_P_Army_Core_Object();
	}
}
