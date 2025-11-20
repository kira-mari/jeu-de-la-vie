#ifndef JEU_DE_LA_VIE_HPP
#define JEU_DE_LA_VIE_HPP

#include "Grille.hpp"
#include "Constantes.hpp"
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Classe principale orchestrant le jeu de la vie
 * 
 * Gère l'état du jeu, l'historique et l'exécution des itérations.
 */
class JeuDeLaVie {
private:
    std::unique_ptr<Grille> grille;
    int iteration;
    int iterationMax;
    bool modeTorique;
    bool modeParallele;
    std::vector<std::unique_ptr<Grille>> historique;
    bool grandeGrille; // true si grille >= 100x100
    
public:
    /**
     * @brief Constructeur du jeu
     * @param grilleInitiale La grille de départ
     * @param nbIterations Le nombre maximum d'itérations
     * @param torique Active le mode torique si true
     * @param parallele Active la parallélisation si true
     * @throws std::invalid_argument Si la grille initiale est nulle
     */
    JeuDeLaVie(
        std::unique_ptr<Grille> grilleInitiale,
        int nbIterations = 100,
        bool torique = false,
        bool parallele = false
    );
    
    /**
     * @brief Exécute une itération du jeu
     * @return true si le jeu continue, false si terminé (stable ou max atteint)
     */
    bool executerIteration();
    
    /**
     * @brief Revient à l'itération précédente
     * @return true si possible de revenir en arrière, false sinon
     */
    bool revenirEnArriere();
    
    /**
     * @brief Obtient la grille actuelle
     * @return Une référence constante vers la grille
     */
    const Grille& obtenirGrille() const { return *grille; }
    
    /**
     * @brief Obtient le numéro d'itération actuel
     * @return Le numéro d'itération
     */
    int obtenirIteration() const { return iteration; }
    
    /**
     * @brief Vérifie si le mode torique est actif
     * @return true si le mode torique est actif, false sinon
     */
    bool estModeTorique() const { return modeTorique; }
    
    /**
     * @brief Vérifie si le mode parallèle est actif
     * @return true si le mode parallèle est actif, false sinon
     */
    bool estModeParallele() const { return modeParallele; }
    
    /**
     * @brief Obtient le nombre maximum d'itérations
     * @return Le nombre maximum d'itérations
     */
    int obtenirIterationMax() const { return iterationMax; }
    
    /**
     * @brief Définit le nombre maximum d'itérations
     * @param max Le nouveau nombre maximum d'itérations
     */
    void definirIterationMax(int max) { iterationMax = max; }
    
    /**
     * @brief Vérifie si le jeu est terminé
     * @return true si le jeu est terminé, false sinon
     */
    bool estTermine() const;
    
    /**
     * @brief Réinitialise le jeu avec une nouvelle grille
     * @param nouvelleGrille La nouvelle grille de départ
     * @throws std::invalid_argument Si la nouvelle grille est nulle
     */
    void reinitialiser(std::unique_ptr<Grille> nouvelleGrille);
    
    /**
     * @brief Active ou désactive le mode torique
     * @param actif true pour activer, false pour désactiver
     */
    void definirModeTorique(bool actif);
    
    /**
     * @brief Active ou désactive le mode parallèle
     * @param actif true pour activer, false pour désactiver
     */
    void definirModeParallele(bool actif);
    
    /**
     * @brief Définit l'état d'une cellule (utilisé par les interfaces pour le dessin)
     * @param ligne La ligne de la cellule
     * @param colonne La colonne de la cellule
     * @param etat Le nouvel état (ownership transféré)
     */
    void definirEtatCellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etat);
    /**
     * @brief Place un motif sur la grille
     * @param motif Le nom du motif
     * @param ligne La ligne de départ
     * @param colonne La colonne de départ
     * @return true si le motif a été placé, false sinon
     */
    bool placerMotif(const std::string& motif, int ligne, int colonne);
};

#endif // JEU_DE_LA_VIE_HPP
