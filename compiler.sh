#!/bin/bash

# Script de compilation pour Linux/macOS
# Prérequis: CMake, GCC/Clang, SFML

echo "=== Compilation du Jeu de la Vie ==="
echo ""

# Vérifier si CMake est installé
if ! command -v cmake &> /dev/null; then
    echo "ERREUR: CMake n'est pas installé"
    exit 1
fi

# Créer le dossier build
if [ ! -d "build" ]; then
    echo "Création du dossier build..."
    mkdir build
fi

cd build

# Configuration avec CMake
echo "Configuration du projet avec CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "ERREUR lors de la configuration CMake"
    cd ..
    exit 1
fi

# Compilation
echo ""
echo "Compilation du projet..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    echo "ERREUR lors de la compilation"
    cd ..
    exit 1
fi

cd ..

echo ""
echo "=== Compilation réussie! ==="
echo ""
echo "Exécutables créés dans le dossier build/"
echo ""
echo "Pour tester:"
echo "  ./build/jeu_de_la_vie exemples/exemple1_planeur.txt"
echo "  ./build/tests"
echo ""
