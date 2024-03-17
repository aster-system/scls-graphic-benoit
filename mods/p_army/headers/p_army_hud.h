//******************
//
// p_army_hud.h
//
//******************
// Presentation :
//
// P_Army is a Matix mod, made to test the engine.
// It can be tested by everyone.
// This file contains the main HUD of the mod.
//

#ifndef P_ARMY_HUD
#define P_ARMY_HUD

#include "p_army_core.h"

#define P_ARMY_HUD_MOB_PATH (game()->file_formatted("./mods/p_army/mobs/p_army_hud.mob"))
#define P_ARMY_HUD_NAME "p_army_hud"

// Use a namespace to facilite the using of the mod
namespace p_army
{
    class _P_Army_Welcome_HUD_Object : public HUD_Object {
        // Class representing the main HUD object of the welcome HUD
    public:
        // _P_Army_Welcome_HUD_Object constructor
        _P_Army_Welcome_HUD_Object(Advanced_Struct* game, std::string name, HUD_Object* parent, Texture* texture, VAO* vao);
        // _P_Army_Welcome_HUD_Object destructor
        ~_P_Army_Welcome_HUD_Object();

        // Update the HUD
        void update();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline HUD* base_hud() {return reinterpret_cast<HUD*>(parent());};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Game* game() {return a_game;};
    private:
        // Each HUD parts of the HUD_Object
        // Credit of the P_Army_Mod
        HUD_Text* a_credit = 0;
        // Button to go to the information page from the first page
        HUD_Button* a_information_button = 0;
        // Button to launch the mod from the first page
        HUD_Button* a_launch_button = 0;
        // Matix logo
        HUD_Object* a_logo = 0;
        // Title of the HUD
        HUD_Text* a_title = 0;

        // Pointer to the game
        Game* a_game = 0;
    };

    class _P_Army_HUD : public HUD {
        // Class representing the main HUD of the mod
    public:
        // P_Army_HUD constructor
        _P_Army_HUD(Game* game, std::string name);
        // P_Army_HUD destructor
        ~_P_Army_HUD();

        // Change the current page
        // Set the welcome HUD
        void set_welcome_hud();
        // Unset all the pages
        void unset_all();

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Game* game() {return a_game;};
    private:
        // Pointer to the game
        Game* a_game = 0;

        // Welcome main object
        _P_Army_Welcome_HUD_Object* a_welcome_hud = 0;
    };
}

#endif // P_ARMY_HUD
