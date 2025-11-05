#include "RegleJeu.hpp"
#include "Grille.hpp"
#include "Cellule.hpp"
#include "EtatCellule.hpp"
#include <algorithm>

// ==================== RegleStandard ====================

std::unique_ptr<EtatCellule> RegleStandard::calculerNouvelEtat(
    const Grille& grille,
    int ligne,
    int colonne
) const {
    const Cellule& cellule = grille.obtenirCellule(ligne, colonne);
    
    // Les obstacles ne changent jamais d'état
    if (cellule.estObstacle()) {
        return cellule.obtenirEtat().cloner();
    }
    
    int voisinsVivants = compterVoisinsVivants(grille, ligne, colonne);
    bool estVivante = cellule.estVivante();
    
    // Application des règles du jeu de la vie
    if (estVivante) {
        // Une cellule vivante avec 2 ou 3 voisins vivants reste vivante
        if (voisinsVivants == 2 || voisinsVivants == 3) {
            return std::make_unique<CelluleVivante>();
        } else {
            return std::make_unique<CelluleMorte>();
        }
    } else {
        // Une cellule morte avec exactement 3 voisins vivants devient vivante
        if (voisinsVivants == 3) {
            return std::make_unique<CelluleVivante>();
        } else {
            return std::make_unique<CelluleMorte>();
        }
    }
}

int RegleStandard::compterVoisinsVivants(
    const Grille& grille,
    int ligne,
    int colonne
) const {
    int compteur = 0;
    
    // Parcours des 8 voisins
    for (int dl = -1; dl <= 1; ++dl) {
        for (int dc = -1; dc <= 1; ++dc) {
            // Ignorer la cellule elle-même
            if (dl == 0 && dc == 0) continue;
            
            int nouvelleLigne = ligne + dl;
            int nouvelleColonne = colonne + dc;
            
            // Vérifier si le voisin est dans les limites de la grille
            if (grille.estPositionValide(nouvelleLigne, nouvelleColonne)) {
                if (grille.obtenirCellule(nouvelleLigne, nouvelleColonne).estVivante()) {
                    ++compteur;
                }
            }
        }
    }
    
    return compteur;
}

// ==================== RegleTorique ====================

std::unique_ptr<EtatCellule> RegleTorique::calculerNouvelEtat(
    const Grille& grille,
    int ligne,
    int colonne
) const {
    const Cellule& cellule = grille.obtenirCellule(ligne, colonne);
    
    // Les obstacles ne changent jamais d'état
    if (cellule.estObstacle()) {
        return cellule.obtenirEtat().cloner();
    }
    
    int voisinsVivants = compterVoisinsVivants(grille, ligne, colonne);
    bool estVivante = cellule.estVivante();
    
    // Application des règles du jeu de la vie
    if (estVivante) {
        if (voisinsVivants == 2 || voisinsVivants == 3) {
            return std::make_unique<CelluleVivante>();
        } else {
            return std::make_unique<CelluleMorte>();
        }
    } else {
        if (voisinsVivants == 3) {
            return std::make_unique<CelluleVivante>();
        } else {
            return std::make_unique<CelluleMorte>();
        }
    }
}

int RegleTorique::compterVoisinsVivants(
    const Grille& grille,
    int ligne,
    int colonne
) const {
    int compteur = 0;
    int nbLignes = grille.obtenirNbLignes();
    int nbColonnes = grille.obtenirNbColonnes();
    
    // Parcours des 8 voisins avec bouclage torique
    for (int dl = -1; dl <= 1; ++dl) {
        for (int dc = -1; dc <= 1; ++dc) {
            // Ignorer la cellule elle-même
            if (dl == 0 && dc == 0) continue;
            
            // Calcul avec bouclage torique (modulo)
            int nouvelleLigne = (ligne + dl + nbLignes) % nbLignes;
            int nouvelleColonne = (colonne + dc + nbColonnes) % nbColonnes;
            
            if (grille.obtenirCellule(nouvelleLigne, nouvelleColonne).estVivante()) {
                ++compteur;
            }
        }
    }
    
    return compteur;
}
