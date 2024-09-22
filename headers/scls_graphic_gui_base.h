//******************
//
// scls_graphic_gui_base.h
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
// This file contains the base of the GUI built-in elements.
//

#ifndef SCLS_GRAPHIC_GUI_BASE
#define SCLS_GRAPHIC_GUI_BASE

#include "scls_graphic_object.h"
#include "scls_graphic_window_advanced_struct.h"

// Using of the "scls" namespace to simplify the programmation
namespace scls {

    //*********
    //
    // GUI core classes
    //
    //*********

    // Possibles alignment of a texture
    enum Alignment_Texture {
        T_Fill,
        T_Fit,
        T_Fit_Horizontally,
        T_Fit_Vertically,
        T_User_Defined
    };

    // Type of size definition
    enum _Size_Definition {
        Absolute_Scale_Size,
        Object_Scale_Size,
        Pixel_Size,
        Scale_Size
    };

    struct __GUI_Style_Transformation {
        // Struct representating the style of a GUI object

        // __GUI_Style_Transformation constructor without parent
        __GUI_Style_Transformation(bool modified = false) {
            height_modified = modified;
            x_modified = modified;
            y_modified = modified;
            width_modified = modified;
        };
        // __GUI_Style_Transformation constructor with parent
        __GUI_Style_Transformation(std::shared_ptr<__GUI_Style_Transformation> parent_style, bool modified = false) {
            height_modified = modified;
            parent = parent_style;
            x_modified = modified;
            y_modified = modified;
            width_modified = modified;
        };

        //*********
        //
        // Border handling
        //
        //*********

        // Width of the border
        glm::vec4 border_width = glm::vec4(0, 0, 0, 0);

        // Each modified attribute
        bool border_width_modified = false;

        //*********
        //
        // Transform handling
        //
        //*********

        // Last transformations
        // Last type of definition of the height
        _Size_Definition height_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the width
        _Size_Definition width_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the x
        _Size_Definition x_definition = _Size_Definition::Scale_Size;
        // Last type of definition of the y
        _Size_Definition y_definition = _Size_Definition::Scale_Size;
        // Height position of the object
        Fraction height = Fraction(1);
        // Width position of the object
        Fraction width = Fraction(1);
        // X position of the object
        Fraction x = Fraction(0);
        // Y position of the object
        Fraction y = Fraction(0);

        // Each modified attributes
        bool height_modified = false;
        bool x_modified = false;
        bool y_modified = false;
        bool width_modified = false;

        //*********
        //
        // Other
        //
        //*********

        // Id of the overflighted cursor
        unsigned long cursor = GLFW_ARROW_CURSOR;

        //*********
        //
        // Datas to apply
        //
        //*********

        // Parent of the style
        std::shared_ptr<__GUI_Style_Transformation> parent;
        // Border
        // Border width to apply
        // Return the border width to apply
        glm::vec4 border_width_to_apply() {
            if(border_width_modified || parent.get() == 0) {
                return border_width;
            }
            return parent.get()->border_width_to_apply();
        };
        // Transformation
        // Return the height to apply
        Fraction height_to_apply() {
            if(height_modified || parent.get() == 0) {
                return height;
            }
            return parent.get()->height_to_apply();
        };
        // Return the height definition to apply
        _Size_Definition height_definition_to_apply() {
            if(height_modified || parent.get() == 0) {
                return height_definition;
            }
            return parent.get()->height_definition_to_apply();
        };
        // Return the width to apply
        Fraction width_to_apply() {
            if(width_modified || parent.get() == 0) {
                return width;
            }
            return parent.get()->width_to_apply();
        };
        // Return the width definition to apply
        _Size_Definition width_definition_to_apply() {
            if(height_modified || parent.get() == 0) {
                return width_definition;
            }
            return parent.get()->width_definition_to_apply();
        };
        // Return the X to apply
        Fraction x_to_apply() {
            if(x_modified || parent.get() == 0) {
                return x;
            }
            return parent.get()->x_to_apply();
        };
        // Return the X definition to apply
        _Size_Definition x_definition_to_apply() {
            if(x_modified || parent.get() == 0) {
                return x_definition;
            }
            return parent.get()->x_definition_to_apply();
        };
        // Return the Y to apply
        Fraction y_to_apply() {
            if(y_modified || parent.get() == 0) {
                return y;
            }
            return parent.get()->y_to_apply();
        };
        // Return the Y definition to apply
        _Size_Definition y_definition_to_apply() {
            if(y_modified || parent.get() == 0) {
                return y_definition;
            }
            return parent.get()->y_definition_to_apply();
        };
    };

