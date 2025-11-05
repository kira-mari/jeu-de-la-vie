#include "ModeConsole.hpp"
#include "GestionnaireFichier.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>

ModeConsole::ModeConsole(
    JeuDeLaVie& jeuRef,
    const std::string& dossierSortie,
    bool afficher
) : jeu(jeuRef),
    dossierSortie(dossierSortie),
    afficherDansConsole(afficher) {
}

void ModeConsole::executer() {
    std::cout << "=== Mode Console - Jeu de la Vie ===" << std::endl;
    std::cout << "Dossier de sortie: " << dossierSortie << std::endl;
    std::cout << std::endl;
    
    // Sauvegarder l'état initial
    sauvegarderIteration(0);
    if (afficherDansConsole) {
        std::cout << "Itération 0 (état initial):" << std::endl;
        afficherGrille();
        std::cout << std::endl;
    }
    
    // Exécuter les itérations
    int iteration = 1;
    while (jeu.executerIteration()) {
        sauvegarderIteration(iteration);
        
        if (afficherDansConsole) {
            std::cout << "Itération " << iteration << ":" << std::endl;
            afficherGrille();
            std::cout << std::endl;
        }
        
        iteration++;
    }
    
    std::cout << "Simulation terminée après " << jeu.obtenirIteration() << " itérations." << std::endl;
    std::cout << "Les résultats ont été sauvegardés dans: " << dossierSortie << std::endl;
}

void ModeConsole::afficherGrille() const {
    const Grille& grille = jeu.obtenirGrille();
    
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            const Cellule& cellule = grille.obtenirCellule(i, j);
            
            if (cellule.estObstacle()) {
                std::cout << (cellule.estVivante() ? "X" : "O");
            } else if (cellule.estVivante()) {
                std::cout << "█";
            } else {
                std::cout << "·";
            }
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void ModeConsole::sauvegarderIteration(int numeroIteration) {
    namespace fs = std::filesystem;
    
    // Créer le nom de fichier avec le numéro d'itération
    std::ostringstream nomFichier;
    nomFichier << "iteration_" << std::setfill('0') << std::setw(4) << numeroIteration << ".txt";
    
    fs::path cheminComplet = fs::path(dossierSortie) / nomFichier.str();
    
    try {
        GestionnaireFichier::sauvegarderGrille(jeu.obtenirGrille(), cheminComplet.string());
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la sauvegarde de l'itération " 
                  << numeroIteration << ": " << e.what() << std::endl;
    }
}
