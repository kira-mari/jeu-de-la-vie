#include "JeuDeLaVie.hpp"
#include "GestionnaireFichier.hpp"
#include "ModeConsole.hpp"
#include "InterfaceSFML.hpp"

// Inclusion conditionnelle de Qt (peut ne pas être disponible sur tous les systèmes)
#ifdef QT_AVAILABLE
    #include "InterfaceQt.hpp"
    #include <QApplication>
#endif

#include "RegleJeu.hpp"
#include <iostream>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#endif

/**
 * @brief Affiche l'aide du programme
 */
void afficherAide() {
    std::cout << "=== Jeu de la Vie - Aide ===" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  jeu_de_la_vie <fichier_entree> [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -m <mode>       Mode d'exécution: 'console', 'sfml' ou 'qt' (défaut: qt)" << std::endl;
    std::cout << "  -n <nombre>     Nombre maximum d'itérations (défaut: 100)" << std::endl;
    std::cout << "  -t              Active le mode torique (grille bouclée)" << std::endl;
    std::cout << "  -p              Active la parallélisation" << std::endl;
    std::cout << "  -h, --help      Affiche cette aide" << std::endl;
    std::cout << std::endl;
    std::cout << "Modes graphiques:" << std::endl;
    std::cout << "  sfml            Interface SFML (légère, jeu vidéo)" << std::endl;
    std::cout << "  qt              Interface Qt (moderne, professionnelle)" << std::endl;
    std::cout << std::endl;
    std::cout << "Contrôles en mode graphique:" << std::endl;
    std::cout << "  ESPACE          Pause/Reprise de la simulation" << std::endl;
    std::cout << "  FLECHE DROITE   Avancer d'une itération" << std::endl;
    std::cout << "  FLECHE GAUCHE   Revenir à l'itération précédente" << std::endl;
    std::cout << "  FLECHE HAUT     Accélérer la simulation" << std::endl;
    std::cout << "  FLECHE BAS      Ralentir la simulation" << std::endl;
    std::cout << "  G               Placer un planeur" << std::endl;
    std::cout << "  B               Placer un bloc" << std::endl;
    std::cout << "  C               Placer un clignotant" << std::endl;
    std::cout << "  H               Placer une ruche" << std::endl;
    std::cout << "  T               Activer/Désactiver le mode torique" << std::endl;
    std::cout << "  P               Activer/Désactiver la parallélisation" << std::endl;
    std::cout << std::endl;
    std::cout << "Format du fichier d'entrée:" << std::endl;
    std::cout << "  Première ligne: <nb_lignes> <nb_colonnes>" << std::endl;
    std::cout << "  Lignes suivantes: matrice de valeurs" << std::endl;
    std::cout << "    0 = cellule morte" << std::endl;
    std::cout << "    1 = cellule vivante" << std::endl;
    std::cout << "    2 = obstacle mort (extension bonus)" << std::endl;
    std::cout << "    3 = obstacle vivant (extension bonus)" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Point d'entrée du programme
 */
int main(int argc, char* argv[]) {
    // Configurer la console Windows pour UTF-8
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // Valeurs par défaut
    std::string fichierEntree;
    #ifdef QT_AVAILABLE
        std::string mode = "qt";  // Qt par défaut si disponible
    #else
        std::string mode = "sfml";  // SFML par défaut si Qt non disponible
    #endif
    int nbIterations = 100;
    bool modeTorique = false;
    bool modeParallele = false;
    
    // Vérifier d'abord si l'aide est demandée
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            afficherAide();
            return 0;
        }
    }
    
    // Analyse des arguments
    if (argc < 2) {
        std::cerr << "Erreur: Fichier d'entrée requis" << std::endl;
        afficherAide();
        return 1;
    }
    
    fichierEntree = argv[1];
    
    // Traiter les options
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-m" && i + 1 < argc) {
            mode = argv[++i];
        }
        else if (arg == "-n" && i + 1 < argc) {
            nbIterations = std::stoi(argv[++i]);
        }
        else if (arg == "-t") {
            modeTorique = true;
        }
        else if (arg == "-p") {
            modeParallele = true;
        }
    }
    
    try {
        // Créer la règle appropriée
        std::shared_ptr<RegleJeu> regle;
        if (modeTorique) {
            regle = std::make_shared<RegleTorique>();
        } else {
            regle = std::make_shared<RegleStandard>();
        }
        
        // Charger la grille depuis le fichier
        std::cout << "Chargement de la grille depuis: " << fichierEntree << std::endl;
        auto grille = GestionnaireFichier::chargerGrille(fichierEntree, regle);
        std::cout << "Grille chargée: " << grille->obtenirNbLignes() 
                  << "x" << grille->obtenirNbColonnes() << std::endl;
        
        // Créer le jeu
        JeuDeLaVie jeu(std::move(grille), nbIterations, modeTorique, modeParallele);
        
        std::cout << "Mode: " << mode << std::endl;
        std::cout << "Itérations max: " << nbIterations << std::endl;
        std::cout << "Mode torique: " << (modeTorique ? "oui" : "non") << std::endl;
        std::cout << "Parallélisation: " << (modeParallele ? "oui" : "non") << std::endl;
        std::cout << std::endl;
        
        // Exécuter selon le mode choisi
        if (mode == "console") {
            std::string dossierSortie = GestionnaireFichier::creerDossierSortie(fichierEntree);
            ModeConsole modeConsole(jeu, dossierSortie, true);
            modeConsole.executer();
        }
        else if (mode == "sfml") {
            try {
                std::cout << "Lancement de l'interface SFML..." << std::endl;
                InterfaceSFML interface(jeu, 1000, 800, "Jeu de la Vie - SFML");
                interface.definirDelaiIteration(0.2f);
                interface.executer();
            } catch (const std::exception& e) {
                std::cerr << "Erreur SFML: " << e.what() << std::endl;
                std::cerr << "Verifiez que SFML est correctement installe." << std::endl;
                return 1;
            }
        }
        else if (mode == "qt") {
            #ifdef QT_AVAILABLE
                try {
                    std::cout << "Lancement de l'interface Qt..." << std::endl;
                    // Qt nécessite QApplication
                    QApplication app(argc, argv);
                    InterfaceQt interfaceQt(jeu);
                    interfaceQt.executer();
                    return app.exec();
                } catch (const std::exception& e) {
                    std::cerr << "Erreur Qt: " << e.what() << std::endl;
                    std::cerr << "Verifiez que Qt est correctement installe." << std::endl;
                    std::cerr << "Verifiez que les plugins Qt sont dans build/Release/platforms/" << std::endl;
                    return 1;
                }
            #else
                std::cerr << "Erreur: Qt n'est pas disponible sur ce systeme." << std::endl;
                std::cerr << "Utilisez le mode 'sfml' ou 'console' a la place." << std::endl;
                return 1;
            #endif
        }
        else {
            std::cerr << "Mode inconnu: " << mode << std::endl;
            std::cerr << "Utilisez 'console', 'sfml' ou 'qt'" << std::endl;
            return 1;
        }
        
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erreur d'argument: " << e.what() << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Erreur d'execution: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Erreur inattendue: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Erreur inconnue" << std::endl;
        return 1;
    }
    
    return 0;
}
