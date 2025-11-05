#!/bin/bash
# Script de génération de la documentation Doxygen
# Jeu de la Vie - Projet POO A2

echo "========================================"
echo "=== Génération de la documentation ==="
echo "========================================"
echo ""

# Vérifier si Doxygen est installé
if ! command -v doxygen &> /dev/null; then
    echo "❌ Erreur: Doxygen n'est pas installé !"
    echo ""
    echo "📥 Installation de Doxygen:"
    echo ""
    echo "Ubuntu/Debian:"
    echo "  sudo apt-get install doxygen graphviz"
    echo ""
    echo "macOS (Homebrew):"
    echo "  brew install doxygen graphviz"
    echo ""
    echo "Fedora/RHEL:"
    echo "  sudo dnf install doxygen graphviz"
    echo ""
    exit 1
fi

echo "✅ Doxygen trouvé: $(which doxygen)"

# Vérifier Graphviz
if ! command -v dot &> /dev/null; then
    echo "⚠️  Attention: Graphviz (dot) n'est pas installé"
    echo "   Les diagrammes UML ne seront pas générés."
    echo ""
else
    echo "✅ Graphviz trouvé: $(which dot)"
fi

echo ""
echo "📚 Génération de la documentation..."

# Nettoyer l'ancienne documentation
if [ -d "docs" ]; then
    echo "🧹 Nettoyage de l'ancienne documentation..."
    rm -rf docs
fi

# Générer la documentation
echo "⚙️  Exécution de Doxygen..."
doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo ""
    echo "========================================"
    echo "=== Documentation générée avec succès ==="
    echo "========================================"
    echo ""
    echo "📂 Emplacement: $(pwd)/docs/html/"
    echo ""
    echo "🌐 Pour visualiser:"
    echo "   Ouvrez: docs/html/index.html"
    echo ""
    
    # Compter les fichiers générés
    html_count=$(find docs/html -name "*.html" | wc -l)
    echo "📊 Statistiques de la documentation:"
    echo "   Fichiers HTML générés: $html_count"
    
    if [ -f "docs/html/annotated.html" ]; then
        echo "   ✅ Index des classes généré"
    fi
    if [ -f "docs/html/files.html" ]; then
        echo "   ✅ Liste des fichiers générée"
    fi
    if [ -f "docs/html/hierarchy.html" ]; then
        echo "   ✅ Hiérarchie des classes générée"
    fi
    
    echo ""
    echo "💡 Pour ouvrir la documentation:"
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "   open docs/html/index.html"
    else
        echo "   xdg-open docs/html/index.html"
    fi
    
    echo ""
    echo "💡 Astuce: Déployez sur GitHub Pages avec:"
    echo "   git add docs/"
    echo "   git commit -m 'Add documentation'"
    echo "   git push"
    
else
    echo ""
    echo "❌ Erreur lors de la génération de la documentation"
    echo "   Code de sortie: $?"
    echo ""
    echo "💡 Vérifiez:"
    echo "   - Le fichier Doxyfile existe"
    echo "   - Les dossiers include/ et src/ existent"
    echo "   - Les permissions d'écriture dans le dossier"
    exit 1
fi

echo ""
