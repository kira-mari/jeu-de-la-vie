@echo off
REM Script de lancement rapide - Interface Qt
REM Usage: lancer_qt.bat [fichier_grille] [options]

REM Verifier qu'un fichier est fourni
if "%~1"=="" goto :aide

REM Verifier que le fichier existe
if not exist "%~1" (
    echo Erreur: Le fichier '%~1' n'existe pas
    echo.
    exit /b 1
)

REM Verifier que l'executable existe
if not exist "build\Release\jeu_de_la_vie.exe" (
    echo Erreur: L'executable n'existe pas
    echo Veuillez compiler le projet d'abord
    echo.
    exit /b 1
)

echo ========================================
echo   Jeu de la Vie - Interface Qt
echo ========================================
echo.
echo Lancement avec: %~1
if not "%~2"=="" echo Options: %~2 %~3 %~4 %~5 %~6
echo.

REM Lancer en mode Qt avec tous les arguments restants
build\Release\jeu_de_la_vie.exe %~1 -m qt %~2 %~3 %~4 %~5 %~6 %~7 %~8

exit /b 0

:aide
echo ========================================
echo   Jeu de la Vie - Interface Qt
echo ========================================
echo.
echo Erreur: Aucun fichier fourni
echo.
echo Usage: lancer_qt.bat [fichier_grille] [options]
echo.
echo Exemples:
echo   lancer_qt.bat exemples\exemple1_planeur.txt
echo   lancer_qt.bat exemples\exemple3_complexe.txt -n 200
echo   lancer_qt.bat exemples\grande_grille_500x500.txt -t -p
echo.
echo Options:
echo   -n NUM  Nombre maximum d'iterations (0 = illimite)
echo   -t      Mode torique (grille bouclee)
echo   -p      Parallelisation (multi-thread)
echo.
exit /b 1
