//******************
//
// nuclear_energy_usage.h
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
// This file contains a mod made for a school project.
// The documentations / comments will be made in french for clarity purpose.
//

#ifndef PLEOS_NUCLEAR_ENERGY_USAGE
#define PLEOS_NUCLEAR_ENERGY_USAGE

#include "../../scls_graphic.h"
#include "../../mods_hidden/scls_documentalist.h"

// Utilisation de l'espace de nom "pleos" pour simplifier le code
namespace pleos {
    class Nuclear_Energy_Usage : public scls::Window {
        // Classe représentant la base du project Nuclear_Energy_Usage
    public:
        // Constructeur de base de Nuclear_Energy_Usage
        Nuclear_Energy_Usage(int window_width, int window_height, std::string exec_path);
        // Destructeur de base de Nuclear_Energy_Usage
        virtual ~Nuclear_Energy_Usage();

        // Function called after that the window has been resized
        virtual void after_window_resizing(glm::vec2 last_size){Window::after_window_resizing(last_size);set_navigation_button();};

        //*********
        //
        // Trucs de GUI ennuyants
        //
        //*********

        // Charger toute la fenêtre
        void load();
        // Charger la langue anglaise
        void load_en();
        // Charger la page d'explication
        void load_explaination_page();
        // Charger la langue française
        void load_fr();
        // Charger la page de navigation
        void load_navigation_page();
        // Charger la page d'accueil
        void load_welcome_page();
        // Affiche la page d'explication de l'énergie
        void set_explaination_energy_page();
        // Affiche la page d'explication
        void set_explaination_page();
        // Met les boutons à leur endroit actuel
        void set_navigation_button();
        // Affiche la page d'accueil
        void set_welcome_page();
        // Remet les boutons à leur endroit initial
        void unset_navigation_buttons();

        // Commencer le programme
        void start();
    private:

        // Contenu des différents textes dans le programme
        std::map<std::string, std::string> a_text_content = std::map<std::string, std::string>();

        //*********
        //
        // Trucs de GUI ennuyants
        //
        //*********

        // Page actuellement chargée
        std::string a_current_page = "";

        // Page d'explication
        // Page parent de la page d'explication
        scls::HUD_Page* a_explaination_page = 0;
        // Texte d'explication de la page d'explication
        scls::HUD_Text* a_explaination_text = 0;

        // Page d'explication de l'énergie
        // Page parent de la page d'explication
        scls::HUD_Page* a_explaination_energy_page = 0;
        // Texte d'explication de la page d'explication
        scls::HUD_Text* a_explaination_energy_text = 0;

        // Page de navigation
        // Page parent de la page de navigation
        scls::HUD_Page* a_navigation_page = 0;
        // Bouton d'accueil de la page de navigation
        scls::HUD_Text* a_navigation_welcome_button = 0;
        // Bouton d'explication des atomes de la page de navigation
        scls::HUD_Text* a_navigation_explaination_atoms_button = 0;
        // Bouton d'explication de la page de navigation
        scls::HUD_Text* a_navigation_explaination_button = 0;
        // Bouton d'explication de l'énergie nucléaire de la page de navigation
        scls::HUD_Text* a_navigation_explaination_energy_button = 0;
        // Bouton de politique de la page de navigation
        scls::HUD_Text* a_navigation_politic_button = 0;
        // Bouton de production de la page de navigation
        scls::HUD_Text* a_navigation_production_button = 0;
        // Bouton d'ecology de la page de navigation
        scls::HUD_Text* a_navigation_ecology_button = 0;

        // Page d'accueil
        // Page parent de la page d'accueil
        scls::HUD_Page* a_welcome_page = 0;
    };

    // Utiliser facilement le projet
    void use_nuclear_energy_usage(std::string exec_path);
}

#endif // PLEOS_NUCLEAR_ENERGY_USAGE
