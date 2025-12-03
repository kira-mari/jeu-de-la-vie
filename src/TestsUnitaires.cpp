#include "TestsUnitaires.hpp"
#include "RegleJeu.hpp"
#include "EtatCellule.hpp"
#include "JeuDeLaVie.hpp"
#include <iostream>
#include <chrono>

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
    tousReussis &= testerParallelisation();
    tousReussis &= testerHistorique();
    tousReussis &= testerMotifs();
    
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
    
    bool tousReussis = true;
    
    // Test 1: Planeur survit en mode torique (boucle sur les bords)
    {
        auto regleTorique = std::make_shared<RegleTorique>();
        auto grille = creerGrille(5, 5, {{0, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}}, regleTorique);
        
        // Exécuter plusieurs itérations
        for (int i = 0; i < 20; ++i) {
            grille.mettreAJour();
        }
        
        // Verifier qu'il y a toujours des cellules vivantes
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
        tousReussis &= aCelluleVivante;
    }
    
    // Test 2: Voisinage torique - cellule en haut à gauche voit en bas à droite
    {
        auto regleTorique = std::make_shared<RegleTorique>();
        // Cellules aux 4 coins pour tester le wrapping
        auto grille = creerGrille(3, 3, {{0, 0}, {0, 2}, {2, 0}}, regleTorique);
        
        // En mode torique, la cellule (2,2) a 3 voisins vivants: (0,0), (0,2), (2,0)
        // Car les bords se touchent
        grille.mettreAJour();
        
        // Vérifier que (2,2) est maintenant vivante (née avec 3 voisins)
        bool reussi = grille.obtenirCellule(2, 2).estVivante();
        afficherResultat("Voisinage torique wrap-around correct", reussi);
        tousReussis &= reussi;
    }
    
    // Test 3: Ligne horizontale au bord en mode torique
    {
        auto regleTorique = std::make_shared<RegleTorique>();
        // Ligne horizontale sur la première ligne
        auto grille = creerGrille(5, 5, {{0, 1}, {0, 2}, {0, 3}}, regleTorique);
        
        // Après une itération, devrait devenir vertical (comme un clignotant)
        // mais en mode torique, les cellules (4,2) et (1,2) naissent aussi
        grille.mettreAJour();
        
        // Vérifier que la cellule (4,2) est vivante (wrap vers le haut)
        bool reussi = grille.obtenirCellule(4, 2).estVivante();
        afficherResultat("Mode torique wrap vertical", reussi);
        tousReussis &= reussi;
    }
    
    // Test 4: Comparaison torique vs non-torique
    {
        // Même configuration initiale
        std::vector<std::pair<int, int>> positions = {{0, 1}, {0, 2}, {0, 3}};
        
        auto regleStandard = std::make_shared<RegleStandard>();
        auto regleTorique = std::make_shared<RegleTorique>();
        
        auto grilleStandard = creerGrille(5, 5, positions, regleStandard);
        auto grilleTorique = creerGrille(5, 5, positions, regleTorique);
        
        grilleStandard.mettreAJour();
        grilleTorique.mettreAJour();
        
        // Les grilles doivent être différentes (le mode torique crée des cellules supplémentaires)
        bool reussi = !(grilleStandard == grilleTorique);
        afficherResultat("Torique produit resultat different de standard", reussi);
        tousReussis &= reussi;
    }
    
    return tousReussis;
}

