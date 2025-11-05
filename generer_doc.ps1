# Script de génération de la documentation Doxygen
# Jeu de la Vie - Projet POO A2

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "=== Génération de la documentation ===" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Vérifier si Doxygen est installé
$doxygenPath = Get-Command doxygen -ErrorAction SilentlyContinue

if (-not $doxygenPath) {
    Write-Host "❌ Erreur: Doxygen n'est pas installé !" -ForegroundColor Red
    Write-Host ""
    Write-Host "📥 Installation de Doxygen:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Option 1 - Chocolatey (recommandé):" -ForegroundColor Green
    Write-Host "  choco install doxygen.install graphviz" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2 - Téléchargement manuel:" -ForegroundColor Green
    Write-Host "  1. Télécharger depuis: https://www.doxygen.nl/download.html" -ForegroundColor White
    Write-Host "  2. Installer Doxygen" -ForegroundColor White
    Write-Host "  3. Installer Graphviz: https://graphviz.org/download/" -ForegroundColor White
    Write-Host "  4. Ajouter au PATH système" -ForegroundColor White
    Write-Host ""
    exit 1
}

Write-Host "✅ Doxygen trouvé: $($doxygenPath.Source)" -ForegroundColor Green

# Vérifier Graphviz (pour les diagrammes)
$dotPath = Get-Command dot -ErrorAction SilentlyContinue
if (-not $dotPath) {
    Write-Host "⚠️  Attention: Graphviz (dot) n'est pas installé" -ForegroundColor Yellow
    Write-Host "   Les diagrammes UML ne seront pas générés." -ForegroundColor Yellow
    Write-Host "   Installation: choco install graphviz" -ForegroundColor Yellow
    Write-Host ""
} else {
    Write-Host "✅ Graphviz trouvé: $($dotPath.Source)" -ForegroundColor Green
}

Write-Host ""
Write-Host "📚 Génération de la documentation..." -ForegroundColor Cyan

# Nettoyer l'ancienne documentation
if (Test-Path "docs") {
    Write-Host "🧹 Nettoyage de l'ancienne documentation..." -ForegroundColor Yellow
    Remove-Item -Path "docs" -Recurse -Force
}

# Générer la documentation
Write-Host "⚙️  Exécution de Doxygen..." -ForegroundColor Cyan
doxygen Doxyfile

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "=== Documentation générée avec succès ===" -ForegroundColor Green
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "📂 Emplacement: $(Get-Location)\docs\html\" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "🌐 Pour visualiser:" -ForegroundColor Yellow
    Write-Host "   Ouvrez: docs\html\index.html" -ForegroundColor White
    Write-Host ""
    
    # Proposer d'ouvrir la documentation
    $response = Read-Host "Voulez-vous ouvrir la documentation maintenant? (O/N)"
    if ($response -eq 'O' -or $response -eq 'o') {
        Start-Process "docs\html\index.html"
    }
    
    Write-Host ""
    Write-Host "📊 Statistiques de la documentation:" -ForegroundColor Cyan
    $htmlFiles = Get-ChildItem -Path "docs\html" -Filter "*.html" -Recurse
    Write-Host "   Fichiers HTML générés: $($htmlFiles.Count)" -ForegroundColor White
    
    if (Test-Path "docs\html\annotated.html") {
        Write-Host "   ✅ Index des classes généré" -ForegroundColor Green
    }
    if (Test-Path "docs\html\files.html") {
        Write-Host "   ✅ Liste des fichiers générée" -ForegroundColor Green
    }
    if (Test-Path "docs\html\hierarchy.html") {
        Write-Host "   ✅ Hiérarchie des classes générée" -ForegroundColor Green
    }
    
    Write-Host ""
    Write-Host "💡 Astuce: Déployez sur GitHub Pages avec:" -ForegroundColor Cyan
    Write-Host "   git add docs/" -ForegroundColor White
    Write-Host "   git commit -m 'Add documentation'" -ForegroundColor White
    Write-Host "   git push" -ForegroundColor White
    Write-Host "   Puis activez GitHub Pages dans Settings > Pages > Source: main/docs" -ForegroundColor White
    
} else {
    Write-Host ""
    Write-Host "❌ Erreur lors de la génération de la documentation" -ForegroundColor Red
    Write-Host "   Code de sortie: $LASTEXITCODE" -ForegroundColor Red
    Write-Host ""
    Write-Host "💡 Vérifiez:" -ForegroundColor Yellow
    Write-Host "   - Le fichier Doxyfile existe" -ForegroundColor White
    Write-Host "   - Les dossiers include/ et src/ existent" -ForegroundColor White
    Write-Host "   - Les permissions d'écriture dans le dossier" -ForegroundColor White
    exit 1
}

Write-Host ""
