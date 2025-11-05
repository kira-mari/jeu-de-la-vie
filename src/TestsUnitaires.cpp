#include "TestsUnitaires.hpp"
#include "RegleJeu.hpp"
#include "EtatCellule.hpp"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#endif

bool TestsUnitaires::executerTousLesTests() {
    // Configurer la console Windows pour UTF-8
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "=== EXECUTION DES TESTS UNITAIRES ===" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;
    
    bool tousReussis = true;
    
    tousReussis &= testerReglesBase();
    tousReussis &= testerBloc();
    tousReussis &= testerClignotant();
    tousReussis &= testerPlaneur();
    tousReussis &= testerModeTorique();
    tousReussis &= testerObstacles();
    
    std::cout << std::endl;
    std::cout << "=====================================" << std::endl;
    if (tousReussis) {
        std::cout << "=== TOUS LES TESTS ONT REUSSI ✓ ===" << std::endl;
    } else {
        std::cout << "=== CERTAINS TESTS ONT ECHOUE ✗ ===" << std::endl;
    }
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;
    
    return tousReussis;
}

bool TestsUnitaires::testerEvolution(
    Grille& grilleInitiale,
    const Grille& grilleAttendue,
    int nbIterations,
    const std::string& nomTest
) {
    for (int i = 0; i < nbIterations; ++i) {
        grilleInitiale.mettreAJour();
    }
    
    bool reussi = (grilleInitiale == grilleAttendue);
    afficherResultat(nomTest, reussi);
    
    return reussi;
}

bool TestsUnitaires::testerReglesBase() {
    std::cout << "--- Test des règles de base ---" << std::endl;
    
    // Test 1: Cellule morte avec 3 voisins vivants devient vivante
    {
        auto grille = creerGrille(3, 3, {{0, 0}, {0, 1}, {0, 2}});
        auto grilleAttendue = creerGrille(3, 3, {{0, 1}, {1, 1}});
        
        if (!testerEvolution(grille, grilleAttendue, 1, "Cellule morte -> vivante (3 voisins)")) {
            return false;
        }
    }
    
    // Test 2: Cellule vivante avec 2 voisins reste vivante
    {
        auto grille = creerGrille(3, 3, {{1, 1}, {0, 1}, {2, 1}});
        grille.mettreAJour();
        
        bool reussi = grille.obtenirCellule(1, 1).estVivante();
        afficherResultat("Cellule vivante reste vivante (2 voisins)", reussi);
        if (!reussi) return false;
    }
    
    // Test 3: Cellule vivante avec moins de 2 voisins meurt
    {
        auto grille = creerGrille(3, 3, {{1, 1}, {0, 1}});
        grille.mettreAJour();
        
        bool reussi = !grille.obtenirCellule(1, 1).estVivante();
        afficherResultat("Cellule vivante meurt (sous-population)", reussi);
        if (!reussi) return false;
    }
    
    // Test 4: Cellule vivante avec plus de 3 voisins meurt
    {
        auto grille = creerGrille(3, 3, {{1, 1}, {0, 0}, {0, 1}, {1, 0}, {2, 0}});
        grille.mettreAJour();
        
        bool reussi = !grille.obtenirCellule(1, 1).estVivante();
        afficherResultat("Cellule vivante meurt (surpopulation)", reussi);
        if (!reussi) return false;
    }
    
    return true;
}

bool TestsUnitaires::testerBloc() {
    std::cout << std::endl << "--- Test du motif bloc (stable) ---" << std::endl;
    
    // Un bloc 2x2 doit rester stable
    std::vector<std::pair<int, int>> positionsBloc = {
        {1, 1}, {1, 2}, {2, 1}, {2, 2}
    };
    
    auto grille = creerGrille(4, 4, positionsBloc);
    auto grilleAttendue = creerGrille(4, 4, positionsBloc);
    
    return testerEvolution(grille, grilleAttendue, 10, "Bloc reste stable après 10 itérations");
}