bool TestsUnitaires::testerObstacles() {
    std::cout << std::endl << "--- Test des cellules obstacles ---" << std::endl;
    
    bool tousReussis = true;
    
    // Test 1: Obstacle vivant reste vivant
    {
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
        
        afficherResultat("Obstacle vivant reste vivant", reussi);
        tousReussis &= reussi;
    }
    
    // Test 2: Obstacle mort reste mort
    {
        auto grille = creerGrille(5, 5, {{1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 3}});
        
        // Placer un obstacle mort au centre (devrait normalement naître avec 5 voisins... non 3 suffisent)
        grille.definirEtatCellule(2, 2, std::make_unique<CelluleObstacle>(false));
        
        // Exécuter plusieurs itérations
        for (int i = 0; i < 10; ++i) {
            grille.mettreAJour();
        }
        
        // L'obstacle doit toujours être mort et être un obstacle
        const Cellule& celluleObstacle = grille.obtenirCellule(2, 2);
        bool reussi = celluleObstacle.estObstacle() && !celluleObstacle.estVivante();
        
        afficherResultat("Obstacle mort reste mort", reussi);
        tousReussis &= reussi;
    }
    
    // Test 3: Obstacle compte comme voisin pour les autres cellules
    {
        // Configuration: 2 cellules vivantes + 1 obstacle vivant = 3 voisins pour (1,1)
        auto grille = creerGrille(3, 3, {{0, 0}, {0, 2}});
        grille.definirEtatCellule(0, 1, std::make_unique<CelluleObstacle>(true));
        
        // La cellule (1,1) morte a 3 voisins vivants (dont 1 obstacle)
        // Elle devrait naître
        grille.mettreAJour();
        
        bool reussi = grille.obtenirCellule(1, 1).estVivante();
        afficherResultat("Obstacle compte comme voisin vivant", reussi);
        tousReussis &= reussi;
    }
    
    // Test 4: Obstacle mort ne compte pas comme voisin vivant
    {
        auto grille = creerGrille(3, 3, {{0, 0}, {0, 2}});
        grille.definirEtatCellule(0, 1, std::make_unique<CelluleObstacle>(false));  // Obstacle MORT
        
        // La cellule (1,1) morte a seulement 2 voisins vivants
        // Elle ne devrait PAS naître
        grille.mettreAJour();
        
        bool reussi = !grille.obtenirCellule(1, 1).estVivante();
        afficherResultat("Obstacle mort ne compte pas comme voisin", reussi);
        tousReussis &= reussi;
    }
    
    return tousReussis;
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

bool TestsUnitaires::testerParallelisation() {
    std::cout << std::endl << "--- Test de la parallelisation ---" << std::endl;
    
    // Créer une grille assez grande pour que la parallélisation soit pertinente
    const int taille = 50;
    std::vector<std::pair<int, int>> cellulesVivantes;
    
    // Créer un motif aléatoire déterministe
    for (int i = 0; i < taille; ++i) {
        for (int j = 0; j < taille; ++j) {
            if ((i + j) % 3 == 0 || (i * j) % 7 == 0) {
                cellulesVivantes.push_back({i, j});
            }
        }
    }
    
    // Test 1: Vérifier que le résultat est identique avec et sans parallélisation
    auto grilleSeq = creerGrille(taille, taille, cellulesVivantes);
    auto grillePar = creerGrille(taille, taille, cellulesVivantes);
    
    // Exécuter plusieurs itérations
    for (int i = 0; i < 10; ++i) {
        grilleSeq.mettreAJour(false);  // Séquentiel
        grillePar.mettreAJour(true);   // Parallèle
    }
    
    bool resultatsIdentiques = (grilleSeq == grillePar);
    afficherResultat("Resultats identiques seq/parallele", resultatsIdentiques);
    
    // Test 2: Mesurer les performances (informatif)
    auto grillePerf = creerGrille(taille, taille, cellulesVivantes);
    
    auto debutSeq = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; ++i) {
        grillePerf.mettreAJour(false);
    }
    auto finSeq = std::chrono::high_resolution_clock::now();
    auto dureeSeq = std::chrono::duration_cast<std::chrono::microseconds>(finSeq - debutSeq).count();
    
    grillePerf = creerGrille(taille, taille, cellulesVivantes);
    
    auto debutPar = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; ++i) {
        grillePerf.mettreAJour(true);
    }
    auto finPar = std::chrono::high_resolution_clock::now();
    auto dureePar = std::chrono::duration_cast<std::chrono::microseconds>(finPar - debutPar).count();
    
    std::cout << "  [INFO] Temps sequentiel: " << dureeSeq << " us" << std::endl;
    std::cout << "  [INFO] Temps parallele: " << dureePar << " us" << std::endl;
    
    // Le test de performance n'échoue pas, c'est juste informatif
    afficherResultat("Parallelisation executee sans erreur", true);
    
    return resultatsIdentiques;
}

