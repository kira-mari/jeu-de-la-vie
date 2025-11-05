#include "GestionnaireFichier.hpp"
#include "EtatCellule.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>

std::unique_ptr<Grille> GestionnaireFichier::chargerGrille(
    const std::string& cheminFichier,
    std::shared_ptr<RegleJeu> regle
) {
    std::ifstream fichier(cheminFichier);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + cheminFichier);
    }
    
    int nbLignes, nbColonnes;
    fichier >> nbLignes >> nbColonnes;
    
    if (nbLignes <= 0 || nbColonnes <= 0) {
        throw std::runtime_error("Dimensions de grille invalides dans le fichier");
    }
    
    auto grille = std::make_unique<Grille>(nbLignes, nbColonnes, regle);
    
    // Lire l'état de chaque cellule
    for (int i = 0; i < nbLignes; ++i) {
        for (int j = 0; j < nbColonnes; ++j) {
            int valeur;
            if (!(fichier >> valeur)) {
                throw std::runtime_error("Erreur de lecture des données de la grille");
            }
            
            std::unique_ptr<EtatCellule> etat;
            switch (valeur) {
                case 0:
                    etat = std::make_unique<CelluleMorte>();
                    break;
                case 1:
                    etat = std::make_unique<CelluleVivante>();
                    break;
                case 2:
                    etat = std::make_unique<CelluleObstacle>(false);
                    break;
                case 3:
                    etat = std::make_unique<CelluleObstacle>(true);
                    break;
                default:
                    throw std::runtime_error("Valeur de cellule invalide : " + std::to_string(valeur));
            }
            
            grille->definirEtatCellule(i, j, std::move(etat));
        }
    }
    
    fichier.close();
    return grille;
}

void GestionnaireFichier::sauvegarderGrille(const Grille& grille, const std::string& cheminFichier) {
    std::ofstream fichier(cheminFichier);
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible de créer le fichier : " + cheminFichier);
    }
    
    // Écrire les dimensions
    fichier << grille.obtenirNbLignes() << " " << grille.obtenirNbColonnes() << "\n";
    
    // Écrire l'état de chaque cellule
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            const Cellule& cellule = grille.obtenirCellule(i, j);
            fichier << cellule.obtenirEtat().versEntier();
            
            if (j < grille.obtenirNbColonnes() - 1) {
                fichier << " ";
            }
        }
        fichier << "\n";
    }
    
    fichier.close();
}

std::string GestionnaireFichier::creerDossierSortie(const std::string& cheminFichierEntree) {
    namespace fs = std::filesystem;
    
    std::string nomFichier = extraireNomFichier(cheminFichierEntree);
    fs::path cheminFichier(cheminFichierEntree);
    fs::path dossierParent = cheminFichier.parent_path();
    
    std::string nomDossier = nomFichier + "_out";
    fs::path cheminDossierSortie = dossierParent / nomDossier;
    
    // Créer le dossier s'il n'existe pas
    if (!fs::exists(cheminDossierSortie)) {
        fs::create_directories(cheminDossierSortie);
    }
    
    return cheminDossierSortie.string();
}

std::string GestionnaireFichier::extraireNomFichier(const std::string& chemin) {
    namespace fs = std::filesystem;
    fs::path cheminFichier(chemin);
    return cheminFichier.stem().string();
}
