#ifndef REGLE_JEU_HPP
#define REGLE_JEU_HPP

#include <vector>
#include <memory>

class Cellule;
class Grille;

/**
 * @brief Classe abstraite représentant les règles du jeu
 * 
 * Permet de définir différentes règles de calcul (standard, torique, etc.).
 */
class RegleJeu {
public:
    virtual ~RegleJeu() = default;
    
    /**
     * @brief Calcule le nouvel état d'une cellule selon les règles du jeu
     * @param grille La grille de jeu
     * @param ligne La ligne de la cellule
     * @param colonne La colonne de la cellule
     * @return Un pointeur unique vers le nouvel état
     */
    virtual std::unique_ptr<class EtatCellule> calculerNouvelEtat(
        const Grille& grille, 
        int ligne, 
        int colonne
    ) const = 0;
    
protected:
    /**
     * @brief Compte le nombre de voisins vivants
     * @param grille La grille de jeu
     * @param ligne La ligne de la cellule
     * @param colonne La colonne de la cellule
     * @return Le nombre de voisins vivants
     */
    virtual int compterVoisinsVivants(
        const Grille& grille,
        int ligne,
        int colonne
    ) const = 0;
};

/**
 * @brief Implémentation des règles standard du jeu de la vie
 * 
 * Les bordures de la grille sont considérées comme des limites fixes.
 */
class RegleStandard : public RegleJeu {
public:
    std::unique_ptr<EtatCellule> calculerNouvelEtat(
        const Grille& grille,
        int ligne,
        int colonne
    ) const override;
    
protected:
    int compterVoisinsVivants(
        const Grille& grille,
        int ligne,
        int colonne
    ) const override;
};

/**
 * @brief Implémentation des règles pour une grille torique (extension bonus)
 * 
 * Les cellules aux extrémités de la grille sont adjacentes.
 * La grille "boucle" sur elle-même horizontalement et verticalement.
 */
class RegleTorique : public RegleJeu {
public:
    std::unique_ptr<EtatCellule> calculerNouvelEtat(
        const Grille& grille,
        int ligne,
        int colonne
    ) const override;
    
protected:
    int compterVoisinsVivants(
        const Grille& grille,
        int ligne,
        int colonne
    ) const override;
};

#endif // REGLE_JEU_HPP
