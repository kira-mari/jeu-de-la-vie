#include "Cellule.hpp"

Cellule::Cellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etatInitial)
    : ligne(ligne), colonne(colonne) {
    if (etatInitial) {
        etat = std::move(etatInitial);
    } else {
        etat = std::make_unique<CelluleMorte>();
    }
}

Cellule::Cellule(const Cellule& autre)
    : ligne(autre.ligne), colonne(autre.colonne) {
    etat = autre.etat->cloner();
}

Cellule& Cellule::operator=(const Cellule& autre) {
    if (this != &autre) {
        ligne = autre.ligne;
        colonne = autre.colonne;
        etat = autre.etat->cloner();
    }
    return *this;
}

void Cellule::definirEtat(std::unique_ptr<EtatCellule> nouvelEtat) {
    etat = std::move(nouvelEtat);
}
