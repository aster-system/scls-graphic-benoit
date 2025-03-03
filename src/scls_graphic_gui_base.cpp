//******************
//
// scls_graphic_gui_base.cpp
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
// This file contains the code in scls_graphic_gui_base.h
//

#include "../headers/scls_graphic_gui_base.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // __GUI_Transformation main functions
    //
    //*********

    // Most basic __GUI_Transformation constructor
    __GUI_Transformation::__GUI_Transformation(unsigned int window_height, unsigned int window_width, std::shared_ptr<__GUI_Transformation> parent) : a_window_height(window_height), a_window_width(window_width), a_parent(parent) {}

    // Calculate the position of the transformation
    void __GUI_Transformation::calculate_position() {
        Fraction divisor_x = Fraction(1, window_width());
        Fraction divisor_y = Fraction(1, window_height());

        a_x_in_adapted_absolute_scale = divisor_x * x_in_absolute_pixel() * 2 + divisor_x / 4;
        a_y_in_adapted_absolute_scale = divisor_y * y_in_absolute_pixel() * 2 + divisor_y / 4;

        Fraction absolute_height_to_apply = height_in_adapted_absolute_scale();

        // Calculate the y minimum and maximum scale
        a_object_y_maximum = object_absolute_y_last_extremum();
        a_object_y_minimum = object_absolute_y_first_extremum();
        if(parent() != 0) {
            a_object_y_maximum -= y_in_absolute_pixel();
            a_object_y_minimum -= y_in_absolute_pixel();
        }
        // Apply the local transformations
        a_object_y_maximum *= one_pixel_height_in_scale();
        a_object_y_minimum *= one_pixel_height_in_scale();
    }

    // Calculate the size of the transformation
    void __GUI_Transformation::calculate_size() {
        Fraction divisor_x = Fraction(1, window_width());
        Fraction divisor_y = Fraction(1, window_height());

        // Calculate the real height with the pixel perfect system
        a_height_in_adapted_absolute_scale = divisor_y * height_in_pixel();
        a_height_in_adapted_scale = a_height_in_adapted_absolute_scale;
        if(parent() != 0) a_height_in_adapted_scale /= parent()->height_in_adapted_absolute_scale();
        a_height_in_adapted_scale -= divisor_y / 2;

        // Calculate the real width with the pixel perfect system
        a_width_in_adapted_absolute_scale = divisor_x * width_in_pixel() + divisor_x;
        a_width_in_adapted_scale = a_width_in_adapted_absolute_scale;
        if(parent() != 0) a_width_in_adapted_scale /= parent()->width_in_adapted_absolute_scale();
        a_width_in_adapted_scale -= divisor_x / 2;

        // Calculate the y minimum and maximum scale
        a_object_y_maximum = object_absolute_y_last_extremum();
        a_object_y_minimum = object_absolute_y_first_extremum();
        if(parent() != 0) {
            a_object_y_maximum -= y_in_absolute_pixel();
            a_object_y_minimum -= y_in_absolute_pixel();
        }
        // Apply the local transformations
        a_object_y_maximum *= one_pixel_height_in_scale();
        a_object_y_minimum *= one_pixel_height_in_scale();
    }

    // Calculate the transformation
    void __GUI_Transformation::calculate_transformation() {

        // Calculate the size
        calculate_size();
        // Calculate the position
        calculate_position();
    }

    //*********
    //
    // Border handling
    //
    //*********

    // Returns the first absolute extremum of the object in the Y axis
    Fraction __GUI_Transformation::object_absolute_y_first_extremum(bool remove_border) {
        Fraction to_return = y_in_absolute_pixel();
        if(remove_border) to_return += border_width_in_pixel()[2];
        Fraction parent_extremum = Fraction(-1);
        if(parent() != 0) { parent_extremum = parent()->object_absolute_y_first_extremum(true); }
        if(parent_extremum > to_return) to_return = parent_extremum;
        return to_return;
    }

    // Returns the last absolute extremum of the object in the Y axis
    Fraction __GUI_Transformation::object_absolute_y_last_extremum(bool remove_border) {
        Fraction to_return = (y_in_absolute_pixel() + height_in_pixel());
        if(remove_border) to_return -= border_width_in_pixel()[0];
        Fraction parent_extremum = Fraction(window_height());
        if(parent() != 0) { parent_extremum = parent()->object_absolute_y_last_extremum(true); }
        if(parent_extremum < to_return) to_return = parent_extremum;
        return to_return;
    }

    // Returns the extremum of the object
    glm::vec4 __GUI_Transformation::object_extremum() { return glm::vec4(-1.0, a_object_y_minimum.to_double(), 2.0, a_object_y_maximum.to_double()); }

    //*********
    //
    // Absolute scale plan
    //
    //*********

    // Returns the height in absolute scale of the object
    Fraction __GUI_Transformation::height_in_absolute_scale() const {
        if(a_last_height_definition == _Size_Definition::Absolute_Scale_Size || (a_last_height_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_height;
        Fraction to_return = a_height;

        if(a_last_height_definition == _Size_Definition::Pixel_Size) {to_return *= one_pixel_height_in_absolute_scale();}
        else {if(parent() != 0) { to_return *= parent()->height_in_absolute_scale(); }}

        return to_return;
    }

    // Returns the height in absolute scale and by counting the window of the object
    Fraction __GUI_Transformation::height_in_absolute_scale_and_window() const { return height_in_absolute_scale() / Fraction(window_width(), window_height()); }

    // Returns the width in absolute scale of the object
    Fraction __GUI_Transformation::width_in_absolute_scale() const {
        if(a_last_width_definition == _Size_Definition::Absolute_Scale_Size || (a_last_width_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_width;
        Fraction to_return = a_width;

        if(a_last_width_definition == _Size_Definition::Pixel_Size) {to_return *= one_pixel_width_in_absolute_scale();}
        else if(a_last_width_definition == _Size_Definition::Scale_Size && parent() != 0) {to_return *= parent()->width_in_absolute_scale();}

        return to_return;
    }

    // Returns the x position in absolute scale of the object
    Fraction __GUI_Transformation::x_in_absolute_scale() const {
        if(a_last_x_definition == _Size_Definition::Absolute_Scale_Size || (a_last_x_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_x;
        Fraction to_return = Fraction(0);
        Fraction to_add = Fraction(0);
        if(parent() != 0) to_add += parent()->x_in_absolute_scale();

        // Check for the needed convertions
        if(a_last_x_definition == _Size_Definition::Pixel_Size) {to_return = a_x * one_pixel_width_in_absolute_scale();}
        else if(a_last_x_definition == _Size_Definition::Object_Scale_Size) {
            to_return = (width_in_absolute_scale() * Fraction(-1, 2));
            if(parent() == 0) to_return += a_x;
            else to_return += a_x * parent()->width_in_absolute_scale();
        }
        else {
            to_return = a_x;
            if(parent() != 0) to_return *= parent()->width_in_absolute_scale();
        }

        to_return += to_add;
        return to_return;
    }

    // Returns the y position in absolute scale of the object
    Fraction __GUI_Transformation::y_in_absolute_scale() const {
        if(a_last_y_definition == _Size_Definition::Absolute_Scale_Size || (a_last_y_definition == _Size_Definition::Scale_Size && parent() == 0)) return a_y;
        Fraction to_return = Fraction(0);
        Fraction to_add = Fraction(0);
        if(parent() != 0) to_add += parent()->y_in_absolute_scale();

        // Check for the needed convertions
        if(a_last_y_definition == _Size_Definition::Pixel_Size) {to_return = a_y * one_pixel_height_in_absolute_scale();}
        else if(a_last_y_definition == _Size_Definition::Object_Scale_Size) {
            to_return = (height_in_absolute_scale() * Fraction(-1, 2));
            if(parent() == 0) to_return += a_y;
            else to_return += a_y * parent()->height_in_absolute_scale();
        }
        else {
            to_return = a_y;
            if(parent() != 0) to_return *= parent()->height_in_absolute_scale();
        }

        to_return += to_add;
        return to_return;
    }

    //*********
    //
    // Pixel plan scale
    //
    //*********

    // Returns the height in pixel of the object
    unsigned int __GUI_Transformation::height_in_pixel() const {
        if(a_last_height_definition == _Size_Definition::Pixel_Size) return static_cast<unsigned int>(a_height.to_double());

        // Calculate the real height with the pixel perfect system
        Fraction divisor_y = Fraction(1, window_height());
        Fraction start_height = height_in_absolute_scale();
        unsigned int real_height_in_pixel = 0;
        while(start_height >= divisor_y) {
            start_height -= divisor_y;
            real_height_in_pixel++;
        }
        if(start_height > 0 && start_height < divisor_y / 2.0) real_height_in_pixel++;
        return real_height_in_pixel;
    }

    // Returns the width in pixel of the object
    unsigned int __GUI_Transformation::width_in_pixel() const {
        if(a_last_width_definition == _Size_Definition::Pixel_Size) return a_width.to_double();

        // Calculate the real width with the pixel perfect system
        Fraction divisor_x = Fraction(1, window_width());
        Fraction start_width = width_in_absolute_scale();
        unsigned int real_width_in_pixel = 0;
        while(start_width >= divisor_x) {
            start_width -= divisor_x;
            real_width_in_pixel++;
        }
        if(start_width > 0 && start_width < divisor_x / 2.0) real_width_in_pixel++;
        return real_width_in_pixel;
    }

    // Returns the x position in pixel of the object
    int __GUI_Transformation::x_in_pixel() const {
        if(a_last_x_definition == _Size_Definition::Pixel_Size) return static_cast<int>(a_x.to_double());
        Fraction to_return = (x_in_absolute_scale() / one_pixel_width_in_absolute_scale());
        if(parent() != 0) to_return -= parent()->x_in_absolute_pixel();
        return static_cast<int>(to_return.to_double());
    }

    // Returns the y position in pixel of the object
    int __GUI_Transformation::y_in_pixel() const {
        if(a_last_y_definition == _Size_Definition::Pixel_Size) return static_cast<int>(a_y.to_double());
        Fraction to_return = (y_in_absolute_scale() / one_pixel_height_in_absolute_scale());
        if(parent() != 0) to_return -= parent()->y_in_absolute_pixel();
        return static_cast<int>(to_return.to_double());
    }

    //*********
    //
    // Scale plan
    //
    //*********

    // Returns the scale of the border width
    glm::vec4 __GUI_Transformation::border_width_in_scale() const {
        double one_pixel_height = one_pixel_height_in_scale().to_double();
        double one_pixel_width = one_pixel_width_in_scale().to_double();
        return a_border_width * glm::vec4(one_pixel_height, one_pixel_width, one_pixel_height, one_pixel_width);
    }

    // Returns the height in scale of the object
    Fraction __GUI_Transformation::height_in_scale() const {
        if(a_last_height_definition == _Size_Definition::Scale_Size || (a_last_height_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_height;
        Fraction to_return = height_in_absolute_scale();
        if(parent() != 0) to_return /= parent()->height_in_absolute_scale();

        return to_return;
    }

    // Returns the width in scale of the object
    Fraction __GUI_Transformation::width_in_scale() const {
        if(a_last_width_definition == _Size_Definition::Scale_Size || (a_last_width_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_width;
        if(parent() != 0) return width_in_absolute_scale() / parent()->width_in_absolute_scale();
        return width_in_absolute_scale();
    }

    // Returns the x position in scale of the object
    Fraction __GUI_Transformation::x_in_scale() const {
        if(a_last_x_definition == _Size_Definition::Scale_Size || (a_last_width_definition == _Size_Definition::Absolute_Scale_Size && parent() == 0)) return a_x;
        if(parent() != 0) return x_in_absolute_scale() / parent()->x_in_absolute_scale();
        return x_in_absolute_scale();
    }

    // Returns the y position in scale of the object
    Fraction __GUI_Transformation::y_in_scale() const {
        if(a_last_y_definition == _Size_Definition::Scale_Size) { return a_y; }

        Fraction to_return = y_in_absolute_scale();
        if(parent() != 0) { to_return = (to_return - parent()->y_in_absolute_scale()) / parent()->height_in_absolute_scale(); }

        return to_return;
    }

    //*********
    //
    // __GUI_Object_Core main functions
    //
    //*********

    // Most basic __GUI_Object_Core constructor
    __GUI_Object_Core::__GUI_Object_Core(_Window_Advanced_Struct& window, __GUI_Object_Core* parent) : a_window(window) {
         if(parent != 0){a_transformation = std::make_shared<__GUI_Transformation>(window_struct().window_height(), window_struct().window_width(), parent->transformation_shared_ptr());}
         else {a_transformation = std::make_shared<__GUI_Transformation>(window_struct().window_height(), window_struct().window_width(), std::shared_ptr<__GUI_Transformation>());}
    }

    // Calculate the transformation of the object
    void __GUI_Object_Core::_apply_calculate_transformation(std::shared_ptr<__GUI_Transformation> current_transformation) {
        // Calculate the transformation
        // Pass the border
        current_transformation.get()->set_border_width_in_pixel(__current_style_transformation()->border_width_to_apply());
        // Pass the height
        if(__current_style_transformation()->height_definition_to_apply() == _Size_Definition::Scale_Size || __current_style_transformation()->height_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_height_in_scale(__current_style_transformation()->height_to_apply());
        else if(__current_style_transformation()->height_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_height_in_pixel(__current_style_transformation()->height_to_apply());
        // Pass the width
        if(__current_style_transformation()->width_definition_to_apply() == _Size_Definition::Scale_Size || __current_style_transformation()->width_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_width_in_scale(__current_style_transformation()->width_to_apply());
        else if(__current_style_transformation()->width_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_width_in_pixel(__current_style_transformation()->width_to_apply());
        // Pass the X
        if(__current_style_transformation()->x_definition_to_apply() == _Size_Definition::Scale_Size) current_transformation.get()->set_x_in_scale(__current_style_transformation()->x_to_apply());
        else if(__current_style_transformation()->x_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_x_in_object_scale(__current_style_transformation()->x_to_apply());
        else if(__current_style_transformation()->x_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_x_in_pixel(__current_style_transformation()->x_to_apply());
        // Pass the Y
        if(__current_style_transformation()->y_definition_to_apply() == _Size_Definition::Scale_Size) current_transformation.get()->set_y_in_scale(__current_style_transformation()->y_to_apply());
        else if(__current_style_transformation()->y_definition_to_apply() == _Size_Definition::Object_Scale_Size) current_transformation.get()->set_y_in_object_scale(__current_style_transformation()->y_to_apply());
        else if(__current_style_transformation()->y_definition_to_apply() == _Size_Definition::Pixel_Size) current_transformation.get()->set_y_in_pixel(__current_style_transformation()->y_to_apply());
    }
}
