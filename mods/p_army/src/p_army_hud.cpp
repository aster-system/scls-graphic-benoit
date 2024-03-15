//******************
//
// p_army_hud.h
//
//******************
// Presentation :
//
// P_Army is a Matix mod, made to test the engine.
// It can be tested by everyone.
// This file contains the CPP of p_army_hud.h.
//

#include "../headers/p_army_hud.h"

namespace p_army {

    // _P_Army_Welcome_HUD_Object constructor
    _P_Army_Welcome_HUD_Object::_P_Army_Welcome_HUD_Object(Advanced_Struct* game, std::string name, HUD_Object* parent, Texture* texture, VAO* vao) : HUD_Object(reinterpret_cast<Base_Struct*>(game), name, parent, texture, vao) {
        a_game = reinterpret_cast<Game*>(game);

        set_position(glm::vec3(0, 0, 0));

        // Load the credi
        a_credit = base_hud()->new_hud_object<HUD_Text>(name + ";credit", this, 1, 1, glm::vec4(0, 0, 0, 1), "hud_default");
        a_credit->set_position(glm::vec3(0, -0.5, 0));
        a_credit->set_scale(glm::vec3(0.5, 0.1, 1));
        a_credit->set_text(basix::to_utf_8("By Aster Système, founded by Matt Code"));
        // Load the logo
        a_logo = base_hud()->new_hud_object(name + ";logo", this, "matix_logo", "hud_default");
        a_logo->set_position(glm::vec3(0, 0, 0));
        a_logo->set_scale(glm::vec3(0.1, 0.1, 1));
    }

    // _P_Army_Welcome_HUD_Object destructor
    _P_Army_Welcome_HUD_Object::~_P_Army_Welcome_HUD_Object() {
        HUD_Object::~HUD_Object();
    }

    // P_Army_HUD constructor
    _P_Army_HUD::_P_Army_HUD(Game* game_struct, std::string name) : HUD(reinterpret_cast<Advanced_Struct*>(game_struct), name)
    {
        a_game = game_struct;

        // Load the mob containing the HUD
        mob_loader::MOB_Loader loader;
        loader.load_from_mob(P_ARMY_HUD_MOB_PATH, game()->get_scene(P_ARMY_SCENE_NAME));

        a_welcome_hud = new_hud_object<_P_Army_Welcome_HUD_Object>("welcome_hud", this, "p_army_welcome_hud", "hud_default");
    }

    // Set the welcome HUD
    void _P_Army_HUD::set_welcome_hud() {
        game()->set_background_color(glm::vec4(1, 1, 1, 1));

        unset_all();
        a_welcome_hud->set_visible(true);
    }

    // Unset all the pages
    void _P_Army_HUD::unset_all() {
        a_welcome_hud->set_visible(false);
    };

    // P_Army_HUD destructor
    _P_Army_HUD::~_P_Army_HUD() {
        HUD::~HUD();
    }
}
