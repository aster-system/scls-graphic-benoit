/*
*
* This file represents the core of the "P_Army" mod.
* It represents a test to some Matix things.
* It contains a lot of military things.
*
*/

#pragma once

#include "../../../headers/game.h"
#include "../../headers/mob_loader.h"
#include "../../headers/player.h"

namespace p_army
{
	// Each types of tanks in the army
	enum Tank_Types {
		P_Tank
	};

	class _P_Army_Core_Object
	{
		/*
		* Class representing the core of a P_Army mod object.
		*/
	public:
		// _P_Army_Core_Object constructor
		_P_Army_Core_Object(std::string name, void* core);
		// _P_Army_Core_Object destructor
		~_P_Army_Core_Object();
	protected:
		// Pointer to the _P_Army_Core object
		void* a_core = 0;
	private:
		// Name of the object
		std::string a_name = "";
	};

	class _P_Army_Core
	{
		/*
		* Class representing the core of the P_Army mod.
		*/
	public:
		// _P_Army_Core constructor
		_P_Army_Core(unsigned short window_width, unsigned short window_height, std::string exec_path);
		// _P_Army_Core destructor
		~_P_Army_Core();

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline Player::Camera_Handler* camera() { return a_camera; };
		inline Game* game() { return a_game; };
		inline std::map<std::string, _P_Army_Core_Object*>& objects() { return a_objects; };
	private:
		// Pointer to the P_Army camera handler
		Player::Camera_Handler* a_camera = 0;

		// Pointer to the P_Army game
		Game* a_game = 0;

		// Object in the P_Army
		std::map<std::string, _P_Army_Core_Object*> a_objects = std::map<std::string, _P_Army_Core_Object*>();
	};

	class _P_Army_Object : public _P_Army_Core_Object
	{
		/*
		* Class representing a P_Army mod object.
		*/
	public:
		// _P_Army_Object constructor
		_P_Army_Object(std::string name, void* core);
		// _P_Army_Object destructor
		~_P_Army_Object();

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline _P_Army_Core* core() { return static_cast<_P_Army_Core*>(a_core); };
		inline Game* game() { return core()->game(); };
	private:
		
	};
}