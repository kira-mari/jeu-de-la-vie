#ifndef ETAT_CELLULE_HPP
#define ETAT_CELLULE_HPP

#include <memory>
#include <string>

/**
 * @brief Classe abstraite représentant l'état d'une cellule
 * 
 * États possibles : Vivante, Morte, Obstacle (mort ou vivant).
 */
class EtatCellule {
public:
    virtual ~EtatCellule() = default;
    
    /**
     * @brief Indique si la cellule est vivante
     * @return true si la cellule est vivante, false sinon
     */
    virtual bool estVivante() const = 0;
    
    /**
     * @brief Indique si la cellule est un obstacle
     * @return true si la cellule est un obstacle, false sinon
     */
    virtual bool estObstacle() const = 0;
    
    /**
     * @brief Crée une copie de l'état
     * @return Un pointeur unique vers une copie de l'état
     */
    virtual std::unique_ptr<EtatCellule> cloner() const = 0;
    
    /**
     * @brief Obtient une représentation textuelle de l'état
     * @return Une chaîne de caractères représentant l'état
     */
    virtual std::string versChaine() const = 0;
    
    /**
     * @brief Obtient la valeur entière de l'état (pour sauvegarder)
     * @return 0 pour mort, 1 pour vivant, 2 pour obstacle
     */
    virtual int versEntier() const = 0;
};

/**
 * @brief Classe représentant une cellule morte
 */
class CelluleMorte : public EtatCellule {
public:
    bool estVivante() const override { return false; }
    bool estObstacle() const override { return false; }
    std::unique_ptr<EtatCellule> cloner() const override;
    std::string versChaine() const override { return "Morte"; }
    int versEntier() const override { return 0; }
};

/**
 * @brief Classe représentant une cellule vivante
 */
class CelluleVivante : public EtatCellule {
public:
    bool estVivante() const override { return true; }
    bool estObstacle() const override { return false; }
    std::unique_ptr<EtatCellule> cloner() const override;
    std::string versChaine() const override { return "Vivante"; }
    int versEntier() const override { return 1; }
};

/**
 * @brief Classe représentant une cellule obstacle (extension bonus)
 * Les cellules obstacles ne changent jamais d'état
 */
class CelluleObstacle : public EtatCellule {
private:
    bool vivante; // Un obstacle peut être vivant ou mort
    
public:
    explicit CelluleObstacle(bool estVivante = false) : vivante(estVivante) {}
    
    bool estVivante() const override { return vivante; }
    bool estObstacle() const override { return true; }
    std::unique_ptr<EtatCellule> cloner() const override;
    std::string versChaine() const override { return vivante ? "Obstacle Vivant" : "Obstacle Mort"; }
    int versEntier() const override { return vivante ? 3 : 2; }
};

#endif // ETAT_CELLULE_HPP