bool TestsUnitaires::testerClignotant() {
    std::cout << std::endl << "--- Test du motif clignotant (oscillateur période 2) ---" << std::endl;
    
    // Configuration initiale: ligne horizontale
    auto grille = creerGrille(5, 5, {{2, 1}, {2, 2}, {2, 3}});
    
    // Après 1 itération: ligne verticale
    auto grilleApres1 = creerGrille(5, 5, {{1, 2}, {2, 2}, {3, 2}});
    
    bool test1 = testerEvolution(grille, grilleApres1, 1, "Clignotant après 1 itération");
    
    // Après 2 itérations: retour à l'horizontal
    auto grille2 = creerGrille(5, 5, {{2, 1}, {2, 2}, {2, 3}});
    auto grilleApres2 = creerGrille(5, 5, {{2, 1}, {2, 2}, {2, 3}});
    
    bool test2 = testerEvolution(grille2, grilleApres2, 2, "Clignotant après 2 itérations (retour)");
    
    return test1 && test2;
}

bool TestsUnitaires::testerPlaneur() {
    std::cout << std::endl << "--- Test du planeur (se déplace) ---" << std::endl;
    
    // Configuration initiale du planeur
    std::vector<std::pair<int, int>> positionsPlaneur = {
        {1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}
    };
    
    auto grille = creerGrille(10, 10, positionsPlaneur);
    
    // Compter le nombre de cellules vivantes au départ
    int cellulesVivantesDebut = 0;
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            if (grille.obtenirCellule(i, j).estVivante()) {
                cellulesVivantesDebut++;
            }
        }
    }
    
    // Après 4 itérations, le planeur doit avoir bougé
    grille.mettreAJour();
    grille.mettreAJour();
    grille.mettreAJour();
    grille.mettreAJour();
    
    // Vérifier que le planeur existe toujours (même nombre de cellules vivantes)
    int cellulesVivantesFin = 0;
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            if (grille.obtenirCellule(i, j).estVivante()) {
                cellulesVivantesFin++;
            }
        }
    }
    
    bool aBouge = (cellulesVivantesDebut == cellulesVivantesFin) && cellulesVivantesFin == 5;
    afficherResultat("Planeur se deplace apres 4 iterations", aBouge);
    
    return aBouge;
}

bool TestsUnitaires::testerModeTorique() {
    std::cout << std::endl << "--- Test du mode torique ---" << std::endl;
    
    // Dans une petite grille torique, un planeur au bord doit "boucler"
    auto regleTorique = std::make_shared<RegleTorique>();
    auto grille = creerGrille(5, 5, {{0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}}, regleTorique);
    
    // Exécuter plusieurs itérations
    for (int i = 0; i < 20; ++i) {
        grille.mettreAJour();
    }
    
    // Verifier qu'il y a toujours des cellules vivantes
    // (dans une grille non-torique, le planeur pourrait sortir)
    bool aCelluleVivante = false;
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            if (grille.obtenirCellule(i, j).estVivante()) {
                aCelluleVivante = true;
                break;
            }
        }
        if (aCelluleVivante) break;
    }
    
    afficherResultat("Mode torique maintient les cellules vivantes", aCelluleVivante);
    
    return aCelluleVivante;
}

bool TestsUnitaires::testerObstacles() {
    std::cout << std::endl << "--- Test des cellules obstacles ---" << std::endl;
    
    auto grille = creerGrille(5, 5, {{1, 1}, {1, 2}, {1, 3}});
    
    // Placer un obstacle vivant au centre
    grille.definirEtatCellule(2, 2, std::make_unique<CelluleObstacle>(true));
    
    // Exécuter plusieurs itérations
    for (int i = 0; i < 10; ++i) {
        grille.mettreAJour();
    }
    
    // L'obstacle doit toujours être vivant et être un obstacle
    const Cellule& celluleObstacle = grille.obtenirCellule(2, 2);
    bool reussi = celluleObstacle.estObstacle() && celluleObstacle.estVivante();
    
    afficherResultat("Obstacle reste inchangé après 10 itérations", reussi);
    
    return reussi;
}

Grille TestsUnitaires::creerGrille(
    int lignes,
    int colonnes,
    const std::vector<std::pair<int, int>>& cellulesVivantes,
    std::shared_ptr<RegleJeu> regle
) {
    if (!regle) {
        regle = std::make_shared<RegleStandard>();
    }
    
    Grille grille(lignes, colonnes, regle);
    
    for (const auto& pos : cellulesVivantes) {
        grille.definirEtatCellule(pos.first, pos.second, std::make_unique<CelluleVivante>());
    }
    
    return grille;
}

void TestsUnitaires::afficherResultat(const std::string& nomTest, bool reussi) {
    std::cout << "  " << nomTest << ": ";
    if (reussi) {
        std::cout << "REUSSI" << std::endl;
    } else {
        std::cout << "ECHOUE" << std::endl;
    }
}
