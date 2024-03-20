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
        set_height(1, false);
        set_width(1, false);
        set_sized_according_to_ratio(false);
        set_use_parent_rescaling(false);

        // Load the footer
        a_footer = base_hud()->new_hud_object<HUD_Object>(name + ";footer", this, 1, 1, glm::vec4(0, 0, 0, 255), "hud_default");
        a_footer->set_scale(glm::vec2(16.0 / 9.0, 0.04));
        a_footer->set_position(glm::vec2(0, -0.96));
        a_footer->set_use_parent_rescaling(false);
        // Load the credits
        a_credits = base_hud()->new_hud_object<HUD_Text>(name + ";credits", a_footer, 1, 1, glm::vec4(0, 0, 0, 0), "hud_default");
        a_credits->set_font_color(glm::vec4(255, 255, 255, 255));
        a_credits->set_font_size(50);
        a_credits->set_text(basix::to_utf_8("By Aster Système, founded by Matt Code and Xonix")); // By Aster Système, founded by Matt Code and Xonix
        a_credits->set_height(0.8);
        a_credits->set_position(glm::vec2(16.0/9.0 * (-0.15), 0));
        // Load the header
        a_header = base_hud()->new_hud_object<HUD_Object>(name + ";header", this, game->get_texture("p_army_welcome_hud", true), "hud_default");
        a_header->set_position(glm::vec2(0, 1.0 - 125.0/900.0));
        a_header->set_scale(glm::vec2(16.0/9.0, 125.0/900.0));
        a_header->set_use_parent_rescaling(false);
        a_header->get_texture()->get_image()->flip_y(); a_header->get_texture()->change_texture();
        // Load the information button
        a_information_button = base_hud()->new_hud_object<HUD_Button>(name + ";information_button", a_header, 1, 1, glm::vec4(0, 0, 0, 0), "hud_default");
        a_information_button->set_background_color(glm::vec4(0, 75.0/255.0, 109.0/255.0, 1));
        a_information_button->set_font_color(glm::vec4(255, 255, 255, 255));
        a_information_button->set_font_size(250);
        a_information_button->set_ouf_offset(0.25);
        a_information_button->set_text(basix::to_utf_8("INFORMATION"));
        a_information_button->set_position(glm::vec2(-0.35, 0));
        a_information_button->set_height(0.5);
        // Load the launch button
        a_launch_button = base_hud()->new_hud_object<HUD_Button>(name + ";launch_button", a_header, 1, 1, glm::vec4(0, 0, 0, 0), "hud_default");
        a_launch_button->set_background_color(glm::vec4(0, 75.0/255.0, 109.0/255.0, 1));
        a_launch_button->set_font_color(glm::vec4(255, 255, 255, 255));
        a_launch_button->set_font_size(250);
        a_launch_button->set_ouf_offset(0.25);
        a_launch_button->set_text(basix::to_utf_8("LAUNCH"));
        a_launch_button->set_position(glm::vec2(0, 0));
        a_launch_button->set_height(0.5);
        // Load the media
        a_medias = base_hud()->new_hud_object<HUD_Object>(name + ";medias", this, "p_army_media_hud", "hud_default");
        a_medias->set_position(glm::vec2(0.45, -0.52));
        a_medias->set_scale(glm::vec2(0.8, 0.3));
        // Load the news
        a_news = base_hud()->new_hud_object<HUD_Object>(name + ";news", this, 1, 1, glm::vec4(115, 115, 115, 255), "hud_default");
        a_news->set_position(glm::vec2(-0.5, -0.22));
        a_news->set_width(0.7);
        // Load the title of the news
        a_news_title = base_hud()->new_hud_object<HUD_Text>(name + ";news_title", a_news, 1, 1, glm::vec4(0, 0, 0, 0), "hud_default");
        a_news_title->set_background_color(glm::vec4(0, 0, 0, 0));
        a_news_title->set_font_color(glm::vec4(255, 255, 255, 255));
        a_news_title->set_font_size(150);
        a_news_title->set_text("NEWS");
        a_news_title->set_height(0.1);
        a_news_title->set_position(glm::vec2(0, 0.85));
        // Load the other part
        a_others = base_hud()->new_hud_object<HUD_Object>(name + ";others", this, "p_army_other_hud", "hud_default");
        a_others->set_position(glm::vec2(0.45, 0.18));
        a_others->set_scale(glm::vec2(0.8, 0.3));
    }

    // Update the HUD
    void _P_Army_Welcome_HUD_Object::update() {

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
