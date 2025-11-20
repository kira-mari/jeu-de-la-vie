# 🎮 Jeu de la Vie de Conway - Projet POO C++

> Implémentation complète en C++17 avec architecture orientée objet, patterns de conception, et **deux interfaces graphiques modernes** (SFML + Qt)

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15+-green.svg)](https://cmake.org/)
[![SFML](https://img.shields.io/badge/SFML-3.0-red.svg)](https://www.sfml-dev.org/)
[![Qt](https://img.shields.io/badge/Qt-6.9-brightgreen.svg)](https://www.qt.io/)

[![Build and Test](https://github.com/kira-mari/jeu-de-la-vie/workflows/Build%20and%20Test/badge.svg)](https://github.com/kira-mari/jeu-de-la-vie/actions)
[![License](https://img.shields.io/badge/license-Academic-blue.svg)](LICENSE)
[![Documentation](https://img.shields.io/badge/docs-Doxygen-blue.svg)](https://kira-mari.github.io/jeu-de-la-vie/)

---

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Interfaces disponibles](#interfaces-disponibles)
- [Installation rapide](#installation-rapide)
- [Utilisation](#utilisation)
- [Architecture POO](#architecture-poo)
- [Format des fichiers](#format-des-fichiers)
- [Tests](#tests)
- [Structure du projet](#structure-du-projet)
- [Documentation technique](#documentation-technique)

---

## Fonctionnalités

### Fonctionnalités de base
- Simulation du jeu de la vie selon les règles de Conway
- Lecture de fichiers d'entrée avec état initial
- 3 modes d'affichage : Console, SFML, Qt
- Tests unitaires complets (10/10 réussis)
- Architecture POO avec principes SOLID

### Extensions bonus (toutes implémentées)
- **Grille torique** : Bords connectés (bouclés)
- **Cellules obstacles** : Cellules dont l'état ne change jamais
- **Motifs préprogrammés** : Placement interactif (planeur, bloc, clignotant, ruche)
- **Parallélisation optimisée** : Multi-thread sans mutex (+15% performance)
- **Historique/Undo** : Retour aux itérations précédentes (flèche gauche)

### Fonctionnalités avancées
- Deux interfaces graphiques modernes (SFML + Qt)
- Taille adaptative des fenêtres selon la grille
- Contrôle de vitesse en temps réel
- Limite d'itérations configurable
- Statistiques en temps réel (cellules vivantes, itération actuelle)

---

## Interfaces disponibles

### 1. Interface Qt (Moderne et professionnelle) - RECOMMANDÉ

**Caractéristiques** :
- Design moderne et épuré
- Contrôles riches : boutons, sliders, spinbox, checkboxes
- Statistiques en temps réel
- Options configurables (torique, parallélisation, itération max)
- Placement de motifs via menu déroulant
- Raccourcis clavier intuitifs

**Lancement** :
```powershell
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt
# ou explicitement :
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt -m qt
```

---

### 2. Interface SFML (Légère et rapide)

**Caractéristiques** :
- Très performante pour grandes grilles (500x500+)
- Style minimaliste
- Contrôle 100% clavier
- Affichage fluide 60 FPS

**Lancement** :
```powershell
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple3_complexe.txt -m sfml
```

### Dessin interactif (SFML)

- Interaction souris :
  - Clic gauche : place une cellule vivante
  - Clic droit : place une cellule morte
  - Maintenir `Ctrl` + clic gauche : place un obstacle vivant
  - Maintenir `Ctrl` + clic droit : place un obstacle mort
- Cliquer-glisser (drag) : peint en continu sur la grille
- Le mapping utilise `fenetre->mapPixelToCoords(...)` pour convertir les pixels en coordonnées monde, garantissant un alignement exact même après redimensionnement.
- Une `sf::View` est utilisée avec un viewport centré (letterboxing) pour préserver l'aspect carré des cellules lors du redimensionnement de la fenêtre.

---

### 3. **Mode Console** (Export fichiers)

**Caractéristiques** :
- Sauvegarde de toutes les itérations dans des fichiers
- Affichage ASCII dans la console
- Idéal pour traitement batch

**Lancement** :
```powershell
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt -m console -n 50
```

---

## Installation rapide

### Prérequis
- Windows 10/11 avec PowerShell
- Visual Studio 2022 BuildTools (compilateur MSVC)
- CMake 3.15+
- vcpkg (gestionnaire de packages)

### 1. Installer les dépendances

```powershell
# SFML (interface légère)
C:\vcpkg\vcpkg.exe install sfml:x64-windows

# Qt (interface moderne)
C:\vcpkg\vcpkg.exe install qtbase[widgets,gui]:x64-windows
```

> **Note** : Le projet compile avec n'importe quelle combinaison !

### 2. Compiler le projet

```powershell
# Configuration
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

# Compilation
cmake --build build --config Release
```

### 3. Copier les plugins Qt (si Qt installé)

```powershell
Copy-Item -Path "C:\vcpkg\installed\x64-windows\Qt6\plugins\platforms" -Destination ".\build\Release\" -Recurse -Force
```

---

## Utilisation

### Arguments de ligne de commande

```
jeu_de_la_vie.exe <fichier> [options]

Options:
  -m <mode>       Mode: 'console', 'sfml', 'qt' (défaut: qt)
  -n <nombre>     Nombre max d'itérations (défaut: 100, 0 = illimité)
  -t              Active le mode torique
  -p              Active la parallélisation
  -h, --help      Affiche l'aide
```


### Scripts de lancement rapide (Windows)

Pour faciliter le lancement, utilisez les scripts batch :

```powershell
# Interface SFML
.\lancer_sfml.bat .\exemples\exemple1_planeur.txt -t -p

# Interface Qt
.\lancer_qt.bat .\exemples\exemple3_complexe.txt -n 200
```

### Exemples d'utilisation

```powershell
# Interface Qt par défaut
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt

# Interface SFML rapide
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple3_complexe.txt -m sfml

# Mode console avec mode torique
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt -m console -t -n 100

# Grande grille avec parallélisation
.\build\Release\jeu_de_la_vie.exe .\exemples\grande_grille_500x500.txt -p -m sfml

# Obstacles vivants et morts (différenciés par couleur)
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple6_obstacles_vivants.txt
```

### Raccourcis clavier (SFML et Qt)

| Touche | Action |
|--------|--------|
| **ESPACE** | Pause/Reprise |
| **→** | Itération suivante |
| **←** | Itération précédente (undo) |
| **↑** | Accélérer |
| **↓** | Ralentir |
| **T** | Toggle mode torique |
| **P** | Toggle parallélisation |
| **G** | Placer un planeur |
| **B** | Placer un bloc |
| **C** | Placer un clignotant |
| **H** | Placer une ruche |

---

## Architecture POO

### Principes SOLID appliqués

- **S**ingle Responsibility : Chaque classe a une seule responsabilité
- **O**pen/Closed : Extensible sans modification (hiérarchies polymorphiques)
- **L**iskov Substitution : Tous les états/règles sont interchangeables
- **I**nterface Segregation : Interfaces minimales et spécifiques
- **D**ependency Inversion : Dépendances vers abstractions

### Patterns de conception utilisés

- **Strategy Pattern** : `RegleJeu` (RegleStandard, RegleTorique)
- **State Pattern** : `EtatCellule` (CelluleMorte, CelluleVivante, CelluleObstacle)
- **Template Method** : `RegleJeu::calculerNouvelEtat()` utilise `compterVoisinsVivants()`
- **Dependency Injection** : Règles injectées dans Grille
- **Observer Pattern** : Qt signals/slots pour événements GUI
- **MVC Pattern** : Model (JeuDeLaVie), View (Interfaces), Controller (événements)

### Hiérarchie des classes

```
Logique métier
==============
EtatCellule (abstract)
├── CelluleMorte
├── CelluleVivante
└── CelluleObstacle

RegleJeu (abstract)
├── RegleStandard
└── RegleTorique

Cellule → contient EtatCellule
Grille → contient Cellule[][], utilise RegleJeu
JeuDeLaVie → contient Grille, gère historique

Interfaces
==========
InterfaceSFML (SFML) → utilise JeuDeLaVie
InterfaceQt (Qt6) → utilise JeuDeLaVie
  └── GrilleWidget → dessine la grille
ModeConsole → utilise JeuDeLaVie

Utilitaires
===========
GestionnaireFichier → charge/sauvegarde Grille
TestsUnitaires → tests automatisés
```

---

## Format des fichiers

### Fichier d'entrée

```
<nb_lignes> <nb_colonnes>
<ligne_1>
<ligne_2>
...
<ligne_n>
```

**Valeurs des cellules** :
- `0` : Cellule morte (blanc/gris)
- `1` : Cellule vivante (vert)
- `2` : Obstacle mort (rouge) - ne change jamais d'état
- `3` : Obstacle vivant (orange) - ne change jamais d'état

**Note sur les obstacles** : Les obstacles sont des cellules "figées" qui ne suivent pas les règles de Conway. Ils restent dans leur état initial (vivant ou mort) durant toute la simulation. Visuellement :
- Obstacle mort (2) = 🔴 ROUGE
- Obstacle vivant (3) = 🟠 ORANGE

### Exemple

```
5 10
0 0 1 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0 0
0 1 1 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0
```

---

## Tests

### Exécuter les tests

```powershell
.\build\Release\tests.exe
```

### Tests couverts (10/10 passing)

-  Règles de base du jeu de la vie
-  Motifs stables (bloc 2x2)
-  Oscillateurs (clignotant période 2)
-  Motifs mobiles (planeur)
-  Mode torique
-  Cellules obstacles
-  Grilles complexes
-  Parallélisation
-  Undo/Redo
-  Placement de motifs

---

## Structure du projet

```
CODE/
├── include/              # Headers (.hpp)
│   ├── EtatCellule.hpp
│   ├── Cellule.hpp
│   ├── RegleJeu.hpp
│   ├── Grille.hpp
│   ├── JeuDeLaVie.hpp
│   ├── GestionnaireFichier.hpp
│   ├── InterfaceSFML.hpp       # SFML
│   ├── InterfaceQt.hpp         # Qt 6
│   ├── ModeConsole.hpp
│   ├── Constantes.hpp          # Configuration centralisée
│   └── TestsUnitaires.hpp
├── src/                  # Sources (.cpp)
│   ├── EtatCellule.cpp
│   ├── Cellule.cpp
│   ├── RegleJeu.cpp
│   ├── Grille.cpp
│   ├── JeuDeLaVie.cpp
│   ├── GestionnaireFichier.cpp
│   ├── InterfaceSFML.cpp       # SFML
│   ├── InterfaceQt.cpp         # Qt 6
│   ├── ModeConsole.cpp
│   ├── TestsUnitaires.cpp
│   ├── main.cpp
│   └── test_main.cpp
├── exemples/             # Fichiers de test
│   ├── exemple1_planeur.txt
│   ├── exemple2_clignotant.txt
│   ├── exemple3_complexe.txt
│   ├── exemple4_avec_obstacles.txt
│   ├── exemple5_grande_grille.txt
│   ├── exemple6_obstacles_vivants.txt
│   ├── grande_grille_500x500.txt
│   └── enorme_grille_1000x1000.txt
├── build/                # Fichiers de compilation
│   └── Release/
│       ├── jeu_de_la_vie.exe
│       ├── tests.exe
│       └── platforms/       # Plugins Qt
├── docs/                 # Documentation Doxygen
│   ├── index.html           # Redirection vers html/
│   └── html/                # Documentation générée
│       ├── index.html
│       ├── annotated.html   # Liste des classes
│       ├── files.html       # Liste des fichiers
│       └── ...
├── CMakeLists.txt
├── README.md
├── CONCEPTION.md         # Diagrammes UML (Mermaid)
├── Doxyfile              # Configuration Doxygen
├── generer_doc.ps1       # Script génération doc (Windows)
├── generer_doc.sh        # Script génération doc (Linux/macOS)
├── lancer_qt.bat         # Script de lancement Qt
└── lancer_sfml.bat       # Script de lancement SFML
```

---

## Documentation technique

### Générer la documentation Doxygen

#### Installation de Doxygen

**Windows (avec Chocolatey)** :
```powershell
choco install doxygen.install graphviz
```

**Ubuntu/Debian** :
```bash
sudo apt-get install doxygen graphviz
```

**macOS (avec Homebrew)** :
```bash
brew install doxygen graphviz
```

#### Générer la documentation

**Windows** :
```powershell
.\generer_doc.ps1
```

**Linux/macOS** :
```bash
chmod +x generer_doc.sh
./generer_doc.sh
```

**Manuellement** :
```bash
doxygen Doxyfile
```

La documentation sera générée dans le dossier `docs/html/`. Ouvrez `docs/html/index.html` dans votre navigateur.

#### Contenu de la documentation

- **Pages principales** : README et CONCEPTION intégrés
- **Diagrammes UML** : Classes, hiérarchies, graphes d'appels
- **Index des classes** : Toutes les classes documentées
- **Liste des fichiers** : Organisation du code source
- **Graphes de dépendances** : Relations entre classes
- **Documentation API** : Tous les paramètres @param, @return, @throws

#### Déployer sur GitHub Pages

```bash
git add docs/
git commit -m "Add Doxygen documentation"
git push
```

Puis dans GitHub : `Settings` → `Pages` → `Source: main` → `Folder: /docs`

Votre documentation sera disponible à : `https://votre-username.github.io/jeu-de-la-vie/`

---

### Diagrammes UML

Consultez **`CONCEPTION.md`** pour :
- Diagrammes de classes (Mermaid)
- Diagrammes de séquence
- Diagrammes d'activité
- Explication des patterns de conception
- Analyse de performance

### Performances

**Benchmark** (grille 1000×1000, 100 itérations) :
- Mode séquentiel : ~2.5s
- Mode parallèle (8 threads) : ~2.2s
- **Gain : 15%** grâce à l'optimisation sans mutex

**Optimisations implémentées** :
- Parallélisation sans mutex (division par lignes complètes)
- Move semantics (std::move) pour éviter copies
- Taille de cellule adaptative selon grille
- Réservation mémoire (vector::reserve)
- **Historique limité à 50 itérations** (économie mémoire)
- **Fichier de constantes centralisé** (maintenabilité)

### Comparaison des interfaces

| Fonctionnalité | Qt | SFML | Console |
|----------------|----|----|---------|
| **Boutons graphiques** | ✅ | ❌ | ❌ |
| **Sliders** | ✅ | ❌ | ❌ |
| **Statistiques visuelles** | ✅ | ⚠️ Console | ✅ |
| **Performance** | ⚪ Bonne | ✅ Excellente | ✅ |
| **Facilité d'usage** | ⚡⚡⚡ | ⚡⚡ | ⚡ |
| **Taille exe** | ~5 MB | ~500 KB | Minimal |
| **Idéal pour** | Démo/Présentation | Grandes grilles | Export batch |

---

## Pour le projet scolaire

### Points forts à présenter

- **Architecture** : Tous les principes SOLID appliqués
- **Patterns** : 6 patterns de conception utilisés
- **Extensibilité** : Facile d'ajouter nouveaux états/règles
- **Tests** : 100% de réussite (10/10)
- **Performance** : Optimisation multi-thread (+15%)
- **UX** : 2 interfaces modernes + mode console
- **Bonus** : TOUTES les extensions implémentées  

### Démarrage rapide pour démo

```powershell
# 1. Lancer interface Qt (impressionnant visuellement)
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple3_complexe.txt

# 2. Montrer performance SFML (grille 500x500)
.\build\Release\jeu_de_la_vie.exe .\exemples\grande_grille_500x500.txt -m sfml -p

# 3. Démontrer mode console avec export
.\build\Release\jeu_de_la_vie.exe .\exemples\exemple1_planeur.txt -m console -n 20

# 4. Exécuter les tests
.\build\Release\tests.exe
```

---

## Auteurs

- Ahmed GHEMARI

**Date** : Novembre 2025  
**Cours** : Programmation Orientée Objet (POO) - A2

---

## Licence

Projet académique - Tous droits réservés