    class __GUI_Transformation {
        // Class containing every needed thing to do a good transformation system for a GUI
    public:

        //*********
        //
        // __GUI_Transformation main functions
        //
        //*********

        // Most basic __GUI_Transformation constructor
        __GUI_Transformation(unsigned int window_height, unsigned int window_width, std::shared_ptr<__GUI_Transformation> parent);
        // __GUI_Transformation destructor
        virtual ~__GUI_Transformation(){};

        // Calculate the position of the transformation
        void calculate_position();
        // Calculate the size of the transformation
        void calculate_size();
        // Calculate the transformations
        void calculate_transformation();

        // Getters and setters
        inline __GUI_Transformation* parent() const {return a_parent.get();};
        inline std::shared_ptr<__GUI_Transformation>& parent_shared_ptr() {return a_parent;};
        inline unsigned int window_height() const {return a_window_height;};
        inline unsigned int window_width() const {return a_window_width;};

        //*********
        //
        // Border handling
        //
        //*********

        // Returns the border in the pixel plan
        glm::vec4 border_width_in_pixel() const {return a_border_width;};
        // Returns the first absolute extremum of the object in the Y axis
        Fraction object_absolute_y_first_extremum(bool remove_border = false);
        // Returns the last absolute extremum of the object in the Y axis
        Fraction object_absolute_y_last_extremum(bool remove_border = false);
        // Returns the extremum of the object
        glm::vec4 object_extremum();

        //*********
        //
        // Absolute scale plan
        //
        //*********

        // Returns the height of the object in absolute scale
        Fraction height_in_absolute_scale() const;
        // Returns the height of the object in absolute scale from the main window
        Fraction height_in_absolute_scale_and_window() const;
        // Returns the width of the object in absolute scale
        Fraction width_in_absolute_scale() const;
        // Returns the x of the object in absolute scale
        Fraction x_in_absolute_scale() const;
        // Returns the y of the object in absolute scale
        Fraction y_in_absolute_scale() const;

        // One pixel in absolute height
        inline Fraction one_pixel_height_in_absolute_scale() const {return Fraction(1, window_height());};
        // One pixel in absolute width
        inline Fraction one_pixel_width_in_absolute_scale() const {return Fraction(1, window_width());};

        //*********
        //
        // Adapted scale
        //
        //*********

        // Adapted absolute scale plan
        // Returns the height in adapted absolute scale
        inline Fraction height_in_adapted_absolute_scale() const {return a_height_in_adapted_absolute_scale;};
        // Returns the width in adapted absolute scale
        inline Fraction width_in_adapted_absolute_scale() const {return a_width_in_adapted_absolute_scale;};
        // Returns the x in adapted absolute scale
        inline Fraction x_in_adapted_absolute_scale() const {return a_x_in_adapted_absolute_scale;};
        // Returns the y in adapted absolute scale
        inline Fraction y_in_adapted_absolute_scale() const {return a_y_in_adapted_absolute_scale;};

        //*********
        //
        // Object scale plan
        //
        //*********

        // Returns the x in object scale
        inline Fraction x_in_object_scale() { return x_in_scale() - (width_in_scale() * Fraction(1, 2)); }
        // Returns the y in object scale
        inline Fraction y_in_object_scale() { return y_in_scale() - (height_in_scale() * Fraction(1, 2)); }
        // Setters
        inline void set_x_in_object_scale(Fraction new_x) {a_x=new_x;a_last_x_definition=_Size_Definition::Object_Scale_Size;};
        inline void set_y_in_object_scale(Fraction new_y) {a_y=new_y;a_last_y_definition=_Size_Definition::Object_Scale_Size;};

        //*********
        //
        // Pixel plan scale
        //
        //*********

        // Returns the height of the object in pixel plan
        unsigned int height_in_pixel() const;
        // Returns if a pixel is in the object
        inline bool is_in_rect_in_pixel(int x_position, int y_position) const { return x_position >= x_in_absolute_pixel() && y_position >= y_in_absolute_pixel() && x_position < x_in_absolute_pixel() + width_in_pixel() && y_position < y_in_absolute_pixel() + height_in_pixel(); };
        // Returns the width of the object in pixel plan
        unsigned int width_in_pixel() const;
        // Returns the x of the object in pixel plan
        int x_in_pixel() const;
        // Returns the y of the object in pixel plan
        int y_in_pixel() const;

