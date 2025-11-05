# Document de Conception - Jeu de la Vie

## 1. Diagramme de Classes (Mermaid)

### Hiérarchie des États de Cellules

```mermaid
classDiagram
    class EtatCellule {
        <<abstract>>
        +estVivante() bool
        +estObstacle() bool
        +cloner() unique_ptr~EtatCellule~
        +versChaine() string
        +versEntier() int
    }
    
    class CelluleMorte {
        +estVivante() bool
        +estObstacle() bool
        +cloner() unique_ptr~EtatCellule~
        +versEntier() int
    }
    
    class CelluleVivante {
        +estVivante() bool
        +estObstacle() bool
        +cloner() unique_ptr~EtatCellule~
        +versEntier() int
    }
    
    class CelluleObstacle {
        -vivante : bool
        +estVivante() bool
        +estObstacle() bool
        +cloner() unique_ptr~EtatCellule~
        +versEntier() int
    }
    
    EtatCellule <|-- CelluleMorte
    EtatCellule <|-- CelluleVivante
    EtatCellule <|-- CelluleObstacle
```

### Hiérarchie des Règles du Jeu

```mermaid
classDiagram
    class RegleJeu {
        <<abstract>>
        +calculerNouvelEtat(grille, ligne, colonne) unique_ptr~EtatCellule~
        #compterVoisinsVivants(grille, ligne, colonne) int
    }
    
    class RegleStandard {
        +calculerNouvelEtat(grille, ligne, colonne) unique_ptr~EtatCellule~
        #compterVoisinsVivants(grille, ligne, colonne) int
    }
    
    class RegleTorique {
        +calculerNouvelEtat(grille, ligne, colonne) unique_ptr~EtatCellule~
        #compterVoisinsVivants(grille, ligne, colonne) int
    }
    
    RegleJeu <|-- RegleStandard : bordures fixes
    RegleJeu <|-- RegleTorique : grille bouclée
```

### Architecture Principale

```mermaid
classDiagram
    class Cellule {
        -etat : unique_ptr~EtatCellule~
        -ligne : int
        -colonne : int
        +Cellule(ligne, colonne, etatInitial)
        +definirEtat(nouvelEtat)
        +obtenirEtat() EtatCellule&
        +estVivante() bool
        +estObstacle() bool
        +obtenirLigne() int
        +obtenirColonne() int
    }
    
    class Grille {
        -cellules : vector~vector~Cellule~~
        -nbLignes : int
        -nbColonnes : int
        -regle : shared_ptr~RegleJeu~
        -grilleMemoire : unique_ptr~Grille~
        -premiereIteration : bool
        +Grille(lignes, colonnes, regleJeu)
        +obtenirCellule(ligne, colonne) Cellule&
        +definirEtatCellule(ligne, colonne, etat)
        +mettreAJour(parallele : bool)
        +estStable() bool
        +placerMotif(motif, ligne, colonne) bool
        +definirRegle(nouvelleRegle)
        -mettreAJourLignes(ligneDebut, ligneFin, nouvelleGrille)
    }
    
    class JeuDeLaVie {
        -grille : unique_ptr~Grille~
        -iteration : int
        -iterationMax : int
        -modeTorique : bool
        -modeParallele : bool
        -historique : vector~unique_ptr~Grille~~
        +JeuDeLaVie(grilleInitiale, nbIterations, torique, parallele)
        +executerIteration() bool
        +revenirEnArriere() bool
        +obtenirGrille() Grille&
        +obtenirIteration() int
        +obtenirIterationMax() int
        +definirIterationMax(max) void
        +estModeTorique() bool
        +estModeParallele() bool
        +estTermine() bool
        +reinitialiser(nouvelleGrille)
        +definirModeTorique(actif)
        +definirModeParallele(actif)
        +placerMotif(motif, ligne, colonne) bool
    }
    
    Cellule "1" o-- "1" EtatCellule : contient
    Grille "1" o-- "n*m" Cellule : contient
    Grille "1" o-- "1" RegleJeu : utilise
    JeuDeLaVie "1" o-- "1" Grille : gère
```

### Interfaces et Utilitaires