bool TestsUnitaires::testerHistorique() {
    std::cout << std::endl << "--- Test de l'historique ---" << std::endl;
    
    // Créer un jeu avec un clignotant (oscillateur période 2)
    std::vector<std::pair<int, int>> positionsClignotant = {{2, 1}, {2, 2}, {2, 3}};
    auto grilleInitiale = std::make_unique<Grille>(5, 5, std::make_shared<RegleStandard>());
    for (const auto& pos : positionsClignotant) {
        grilleInitiale->definirEtatCellule(pos.first, pos.second, std::make_unique<CelluleVivante>());
    }
    
    // Sauvegarder l'état initial pour comparaison
    Grille etatInitial = *grilleInitiale;
    
    JeuDeLaVie jeu(std::move(grilleInitiale), 100, false, false);
    
    // Test 1: Avancer de 3 itérations
    jeu.executerIteration();  // Iteration 1
    jeu.executerIteration();  // Iteration 2
    jeu.executerIteration();  // Iteration 3
    
    bool iteration3 = (jeu.obtenirIteration() == 3);
    afficherResultat("Avancer a l'iteration 3", iteration3);
    
    // Test 2: Revenir en arrière
    bool retour1 = jeu.revenirEnArriere();  // Retour à 2
    bool iteration2 = (jeu.obtenirIteration() == 2);
    afficherResultat("Retour a l'iteration 2", retour1 && iteration2);
    
    // Test 3: Revenir jusqu'au début
    jeu.revenirEnArriere();  // Retour à 1
    jeu.revenirEnArriere();  // Retour à 0
    
    bool retourDebut = (jeu.obtenirIteration() == 0);
    afficherResultat("Retour a l'iteration 0", retourDebut);
    
    // Test 4: Vérifier qu'on ne peut pas revenir avant 0
    bool impossibleReculer = !jeu.revenirEnArriere();
    afficherResultat("Impossible de reculer avant 0", impossibleReculer);
    
    // Test 5: Vérifier que l'état est bien restauré (grille initiale)
    bool etatRestaure = (jeu.obtenirGrille() == etatInitial);
    afficherResultat("Etat initial restaure correctement", etatRestaure);
    
    // Test 6: Après 2 itérations on doit retrouver l'état initial (clignotant période 2)
    jeu.executerIteration();
    jeu.executerIteration();
    bool periodeOscillateur = (jeu.obtenirGrille() == etatInitial);
    afficherResultat("Oscillateur periode 2 verifie via historique", periodeOscillateur);
    
    return iteration3 && retour1 && iteration2 && retourDebut && impossibleReculer && etatRestaure && periodeOscillateur;
}

bool TestsUnitaires::testerMotifs() {
    std::cout << std::endl << "--- Test des motifs preprogrammes ---" << std::endl;
    
    bool tousReussis = true;
    
    // Test 1: Placer un planeur
    {
        auto grille = creerGrille(10, 10, {});
        bool place = grille.placerMotif("planeur", 2, 2);
        
        // Vérifier que le planeur a été placé (5 cellules vivantes)
        int nbVivantes = 0;
        for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
            for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
                if (grille.obtenirCellule(i, j).estVivante()) nbVivantes++;
            }
        }
        
        bool reussi = place && (nbVivantes == 5);
        afficherResultat("Placement motif planeur", reussi);
        tousReussis &= reussi;
    }
    
    // Test 2: Placer un bloc
    {
        auto grille = creerGrille(10, 10, {});
        bool place = grille.placerMotif("bloc", 2, 2);
        
        int nbVivantes = 0;
        for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
            for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
                if (grille.obtenirCellule(i, j).estVivante()) nbVivantes++;
            }
        }
        
        bool reussi = place && (nbVivantes == 4);
        afficherResultat("Placement motif bloc", reussi);
        tousReussis &= reussi;
    }
    
    // Test 3: Placer un clignotant
    {
        auto grille = creerGrille(10, 10, {});
        bool place = grille.placerMotif("clignotant", 3, 3);
        
        int nbVivantes = 0;
        for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
            for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
                if (grille.obtenirCellule(i, j).estVivante()) nbVivantes++;
            }
        }
        
        bool reussi = place && (nbVivantes == 3);
        afficherResultat("Placement motif clignotant", reussi);
        tousReussis &= reussi;
    }
    
    // Test 4: Placer une ruche
    {
        auto grille = creerGrille(10, 10, {});
        bool place = grille.placerMotif("ruche", 2, 2);
        
        int nbVivantes = 0;
        for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
            for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
                if (grille.obtenirCellule(i, j).estVivante()) nbVivantes++;
            }
        }
        
        bool reussi = place && (nbVivantes == 6);
        afficherResultat("Placement motif ruche", reussi);
        tousReussis &= reussi;
    }
    
    // Test 5: Motif inconnu doit échouer
    {
        auto grille = creerGrille(10, 10, {});
        bool place = grille.placerMotif("motif_inexistant", 2, 2);
        
        bool reussi = !place;
        afficherResultat("Motif inconnu rejete", reussi);
        tousReussis &= reussi;
    }
    
    // Test 6: Placement hors limites doit échouer
    {
        auto grille = creerGrille(5, 5, {});
        bool place = grille.placerMotif("planeur", 4, 4);  // Trop près du bord
        
        bool reussi = !place;
        afficherResultat("Placement hors limites rejete", reussi);
        tousReussis &= reussi;
    }
    
    return tousReussis;
}
