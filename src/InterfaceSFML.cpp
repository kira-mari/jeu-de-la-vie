#include "InterfaceSFML.hpp"
#include "Constantes.hpp"
#include <iostream>
#include <sstream>

// Détection de la version SFML
#if SFML_VERSION_MAJOR >= 3
    #define SFML_VERSION_3
#else
    #define SFML_VERSION_2
#endif

InterfaceSFML::InterfaceSFML(
    JeuDeLaVie& jeuRef,
    int largeur,
    int hauteur,
    const std::string& titre
) : jeu(jeuRef),
    enPause(false),
    delaiIteration(Config::DELAI_ITERATION_DEFAUT),
    couleurVivante(sf::Color(Config::CouleurVivanteSFML::R, Config::CouleurVivanteSFML::G, Config::CouleurVivanteSFML::B)),
    couleurMorte(sf::Color(Config::CouleurMorteSFML::R, Config::CouleurMorteSFML::G, Config::CouleurMorteSFML::B)),
    couleurObstacle(sf::Color(Config::CouleurObstacleMortSFML::R, Config::CouleurObstacleMortSFML::G, Config::CouleurObstacleMortSFML::B)),
    couleurGrille(sf::Color(Config::CouleurGrilleSFML::R, Config::CouleurGrilleSFML::G, Config::CouleurGrilleSFML::B)) {
    
    // Calculer la taille optimale de la fenêtre selon la grille
    const Grille& grille = jeu.obtenirGrille();
    int nbLignes = grille.obtenirNbLignes();
    int nbColonnes = grille.obtenirNbColonnes();
    
    int tailleCelluleCible = Config::TAILLE_CELLULE_DEFAUT_SFML;
    int largeurCalculee = nbColonnes * tailleCelluleCible;
    int hauteurCalculee = nbLignes * tailleCelluleCible;
    
    while ((largeurCalculee > largeur || hauteurCalculee > hauteur) && tailleCelluleCible > Config::TAILLE_CELLULE_MIN) {
        tailleCelluleCible -= 5;
        largeurCalculee = nbColonnes * tailleCelluleCible;
        hauteurCalculee = nbLignes * tailleCelluleCible;
    }
    
    int largeurAdaptee = nbColonnes * tailleCelluleCible;
    int hauteurAdaptee = nbLignes * tailleCelluleCible;
    
    #ifdef SFML_VERSION_3
        // SFML 3.0 : VideoMode prend un Vector2u
        fenetre = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(sf::Vector2u(largeurAdaptee, hauteurAdaptee)),
            titre
        );
    #else
        // SFML 2.x : VideoMode prend (width, height)
        fenetre = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(largeurAdaptee, hauteurAdaptee),
            titre
        );
    #endif
    
    fenetre->setFramerateLimit(Config::FPS_LIMITE);
    calculerTailleCellule();
}

void InterfaceSFML::calculerTailleCellule() {
    const Grille& grille = jeu.obtenirGrille();
    int largeurFenetre = fenetre->getSize().x;
    int hauteurFenetre = fenetre->getSize().y;
    
    int tailleParLargeur = largeurFenetre / grille.obtenirNbColonnes();
    int tailleParHauteur = hauteurFenetre / grille.obtenirNbLignes();
    
    tailleCellule = std::min(tailleParLargeur, tailleParHauteur);
    
    // Assurer une taille minimale
    if (tailleCellule < Config::TAILLE_CELLULE_MIN) {
        tailleCellule = Config::TAILLE_CELLULE_MIN;
    }
}

void InterfaceSFML::executer() {
    while (fenetre->isOpen()) {
        gererEvenements();
        
        if (!enPause && horloge.getElapsedTime().asSeconds() >= delaiIteration) {
            if (!jeu.executerIteration()) {
                std::cout << "Simulation terminée à l'itération " << jeu.obtenirIteration() << std::endl;
                enPause = true;
            }
            horloge.restart();
            
            std::ostringstream titre;
            titre << "Jeu de la Vie - Iteration " << jeu.obtenirIteration();
            if (jeu.obtenirIterationMax() > 0) {
                titre << "/" << jeu.obtenirIterationMax();
            }
            fenetre->setTitle(titre.str());
        }
        
        afficher();
    }
}