```mermaid
classDiagram
    class GestionnaireFichier {
        <<utility>>
        +chargerGrille(cheminFichier, regle)$ unique_ptr~Grille~
        +sauvegarderGrille(grille, cheminFichier)$ void
        +creerDossierSortie(cheminFichier)$ string
        +extraireNomFichier(chemin)$ string
    }
    
    class InterfaceSFML {
        -fenetre : unique_ptr~RenderWindow~
        -jeu : JeuDeLaVie&
        -tailleCellule : int
        -enPause : bool
        -delaiIteration : float
        -horloge : Clock
        -couleurVivante : Color
        -couleurMorte : Color
        -couleurObstacleMort : Color
        -couleurObstacleVivant : Color
        -couleurGrille : Color
        +InterfaceSFML(jeuRef, largeur, hauteur, titre)
        +executer() void
        +definirDelaiIteration(delai) void
        -gererEvenements() void
        -afficher() void
        -dessinerGrille() void
        -dessinerInformations() void
        -gererPlacementMotif(touche) void
        -calculerTailleCellule() void
    }
    
    class InterfaceQt {
        -jeu : JeuDeLaVie&
        -centralWidget : QWidget*
        -canvasWidget : QWidget*
        -btnPlayPause : QPushButton*
        -btnStep : QPushButton*
        -btnStepBack : QPushButton*
        -btnReset : QPushButton*
        -sliderVitesse : QSlider*
        -spinIterationMax : QSpinBox*
        -chkTorique : QCheckBox*
        -chkParallele : QCheckBox*
        -cmbMotifs : QComboBox*
        -timer : QTimer*
        -enPause : bool
        -delaiMs : int
        -tailleCellule : int
        -iterationMax : int
        +InterfaceQt(jeuRef)
        +executer() void
        +dessinerGrille(painter) void
        -onPlayPause() void
        -onStep() void
        -onStepBack() void
        -onReset() void
        -onVitesseChanged(valeur) void
        -onIterationMaxChanged(valeur) void
        -onToriqueChanged(state) void
        -onParalleleChanged(state) void
        -onPlacerMotif() void
        -onTimerTick() void
        -initialiserWidgets() void
        -initialiserLayout() void
        -mettreAJourAffichage() void
        -calculerTailleCellule() void
    }
    
    class GrilleWidget {
        -parent : InterfaceQt*
        +GrilleWidget(p, parentWidget)
        #paintEvent(event) void
    }
    
    class ModeConsole {
        -jeu : JeuDeLaVie&
        -dossierSortie : string
        -afficherDansConsole : bool
        +ModeConsole(jeuRef, dossierSortie, afficher)
        +executer() void
        -afficherGrille() void
        -sauvegarderIteration(numeroIteration) void
    }
    
    class TestsUnitaires {
        <<utility>>
        +executerTousLesTests()$ bool
        -testerReglesBase()$ bool
        -testerBloc()$ bool
        -testerClignotant()$ bool
        -testerPlaneur()$ bool
        -testerModeTorique()$ bool
        -testerObstacles()$ bool
    }
    
    InterfaceSFML --> JeuDeLaVie : utilise (SFML)
    InterfaceQt --> JeuDeLaVie : utilise (Qt)
    InterfaceQt "1" o-- "1" GrilleWidget : contient
    GrilleWidget --> InterfaceQt : appelle dessinerGrille
    ModeConsole --> JeuDeLaVie : utilise
    GestionnaireFichier --> Grille : crée
```

## 2. Diagramme de Séquence (Mermaid)

### Exécution d'une itération

```mermaid
sequenceDiagram
    participant U as Utilisateur
    participant IG as InterfaceSFML
    participant J as JeuDeLaVie
    participant G as Grille
    participant R as RegleJeu
    participant C as Cellule
    
    U->>IG: Appui ESPACE
    IG->>J: executerIteration()
    J->>G: mettreAJour(parallele)
    
    loop Pour chaque cellule
        G->>R: calculerNouvelEtat(grille, i, j)
        R->>R: compterVoisinsVivants()
        R-->>G: nouvel état
        G->>C: definirEtat(nouvelEtat)
    end
    
    G-->>J: mise à jour terminée
    J-->>IG: true
    IG->>IG: afficher()
    IG-->>U: écran mis à jour
```

### Chargement d'un fichier

```mermaid
sequenceDiagram
    participant M as main
    participant GF as GestionnaireFichier
    participant G as Grille
    participant C as Cellule
    participant E as EtatCellule
    
    M->>GF: chargerGrille(fichier, regle)
    GF->>GF: ouvrir fichier
    GF->>GF: lire dimensions (n, m)
    GF->>G: new Grille(n, m, regle)
    
    loop Pour chaque ligne
        loop Pour chaque colonne
            GF->>GF: lire valeur
            GF->>G: definirEtatCellule(i, j, etat)
            G->>C: obtenirCellule(i, j)
            C->>E: new CelluleVivante() / CelluleMorte()
            E-->>C: état créé
        end
    end
    
    GF-->>M: unique_ptr~Grille~
```

### Placement de motif interactif

