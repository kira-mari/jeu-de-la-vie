#ifndef GESTIONNAIRE_FICHIER_HPP
#define GESTIONNAIRE_FICHIER_HPP

#include "Grille.hpp"
#include <string>
#include <memory>

/**
 * @brief Classe responsable de la lecture et l'écriture des fichiers de grilles
 */
class GestionnaireFichier {
public:
    /**
     * @brief Charge une grille depuis un fichier
     * @param cheminFichier Le chemin du fichier à lire
     * @param regle Les règles à appliquer à la grille
     * @return Un pointeur unique vers la grille chargée
     * @throws std::runtime_error si le fichier ne peut pas être lu
     */
    static std::unique_ptr<Grille> chargerGrille(
        const std::string& cheminFichier,
        std::shared_ptr<RegleJeu> regle = nullptr
    );
    
    /**
     * @brief Sauvegarde une grille dans un fichier
     * @param grille La grille à sauvegarder
     * @param cheminFichier Le chemin du fichier de sortie
     * @throws std::runtime_error si le fichier ne peut pas être écrit
     */
    static void sauvegarderGrille(const Grille& grille, const std::string& cheminFichier);
    
    /**
     * @brief Crée le dossier de sortie pour les résultats
     * @param cheminFichierEntree Le chemin du fichier d'entrée
     * @return Le chemin du dossier de sortie créé
     */
    static std::string creerDossierSortie(const std::string& cheminFichierEntree);
    
    /**
     * @brief Extrait le nom du fichier sans l'extension
     * @param chemin Le chemin complet du fichier
     * @return Le nom du fichier sans extension
     */
    static std::string extraireNomFichier(const std::string& chemin);
};

#endif // GESTIONNAIRE_FICHIER_HPP
