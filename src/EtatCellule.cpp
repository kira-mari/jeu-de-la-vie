#include "EtatCellule.hpp"

std::unique_ptr<EtatCellule> CelluleMorte::cloner() const {
    return std::make_unique<CelluleMorte>();
}

std::unique_ptr<EtatCellule> CelluleVivante::cloner() const {
    return std::make_unique<CelluleVivante>();
}

std::unique_ptr<EtatCellule> CelluleObstacle::cloner() const {
    return std::make_unique<CelluleObstacle>(vivante);
}