        // Returns the x of the object in absolute pixel plan
        inline int x_in_absolute_pixel() const {if(parent()==0)return x_in_pixel();return x_in_pixel() + parent()->x_in_absolute_pixel();};
        // Returns the y of the object in absolute pixel plan
        inline int y_in_absolute_pixel() const {if(parent()==0)return y_in_pixel();return y_in_pixel() + parent()->y_in_absolute_pixel();};

        // Setters
        inline void set_border_width_in_pixel(int new_width) {set_border_width_in_pixel(glm::vec4(new_width));};
        inline void set_border_width_in_pixel(glm::vec4 new_width) {a_border_width = new_width;};

        // Setters
        inline void set_height_in_pixel(unsigned int new_height) {a_height=new_height;a_last_height_definition=_Size_Definition::Pixel_Size;};
        inline void set_height_in_pixel(Fraction new_height) {set_height_in_pixel(static_cast<unsigned int>(new_height.to_double()));};
        inline void set_position_in_pixel(int new_x, int new_y) {set_x_in_pixel(new_x);set_y_in_pixel(new_y);};
        inline void set_width_in_pixel(unsigned int new_width) {a_width=new_width;a_last_width_definition=_Size_Definition::Pixel_Size;};
        inline void set_width_in_pixel(Fraction new_width) {set_width_in_pixel(static_cast<unsigned int>(new_width.to_double()));};
        inline void set_x_in_pixel(Fraction new_x) {a_x=new_x;a_last_x_definition=_Size_Definition::Pixel_Size;};
        inline void set_x_in_pixel(int x) {set_x_in_pixel(Fraction(x));};
        inline void set_y_in_pixel(Fraction new_y) {a_y=new_y;a_last_y_definition=_Size_Definition::Pixel_Size;};
        inline void set_y_in_pixel(int new_y) {set_y_in_pixel(Fraction(new_y));};
        inline void set_window_height(unsigned int new_height) {a_window_height = new_height;};
        inline void set_window_width(unsigned int new_width) {a_window_width = new_width;};

        //*********
        //
        // Scale plan
        //
        //*********

        // Returns the width of the border in scale
        glm::vec4 border_width_in_scale() const;
        // Returns the height of the object in scale
        Fraction height_in_scale() const;
        // One pixel in height
        inline Fraction one_pixel_height_in_scale() const {if(parent()==0)return one_pixel_height_in_absolute_scale();return Fraction(1, height_in_pixel());};
        // One pixel in width
        inline Fraction one_pixel_width_in_scale() const {if(parent()==0)return one_pixel_width_in_absolute_scale();return Fraction(1, width_in_pixel());};
        // Returns the width of the object in scale
        Fraction width_in_scale() const;
        // Returns the x of the object in scale
        Fraction x_in_scale() const;
        // Returns the y of the object in scale
        Fraction y_in_scale() const;

        // Setters
        inline void set_height_in_scale(Fraction new_height) {a_height=new_height;a_last_height_definition=_Size_Definition::Scale_Size;};
        inline void set_position_in_scale(Fraction new_x, Fraction new_y) {set_x_in_scale(new_x);set_y_in_scale(new_y);};
        inline void set_width_in_scale(Fraction new_width) {a_width=new_width;a_last_width_definition=_Size_Definition::Scale_Size;};
        inline void set_x_in_scale(Fraction new_x) {a_x=new_x;a_last_x_definition=_Size_Definition::Scale_Size;};
        inline void set_y_in_scale(Fraction new_y) {a_y=new_y;a_last_y_definition=_Size_Definition::Scale_Size;};

    private:

        //*********
        //
        // __GUI_Transformation main attributes
        //
        //*********

        // Height of the window
        unsigned int a_window_height = 0;
        // Width of the window
        unsigned int a_window_width = 0;

        //*********
        //
        // Border handling
        //
        //*********

        // Width of the border
        glm::vec4 a_border_width = glm::vec4(0, 0, 0, 0);

        //*********
        //
        // Transform handling
        //
        //*********

