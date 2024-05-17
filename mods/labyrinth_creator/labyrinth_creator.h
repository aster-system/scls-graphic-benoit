//******************
//
// labyrinth_creator.h
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Graphic "Benoit" part allows the user to simply display a graphic window.
// It is named after the Mandelbrot fractal discoverer, Benoit Mandelbrot.
//
// This file contains the features to test the GUI part, with a creator of labyrinth.
//

#ifndef LABYRINTH_CREATOR
#define LABYRINTH_CREATOR

#include <random>
#include "../../scls_graphic.h"

// The namespace "labyrinth_creator" is used to simplify the all.
namespace labyrinth_creator {
    class Labyrinth_Creator : scls::Window {
        // Class representating a creator of labyrinth
    public:
        // Most basic Labyrinth_Creator constructor
        Labyrinth_Creator(std::string exec_path);

        // Apply a color according to the numbers
        void apply_color_according_to_numbers();
        // Apply a number to a case
        void apply_number_to_case(int x, int y, unsigned char number);
        // Create the cases in the labyrinth
        void create_cases();
        // Create the labyrinth
        void create_labyrinth();
        // Draw each cases according to the numbers
        void draw_cases();
        // Start the creator
        void start();
        // Unload the creator
        void unload();

        // Getters and setters
        inline unsigned short labyrinth_height() {return a_labyrinth_height;};
        inline unsigned short labyrinth_width() {return a_labyrinth_width;};
    private:

        //*********
        //
        // Datas about the labyrinth
        //
        //*********

        // Number in each cases of the labyrinth
        std::vector<unsigned char> a_cases_number = std::vector<unsigned char>();
        // Height of the labyrinth
        unsigned short a_labyrinth_height = 22;
        // Width of the labyrinth
        unsigned short a_labyrinth_width = 22;

        // Cases of the labyrinth
        std::vector<scls::GUI_Object*> a_cases = std::vector<scls::GUI_Object*>();
        // Color in each cases of the labyrinth
        std::vector<scls::Color> a_cases_color = std::vector<scls::Color>();
        // Container of the labyrinth
        scls::GUI_Object* a_container = 0;
    };

    // Create and use a labyrinth creator
    void use_labyrinth_creator(std::string exec_path);
}

#endif // LABYRINTH_CREATOR
