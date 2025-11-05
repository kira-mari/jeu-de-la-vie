# Script de compilation pour Windows
# Prérequis: CMake, Visual Studio ou MinGW, SFML

Write-Host "=== Compilation du Jeu de la Vie ===" -ForegroundColor Green
Write-Host ""

# Vérifier si CMake est installé
if (!(Get-Command cmake -ErrorAction SilentlyContinue)) {
    Write-Host "ERREUR: CMake n'est pas installé ou n'est pas dans le PATH" -ForegroundColor Red
    exit 1
}

# Créer le dossier build
if (!(Test-Path "build")) {
    Write-Host "Création du dossier build..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "build" | Out-Null
}

Set-Location "build"

# Configuration avec CMake
Write-Host "Configuration du projet avec CMake..." -ForegroundColor Yellow

# Si vcpkg est utilisé, décommenter et ajuster le chemin
# $vcpkgPath = "C:\path\to\vcpkg"
# cmake .. -DCMAKE_TOOLCHAIN_FILE="$vcpkgPath/scripts/buildsystems/vcpkg.cmake"

cmake ..

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERREUR lors de la configuration CMake" -ForegroundColor Red
    Set-Location ..
    exit 1
}

# Compilation
Write-Host ""
Write-Host "Compilation du projet..." -ForegroundColor Yellow
cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERREUR lors de la compilation" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

Write-Host ""
Write-Host "=== Compilation réussie! ===" -ForegroundColor Green
Write-Host ""
Write-Host "Exécutables créés dans le dossier build/Release/ (ou build/)" -ForegroundColor Cyan
Write-Host ""
Write-Host "Pour tester:" -ForegroundColor Yellow
Write-Host "  .\build\Release\jeu_de_la_vie.exe exemples\exemple1_planeur.txt" -ForegroundColor White
Write-Host "  .\build\Release\tests.exe" -ForegroundColor White
Write-Host ""