        // Height of the object
        Fraction a_height = Fraction(1, 1);
        // Height of the object in absolute adapted scale
        Fraction a_height_in_adapted_absolute_scale = Fraction(1, 1);
        // Height of the object in adapted scale
        Fraction a_height_in_adapted_scale = Fraction(1, 1);
        // Last type of definition of the border width
        _Size_Definition a_last_border_width_definition_type = _Size_Definition::Pixel_Size;
        // Last type of definition of the height
        _Size_Definition a_last_height_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the width
        _Size_Definition a_last_width_definition = _Size_Definition::Pixel_Size;
        // Last type of definition of the x
        _Size_Definition a_last_x_definition = _Size_Definition::Scale_Size;
        // Last type of definition of the y
        _Size_Definition a_last_y_definition = _Size_Definition::Scale_Size;
        // Maximum y position of the object
        Fraction a_object_y_maximum = Fraction(0);
        // Minimum y position of the object
        Fraction a_object_y_minimum = Fraction(0);
        // Parent of this object
        std::shared_ptr<__GUI_Transformation> a_parent;
        // Width of the object
        Fraction a_width = Fraction(1, 1);
        // Width of the object in absolute adapted scale
        Fraction a_width_in_adapted_absolute_scale = Fraction(1, 1);
        // Width of the object in adapted scale
        Fraction a_width_in_adapted_scale = Fraction(1, 1);
        // X position of the object
        Fraction a_x = Fraction(0, 1);
        // X position of the object in absolute adapted scale
        Fraction a_x_in_adapted_absolute_scale = Fraction(0, 1);
        // X position of the object in adapted scale
        Fraction a_x_in_adapted_scale = Fraction(0, 1);
        // X position of the object in real pixel
        int a_x_in_real_pixel = 0;
        // Y position of the object
        Fraction a_y = Fraction(0, 1);
        // Y position of the object in absolute adapted scale
        Fraction a_y_in_adapted_absolute_scale = Fraction(0, 1);
        // Y position of the object in adapted scale
        Fraction a_y_in_adapted_scale = Fraction(0, 1);
        // X position of the object in real pixel
        int a_y_in_real_pixel = 0;
    };

    class __GUI_Object_Core {
        // Class containing the core features for a GUI object
    public:

        //*********
        //
        // __GUI_Object_Core main functions
        //
        //*********

        // Most basic __GUI_Object_Core constructor
        __GUI_Object_Core(_Window_Advanced_Struct& window, __GUI_Object_Core* parent);
        // __GUI_Object_Core destructor
        virtual ~__GUI_Object_Core(){};

        //*********
        //
        // Input handling
        //
        //*********

        // Getters or setters
        inline short focused_state() const {return a_focusing_state;};
        inline bool has_child_clicked_during_this_frame(unsigned int button) { return has_child_overflighted() && window_struct().mouse_button_clicked_during_this_frame(button); };
        inline bool has_child_focused() const {return a_focusing_state > 0;};
        inline bool has_child_overflighted() const {return a_overflighting_state > 0;};
        inline bool is_clicked(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked(button); };
        inline bool is_clicked_during_this_frame(unsigned int button) { return is_overflighted() && window_struct().mouse_button_clicked_during_this_frame(button); };
        inline bool is_focused() const {return a_focusing_state == 0;};
        inline bool is_overflighted() const {return a_overflighting_state == 0;};
        inline unsigned long overflighted_cursor() const {return __overflighted_style_transformation()->cursor;};
        inline short overflighting_state() const {return a_overflighting_state;};
        inline void set_focusing_state(short new_focusing_state) {a_focusing_state = new_focusing_state;};
        inline void set_overflighted_cursor(unsigned long new_overflighted_cursor) {__overflighted_style_transformation()->cursor = new_overflighted_cursor;};
        inline void set_overflighting_state(short new_overflighting_state) {a_overflighting_state = new_overflighting_state;};
        inline _Window_Advanced_Struct& window_struct() const {return a_window;};

        //*********
        //
        // Style handling
        //
        //*********

        // Returns the current style transformation of the GUI
        virtual __GUI_Style_Transformation* __current_style_transformation() const = 0;
        // Returns the overflighted style transformation of the GUI
        virtual __GUI_Style_Transformation* __overflighted_style_transformation() const = 0;

        //*********
        //
        // Transform handling
        //
        //*********

        // Calculate the transformation of the object
        void _apply_calculate_transformation(std::shared_ptr<__GUI_Transformation> current_transformation);
        // Calculate the transformation of the object
        virtual void calculate_transformation(bool force = false, bool with_children = true) {};

        // Getters
        inline __GUI_Transformation* last_transformation() {return a_last_transformation.get();};
        inline  std::shared_ptr<__GUI_Transformation>& last_transformation_shared_ptr() {return a_last_transformation;};
        inline __GUI_Transformation* transformation() {return a_transformation.get();};
        inline std::shared_ptr<__GUI_Transformation>& transformation_shared_ptr() {return a_transformation;};
        inline std::shared_ptr<__GUI_Transformation>& transformation_parent() {return transformation()->parent_shared_ptr();};

