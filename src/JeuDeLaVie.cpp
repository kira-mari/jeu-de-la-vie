#include "JeuDeLaVie.hpp"
#include "RegleJeu.hpp"
#include <stdexcept>

JeuDeLaVie::JeuDeLaVie(
    std::unique_ptr<Grille> grilleInitiale,
    int nbIterations,
    bool torique,
    bool parallele
) : grille(std::move(grilleInitiale)),
    iteration(0),
    iterationMax(nbIterations),
    modeTorique(torique),
    modeParallele(parallele),
    grandeGrille(false) {
    
    if (!grille) {
        throw std::invalid_argument("La grille initiale ne peut pas être nulle");
    }
    
    // Déterminer si c'est une grande grille (>= 100x100)
    grandeGrille = (grille->obtenirNbLignes() >= Config::SEUIL_GRANDE_GRILLE || 
                    grille->obtenirNbColonnes() >= Config::SEUIL_GRANDE_GRILLE);
    
    // Appliquer les règles appropriées
    if (modeTorique) {
        grille->definirRegle(std::make_shared<RegleTorique>());
    } else {
        grille->definirRegle(std::make_shared<RegleStandard>());
    }
    
    // Sauvegarder l'état initial dans l'historique
    historique.push_back(std::make_unique<Grille>(*grille));
}

bool JeuDeLaVie::executerIteration() {
    if (estTermine()) {
        return false;
    }
    
    // Gestion adaptative de l'historique selon la taille de la grille
    if (grandeGrille) {
        // Grande grille (>= 100x100) : limiter l'historique à 50 pour économiser la RAM
        if (historique.size() >= Config::TAILLE_MAX_HISTORIQUE_GRANDE) {
            historique.erase(historique.begin());
        }
    }
    // Sinon petite grille (< 100x100) : historique illimité
    
    // Sauvegarder l'état actuel avant de mettre à jour
    historique.push_back(std::make_unique<Grille>(*grille));
    
    grille->mettreAJour(modeParallele);
    ++iteration;
    
    return !estTermine();
}

bool JeuDeLaVie::revenirEnArriere() {
    // On ne peut pas revenir en arrière si on est à l'état initial
    if (iteration <= 0 || historique.empty()) {
        return false;
    }
    
    // Supprimer l'état actuel de l'historique
    historique.pop_back();
    
    // Restaurer l'état précédent
    if (!historique.empty()) {
        grille = std::make_unique<Grille>(*historique.back());
        --iteration;
        return true;
    }
    
    return false;
}

bool JeuDeLaVie::estTermine() const {
    if (iterationMax == 0) {
        return grille->estStable();
    }
    return iteration >= iterationMax || grille->estStable();
}

void JeuDeLaVie::reinitialiser(std::unique_ptr<Grille> nouvelleGrille) {
    if (!nouvelleGrille) {
        throw std::invalid_argument("La nouvelle grille ne peut pas être nulle");
    }
    
    grille = std::move(nouvelleGrille);
    iteration = 0;
    
    // Recalculer si c'est une grande grille
    grandeGrille = (grille->obtenirNbLignes() >= Config::SEUIL_GRANDE_GRILLE || 
                    grille->obtenirNbColonnes() >= Config::SEUIL_GRANDE_GRILLE);
    
    // Réappliquer les règles
    if (modeTorique) {
        grille->definirRegle(std::make_shared<RegleTorique>());
    } else {
        grille->definirRegle(std::make_shared<RegleStandard>());
    }
    
    // Réinitialiser l'historique à 0
    historique.clear();
    historique.push_back(std::make_unique<Grille>(*grille));
}

void JeuDeLaVie::definirModeTorique(bool actif) {
    modeTorique = actif;
    
    if (modeTorique) {
        grille->definirRegle(std::make_shared<RegleTorique>());
    } else {
        grille->definirRegle(std::make_shared<RegleStandard>());
    }
}

void JeuDeLaVie::definirModeParallele(bool actif) {
    modeParallele = actif;
}

bool JeuDeLaVie::placerMotif(const std::string& motif, int ligne, int colonne) {
    return grille->placerMotif(motif, ligne, colonne);
}

void JeuDeLaVie::definirEtatCellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etat) {
    if (!grille) return;
    if (!grille->estPositionValide(ligne, colonne)) return;

    // Appliquer l'état demandé
    grille->definirEtatCellule(ligne, colonne, std::move(etat));

    // Après modification manuelle depuis l'interface, réinitialiser l'historique
    historique.clear();
    historique.push_back(std::make_unique<Grille>(*grille));
    iteration = 0;
}
