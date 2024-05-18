//******************
//
// p_figther_cockpit.h
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
// This file contains the features for the mod "P-Fighter cockpit", made to test the new version of Benoit.
//

#ifndef P_FIGHTER_COCKPIT
#define P_FIGHTER_COCKPIT

#include "../../scls_graphic.h"

// The namespace "scls_documentalist_gui" is used to simplify the all.
namespace p_fighter_cockpit {
    class P_Fighter_Cockpit : public scls::GUI_Page {
        // Class representating a page of P_Fighter_Cockpit
    public:
        //*********
        //
        // Main P_Fighter_Cockpit functions
        //
        //*********

        // Most basic P_Fighter_Cockpit constructor
        P_Fighter_Cockpit(scls::_Window_Advanced_Struct* window, std::string name);
        // P_Fighter_Cockpit destructor
        virtual ~P_Fighter_Cockpit();
    private:

    };

    // Use the P-Fighter cockpit mod
    void use_p_fighter_cockpit(std::string exec_path);
}

#endif // P_FIGHTER_COCKPIT
