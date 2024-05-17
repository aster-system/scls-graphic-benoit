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
// This file contains the source code of labyrinth_creator.cpp.
//

#include "labyrinth_creator.h"

// The namespace "labyrinth_creator" is used to simplify the all.
namespace labyrinth_creator {
    // Most basic Labyrinth_Creator constructor
    Labyrinth_Creator::Labyrinth_Creator(std::string exec_path) : Window(900, 1000, exec_path) {
        scls::GUI_Page* page = new_page<scls::GUI_Page>("gui");
        page->set_position(glm::vec3(0, 0, 0));
        page->set_scale(2);

        a_container = page->parent_object();
        a_container->set_background_color(scls::white);
        a_container->set_size_in_pixel(glm::vec2(900, 900));

        display_page("gui");
    }

    // Apply a color according to the numbers
    void Labyrinth_Creator::apply_color_according_to_numbers() {
        for(int i = 0;i<static_cast<int>(a_cases_number.size());i++) {
            double proportion = (255.0 / 10.0) * a_cases_number[i];
            a_cases_color[i] = scls::Color(proportion, proportion, proportion);
        }
    }

    // Apply a number to a case
    void Labyrinth_Creator::apply_number_to_case(int x, int y, unsigned char number) {
        unsigned short case_height = labyrinth_height() * 2 + 1;
        unsigned short case_width = labyrinth_width() * 2 + 1;
        unsigned short position = case_width * y + x;

        unsigned char base_number = a_cases_number[position];
        a_cases_number[position] = number;
        if(x > 0) {
            unsigned short position = (case_width * y + x) - 1;
            if(a_cases_number[position] == base_number) apply_number_to_case(x - 1, y, number);
        }
        if(x < case_width - 1) {
            unsigned short position = (case_width * y + x) + 1;
            if(a_cases_number[position] == base_number) apply_number_to_case(x + 1, y, number);
        }
        if(y > 0) {
            unsigned short position = (case_width * (y - 1) + x);
            if(a_cases_number[position] == base_number) apply_number_to_case(x, y - 1, number);
        }
        if(y < case_height - 1) {
            unsigned short position = (case_width * (y + 1) + x);
            if(a_cases_number[position] == base_number) apply_number_to_case(x, y + 1, number);
        }
    }

    // Create the cases in the labyrinth
    void Labyrinth_Creator::create_cases() {
        unsigned short case_height = labyrinth_height() * 2 + 1;
        unsigned short case_width = labyrinth_width() * 2 + 1;
        unsigned int square_height = static_cast<double>(900.0 / static_cast<unsigned int>(case_height));
        unsigned int square_width = static_cast<double>(900.0 / static_cast<unsigned int>(case_width));
        unload();

        // Create the title
        scls::GUI_Text* title = a_container->new_object<scls::GUI_Text>("title", 0, 0);
        title->set_font_size(75);
        title->set_size_in_pixel(glm::vec2(900, 100));
        title->set_text("Labyrinthe");
        title->set_text_alignment_horizontal(scls::Alignment_Horizontal::H_Center);

        // Create the case
        for(unsigned short j = 0;j<case_width;j++) {
            for(unsigned short i = 0;i<case_height;i++) {
                glm::vec2 square_position = glm::vec2(square_width * i, square_height * j + 100);
                scls::GUI_Object* square = a_container->new_object<scls::GUI_Object>("square_" + std::to_string(i) + "_" + std::to_string(j), square_position[0], square_position[1]);

                a_cases.push_back(square);
                if(i % 2 == 0 || j % 2 == 0) {
                    square->set_background_color(scls::Color(0, 0, 0));
                    a_cases_color.push_back(scls::Color(0, 0, 0));
                }
                else {
                    square->set_background_color(scls::Color(255, 255, 255));
                    a_cases_color.push_back(scls::Color(255, 255, 255));
                }
                square->set_size_in_pixel(glm::vec2(square_width, square_height));
            }
        }

        // Generate each colors
        for(int i = 0;i<case_width;i++) {
            for(int j = 0;j<case_height;j++) {
                if(i % 2 == 0 || j % 2 == 0) a_cases_number.push_back(0);
                else {
                    unsigned char random_number = rand()%10 + 1;
                    a_cases_number.push_back(random_number);
                }
            }
        }
    }

