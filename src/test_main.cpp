#include "TestsUnitaires.hpp"
#include <iostream>

/**
 * @brief Programme principal pour exécuter les tests unitaires
 */
int main() {
    bool tousReussis = TestsUnitaires::executerTousLesTests();
    
    return tousReussis ? 0 : 1;
}
