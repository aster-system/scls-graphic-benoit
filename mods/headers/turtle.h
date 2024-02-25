/*
*
* This class represents the "Turtle" mod.
* It adds a turtle, likes in Python.
*
*/

#pragma once

#include "../../headers/game.h"

namespace turtle
{
	class Turtle : public HUD_Object
	{
		/*
		* Class representing the turtle object.
		*/
	public:
		// Turtle constructor
		Turtle(Base_Struct* a_base_struct, std::string a_name, Texture* a_texture, VAO* a_vao);
		// Turtle destructor
		~Turtle();

		// Draw the turtle on the object
		void draw_turtle();
		// Reset the texture of the turtle
		void reset();

		// Getters and setter
		inline glm::vec2 get_last_turtle_position() { return a_last_turtle_position; };
		inline glm::vec4 get_turtle_color(){ return a_turtle_color; };
		inline glm::vec2 get_turtle_position() { return a_turtle_position; };
		inline float get_turtle_speed() { return a_turtle_speed; };
		inline unsigned int get_turtle_width() { return a_turtle_width; };
		inline void set_turtle_position(glm::vec2 new_position) { a_turtle_position = new_position; };
	private:
		// Last position of the turtle
		glm::vec2 a_last_turtle_position = glm::vec2(1, 0);
		// Color of the turtle
		glm::vec4 a_turtle_color = glm::vec4(255, 0, 0, 255);
		// Current position of the turtle
		glm::vec2 a_turtle_position = glm::vec2(0, 0);
		// Speed of the turtle
		float a_turtle_speed = 250;
		// Width of the turtle
		unsigned int a_turtle_width = 10;
	};

	class Turtle_HUD : public HUD
	{
		/*
		* Class representing the an HUD for handle the turtle.
		*/
	public:
		// Turtle_HUD constructor
		Turtle_HUD(Advanced_Struct* a_advanced_struct, std::string a_name);
		// Turtle_HUD destructor
		~Turtle_HUD();

		// Create the Turtle_HUD
		void create();
		// Update the Turlthe_HUD
		void update();
	private:
		// Pointer to the game
		Game* a_game = 0;

		// Turtle object
		Turtle* a_turtle = 0;
	};

	// Load the Turtle_HUD
	void load_turtle_hud(Game* game);

	// Set the Turtle_HUD
	void set_turtle_hud(Game* game);
}