#ifndef CELLULE_HPP
#define CELLULE_HPP

#include "EtatCellule.hpp"
#include <memory>

/**
 * @brief Classe représentant une cellule dans le jeu de la vie
 * 
 * Une cellule possède un état (vivante, morte, obstacle) et des coordonnées.
 * Elle utilise la composition avec EtatCellule pour son comportement.
 */
class Cellule {
private:
    std::unique_ptr<EtatCellule> etat;
    int ligne;
    int colonne;
    
public:
    /**
     * @brief Constructeur de cellule
     * @param ligne La ligne de la cellule dans la grille
     * @param colonne La colonne de la cellule dans la grille
     * @param etatInitial L'état initial de la cellule (mort par défaut)
     */
    Cellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etatInitial = nullptr);
    
    // Constructeur de copie
    Cellule(const Cellule& autre);
    
    // Opérateur d'affectation
    Cellule& operator=(const Cellule& autre);
    
    // Destructeur
    ~Cellule() = default;
    
    /**
     * @brief Définit le nouvel état de la cellule
     * @param nouvelEtat Le nouvel état à assigner
     */
    void definirEtat(std::unique_ptr<EtatCellule> nouvelEtat);
    
    /**
     * @brief Obtient l'état actuel de la cellule
     * @return Une référence constante vers l'état
     */
    const EtatCellule& obtenirEtat() const { return *etat; }
    
    /**
     * @brief Vérifie si la cellule est vivante
     * @return true si la cellule est vivante, false sinon
     */
    bool estVivante() const { return etat->estVivante(); }
    
    /**
     * @brief Vérifie si la cellule est un obstacle
     * @return true si la cellule est un obstacle, false sinon
     */
    bool estObstacle() const { return etat->estObstacle(); }
    
    /**
     * @brief Obtient la ligne de la cellule
     * @return Le numéro de ligne
     */
    int obtenirLigne() const { return ligne; }
    
    /**
     * @brief Obtient la colonne de la cellule
     * @return Le numéro de colonne
     */
    int obtenirColonne() const { return colonne; }
};

#endif // CELLULE_HPP