    // Create the labyrinth
    void Labyrinth_Creator::create_labyrinth() {
        // Generate the labyrinth
        unsigned short case_height = labyrinth_height() * 2 + 1;
        unsigned short case_width = labyrinth_width() * 2 + 1;
        for(int i = 0;i<a_cases_number.size() - 1;i++) {
            unsigned short random_y = 1 + (rand() % labyrinth_height()) * 2;
            unsigned short random_x = 1 + (rand() % labyrinth_width()) * 2;
            unsigned char selected_number = a_cases_number[case_width * random_y + random_x];
            scls::Color final_color = scls::Color((255.0/10.0) * selected_number, 0, 0);

            bool zero_tested = false;
            bool one_tested = false;
            bool two_tested = false;
            bool three_tested = false;
            bool unedited = true;
            while(unedited && !(zero_tested && one_tested && two_tested && three_tested)) {
                unsigned char random_direction = rand() % 4;
                if(random_direction == 0) {
                    zero_tested = true;

                    if(random_x > 1) {
                        unsigned char direction_number = a_cases_number[(case_width * random_y + random_x) - 2];
                        if(direction_number != selected_number) {
                            apply_number_to_case(random_x - 2, random_y, selected_number);
                            a_cases_number[(case_width * random_y + random_x) - 1] = selected_number;
                        }
                    }
                }
                else if(random_direction == 1) {
                    one_tested = true;

                    if(random_x < case_width - 2) {
                        unsigned char direction_number = a_cases_number[(case_width * random_y + random_x) + 2];
                        if(direction_number != selected_number) {
                            apply_number_to_case(random_x + 2, random_y, selected_number);
                            a_cases_number[(case_width * random_y + random_x) + 1] = selected_number;
                        }
                    }
                }
                else if(random_direction == 2) {
                    two_tested = true;

                    if(random_y > 1) {
                        unsigned char direction_number = a_cases_number[case_width * (random_y - 2) + random_x];
                        if(direction_number != selected_number) {
                            apply_number_to_case(random_x, random_y - 2, selected_number);
                            a_cases_number[case_width * (random_y - 1) + random_x] = selected_number;
                        }
                    }
                }
                else if(random_direction == 3) {
                    three_tested = true;

                    if(random_y > case_height - 2) {
                        unsigned char direction_number = a_cases_number[case_width * (random_y + 2) + random_x];
                        if(direction_number != selected_number) {
                            apply_number_to_case(random_x, random_y + 2, selected_number);
                            a_cases_number[case_width * (random_y + 1) + random_x] = selected_number;
                        }
                    }
                }
            }
        }

        // Make the labyrinth more beautiful
        apply_number_to_case(1, 1, 10);

        // Apply the final colors
        apply_color_according_to_numbers();
    }

    // Draw each cases according to the numbers
    void Labyrinth_Creator::draw_cases() {
        // Colorate the cases
        for(int i = 0;i<static_cast<int>(a_cases_number.size());i++) {
            a_cases[i]->set_background_color(a_cases_color[i]);
        }
    }

    // Start the creator
    void Labyrinth_Creator::start() {
        create_cases();
        create_labyrinth();
        draw_cases();

        while(run()) {
            update_event();
            update();



            render();
        }

        a_container->texture()->flip_x();
        a_container->texture()->save_png("labyrinth.png");
    }

    // Unload the creator
    void Labyrinth_Creator::unload() {
        a_cases.clear();
        a_cases_color.clear();
        a_cases_number.clear();
    }

    // Create and use a labyrinth creator
    void use_labyrinth_creator(std::string exec_path) {
        srand(time(0));

        Labyrinth_Creator* lc = new Labyrinth_Creator(exec_path);
        lc->start();
        delete lc; lc = 0;
    }
}
