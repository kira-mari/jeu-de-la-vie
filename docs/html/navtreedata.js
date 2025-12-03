/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Jeu de la Vie de Conway", "index.html", [
    [ "🎮 Jeu de la Vie de Conway - Projet POO C++", "index.html", "index" ],
    [ "Document de Conception - Jeu de la Vie", "md_CONCEPTION.html", [
      [ "1. Diagramme de Classes (Mermaid)", "md_CONCEPTION.html#autotoc_md63", [
        [ "Hiérarchie des États de Cellules", "md_CONCEPTION.html#autotoc_md64", null ],
        [ "Hiérarchie des Règles du Jeu", "md_CONCEPTION.html#autotoc_md65", null ],
        [ "Architecture Principale", "md_CONCEPTION.html#autotoc_md66", null ],
        [ "Interfaces et Utilitaires", "md_CONCEPTION.html#autotoc_md67", null ]
      ] ],
      [ "2. Diagramme de Séquence (Mermaid)", "md_CONCEPTION.html#autotoc_md68", [
        [ "Exécution d'une itération", "md_CONCEPTION.html#autotoc_md69", null ],
        [ "Chargement d'un fichier", "md_CONCEPTION.html#autotoc_md70", null ],
        [ "Placement de motif interactif", "md_CONCEPTION.html#autotoc_md71", null ],
        [ "Séquence : Dessin souris (SFML)", "md_CONCEPTION.html#autotoc_md72", null ],
        [ "Séquence : Zoom molette (SFML/Qt)", "md_CONCEPTION.html#autotoc_md73", null ],
        [ "Séquence : Historique (retour en arrière)", "md_CONCEPTION.html#autotoc_md74", null ],
        [ "Interface Qt - Interaction utilisateur", "md_CONCEPTION.html#autotoc_md75", null ]
      ] ],
      [ "3. Diagramme d'Activité (Mermaid)", "md_CONCEPTION.html#autotoc_md76", [
        [ "Activité principale du programme", "md_CONCEPTION.html#autotoc_md77", null ],
        [ "Mise à jour de la grille avec parallélisation", "md_CONCEPTION.html#autotoc_md78", null ]
      ] ],
      [ "4. Patterns de Conception Utilisés", "md_CONCEPTION.html#autotoc_md79", [
        [ "Strategy Pattern", "md_CONCEPTION.html#autotoc_md80", null ],
        [ "State Pattern", "md_CONCEPTION.html#autotoc_md81", null ],
        [ "Template Method Pattern", "md_CONCEPTION.html#autotoc_md82", null ],
        [ "Dependency Injection", "md_CONCEPTION.html#autotoc_md83", null ],
        [ "Observer Pattern (implicite via événements Qt)", "md_CONCEPTION.html#autotoc_md84", null ],
        [ "MVC Pattern (Model-View-Controller)", "md_CONCEPTION.html#autotoc_md85", null ]
      ] ],
      [ "5. Justification des Choix Techniques", "md_CONCEPTION.html#autotoc_md86", [
        [ "Utilisation de unique_ptr et shared_ptr", "md_CONCEPTION.html#autotoc_md87", null ],
        [ "Polymorphisme", "md_CONCEPTION.html#autotoc_md88", null ],
        [ "Séparation GUI/Logique", "md_CONCEPTION.html#autotoc_md89", null ],
        [ "Multithreading optimisé", "md_CONCEPTION.html#autotoc_md90", null ],
        [ "Choix de Qt vs SFML", "md_CONCEPTION.html#autotoc_md91", null ]
      ] ],
      [ "6. Complexité et Performance", "md_CONCEPTION.html#autotoc_md92", [
        [ "Complexité temporelle", "md_CONCEPTION.html#autotoc_md93", null ],
        [ "Complexité spatiale", "md_CONCEPTION.html#autotoc_md94", null ],
        [ "Optimisations implémentées", "md_CONCEPTION.html#autotoc_md95", null ],
        [ "Benchmark (grille 1000×1000, 100 itérations)", "md_CONCEPTION.html#autotoc_md96", null ]
      ] ],
      [ "7. Tests Unitaires", "md_CONCEPTION.html#autotoc_md97", [
        [ "Couverture des tests", "md_CONCEPTION.html#autotoc_md98", null ],
        [ "Exécution des tests", "md_CONCEPTION.html#autotoc_md99", null ],
        [ "Sortie attendue", "md_CONCEPTION.html#autotoc_md100", null ]
      ] ]
    ] ],
    [ "Espaces de nommage", "namespaces.html", [
      [ "Liste des espaces de nommage", "namespaces.html", "namespaces_dup" ],
      [ "Membres de l'espace de nommage", "namespacemembers.html", [
        [ "Tout", "namespacemembers.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ],
        [ "Définitions de type", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Liste des classes", "annotated.html", "annotated_dup" ],
      [ "Index des classes", "classes.html", null ],
      [ "Hiérarchie des classes", "hierarchy.html", "hierarchy" ],
      [ "Membres de classe", "functions.html", [
        [ "Tout", "functions.html", "functions_dup" ],
        [ "Fonctions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Énumérations", "functions_enum.html", null ],
        [ "Fonctions associées", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Fichiers", "files.html", [
      [ "Liste des fichiers", "files.html", "files_dup" ],
      [ "Membres de fichier", "globals.html", [
        [ "Tout", "globals.html", null ],
        [ "Fonctions", "globals_func.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"Cellule_8cpp.html",
"classRegleTorique.html#a5f15a29a47c045ccd89a64c72de030e1"
];

var SYNCONMSG = 'cliquez pour désactiver la synchronisation du panel';
var SYNCOFFMSG = 'cliquez pour activer la synchronisation du panel';
var LISTOFALLMEMBERS = 'Liste de tous les membres';