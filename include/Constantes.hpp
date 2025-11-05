#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

/**
 * @file Constantes.hpp
 * @brief Configuration centralisée du projet
 */

namespace Config {
    // ========== Interfaces graphiques ==========
    
    /// @brief Taille par défaut des cellules en pixels pour SFML
    constexpr int TAILLE_CELLULE_DEFAUT_SFML = 80;
    
    /// @brief Taille par défaut des cellules en pixels pour Qt
    constexpr int TAILLE_CELLULE_DEFAUT_QT = 80;
    
    /// @brief Taille minimale d'une cellule en pixels
    constexpr int TAILLE_CELLULE_MIN = 2;
    
    /// @brief Hauteur du panneau de contrôle Qt en pixels
    constexpr int HAUTEUR_PANNEAU_QT = 420;
    
    /// @brief Largeur maximale de la fenêtre en pixels
    constexpr int LARGEUR_MAX_FENETRE = 1600;
    
    /// @brief Hauteur maximale de la fenêtre en pixels
    constexpr int HAUTEUR_MAX_FENETRE = 900;
    
    /// @brief Fréquence de rafraîchissement en FPS
    constexpr int FPS_LIMITE = 60;
    
    // ========== Jeu de la vie ==========
    
    /// @brief Nombre maximum d'itérations dans l'historique pour grandes grilles (>100x100)
    constexpr int TAILLE_MAX_HISTORIQUE_GRANDE = 50;
    
    /// @brief Seuil de taille pour considérer une grille comme "grande"
    constexpr int SEUIL_GRANDE_GRILLE = 100;
    
    /// @brief Nombre d'itérations par défaut
    constexpr int ITERATIONS_DEFAUT = 100;
    
    /// @brief Délai par défaut entre deux itérations (secondes)
    constexpr float DELAI_ITERATION_DEFAUT = 0.2f;
    
    /// @brief Délai minimum entre deux itérations (secondes)
    constexpr float DELAI_ITERATION_MIN = 0.01f;
    
    /// @brief Délai maximum entre deux itérations (secondes)
    constexpr float DELAI_ITERATION_MAX = 2.0f;
    
    /// @brief Pas de changement du délai (secondes)
    constexpr float DELAI_ITERATION_PAS = 0.1f;
    
    // ========== Parallélisation ==========
    
    /// @brief Taille minimale de grille pour activer la parallélisation
    /// @details En dessous de 100x100, le multithreading ralentit
    constexpr int TAILLE_MIN_PARALLELE = 100;
    
    // ========== Couleurs (SFML) ==========
    
    /// @brief Couleur des cellules vivantes (vert)
    struct CouleurVivanteSFML {
        static constexpr unsigned char R = 0;
        static constexpr unsigned char G = 255;
        static constexpr unsigned char B = 0;
    };
    
    /// @brief Couleur des cellules mortes (blanc)
    struct CouleurMorteSFML {
        static constexpr unsigned char R = 255;
        static constexpr unsigned char G = 255;
        static constexpr unsigned char B = 255;
    };
    
    /// @brief Couleur des obstacles morts (rouge)
    struct CouleurObstacleMortSFML {
        static constexpr unsigned char R = 255;
        static constexpr unsigned char G = 0;
        static constexpr unsigned char B = 0;
    };
    
    /// @brief Couleur des obstacles vivants (orange)
    struct CouleurObstacleVivantSFML {
        static constexpr unsigned char R = 255;
        static constexpr unsigned char G = 140;
        static constexpr unsigned char B = 0;
    };
    
    /// @brief Couleur de la grille (gris foncé)
    struct CouleurGrilleSFML {
        static constexpr unsigned char R = 50;
        static constexpr unsigned char G = 50;
        static constexpr unsigned char B = 50;
    };
    
    // ========== Couleurs (Qt) - Codes hex ==========
    
    /// @brief Couleur du bouton Play (vert Material Design)
    constexpr const char* COULEUR_BOUTON_PLAY = "#4CAF50";
    
    /// @brief Couleur du bouton Pause (orange Material Design)
    constexpr const char* COULEUR_BOUTON_PAUSE = "#FF9800";
    
    /// @brief Couleur du bouton Reset (rouge Material Design)
    constexpr const char* COULEUR_BOUTON_RESET = "#f44336";
    
    /// @brief Couleur du bouton Step (bleu Material Design)
    constexpr const char* COULEUR_BOUTON_STEP = "#2196F3";
    
    /// @brief Couleur de fond du panneau (gris clair)
    constexpr const char* COULEUR_FOND_PANNEAU = "#f5f5f5";
}

#endif // CONSTANTES_HPP
