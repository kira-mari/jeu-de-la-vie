#ifndef GRILLE_HPP
#define GRILLE_HPP

#include "Cellule.hpp"
#include "RegleJeu.hpp"
#include "Constantes.hpp"
#include <vector>
#include <memory>

/**
 * @brief Classe représentant la grille du jeu de la vie
 * 
 * La grille gère une collection de cellules et leur évolution selon les règles.
 * Elle supporte le multithreading pour la parallélisation (extension bonus).
 * 
 * @note Le mutex a été supprimé car l'optimisation ne nécessite plus de synchronisation
 */
class Grille {
private:
    std::vector<std::vector<Cellule>> cellules;
    int nbLignes;
    int nbColonnes;
    std::shared_ptr<RegleJeu> regle;
    
public:
    /**
     * @brief Constructeur de la grille
     * @param lignes Nombre de lignes
     * @param colonnes Nombre de colonnes
     * @param regleJeu Les règles à appliquer (standard par défaut)
     * @throws std::invalid_argument Si les dimensions sont invalides (≤ 0)
     */
    Grille(int lignes, int colonnes, std::shared_ptr<RegleJeu> regleJeu = nullptr);
    
    /**
     * @brief Constructeur de copie
     * @param autre La grille à copier
     */
    Grille(const Grille& autre);
    
    /**
     * @brief Opérateur d'affectation
     * @param autre La grille à copier
     * @return Référence vers cette grille
     */
    Grille& operator=(const Grille& autre);
    
    /**
     * @brief Destructeur
     */
    ~Grille();
    
    /**
     * @brief Obtient le nombre de lignes
     * @return Le nombre de lignes
     */
    int obtenirNbLignes() const { return nbLignes; }
    
    /**
     * @brief Obtient le nombre de colonnes
     * @return Le nombre de colonnes
     */
    int obtenirNbColonnes() const { return nbColonnes; }
    
    /**
     * @brief Obtient une cellule à une position donnée
     * @param ligne La ligne de la cellule
     * @param colonne La colonne de la cellule
     * @return Une référence vers la cellule
     * @throws std::out_of_range Si la position est invalide
     */
    Cellule& obtenirCellule(int ligne, int colonne);
    const Cellule& obtenirCellule(int ligne, int colonne) const;
    
    /**
     * @brief Définit l'état d'une cellule
     * @param ligne La ligne de la cellule
     * @param colonne La colonne de la cellule
     * @param etat Le nouvel état
     */
    void definirEtatCellule(int ligne, int colonne, std::unique_ptr<EtatCellule> etat);
    
    /**
     * @brief Vérifie si une position est valide dans la grille
     * @param ligne La ligne à vérifier
     * @param colonne La colonne à vérifier
     * @return true si la position est valide, false sinon
     */
    bool estPositionValide(int ligne, int colonne) const;
    
    /**
     * @brief Met à jour la grille pour passer à l'itération suivante
     * @param parallele Si true, utilise le multithreading (extension bonus)
     * @throws std::runtime_error Si une erreur survient lors de la mise à jour
     */
    void mettreAJour(bool parallele = false);
    
    /**
     * @brief Vérifie si la grille est stable (ne change plus)
     * @return true si la grille est stable, false sinon
     */
    bool estStable() const;
    
    /**
     * @brief Compare deux grilles
     * @param autre La grille à comparer
     * @return true si les grilles sont identiques, false sinon
     */
    bool operator==(const Grille& autre) const;
    
    /**
     * @brief Définit les règles du jeu
     * @param nouvelleRegle Les nouvelles règles à appliquer
     */
    void definirRegle(std::shared_ptr<RegleJeu> nouvelleRegle);
    
    /**
     * @brief Place un motif préprogrammé sur la grille (extension bonus)
     * @param motif Le nom du motif (planeur, oscillateur, etc.)
     * @param ligneDepart La ligne de départ
     * @param colonneDepart La colonne de départ
     * @return true si le motif a été placé, false sinon
     * @throws std::invalid_argument Si le nom du motif est inconnu
     */
    bool placerMotif(const std::string& motif, int ligneDepart, int colonneDepart);
    
private:
    /**
     * @brief Met à jour un ensemble de lignes (parallélisation optimisée)
     * @param ligneDebut Première ligne à traiter
     * @param ligneFin Ligne de fin (exclue)
     * @param nouvelleGrille La grille où stocker les nouveaux états
     * @note Cette méthode ne nécessite pas de mutex car chaque thread traite des lignes distinctes
     */
    void mettreAJourLignes(int ligneDebut, int ligneFin, Grille& nouvelleGrille);
    
    std::unique_ptr<Grille> grilleMemoire; ///< Grille précédente pour vérifier la stabilité
    bool premiereIteration = true; ///< Indique si c'est la première itération
};

#endif // GRILLE_HPP
