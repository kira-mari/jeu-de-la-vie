#include "Grille.hpp"
#include <stdexcept>
#include <thread>
#include <algorithm>

Grille::Grille(int lignes, int colonnes, std::shared_ptr<RegleJeu> regleJeu)
    : nbLignes(lignes), nbColonnes(colonnes), grilleMemoire(nullptr) {
    
    if (lignes <= 0 || colonnes <= 0) {
        throw std::invalid_argument("Les dimensions de la grille doivent être positives");
    }
    
    // Initialiser la grille avec des cellules mortes
    cellules.reserve(nbLignes);
    for (int i = 0; i < nbLignes; ++i) {
        std::vector<Cellule> ligne;
        ligne.reserve(nbColonnes);
        for (int j = 0; j < nbColonnes; ++j) {
            ligne.emplace_back(i, j);
        }
        cellules.push_back(std::move(ligne));
    }
    
    // Définir les règles (standard par défaut)
    if (regleJeu) {
        regle = regleJeu;
    } else {
        regle = std::make_shared<RegleStandard>();
    }
}

Grille::Grille(const Grille& autre)
    : nbLignes(autre.nbLignes), nbColonnes(autre.nbColonnes), 
      regle(autre.regle), grilleMemoire(nullptr), premiereIteration(autre.premiereIteration) {
    
    // Copier toutes les cellules
    cellules.reserve(nbLignes);
    for (int i = 0; i < nbLignes; ++i) {
        std::vector<Cellule> ligne;
        ligne.reserve(nbColonnes);
        for (int j = 0; j < nbColonnes; ++j) {
            ligne.push_back(autre.cellules[i][j]);
        }
        cellules.push_back(std::move(ligne));
    }
}

Grille& Grille::operator=(const Grille& autre) {
    if (this != &autre) {
        nbLignes = autre.nbLignes;
        nbColonnes = autre.nbColonnes;
        regle = autre.regle;
        premiereIteration = autre.premiereIteration;
        grilleMemoire = nullptr;
        
        // Copier toutes les cellules
        cellules.clear();
        cellules.reserve(nbLignes);
        for (int i = 0; i < nbLignes; ++i) {
            std::vector<Cellule> ligne;
            ligne.reserve(nbColonnes);
            for (int j = 0; j < nbColonnes; ++j) {
                ligne.push_back(autre.cellules[i][j]);
            }
            cellules.push_back(std::move(ligne));
        }
    }
    return *this;
}

Grille::~Grille() {
    // Destructeur par défaut (plus besoin de delete mutexPtr)
}

Cellule& Grille::obtenirCellule(int ligne, int colonne) {
    if (!estPositionValide(ligne, colonne)) {
        throw std::out_of_range("Position hors des limites de la grille");
    }
    return cellules[ligne][colonne];
}

const Cellule& Grille::obtenirCellule(int ligne, int colonne) const {
    if (!estPositionValide(ligne, colonne)) {
        throw std::out_of_range("Position hors des limites de la grille");
    }
    return cellules[ligne][colonne];
}

void Grille::definirEtatCellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etat) {
    if (!estPositionValide(ligne, colonne)) {
        throw std::out_of_range("Position hors des limites de la grille");
    }
    cellules[ligne][colonne].definirEtat(std::move(etat));
}

bool Grille::estPositionValide(int ligne, int colonne) const {
    return ligne >= 0 && ligne < nbLignes && colonne >= 0 && colonne < nbColonnes;
}

