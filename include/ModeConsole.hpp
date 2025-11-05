#ifndef MODE_CONSOLE_HPP
#define MODE_CONSOLE_HPP

#include "JeuDeLaVie.hpp"
#include <string>

/**
 * @brief Mode console pour l'exécution et la sauvegarde des itérations
 */
class ModeConsole {
private:
    JeuDeLaVie& jeu;
    std::string dossierSortie;
    bool afficherDansConsole;
    
public:
    /**
     * @brief Constructeur du mode console
     * @param jeuRef Référence vers le jeu
     * @param dossierSortie Le dossier où sauvegarder les résultats
     * @param afficher Si true, affiche aussi dans la console
     */
    ModeConsole(
        JeuDeLaVie& jeuRef,
        const std::string& dossierSortie,
        bool afficher = true
    );
    
    /**
     * @brief Exécute le mode console
     * 
     * Cette méthode exécute toutes les itérations et sauvegarde
     * les résultats dans des fichiers.
     */
    void executer();
    
private:
    /**
     * @brief Affiche la grille dans la console
     */
    void afficherGrille() const;
    
    /**
     * @brief Sauvegarde la grille actuelle dans un fichier
     * @param numeroIteration Le numéro d'itération actuel
     */
    void sauvegarderIteration(int numeroIteration);
};

#endif // MODE_CONSOLE_HPP