        // Returns the first absolute extremum of the object in the Y axis
        Fraction object_absolute_y_first_extremum(bool remove_border = false) {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->object_absolute_y_first_extremum(remove_border);};
        // Returns the last absolute extremum of the object in the Y axis
        Fraction object_absolute_y_last_extremum(bool remove_border = false) {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->object_absolute_y_last_extremum(remove_border);};
        // Returns the extremum of the object
        glm::vec4 object_extremum() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->object_extremum();};

        // Absolute scale plan
        // Returns the height of the object in absolute scale
        Fraction height_in_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->height_in_absolute_scale();};
        // Returns the height of the object in absolute scale from the main window
        Fraction height_in_absolute_scale_and_window() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->height_in_absolute_scale_and_window();};
        // Returns the width of the object in absolute scale
        Fraction width_in_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->width_in_absolute_scale();};
        // One pixel in absolute height
        inline Fraction one_pixel_height_in_absolute_scale() {return Fraction(1, window_struct().window_height());};
        // One pixel in absolute width
        inline Fraction one_pixel_width_in_absolute_scale() {return Fraction(1, window_struct().window_width());};
        // Returns the x of the object in absolute scale
        Fraction x_in_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->x_in_absolute_scale();};
        // Returns the y of the object in absolute scale
        Fraction y_in_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->y_in_absolute_scale();};

        // Adapted absolute scale plan
        // Returns the height in adapted absolute scale
        inline Fraction height_in_adapted_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->height_in_adapted_absolute_scale();};
        // Returns the width in adapted absolute scale
        inline Fraction width_in_adapted_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->width_in_adapted_absolute_scale();};
        // Returns the x in adapted absolute scale
        inline Fraction x_in_adapted_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->x_in_adapted_absolute_scale();};
        // Returns the y in adapted absolute scale
        inline Fraction y_in_adapted_absolute_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->y_in_adapted_absolute_scale();};

        // Object scale plan
        // Returns the x in object scale
        inline Fraction x_in_object_scale() {
            if(a_transformation_updated)calculate_transformation();
            return a_transformation.get()->x_in_object_scale();
        }
        // Returns the y in object scale
        inline Fraction y_in_object_scale() {
            if(a_transformation_updated)calculate_transformation();
            return a_transformation.get()->y_in_object_scale();
        }
        // Setters
        inline void set_x_in_object_scale(Fraction new_x) {
            if(__current_style_transformation()->x_to_apply() == new_x && __current_style_transformation()->x_definition_to_apply() == _Size_Definition::Object_Scale_Size) return;
            __current_style_transformation()->x = new_x;
            __current_style_transformation()->x_definition = _Size_Definition::Object_Scale_Size;
            __current_style_transformation()->x_modified = true;
            a_transformation_updated=true;
        };
        inline void set_y_in_object_scale(Fraction new_y) {
            if(__current_style_transformation()->y_to_apply() == new_y && __current_style_transformation()->y_definition_to_apply() == _Size_Definition::Object_Scale_Size) return;
            __current_style_transformation()->y = new_y;
            __current_style_transformation()->y_definition = _Size_Definition::Object_Scale_Size;
            __current_style_transformation()->y_modified = true;
            a_transformation_updated=true;
        };

        // Pixel plan
        // Returns the height of the object in pixel plan
        unsigned int height_in_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->height_in_pixel();};
        // Returns if a pixel is in the object
        inline bool is_in_rect_in_pixel(int x_position, int y_position) { if(a_transformation_updated)calculate_transformation();return a_transformation.get()->is_in_rect_in_pixel(x_position, y_position); };
        // Returns the width of the object in pixel plan
        unsigned int width_in_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->width_in_pixel();};
        // Returns the x of the object in absolute pixel plan
        int x_in_absolute_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->x_in_absolute_pixel();};
        // Returns the x of the object in pixel plan
        int x_in_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->x_in_pixel();};
        // Returns the y of the object in absolute pixel plan
        inline int y_in_absolute_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->y_in_absolute_pixel();};
        // Returns the y of the object in pixel plan
        int y_in_pixel() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->y_in_pixel();};
        // Setters
        inline void set_border_width_in_pixel(int new_width) {
            __current_style_transformation()->border_width = glm::vec4(new_width);
            __current_style_transformation()->border_width_modified = true;
            a_transformation_updated = true;
        };
        inline void set_height_in_pixel(unsigned int new_height) {
            if(__current_style_transformation()->height_to_apply() == new_height && __current_style_transformation()->height_definition_to_apply() == _Size_Definition::Pixel_Size) return;
            __current_style_transformation()->height = new_height;
            __current_style_transformation()->height_definition = _Size_Definition::Pixel_Size;
            __current_style_transformation()->height_modified = true;
            a_transformation_updated=true;
        };
        inline void set_position_in_pixel(int new_x, int new_y) {set_x_in_pixel(new_x);set_y_in_pixel(new_y);};
        inline void set_width_in_pixel(unsigned int new_width) {
            if(__current_style_transformation()->width_to_apply() == new_width && __current_style_transformation()->width_definition_to_apply() == _Size_Definition::Pixel_Size) return;
            __current_style_transformation()->width = new_width;
            __current_style_transformation()->width_definition = _Size_Definition::Pixel_Size;
            __current_style_transformation()->width_modified = true;
            a_transformation_updated=true;
        };
        inline void set_x_in_pixel(Fraction new_x) {
            if(__current_style_transformation()->x_to_apply() == new_x && __current_style_transformation()->x_definition_to_apply() == _Size_Definition::Pixel_Size) return;
            __current_style_transformation()->x = new_x;
            __current_style_transformation()->x_definition = _Size_Definition::Pixel_Size;
            __current_style_transformation()->x_modified = true;
            a_transformation_updated = true;
        };
        inline void set_x_in_pixel(int x) {set_x_in_pixel(Fraction(x));};
        inline void set_y_in_pixel(Fraction new_y) {
            if(__current_style_transformation()->y_to_apply() == new_y && __current_style_transformation()->y_definition_to_apply() == _Size_Definition::Pixel_Size) return;
            __current_style_transformation()->y = new_y;
            __current_style_transformation()->y_definition = _Size_Definition::Pixel_Size;
            __current_style_transformation()->y_modified = true;
            a_transformation_updated = true;
        };
        inline void set_y_in_pixel(int new_y) {set_y_in_pixel(Fraction(new_y));};

        // Scale plan
        // Returns the width of the border in scale
        glm::vec4 border_width_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->border_width_in_scale();};
        // Returns the height of the object in scale
        Fraction height_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->height_in_scale();};
        // One pixel in height
        inline Fraction one_pixel_height_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->one_pixel_height_in_scale();};
        // One pixel in width
        inline Fraction one_pixel_width_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->one_pixel_width_in_scale();};
        // Returns the width of the object in scale
        Fraction width_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->width_in_scale();};
        // Returns the x of the object in scale
        Fraction x_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->x_in_scale();};
        // Returns the y of the object in scale
        Fraction y_in_scale() {if(a_transformation_updated)calculate_transformation();return a_transformation.get()->y_in_scale();};
        // Setters
        inline void set_height_in_scale(Fraction new_height) {
            if(__current_style_transformation()->height_to_apply() == new_height && __current_style_transformation()->height_definition_to_apply() == _Size_Definition::Scale_Size) return;
            __current_style_transformation()->height = new_height;
            __current_style_transformation()->height_definition = _Size_Definition::Scale_Size;
            __current_style_transformation()->height_modified = true;
            a_transformation_updated=true;
        };
        inline void set_position_in_scale(Fraction new_x, Fraction new_y) {set_x_in_scale(new_x);set_y_in_scale(new_y);};
        inline void set_width_in_scale(Fraction new_width) {
            if(__current_style_transformation()->width_to_apply() == new_width && __current_style_transformation()->width_definition_to_apply() == _Size_Definition::Scale_Size) return;
            __current_style_transformation()->width = new_width;
            __current_style_transformation()->width_definition = _Size_Definition::Scale_Size;
            __current_style_transformation()->width_modified = true;
            a_transformation_updated = true;
        };
        inline void set_x_in_scale(Fraction new_x) {
            if(__current_style_transformation()->x_to_apply() == new_x && __current_style_transformation()->x_definition_to_apply() == _Size_Definition::Scale_Size) return;
            __current_style_transformation()->x = new_x;
            __current_style_transformation()->x_definition = _Size_Definition::Scale_Size;
            __current_style_transformation()->x_modified = true;
            a_transformation_updated=true;
        };
        inline void set_y_in_scale(Fraction new_y) {
            if(__current_style_transformation()->y_to_apply() == new_y && __current_style_transformation()->y_definition_to_apply() == _Size_Definition::Scale_Size) return;
            __current_style_transformation()->y = new_y;
            __current_style_transformation()->y_definition = _Size_Definition::Scale_Size;
            __current_style_transformation()->y_modified = true;
            a_transformation_updated=true;
        };

        // Attach the object
        // Attach the object at the bottom of its parent
        inline void attach_bottom_in_parent(Fraction offset = Fraction(0)) {
            a_transform_attachment.attachment_vertical_offset = offset;
            a_transform_attachment.attachment_vertical_type = 2;
            __move_bottom_in_parent(offset.to_double());
            a_transformation_updated = true;
        };
        // Attach the object bottom of another in the parent
        inline void attach_bottom_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) {
            a_transform_attachment.attached_object_vertical = another_object;
            a_transform_attachment.attachment_vertical_offset = offset;
            a_transform_attachment.attachment_vertical_type = 4;
            __move_bottom_of_object_in_parent(another_object, offset);
            a_transformation_updated = true;
        };
        inline void attach_bottom_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, Fraction offset = Fraction(0)) {attach_bottom_of_object_in_parent(another_object.get(), offset);};
        // Attach the object at the left of its parent
        inline void attach_left_in_parent(int offset = 0) {
            a_transform_attachment.attachment_horizontal_offset = offset;
            a_transform_attachment.attachment_horizontal_type = 1;
            __move_left_in_parent(offset);
            a_transformation_updated = true;
        };
        // Attach the object at the right of its parent
        inline void attach_right_in_parent(int offset = 0) {
            a_transform_attachment.attachment_horizontal_offset = offset;
            a_transform_attachment.attachment_horizontal_type = 2;
            __move_right_in_parent(offset);
            a_transformation_updated = true;
        };
        // Attach the object at the right of an object in its parent
        inline void attach_right_of_object_in_parent(__GUI_Object_Core* another_object, int offset = 0) {
            a_transform_attachment.attached_object_horizontal = another_object;
            a_transform_attachment.attachment_horizontal_offset = offset;
            a_transform_attachment.attachment_horizontal_type = 4;
            __move_right_of_object_in_parent(another_object, offset);
            a_transformation_updated = true;
        };
        // Attach the object at the right of an object in its parent
        inline void attach_right_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, int offset = 0) {attach_right_of_object_in_parent(another_object.get(), offset);};
        // Attach the object at the top of its parent
        inline void attach_top_in_parent(Fraction offset = Fraction(0)) {
            a_transform_attachment.attachment_vertical_offset = offset;
            a_transform_attachment.attachment_vertical_type = 1;
            __move_top_in_parent(offset.to_double());
            a_transformation_updated = true;
        };
        // Attach the object top of another in the parent
        inline void attach_top_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) {
            a_transform_attachment.attached_object_vertical = another_object;
            a_transform_attachment.attachment_vertical_offset = offset;
            a_transform_attachment.attachment_vertical_type = 3;
            __move_top_of_object_in_parent(another_object, offset);
            a_transformation_updated = true;
        };
        inline void attach_top_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, Fraction offset = Fraction(0)) {
            attach_top_of_object_in_parent(another_object.get(), offset);
        };

        // Move the object
        // Move the object at the bottom of its parent
        inline void move_bottom_in_parent(int offset = 0) {
            a_transform_attachment.attachment_vertical_type = 0;
            __move_bottom_in_parent(offset);
            a_transformation_updated = true;
        };
        // Move the object bottom of another in the parent
        inline void move_bottom_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) {
            a_transform_attachment.attachment_vertical_type = 0;
            __move_bottom_of_object_in_parent(another_object, offset);
            a_transformation_updated = true;
        };
        inline void move_bottom_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, Fraction offset = Fraction(0)) { move_bottom_of_object_in_parent(another_object.get(), offset);};
        // Move the object at the left of its parent
        inline void move_left_in_parent(int offset = 0) { a_transform_attachment.attachment_horizontal_type = 0; __move_left_in_parent(offset); a_transformation_updated = true; };
        // Move the object at the right of its parent
        inline void move_right_in_parent(int offset = 0) { a_transform_attachment.attachment_horizontal_type = 0; __move_right_in_parent(offset); a_transformation_updated = true; };
        // Move the object at the right of another of its parent
        inline void move_right_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) { __move_right_of_object_in_parent(another_object, offset); };
        inline void move_right_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, Fraction offset = Fraction(0)) { move_right_of_object_in_parent(another_object.get(), offset); };
        // Move the object at the top of its parent
        inline void move_top_in_parent(int offset = 0) {
            a_transform_attachment.attachment_vertical_type = 0;
            __move_top_in_parent(offset);
            a_transformation_updated = true;
        };
        // Move the object top of another in the parent
        inline void move_top_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) {
            a_transform_attachment.attachment_vertical_type = 0;
            __move_top_of_object_in_parent(another_object, offset);
            a_transformation_updated = true;
        };
        inline void move_top_of_object_in_parent(const std::shared_ptr<__GUI_Object_Core>& another_object, Fraction offset = Fraction(0)) { move_top_of_object_in_parent(another_object.get(), offset); };

        // Move the object without modifications
        // Move the object at the bottom of its parent without influencing attachment
        inline void __move_bottom_in_parent(int offset = 0) {
            set_y_in_pixel(Fraction(offset));
            a_transformation_updated = false;
        };
        // Move the object bottom of another in the parent without influencing attachment
        inline void __move_bottom_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) { set_y_in_pixel(Fraction(another_object->y_in_pixel()) - (Fraction(height_in_pixel()) + offset)); a_transformation_updated = false; };
        // Move the object at the left of its parent without influencing attachment
        inline void __move_left_in_parent(int offset = 0) { set_x_in_pixel(Fraction(offset)); a_transformation_updated = false; };
        // Move the object at the right of its parent without influencing attachment
        inline void __move_right_in_parent(int offset = 0) {
            Fraction width = window_struct().window_width();
            if(transformation_parent()!= 0) width = transformation_parent()->width_in_pixel();
            set_x_in_pixel(width - (Fraction(width_in_pixel()) + Fraction(offset)));
            a_transformation_updated = false;
        };
        // Move the object at the right of an object in its parent without influencing attachment
        inline void __move_right_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = 0) {
            Fraction width = window_struct().window_width();
            if(transformation_parent()!= 0) width = transformation_parent()->width_in_pixel();
            set_x_in_pixel(Fraction(another_object->x_in_pixel() + another_object->width_in_pixel()) + offset);
            a_transformation_updated = false;
        };
        // Move the object at the top of its parent without influencing attachment
        inline void __move_top_in_parent(int offset = 0) {
            Fraction height = window_struct().window_height();
            if(transformation_parent().get()!= 0) height = transformation_parent().get()->height_in_pixel();
            set_y_in_pixel(height - (Fraction(height_in_pixel()) + Fraction(offset)));
            a_transformation_updated = false;
        };
        // Move the object top of another in the parent
        inline void __move_top_of_object_in_parent(__GUI_Object_Core* another_object, Fraction offset = Fraction(0)) { set_y_in_pixel(Fraction(another_object->y_in_pixel() + another_object->height_in_pixel()) + offset); a_transformation_updated = false; };

    protected:

        //*********
        //
        // Transform handling
        //
        //*********

        // If the adapted scale is updated or not
        bool a_transformation_updated = true;

        //*********
        //
        // Transform attachment struct
        //
        //*********

        struct {
            // Horizontal attachment
            // Object attached to this one
            __GUI_Object_Core* attached_object_horizontal = 0;
            // Offset of the horizontal attachment
            Fraction attachment_horizontal_offset = Fraction(0);
            // Type of the horizontal attachment (0 = no attachment, 1 = left, 2 = right)
            char attachment_horizontal_type = 0;

            // Vertical attachment
            // Object attached to this one
            __GUI_Object_Core* attached_object_vertical = 0;
            // Offset of the vertical attachment
            Fraction attachment_vertical_offset = Fraction(0);
            // Type of the vertical attachment (0 = no attachment, 1 = top, 2 = bottom)
            char attachment_vertical_type = 0;
        } a_transform_attachment;

    private:

        //*********
        //
        // __GUI_Object_Core main functions
        //
        //*********

        // Children of this object
        std::vector<std::shared_ptr<__GUI_Object_Core>> a_children = std::vector<std::shared_ptr<__GUI_Object_Core>>();
        // Last transformation of the object
        std::shared_ptr<__GUI_Transformation> a_last_transformation;
        // Transformation of the object
        std::shared_ptr<__GUI_Transformation> a_transformation;
        // Pointer to the main window
        _Window_Advanced_Struct& a_window;

        //*********
        //
        // Input handling
        //
        //*********

        // The state of focusing of the object (0 == focused, > 0 == child focused)
        short a_focusing_state = -1;
        // The state of overflighting of the object (0 == overflighted, > 0 == child overflighted)
        short a_overflighting_state = -1;
    };
}

#endif // SCLS_GRAPHIC_GUI_BASE