void Grille::mettreAJour(bool parallele) {
    // Créer une copie de la grille pour les nouveaux états
    Grille nouvelleGrille(nbLignes, nbColonnes, regle);
    
    // Décider si la parallélisation vaut la peine
    bool utiliserParallele = parallele && (nbLignes >= Config::TAILLE_MIN_PARALLELE);
    
    if (utiliserParallele) {
        // Version parallélisée optimisée
        unsigned int nbThreads = std::thread::hardware_concurrency();
        if (nbThreads == 0) nbThreads = 4;
        // Limiter le nombre de threads pour éviter l'overhead
        nbThreads = std::min(nbThreads, static_cast<unsigned int>(nbLignes / 50));
        nbThreads = std::max(2u, nbThreads); // Au minimum 2 threads
        
        std::vector<std::thread> threads;
        threads.reserve(nbThreads);
        int lignesParThread = nbLignes / nbThreads;
        
        for (unsigned int t = 0; t < nbThreads; ++t) {
            int ligneDebut = t * lignesParThread;
            int ligneFin = (t == nbThreads - 1) ? nbLignes : (ligneDebut + lignesParThread);
            
            threads.emplace_back([this, ligneDebut, ligneFin, &nouvelleGrille]() {
                this->mettreAJourLignes(ligneDebut, ligneFin, nouvelleGrille);
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
    } else {
        // Version séquentielle optimisée
        for (int i = 0; i < nbLignes; ++i) {
            for (int j = 0; j < nbColonnes; ++j) {
                auto nouvelEtat = regle->calculerNouvelEtat(*this, i, j);
                nouvelleGrille.definirEtatCellule(i, j, std::move(nouvelEtat));
            }
        }
    }
    
    // Sauvegarder l'état actuel pour vérifier la stabilité (après le calcul pour réutiliser les données)
    if (!premiereIteration && grilleMemoire) {
        // Swap au lieu de copier - beaucoup plus rapide
        std::swap(grilleMemoire->cellules, cellules);
    } else if (!grilleMemoire) {
        grilleMemoire = std::make_unique<Grille>(nbLignes, nbColonnes, regle);
        grilleMemoire->cellules = cellules;
    }
    
    // Remplacer l'ancienne grille par la nouvelle (move, pas de copie)
    cellules = std::move(nouvelleGrille.cellules);
    
    if (premiereIteration) {
        premiereIteration = false;
    }
}

void Grille::mettreAJourLignes(int ligneDebut, int ligneFin, Grille& nouvelleGrille) {
    // Version optimisée : minimiser les appels de fonction et accès indirects
    for (int i = ligneDebut; i < ligneFin; ++i) {
        for (int j = 0; j < nbColonnes; ++j) {
            // Calcul inline du nouvel état (évite l'appel virtuel répété)
            auto nouvelEtat = regle->calculerNouvelEtat(*this, i, j);
            
            // Écriture directe - chaque thread écrit dans SES lignes uniquement
            nouvelleGrille.cellules[i][j].definirEtat(std::move(nouvelEtat));
        }
    }
}

bool Grille::estStable() const {
    if (premiereIteration || !grilleMemoire) {
        return false;
    }
    return *this == *grilleMemoire;
}

bool Grille::operator==(const Grille& autre) const {
    if (nbLignes != autre.nbLignes || nbColonnes != autre.nbColonnes) {
        return false;
    }
    
    for (int i = 0; i < nbLignes; ++i) {
        for (int j = 0; j < nbColonnes; ++j) {
            if (cellules[i][j].estVivante() != autre.cellules[i][j].estVivante()) {
                return false;
            }
        }
    }
    
    return true;
}

void Grille::definirRegle(std::shared_ptr<RegleJeu> nouvelleRegle) {
    if (!nouvelleRegle) {
        throw std::invalid_argument("La règle ne peut pas être nulle");
    }
    regle = nouvelleRegle;
}

bool Grille::placerMotif(const std::string& motif, int ligneDepart, int colonneDepart) {
    // Motifs préprogrammés (extension bonus)
    if (motif == "planeur") {
        // Motif planeur (glider)
        std::vector<std::pair<int, int>> positions = {
            {0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}
        };
        
        for (const auto& pos : positions) {
            int ligne = ligneDepart + pos.first;
            int colonne = colonneDepart + pos.second;
            if (estPositionValide(ligne, colonne)) {
                definirEtatCellule(ligne, colonne, std::make_unique<CelluleVivante>());
            } else {
                return false;
            }
        }
        return true;
    }
    else if (motif == "clignotant") {
        // Oscillateur simple (blinker)
        for (int i = 0; i < 3; ++i) {
            int ligne = ligneDepart;
            int colonne = colonneDepart + i;
            if (estPositionValide(ligne, colonne)) {
                definirEtatCellule(ligne, colonne, std::make_unique<CelluleVivante>());
            } else {
                return false;
            }
        }
        return true;
    }
    else if (motif == "bloc") {
        // Bloc stable 2x2
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                int ligne = ligneDepart + i;
                int colonne = colonneDepart + j;
                if (estPositionValide(ligne, colonne)) {
                    definirEtatCellule(ligne, colonne, std::make_unique<CelluleVivante>());
                } else {
                    return false;
                }
            }
        }
        return true;
    }
    else if (motif == "ruche") {
        // Ruche (beehive)
        std::vector<std::pair<int, int>> positions = {
            {0, 1}, {0, 2}, {1, 0}, {1, 3}, {2, 1}, {2, 2}
        };
        
        for (const auto& pos : positions) {
            int ligne = ligneDepart + pos.first;
            int colonne = colonneDepart + pos.second;
            if (estPositionValide(ligne, colonne)) {
                definirEtatCellule(ligne, colonne, std::make_unique<CelluleVivante>());
            } else {
                return false;
            }
        }
        return true;
    }
    
    return false;
}
