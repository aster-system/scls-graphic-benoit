//******************
//
// scls_graphic_gui_object.h
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
// This file contains some basics GUI built-in elements.
//

#ifndef SCLS_GRAPHIC_GUI_OBJECT
#define SCLS_GRAPHIC_GUI_OBJECT

#include "scls_graphic_window.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    class HUD_Object : public Object
    {
        // Class representing an HUD object displayed into the window
    public:

        //*********
        //
        // HUD Object main functions
        //
        //*********

        // HUD_Object most basic constructor
        HUD_Object(Window* window_struct);
        // HUD_Object constructor used for displaying
        HUD_Object(Window* window_struct, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // HUD_Object destructor
        ~HUD_Object();
    };
}

#endif // SCLS_GRAPHIC_GUI_OBJECT
