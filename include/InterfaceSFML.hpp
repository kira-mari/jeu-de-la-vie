#ifndef INTERFACE_SFML_HPP
#define INTERFACE_SFML_HPP

#include "JeuDeLaVie.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

// Détection de la version SFML pour compatibilité 2.x et 3.x
#if SFML_VERSION_MAJOR >= 3
    #define SFML_VERSION_3
    // Alias pour compatibilité
    namespace sf { using KeyCode = sf::Keyboard::Key; }
#else
    #define SFML_VERSION_2
    // Alias pour compatibilité
    namespace sf { using KeyCode = sf::Keyboard::Key; }
#endif

/**
 * @brief Interface graphique avec SFML pour le jeu de la vie
 * 
 * Affichage rapide et léger avec support des motifs et contrôles clavier.
 */
class InterfaceSFML {
public:
    // Dessin par souris
    enum class ModeDessin { Vivante = 0, Morte = 1, ObstacleMorte = 2, ObstacleVivante = 3 };
    
private:
    std::unique_ptr<sf::RenderWindow> fenetre;
    JeuDeLaVie& jeu;
    int tailleCellule;
    bool enPause;
    float delaiIteration; // En secondes
    sf::Clock horloge;
    
    // Couleurs
    sf::Color couleurVivante;
    sf::Color couleurMorte;
    sf::Color couleurObstacle;
    sf::Color couleurGrille;
    
    ModeDessin modeDessin;
    bool sourisEnfoncee;
    int boutonSouris; // 0 = gauche, 1 = milieu, 2 = droite
    sf::View vue; // Vue utilisée pour garder les cellules carrées
    
public:
    /**
     * @brief Constructeur de l'interface graphique SFML
     * @param jeuRef Référence vers le jeu
     * @param largeur Largeur de la fenêtre
     * @param hauteur Hauteur de la fenêtre
     * @param titre Titre de la fenêtre
     */
    InterfaceSFML(
        JeuDeLaVie& jeuRef,
        int largeur = 800,
        int hauteur = 600,
        const std::string& titre = "Jeu de la Vie - SFML"
    );
    
    /**
     * @brief Lance la boucle principale de l'interface graphique
     */
    void executer();
    
    /**
     * @brief Définit le délai entre deux itérations
     * @param delai Le délai en secondes
     */
    void definirDelaiIteration(float delai) { delaiIteration = delai; }
    
    /**
     * @brief Définit la couleur des cellules vivantes
     * @param couleur La nouvelle couleur
     */
    void definirCouleurVivante(const sf::Color& couleur) { couleurVivante = couleur; }
    
    /**
     * @brief Définit la couleur des cellules mortes
     * @param couleur La nouvelle couleur
     */
    void definirCouleurMorte(const sf::Color& couleur) { couleurMorte = couleur; }
    
private:
    /**
     * @brief Gère les événements (clavier, souris, fermeture)
     */
    void gererEvenements();
    
    /**
     * @brief Met à jour l'affichage
     */
    void afficher();
    
    /**
     * @brief Dessine la grille
     */
    void dessinerGrille();
    
    /**
     * @brief Dessine les informations à l'écran
     */
    void dessinerInformations();
    
    /**
     * @brief Calcule la taille optimale des cellules
     */
    void calculerTailleCellule();
    
    /**
     * @brief Gère les entrées clavier pour placer des motifs
     * @param touche La touche pressée
     */
    void gererPlacementMotif(sf::KeyCode touche);
    /**
     * @brief Met à jour la vue pour préserver l'aspect carré des cellules
     */
    void mettreAJourVue();
};

#endif // INTERFACE_SFML_HPP
