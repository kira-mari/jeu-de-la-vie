cmake_minimum_required(VERSION 3.15)
project(JeuDeLaVie VERSION 1.0 LANGUAGES CXX)

# Configuration C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options de compilation
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Trouver SFML (version 2.5 ou supérieure)
find_package(SFML 2.5 COMPONENTS graphics window system)

# Si SFML 2.x n'est pas trouvé, essayer SFML 3.x
if(NOT SFML_FOUND)
    find_package(SFML 3 COMPONENTS Graphics Window System)
endif()

# Trouver Qt6
find_package(Qt6 COMPONENTS Core Widgets Gui)

# Si Qt6 n'est pas trouvé, essayer Qt5
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets Gui)
endif()

# Configuration de Qt
if(Qt6_FOUND OR Qt5_FOUND)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    set(QT_FOUND TRUE)
    add_compile_definitions(QT_AVAILABLE)  # Définir la macro pour le code C++
else()
    set(QT_FOUND FALSE)
    message(WARNING "Qt non trouvé - L'interface Qt ne sera pas disponible")
    message(STATUS "Pour installer Qt:")
    message(STATUS "  vcpkg install qt6-base:x64-windows")
endif()

# Si SFML n'est pas trouvé du tout, continuer sans
if(NOT SFML_FOUND)
    message(WARNING "SFML non trouvé - L'interface SFML ne sera pas disponible")
    message(STATUS "Pour installer SFML:")
    message(STATUS "  vcpkg install sfml:x64-windows")
    set(COMPILE_GUI_SFML FALSE)
else()
    set(COMPILE_GUI_SFML TRUE)
endif()

# Inclure les répertoires d'en-têtes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Fichiers sources communs (logique métier)
set(SOURCES_COMMUNS
    src/EtatCellule.cpp
    src/Cellule.cpp
    src/RegleJeu.cpp
    src/Grille.cpp
    src/JeuDeLaVie.cpp
    src/GestionnaireFichier.cpp
)

# Programme principal avec mode graphique
set(SOURCES_MAIN
    ${SOURCES_COMMUNS}
    src/ModeConsole.cpp
    src/main.cpp
)

# Ajouter les sources GUI selon ce qui est disponible
if(COMPILE_GUI_SFML)
    list(APPEND SOURCES_MAIN src/InterfaceSFML.cpp)
endif()

if(QT_FOUND)
    list(APPEND SOURCES_MAIN 
        src/InterfaceQt.cpp
        include/InterfaceQt.hpp
    )
endif()

# Programme de tests
set(SOURCES_TESTS
    ${SOURCES_COMMUNS}
    src/TestsUnitaires.cpp
    src/test_main.cpp
)

# Exécutable de tests (toujours compilé)
add_executable(tests ${SOURCES_TESTS})

# Support du multithreading
find_package(Threads REQUIRED)
target_link_libraries(tests Threads::Threads)

# Support de C++17 filesystem
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(tests stdc++fs)
endif()

# Exécutable principal (seulement si au moins une GUI est trouvée)
if(COMPILE_GUI_SFML OR QT_FOUND)
    add_executable(jeu_de_la_vie ${SOURCES_MAIN})
    
    # Lier SFML si disponible
    if(COMPILE_GUI_SFML)
        if(TARGET SFML::Graphics)
            # SFML 3.x utilise des cibles avec namespace
            target_link_libraries(jeu_de_la_vie SFML::Graphics SFML::Window SFML::System)
        else()
            # SFML 2.x utilise des cibles sans namespace
            target_link_libraries(jeu_de_la_vie sfml-graphics sfml-window sfml-system)
        endif()
        target_compile_definitions(jeu_de_la_vie PRIVATE HAVE_SFML)
    endif()
    
    # Lier Qt si disponible
    if(Qt6_FOUND)
        target_link_libraries(jeu_de_la_vie Qt6::Core Qt6::Widgets Qt6::Gui)
        target_compile_definitions(jeu_de_la_vie PRIVATE HAVE_QT)
    elseif(Qt5_FOUND)
        target_link_libraries(jeu_de_la_vie Qt5::Core Qt5::Widgets Qt5::Gui)
        target_compile_definitions(jeu_de_la_vie PRIVATE HAVE_QT)
    endif()
    
    target_link_libraries(jeu_de_la_vie Threads::Threads)
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        target_link_libraries(jeu_de_la_vie stdc++fs)
    endif()
    
    # Installation
    install(TARGETS jeu_de_la_vie tests RUNTIME DESTINATION bin)
else()
    message(WARNING "Aucune bibliothèque GUI trouvée - Compilation des tests uniquement")
    install(TARGETS tests RUNTIME DESTINATION bin)
endif()

# Installation
install(TARGETS jeu_de_la_vie tests
    RUNTIME DESTINATION bin
)

# Copier les fichiers d'exemple
file(GLOB EXEMPLES "${CMAKE_SOURCE_DIR}/exemples/*.txt")
install(FILES ${EXEMPLES} DESTINATION exemples)

# Support de C++17 filesystem
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(jeu_de_la_vie stdc++fs)
    target_link_libraries(tests stdc++fs)
endif()
