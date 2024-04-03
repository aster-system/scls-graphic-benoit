//******************
//
// scls_graphic_gui_object.cpp
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
// This file contains the code in scls_graphic_gui_object.h
//

#include "../headers/scls_graphic_gui_object.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {
    //*********
    //
    // HUD page
    //
    //*********

    // HUD_Page most basic constructor
    HUD_Page::HUD_Page(_Window_Advanced_Struct* window_struct, std::string name) : _Page(window_struct, name) {

    }

    // HUD_Page destructor
    HUD_Page::~HUD_Page(){

    }

    //*********
    //
    // HUD Object main functions
    //
    //*********

    // HUD_Object most basic constructor
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct)) {

    }

    // HUD_Object constructor used for displaying
    HUD_Object::HUD_Object(_Window_Advanced_Struct* window_struct, std::string name, std::string texture_name, std::string vao_name) : Object(reinterpret_cast<_Window_Advanced_Struct*>(window_struct), name, texture_name, vao_name) {

    }

    // HUD_Object destructor
    HUD_Object::~HUD_Object() {

    }
}