void InterfaceSFML::gererEvenements() {
    #ifdef SFML_VERSION_3
        // SFML 3.0 : pollEvent retourne un optional
        while (auto event = fenetre->pollEvent()) {
            // SFML 3.0 : Event est un std::variant, utiliser std::visit ou if/else avec is<>
            if (event->is<sf::Event::Closed>()) {
                fenetre->close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Space:
                        // Pause/Reprendre
                        enPause = !enPause;
                        horloge.restart();
                        std::cout << (enPause ? "Pause" : "Reprise") << std::endl;
                        break;
                        
                    case sf::Keyboard::Key::Right:
                        // Avancer d'une iteration
                        if (jeu.executerIteration()) {
                            std::cout << "Iteration " << jeu.obtenirIteration() << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::Key::Left:
                        // Revenir en arriere
                        if (jeu.revenirEnArriere()) {
                            std::cout << "Retour a l'iteration " << jeu.obtenirIteration() << std::endl;
                        } else {
                            std::cout << "Impossible de revenir en arriere (debut de la simulation)" << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::Key::Up:
                        // Accelerer
                        delaiIteration = std::max(Config::DELAI_ITERATION_MIN, delaiIteration - Config::DELAI_ITERATION_PAS);
                        std::cout << "Delai: " << delaiIteration << "s" << std::endl;
                        break;
                        
                    case sf::Keyboard::Key::Down:
                        // Ralentir
                        delaiIteration = std::min(Config::DELAI_ITERATION_MAX, delaiIteration + Config::DELAI_ITERATION_PAS);
                        std::cout << "Delai: " << delaiIteration << "s" << std::endl;
                        break;
                        
                    case sf::Keyboard::Key::T:
                        // Toggle mode torique
                        {
                            bool modeTorique = jeu.estModeTorique();
                            jeu.definirModeTorique(!modeTorique);
                            std::cout << "Mode torique: " << (!modeTorique ? "active" : "desactive") << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::Key::P:
                        // Toggle mode parallele
                        {
                            bool modeParallele = jeu.estModeParallele();
                            jeu.definirModeParallele(!modeParallele);
                            std::cout << "Mode parallele: " << (!modeParallele ? "active" : "desactive") << std::endl;
                        }
                        break;
                        
                    default:
                        // Gerer les motifs (G pour planeur, B pour bloc, etc.)
                        gererPlacementMotif(keyPressed->code);
                        break;
                }
            }
        }
    #else
        // SFML 2.x : pollEvent prend une référence à Event
        sf::Event event;
        while (fenetre->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                fenetre->close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Space:
                        // Pause/Reprendre
                        enPause = !enPause;
                        horloge.restart();
                        std::cout << (enPause ? "Pause" : "Reprise") << std::endl;
                        break;
                        
                    case sf::Keyboard::Right:
                        // Avancer d'une iteration
                        if (jeu.executerIteration()) {
                            std::cout << "Iteration " << jeu.obtenirIteration() << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::Left:
                        // Revenir en arriere
                        if (jeu.revenirEnArriere()) {
                            std::cout << "Retour a l'iteration " << jeu.obtenirIteration() << std::endl;
                        } else {
                            std::cout << "Impossible de revenir en arriere (debut de la simulation)" << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::Up:
                        // Accelerer
                        delaiIteration = std::max(Config::DELAI_ITERATION_MIN, delaiIteration - Config::DELAI_ITERATION_PAS);
                        std::cout << "Delai: " << delaiIteration << "s" << std::endl;
                        break;
                        
                    case sf::Keyboard::Down:
                        // Ralentir
                        delaiIteration = std::min(Config::DELAI_ITERATION_MAX, delaiIteration + Config::DELAI_ITERATION_PAS);
                        std::cout << "Delai: " << delaiIteration << "s" << std::endl;
                        break;
                        
                    case sf::Keyboard::T:
                        // Toggle mode torique
                        {
                            bool modeTorique = jeu.estModeTorique();
                            jeu.definirModeTorique(!modeTorique);
                            std::cout << "Mode torique: " << (!modeTorique ? "active" : "desactive") << std::endl;
                        }
                        break;
                        
                    case sf::Keyboard::P:
                        // Toggle mode parallele
                        {
                            bool modeParallele = jeu.estModeParallele();
                            jeu.definirModeParallele(!modeParallele);
                            std::cout << "Mode parallele: " << (!modeParallele ? "active" : "desactive") << std::endl;
                        }
                        break;
                        
                    default:
                        // Gerer les motifs (G pour planeur, B pour bloc, etc.)
                        gererPlacementMotif(event.key.code);
                        break;
                }
            }
        }
    #endif
}

void InterfaceSFML::gererPlacementMotif(sf::KeyCode touche) {
    const Grille& grille = jeu.obtenirGrille();
    int ligneCentre = grille.obtenirNbLignes() / 2;
    int colonneCentre = grille.obtenirNbColonnes() / 2;
    
    bool place = false;
    std::string nomMotif;
    
    #ifdef SFML_VERSION_3
        switch (touche) {
            case sf::Keyboard::Key::G:
                place = jeu.placerMotif("planeur", ligneCentre, colonneCentre);
                nomMotif = "planeur";
                break;
                
            case sf::Keyboard::Key::B:
                place = jeu.placerMotif("bloc", ligneCentre, colonneCentre);
                nomMotif = "bloc";
                break;
                
            case sf::Keyboard::Key::C:
                place = jeu.placerMotif("clignotant", ligneCentre, colonneCentre);
                nomMotif = "clignotant";
                break;
                
            case sf::Keyboard::Key::H:
                place = jeu.placerMotif("ruche", ligneCentre, colonneCentre);
                nomMotif = "ruche";
                break;
                
            default:
                break;
        }
    #else
        switch (touche) {
            case sf::Keyboard::G:
                place = jeu.placerMotif("planeur", ligneCentre, colonneCentre);
                nomMotif = "planeur";
                break;
                
            case sf::Keyboard::B:
                place = jeu.placerMotif("bloc", ligneCentre, colonneCentre);
                nomMotif = "bloc";
                break;
                
            case sf::Keyboard::C:
                place = jeu.placerMotif("clignotant", ligneCentre, colonneCentre);
                nomMotif = "clignotant";
                break;
                
            case sf::Keyboard::H:
                place = jeu.placerMotif("ruche", ligneCentre, colonneCentre);
                nomMotif = "ruche";
                break;
                
            default:
                break;
        }
    #endif
    
    if (place) {
        std::cout << "Motif '" << nomMotif << "' place" << std::endl;
    }
}

void InterfaceSFML::afficher() {
    fenetre->clear(sf::Color(20, 20, 30));
    dessinerGrille();
    dessinerInformations();
    fenetre->display();
}

void InterfaceSFML::dessinerGrille() {
    const Grille& grille = jeu.obtenirGrille();
    
    #ifdef SFML_VERSION_3
        sf::RectangleShape celluleRect({static_cast<float>(tailleCellule - 1), static_cast<float>(tailleCellule - 1)});
    #else
        sf::RectangleShape celluleRect(sf::Vector2f(static_cast<float>(tailleCellule - 1), static_cast<float>(tailleCellule - 1)));
    #endif
    
    for (int ligne = 0; ligne < grille.obtenirNbLignes(); ++ligne) {
        for (int colonne = 0; colonne < grille.obtenirNbColonnes(); ++colonne) {
            const Cellule& cellule = grille.obtenirCellule(ligne, colonne);
            
            // Choisir la couleur selon l'état
            if (cellule.estObstacle()) {
                if (cellule.estVivante()) {
                    // Obstacle vivant = ORANGE
                    celluleRect.setFillColor(sf::Color(
                        Config::CouleurObstacleVivantSFML::R,
                        Config::CouleurObstacleVivantSFML::G,
                        Config::CouleurObstacleVivantSFML::B
                    ));
                } else {
                    // Obstacle mort = ROUGE
                    celluleRect.setFillColor(couleurObstacle);
                }
            } else if (cellule.estVivante()) {
                celluleRect.setFillColor(couleurVivante);
            } else {
                celluleRect.setFillColor(couleurMorte);
            }
            
            // Positionner et dessiner (colonne = x, ligne = y)
            celluleRect.setPosition(sf::Vector2f(
                static_cast<float>(colonne * tailleCellule), 
                static_cast<float>(ligne * tailleCellule)
            ));
            fenetre->draw(celluleRect);
        }
    }
}

void InterfaceSFML::dessinerInformations() {
    static sf::Font police;
    static bool policeChargee = false;
    
    if (!policeChargee) {
        #ifdef SFML_VERSION_3
            #ifdef _WIN32
                policeChargee = police.openFromFile("C:/Windows/Fonts/arial.ttf");
            #else
                policeChargee = police.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
            #endif
        #else
            #ifdef _WIN32
                policeChargee = police.loadFromFile("C:/Windows/Fonts/arial.ttf");
            #else
                policeChargee = police.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
            #endif
        #endif
        
        if (!policeChargee) {
            return;
        }
    }
    
    std::ostringstream oss;
    oss << "Iteration: " << jeu.obtenirIteration();
    if (jeu.obtenirIterationMax() > 0) {
        oss << "/" << jeu.obtenirIterationMax();
    } else {
        oss << " (illimite)";
    }
    
    const Grille& grille = jeu.obtenirGrille();
    int nbVivantes = 0;
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            if (grille.obtenirCellule(i, j).estVivante()) {
                ++nbVivantes;
            }
        }
    }
    
    oss << "\nCellules vivantes: " << nbVivantes
        << "\nGrille: " << grille.obtenirNbLignes() << "x" << grille.obtenirNbColonnes()
        << "\nMode torique: " << (jeu.estModeTorique() ? "ON" : "OFF")
        << "\nMode parallele: " << (jeu.estModeParallele() ? "ON" : "OFF")
        << "\nVitesse: " << (1.0f / delaiIteration) << " it/s"
        << "\n" << (enPause ? "[PAUSE]" : "[EN COURS]")
        << "\n\nControles:"
        << "\n  ESPACE: Pause/Reprise"
        << "\n  <- ->: Reculer/Avancer"
        << "\n  Haut/Bas: Vitesse"
        << "\n  G/B/C/H: Motifs"
        << "\n  T/P: Torique/Parallele";
    
    #ifdef SFML_VERSION_3
        sf::Text texte(police);
        texte.setCharacterSize(16);
        texte.setFillColor(sf::Color::White);
        texte.setString(oss.str());
        texte.setPosition({10.f, 10.f});
        
        sf::RectangleShape fond({280.f, 240.f});
        fond.setPosition({5.f, 5.f});
        fond.setFillColor(sf::Color(0, 0, 0, 200));
        fond.setOutlineThickness(2.f);
        fond.setOutlineColor(sf::Color(50, 150, 50));
    #else
        sf::Text texte;
        texte.setFont(police);
        texte.setCharacterSize(16);
        texte.setFillColor(sf::Color::White);
        texte.setString(oss.str());
        texte.setPosition(10.f, 10.f);
        
        sf::RectangleShape fond(sf::Vector2f(280.f, 240.f));
        fond.setPosition(5.f, 5.f);
        fond.setFillColor(sf::Color(0, 0, 0, 200));
        fond.setOutlineThickness(2.f);
        fond.setOutlineColor(sf::Color(50, 150, 50));
    #endif
    
    fenetre->draw(fond);
    fenetre->draw(texte);
}
