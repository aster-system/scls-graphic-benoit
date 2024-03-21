#include "../headers/p_tank.h"

namespace p_army
{
	// _Tank_Core constructor
	_Tank_Core::_Tank_Core(std::string name, void* core, Tank_Types type) : _P_Army_Object(name, core), a_type(type)
	{
		if (type == P_Tank) // Load the tank as a P_Tank
		{
			Scene* scene = game()->get_scene(P_ARMY_SCENE_NAME);

			mob_loader::MOB_Loader loader;
			loader.load_from_mob(PTANK_MOB_PATH, scene);

			float support_length = 4.0f;
			float support_height = 0.55f;
			float support_width = 3.5f;
			float canon_support_length = 1.0f;
			float turret_support_x = -0.6f;
			float turret_support_y = 0.825f;

			Object* chassis_support = scene->new_object(name + ";chassis_support", "ptank_chassis", 0, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "turret", false, true, false);
			Object* turret_support = scene->new_object(name + ";turret_support", "ptank_turret", 0, glm::vec3(turret_support_x, turret_support_y, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "turret", false, true, false);
			Object* canon = scene->new_object(name + ";canon", "ptank_canon", turret_support->get_attached_transform(), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "canon", false, true, false);
			Object* commander_view = scene->new_object(name + ";commander_view", "ptank_commander_view", turret_support->get_attached_transform(), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false, "commander_view", false, true, false);

			float canon_support_x = support_length / 2.0 + canon_support_length / 2.0 - 0.5;
			float canon_support_y = -0.1f;
			canon->get_attached_transform()->move(glm::vec3(canon_support_x, canon_support_y, 0));

			float commander_view_x = 1.25f;
			float commander_view_y = support_height / 2.0f + 0.15f;
			float commander_view_z = -0.5f;
			commander_view->get_attached_transform()->move(glm::vec3(commander_view_x, commander_view_y, commander_view_z));
			commander_view->get_attached_transform()->rotate_plan(glm::vec3(0, 270, 0));

			// Set the object in the tank
			parts()[PTANK_CANON_NAME] = canon;
			parts()[PTANK_COMMANDER_VIEW_NAME] = commander_view;
			parts()[PTANK_TURRET_SUPPORT_NAME] = turret_support;
		}
	}

	// Returns if the tank contains a parts
	bool _Tank_Core::contains_part(std::string name)
	{
		for (std::map<std::string, Object*>::iterator it = parts().begin(); it != parts().end(); it++)
		{
			if (it->first == name) return true;
		}
		return false;
	}

	// _Tank_Core destructor
	_Tank_Core::~_Tank_Core()
	{

	}
}