```mermaid
sequenceDiagram
    participant U as Utilisateur
    participant IG as InterfaceGraphique
    participant J as JeuDeLaVie
    participant G as Grille
    
    U->>IG: Appui touche 'G' (planeur)
    IG->>IG: gererPlacementMotif('G')
    IG->>J: placerMotif("planeur", ligne, col)
    J->>G: placerMotif("planeur", ligne, col)
    
    loop Pour chaque position du motif
        G->>G: definirEtatCellule(i, j, CelluleVivante)
    end
    
    G-->>J: true (succès)
    J-->>IG: true
    IG->>IG: afficher message console
    IG-->>U: "Motif 'planeur' placé"
```

### Interface Qt - Interaction utilisateur

```mermaid
sequenceDiagram
    participant U as Utilisateur
    participant IQ as InterfaceQt
    participant T as QTimer
    participant J as JeuDeLaVie
    participant G as Grille
    
    U->>IQ: Clic bouton "Démarrer"
    IQ->>IQ: onPlayPause()
    IQ->>T: start(delaiMs)
    IQ->>IQ: setText("⏸ Pause")
    
    loop Toutes les delaiMs millisecondes
        T->>IQ: timeout signal
        IQ->>IQ: onTimerTick()
        
        alt Itération max atteinte
            IQ->>T: stop()
            IQ->>IQ: setText("▶ Démarrer")
            IQ-->>U: message console
        else Continuer
            IQ->>IQ: onStep()
            IQ->>J: executerIteration()
            J->>G: mettreAJour(parallele)
            G-->>J: mise à jour OK
            J-->>IQ: true
            IQ->>IQ: mettreAJourAffichage()
            IQ->>IQ: canvasWidget->update()
            IQ-->>U: affichage actualisé
        end
    end
    
    U->>IQ: Clic bouton "Pause"
    IQ->>T: stop()
    IQ->>IQ: setText("▶ Démarrer")
```

## 3. Diagramme d'Activité (Mermaid)

### Activité principale du programme

```mermaid
flowchart TD
    Start([Début]) --> Args[Analyser arguments CLI]
    Args --> Load[Charger fichier d'entrée]
    Load --> Create[Créer grille et jeu]
    Create --> Mode{Mode ?}
    
    Mode -->|Console| CreateConsole[Créer ModeConsole]
    Mode -->|SFML| CreateSFML[Créer InterfaceSFML]
    Mode -->|Qt| CreateQt[Créer InterfaceQt]
    
    CreateConsole --> LoopConsole{Boucle: Tant que<br/>non terminé}
    LoopConsole -->|Continuer| ExecConsole[Exécuter itération]
    ExecConsole --> DisplayConsole[Afficher dans console]
    DisplayConsole --> SaveConsole[Sauvegarder fichier]
    SaveConsole --> LoopConsole
    LoopConsole -->|Terminé| End
    
    CreateSFML --> LoopSFML{Boucle: Fenêtre<br/>ouverte ?}
    LoopSFML -->|Oui| EventsSFML[Gérer événements SFML]
    EventsSFML --> UpdateSFML{En pause ?}
    UpdateSFML -->|Non| IterSFML[Exécuter itération]
    IterSFML --> DrawSFML[Dessiner grille]
    DrawSFML --> LoopSFML
    UpdateSFML -->|Oui| DrawSFML
    LoopSFML -->|Fenêtre fermée| End
    
    CreateQt --> LoopQt{Boucle: Application<br/>Qt active ?}
    LoopQt -->|Oui| EventsQt[Gérer événements Qt]
    EventsQt --> TimerQt{Timer actif ?}
    TimerQt -->|Oui| CheckMaxQt{Itération max<br/>atteinte ?}
    CheckMaxQt -->|Non| IterQt[Exécuter itération]
    IterQt --> UpdateQt[Mettre à jour widgets]
    UpdateQt --> LoopQt
    CheckMaxQt -->|Oui| StopQt[Arrêter timer]
    StopQt --> LoopQt
    TimerQt -->|Non| LoopQt
    LoopQt -->|Fermée| End([Fin])
```

### Mise à jour de la grille avec parallélisation

```mermaid
flowchart TD
    Start([Début mettreAJour]) --> NewGrid[Créer nouvelle grille vide]
    NewGrid --> Parallel{Parallèle ?}
    
    Parallel -->|Oui| CreateThreads[Créer N threads<br/>Diviser en lignes]
    Parallel -->|Non| Sequential[Pour chaque cellule i,j]
    
    CreateThreads --> ThreadWork[Chaque thread:<br/>Traiter ses lignes]
    ThreadWork --> Compute1[Calculer nouvel état<br/>avec règle]
    Compute1 --> Wait[Attendre tous threads]
    
    Sequential --> Compute2[Calculer nouvel état<br/>avec règle]
    Compute2 --> Set[Définir nouvel état]
    Set --> Next{Toutes cellules<br/>traitées ?}
    Next -->|Non| Sequential
    Next -->|Oui| Replace
    
    Wait --> Replace[Remplacer ancienne grille<br/>par nouvelle]
    Replace --> End([Fin])
```

