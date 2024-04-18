//******************
//
// scls_graphic_object.h
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
// This file contains the features allowing to handle the displayed things on the screen.
//

#include "scls_graphic_window_advanced_struct.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    class Object
    {
        // Class representing an object displayed into the window
    public:

        //*********
        //
        // Object main functions
        //
        //*********

        // Object most basic constructor
        Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent);
        // Most parent Object constructor used for displaying
        Object(_Window_Advanced_Struct* window_struct, Transform_Object* transform_parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // Object constructor used for displaying
        Object(_Window_Advanced_Struct* window_struct, Object* parent, std::string name, std::string texture_name, std::string vao_name = "hud_default");
        // Object destructor
        virtual ~Object();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline bool contains_child(Object* child) { for(int i = 0;i<static_cast<int>(children().size());i++) {if(children()[i] == child) {return true;}}return false;};
        inline bool contains_tag(std::string tag) { for (int i = 0; i < static_cast<int>(tags().size()); i++) { if (tags()[i] == tag) { return true; } } return false; };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline std::vector<Object*>& children() {return a_children;};
        inline Object* parent() const {return a_parent;};
        inline std::string name() const { return a_name; };
        inline void set_parent(Object* new_parent) {
            if(parent() != 0) {
                parent()->_remove_child(this);
            }
            a_parent = new_parent;
            if(parent() != 0) {
                if(!parent()->contains_child(this)) {
                    parent()->children().push_back(this);
                }
            }
        };
        inline void set_visible(bool new_visible) {a_visible = new_visible;};
        inline std::vector<std::string>& tags() { return a_tags; };
        inline Transform_Object* transform() { return a_transform; };
        inline std::vector<std::string> type() {return a_type;};
        inline std::string type(unsigned short position) {if(position >= a_type.size())return "";return a_type[position];};
        inline bool visible() const {return a_visible;};
        inline _Window_Advanced_Struct* window_struct() { return a_window_struct;};

        //*********
        //
        // Object graphic functions
        //
        //*********

        // Hidden parts of the render object
        void _render(glm::mat4 matrix);
        // Render the object on the window
        virtual void render();

        // Getters and setters (ONLY WITHOUT ATTRIBUTES)
        inline double texture_ratio() {return texture()->image_ratio();};

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline Texture* texture() {return a_texture;};
        inline VAO* vao() {return a_vao;};

        //*********
        //
        // Object heritage functions
        //
        //*********

        virtual void after_loading() {}; // Function called after loading
        virtual void* clone(Object* parent, std::string a_name, std::string texture_name, std::string vao_name = "hud_default"); // Clone the object
        virtual void last_update() {}; // Function called after every updates
        // Reset the object without changing it
        virtual void soft_reset() {};
        virtual void update() {}; // Function called during every updates

        //*********
        //
        // Object transformation facilities
        //
        //*********

        // Getters and setters (ONLY WITH ATTRIBUTES)
        virtual void set_normalized_scale(glm::vec3 new_scale) {set_scale(new_scale);};
        virtual void set_scale(double new_scale) {transform()->set_scale(glm::vec3(new_scale, new_scale, new_scale));};
        virtual void set_scale(glm::vec3 new_scale) {transform()->set_scale(new_scale);};
    protected:
        // Basic object descriptor
        // Type of the object
        std::vector<std::string> a_type = std::vector<std::string>();
    private:
        // Remove a children
        inline void _remove_child(Object* child) {
            for(int i = 0;i<static_cast<int>(children().size());i++) {
                if(children()[i] == child) {
                    children().erase(children().begin() + i);
                    break;
                }
            }
        };

        // Basic object descriptor
        // Children of this object
        std::vector<Object*> a_children = std::vector<Object*>();
        // Parent of the object
        Object* a_parent = 0;
        // Name of the object
        std::string a_name = "";
        // Tags about the object
        std::vector<std::string> a_tags = std::vector<std::string>();
        // Tells if the object is visible or not
        bool a_visible = true;

        // Basic object must-have
        // Transform object attached
        Transform_Object* a_transform = 0;
        // Base struct in the game
        _Window_Advanced_Struct* a_window_struct = 0;

        // Necessary things to do the render
        // Pointer to the VAO used to render the object
        VAO* a_vao = 0;
        // Pointer to the Texture used to render the object
        Texture *a_texture = 0;
    };
}
