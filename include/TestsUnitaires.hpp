#ifndef TESTS_UNITAIRES_HPP
#define TESTS_UNITAIRES_HPP

#include "Grille.hpp"
#include <string>
#include <vector>

/**
 * @brief Classe contenant les tests unitaires pour le jeu de la vie
 */
class TestsUnitaires {
public:
    /**
     * @brief Exécute tous les tests
     * @return true si tous les tests passent, false sinon
     */
    static bool executerTousLesTests();
    
    /**
     * @brief Teste l'évolution d'une grille après n itérations
     * @param grilleInitiale La grille de départ
     * @param grilleAttendue La grille attendue après les itérations
     * @param nbIterations Le nombre d'itérations à effectuer
     * @param nomTest Le nom du test (pour l'affichage)
     * @return true si le test passe, false sinon
     */
    static bool testerEvolution(
        Grille& grilleInitiale,
        const Grille& grilleAttendue,
        int nbIterations,
        const std::string& nomTest
    );
    
    /**
     * @brief Teste le motif oscillateur (clignotant)
     */
    static bool testerClignotant();
    
    /**
     * @brief Teste le motif stable (bloc)
     */
    static bool testerBloc();
    
    /**
     * @brief Teste le planeur
     */
    static bool testerPlaneur();
    
    /**
     * @brief Teste la grille torique
     */
    static bool testerModeTorique();
    
    /**
     * @brief Teste les cellules obstacles
     */
    static bool testerObstacles();
    
    /**
     * @brief Teste les règles de base
     */
    static bool testerReglesBase();
    
private:
    /**
     * @brief Crée une grille de test
     * @param lignes Nombre de lignes
     * @param colonnes Nombre de colonnes
     * @param cellulesVivantes Positions des cellules vivantes
     * @param regle Les règles à utiliser
     * @return Une grille configurée
     */
    static Grille creerGrille(
        int lignes,
        int colonnes,
        const std::vector<std::pair<int, int>>& cellulesVivantes,
        std::shared_ptr<RegleJeu> regle = nullptr
    );
    
    /**
     * @brief Affiche le résultat d'un test
     */
    static void afficherResultat(const std::string& nomTest, bool reussi);
};

#endif // TESTS_UNITAIRES_HPP