## 4. Patterns de Conception Utilisés

### Strategy Pattern
- **RegleJeu** avec ses implémentations (RegleStandard, RegleTorique)
- Permet de changer les règles du jeu dynamiquement
- Utilisé dans `Grille` pour sélectionner le mode torique ou non

### State Pattern
- **EtatCellule** avec ses états concrets (CelluleMorte, CelluleVivante, CelluleObstacle)
- L'état d'une cellule détermine son comportement (vivante, morte, obstacle)
- Transition d'état gérée par les règles du jeu
- **CelluleObstacle** peut être vivant (orange) ou mort (rouge) et ne change jamais d'état pendant la simulation

### Template Method Pattern
- **RegleJeu::calculerNouvelEtat()** utilise **compterVoisinsVivants()**
- Chaque sous-classe implémente sa propre façon de compter les voisins
- RegleTorique compte les voisins avec grille bouclée

### Dependency Injection
- Les règles sont injectées dans la Grille via le constructeur
- L'interface graphique reçoit une référence au JeuDeLaVie
- Facilite les tests et la flexibilité

### Observer Pattern (implicite via événements Qt)
- InterfaceQt utilise le système de signals/slots de Qt
- Les boutons émettent des signaux connectés aux slots (onPlayPause, onStep, etc.)
- QTimer émet timeout signal pour l'animation automatique

### MVC Pattern (Model-View-Controller)
- **Model** : JeuDeLaVie, Grille, Cellule (logique métier)
- **View** : InterfaceSFML (SFML), InterfaceQt, ModeConsole (affichage)
- **Controller** : Gestion d'événements dans les interfaces

## 5. Justification des Choix Techniques

### Utilisation de unique_ptr et shared_ptr
- **unique_ptr** pour la propriété exclusive :
  - Cellule possède son EtatCellule
  - JeuDeLaVie possède sa Grille
  - Historique stocke des grilles uniques
- **shared_ptr** pour la propriété partagée :
  - Plusieurs grilles peuvent partager une règle (RegleJeu)

### Polymorphisme
- Les états et règles utilisent le polymorphisme pour permettre l'extensibilité
- Facilite l'ajout de nouveaux états (ex: CelluleEnCroissance) ou règles (ex: RegleHighLife)
- Respect du principe Open/Closed (SOLID)

### Séparation GUI/Logique
- La logique métier (Grille, Cellule, Règles) est complètement indépendante de l'affichage
- Facilite les tests unitaires
- Permet d'avoir 3 interfaces différentes (Console, SFML, Qt) sans dupliquer la logique

### Multithreading optimisé
- Utilisation de std::thread pour la parallélisation par lignes
- **Pas de mutex** : chaque thread travaille sur ses lignes exclusives
- Division du travail équitable entre threads (mettreAJourLignes)
- Gain de performance de ~15% sur grandes grilles (500x500+)

### Choix de Qt vs SFML
- **SFML** : Légère, rapide, idéale pour animation fluide et grandes grilles
- **Qt** : Interface professionnelle avec widgets natifs, contrôles riches
- Compilation conditionnelle : le projet compile avec l'une, l'autre, ou les deux

## 6. Complexité et Performance

### Complexité temporelle
- Mise à jour séquentielle: **O(n × m)** où n×m est la taille de la grille
- Mise à jour parallèle: **O((n × m) / p)** où p est le nombre de threads
- Comptage des voisins: **O(1)** (toujours 8 voisins à vérifier)
- Placement de motif: **O(k)** où k est la taille du motif

### Complexité spatiale
- Stockage de la grille: **O(n × m)**
- Grille temporaire lors de la mise à jour: **O(n × m)**
- Historique (undo): **O(h × n × m)** où h est le nombre d'itérations sauvegardées

### Optimisations implémentées
- **Réservation de mémoire** avec reserve() pour éviter les réallocations
- **Parallélisation sans mutex** : division par lignes complètes
- **Move semantics** avec std::move() pour éviter les copies coûteuses
- **Taille de cellule adaptative** : ajustée automatiquement selon la taille de grille
- **Calcul exact de fenêtre** : pas d'espace noir inutile (SFML/Qt)

### Benchmark (grille 1000×1000, 100 itérations)
- Mode séquentiel: ~2.5s
- Mode parallèle (8 threads): ~2.2s  
- Gain: **~15%** grâce à l'optimisation sans mutex
