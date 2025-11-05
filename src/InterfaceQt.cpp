#include "InterfaceQt.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QApplication>
#include <iostream>

// Détection de la version Qt pour compatibilité Qt5/Qt6
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #define QT_VERSION_6
#else
    #define QT_VERSION_5
#endif

InterfaceQt::InterfaceQt(JeuDeLaVie& jeuRef)
    : QMainWindow(),
      jeu(jeuRef),
      enPause(true),
      delaiMs(200),
      tailleCellule(10),
      iterationMax(0),  // 0 = illimité
      couleurVivante(0, 200, 0),
      couleurMorte(30, 30, 30),
      couleurObstacleMort(255, 0, 0),      // Rouge
      couleurObstacleVivant(255, 140, 0),  // Orange
      couleurGrille(50, 50, 50) {
    
    setWindowTitle("Jeu de la Vie - Interface Qt");
    
    // Calculer la taille optimale de la fenêtre selon la grille
    const Grille& grille = jeu.obtenirGrille();
    int nbLignes = grille.obtenirNbLignes();
    int nbColonnes = grille.obtenirNbColonnes();
    
    // Calculer la taille de cellule optimale
    calculerTailleCellule();
    
    // Taille de la zone de grille
    int largeurGrille = nbColonnes * tailleCellule;
    int hauteurGrille = nbLignes * tailleCellule;
    
    // Taille de la fenêtre = grille + panneau de contrôle (400px) + marges
    int largeurFenetre = largeurGrille + 420;
    int hauteurFenetre = std::max(hauteurGrille + 40, 700);
    
    // Limiter à la taille de l'écran
    resize(std::min(largeurFenetre, 1600), std::min(hauteurFenetre, 900));
    
    initialiserWidgets();
    initialiserLayout();
    
    // Timer pour l'animation
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &InterfaceQt::onTimerTick);
    
    mettreAJourAffichage();
}

InterfaceQt::~InterfaceQt() {
    if (timer) {
        timer->stop();
    }
}

void InterfaceQt::initialiserWidgets() {
    // Boutons de contrôle
    btnPlayPause = new QPushButton("▶ Démarrer");
    btnPlayPause->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
    );
    connect(btnPlayPause, &QPushButton::clicked, this, &InterfaceQt::onPlayPause);
    
    btnStep = new QPushButton("➡ Suivant");
    btnStep->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0b7dda;"
        "}"
    );
    connect(btnStep, &QPushButton::clicked, this, &InterfaceQt::onStep);
    
    btnStepBack = new QPushButton("⬅ Précédent");
    btnStepBack->setStyleSheet(
        "QPushButton {"
        "   background-color: #FF9800;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e68900;"
        "}"
    );
    connect(btnStepBack, &QPushButton::clicked, this, &InterfaceQt::onStepBack);
    
    btnReset = new QPushButton("🔄 Réinitialiser");
    btnReset->setStyleSheet(
        "QPushButton {"
        "   background-color: #f44336;"
        "   color: white;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #da190b;"
        "}"
    );
    connect(btnReset, &QPushButton::clicked, this, &InterfaceQt::onReset);
    
    // Slider vitesse
    sliderVitesse = new QSlider(Qt::Horizontal);
    sliderVitesse->setRange(1, 100);
    sliderVitesse->setValue(50);
    sliderVitesse->setStyleSheet(
        "QSlider::groove:horizontal {"
        "   height: 8px;"
        "   background: #ddd;"
        "   border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "   background: #4CAF50;"
        "   width: 18px;"
        "   margin: -5px 0;"
        "   border-radius: 9px;"
        "}"
    );
    connect(sliderVitesse, &QSlider::valueChanged, this, &InterfaceQt::onVitesseChanged);
    
    // Labels
    lblIteration = new QLabel("Itération: 0");
    lblIteration->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    
    lblVitesse = new QLabel("Vitesse: Moyenne");
    lblVitesse->setStyleSheet("font-size: 14px; color: #666;");
    
    lblCellulesVivantes = new QLabel("Cellules vivantes: 0");
    lblCellulesVivantes->setStyleSheet("font-size: 14px; color: #666;");
    
    // Checkboxes
    chkTorique = new QCheckBox("Mode torique (grille bouclée)");
    chkTorique->setStyleSheet("font-size: 13px;");
    #ifdef QT_VERSION_6
        connect(chkTorique, &QCheckBox::checkStateChanged, this, &InterfaceQt::onToriqueChanged);
    #else
        connect(chkTorique, &QCheckBox::stateChanged, this, &InterfaceQt::onToriqueChanged);
    #endif
    
    chkParallele = new QCheckBox("Parallélisation (multi-thread)");
    chkParallele->setStyleSheet("font-size: 13px;");
    #ifdef QT_VERSION_6
        connect(chkParallele, &QCheckBox::checkStateChanged, this, &InterfaceQt::onParalleleChanged);
    #else
        connect(chkParallele, &QCheckBox::stateChanged, this, &InterfaceQt::onParalleleChanged);
    #endif
    
    // Combo motifs
    cmbMotifs = new QComboBox();
    cmbMotifs->addItem("🚀 Planeur");
    cmbMotifs->addItem("⬛ Bloc");
    cmbMotifs->addItem("↔ Clignotant");
    cmbMotifs->addItem("🏠 Ruche");
    cmbMotifs->setStyleSheet(
        "QComboBox {"
        "   padding: 5px;"
        "   border: 2px solid #ddd;"
        "   border-radius: 4px;"
        "   font-size: 13px;"
        "}"
    );
    
    btnPlacerMotif = new QPushButton("Placer au centre");
    btnPlacerMotif->setStyleSheet(
        "QPushButton {"
        "   background-color: #9C27B0;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 15px;"
        "   font-size: 13px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #7B1FA2;"
        "}"
    );
    connect(btnPlacerMotif, &QPushButton::clicked, this, &InterfaceQt::onPlacerMotif);
    
    // Spinbox itération max
    spinIterationMax = new QSpinBox();
    spinIterationMax->setRange(0, 100000);
    spinIterationMax->setValue(0);  // 0 = illimité
    spinIterationMax->setSpecialValueText("Illimité");
    spinIterationMax->setSuffix(" itérations");
    spinIterationMax->setStyleSheet(
        "QSpinBox {"
        "   padding: 5px;"
        "   border: 2px solid #ddd;"
        "   border-radius: 4px;"
        "   font-size: 13px;"
        "}"
    );
    connect(spinIterationMax, QOverload<int>::of(&QSpinBox::valueChanged), this, &InterfaceQt::onIterationMaxChanged);
    
    lblIterationMax = new QLabel("Itération max: Illimité");
    lblIterationMax->setStyleSheet("font-size: 13px; color: #666;");
}

void InterfaceQt::initialiserLayout() {
    // Widget central
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    
    // Calculer la taille exacte pour la grille
    const Grille& grille = jeu.obtenirGrille();
    int largeurGrille = grille.obtenirNbColonnes() * tailleCellule;
    int hauteurGrille = grille.obtenirNbLignes() * tailleCellule;
    
    // Zone de dessin (gauche) - taille fixe pour correspondre exactement à la grille
    canvasWidget = new GrilleWidget(this);
    canvasWidget->setMinimumSize(largeurGrille, hauteurGrille);
    canvasWidget->setMaximumSize(largeurGrille, hauteurGrille);
    canvasWidget->setStyleSheet("background-color: black; border: 2px solid #333;");
    mainLayout->addWidget(canvasWidget, 0);  // 0 = pas d'étirement
    
    // Panneau de contrôle (droite)
    QVBoxLayout* controlLayout = new QVBoxLayout();
    controlLayout->setSpacing(15);
    
    // Groupe Informations
    QGroupBox* groupInfo = new QGroupBox("📊 Informations");
    groupInfo->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #4CAF50;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    QVBoxLayout* infoLayout = new QVBoxLayout(groupInfo);
    infoLayout->addWidget(lblIteration);
    infoLayout->addWidget(lblCellulesVivantes);
    controlLayout->addWidget(groupInfo);
    
    // Groupe Contrôles
    QGroupBox* groupControles = new QGroupBox("🎮 Contrôles");
    groupControles->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #2196F3;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    QVBoxLayout* controleLayout = new QVBoxLayout(groupControles);
    controleLayout->addWidget(btnPlayPause);
    
    QHBoxLayout* stepLayout = new QHBoxLayout();
    stepLayout->addWidget(btnStepBack);
    stepLayout->addWidget(btnStep);
    controleLayout->addLayout(stepLayout);
    
    controleLayout->addWidget(btnReset);
    controlLayout->addWidget(groupControles);
    
    // Groupe Vitesse
    QGroupBox* groupVitesse = new QGroupBox("⚡ Vitesse");
    groupVitesse->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #FF9800;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    QVBoxLayout* vitesseLayout = new QVBoxLayout(groupVitesse);
    vitesseLayout->addWidget(lblVitesse);
    vitesseLayout->addWidget(sliderVitesse);
    controlLayout->addWidget(groupVitesse);
    
    // Groupe Options
    QGroupBox* groupOptions = new QGroupBox("⚙ Options");
    groupOptions->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #607D8B;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    QVBoxLayout* optionsLayout = new QVBoxLayout(groupOptions);
    optionsLayout->addWidget(chkTorique);
    optionsLayout->addWidget(chkParallele);
    optionsLayout->addWidget(new QLabel(""));  // Séparateur
    optionsLayout->addWidget(new QLabel("Nombre d'itérations max:"));
    optionsLayout->addWidget(spinIterationMax);
    controlLayout->addWidget(groupOptions);
    
    // Groupe Motifs
    QGroupBox* groupMotifs = new QGroupBox("🎨 Motifs pré-programmés");
    groupMotifs->setStyleSheet(
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #9C27B0;"
        "   border-radius: 6px;"
        "   margin-top: 10px;"
        "   padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 10px;"
        "   padding: 0 5px;"
        "}"
    );
    QVBoxLayout* motifsLayout = new QVBoxLayout(groupMotifs);
    motifsLayout->addWidget(cmbMotifs);
    motifsLayout->addWidget(btnPlacerMotif);
    controlLayout->addWidget(groupMotifs);
    
    controlLayout->addStretch();
    
    mainLayout->addLayout(controlLayout, 1);
}

void InterfaceQt::onPlayPause() {
    enPause = !enPause;
    
    if (enPause) {
        timer->stop();
        btnPlayPause->setText("▶ Démarrer");
        btnPlayPause->setStyleSheet(
            "QPushButton {"
            "   background-color: #4CAF50;"
            "   color: white;"
            "   border: none;"
            "   padding: 10px 20px;"
            "   font-size: 16px;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #45a049;"
            "}"
        );
    } else {
        timer->start(delaiMs);
        btnPlayPause->setText("⏸ Pause");
        btnPlayPause->setStyleSheet(
            "QPushButton {"
            "   background-color: #FF9800;"
            "   color: white;"
            "   border: none;"
            "   padding: 10px 20px;"
            "   font-size: 16px;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #e68900;"
            "}"
        );
    }
}

void InterfaceQt::onStep() {
    if (jeu.executerIteration()) {
        mettreAJourAffichage();
    } else {
        std::cout << "Simulation terminée à l'itération " << jeu.obtenirIteration() << std::endl;
        enPause = true;
        timer->stop();
        btnPlayPause->setText("▶ Démarrer");
    }
}

void InterfaceQt::onStepBack() {
    if (jeu.revenirEnArriere()) {
        mettreAJourAffichage();
    } else {
        std::cout << "Impossible de revenir en arrière, vous êtes au début." << std::endl;
    }
}

void InterfaceQt::onReset() {
    // Arrêter la simulation si elle est en cours
    if (!enPause) {
        enPause = true;
        timer->stop();
        btnPlayPause->setText("▶ Démarrer");
    }
    
    // Revenir à l'itération 0 en utilisant l'historique
    while (jeu.revenirEnArriere()) {
        // Continue jusqu'à l'itération 0
    }
    
    // Mettre à jour l'affichage
    mettreAJourAffichage();
    
    // Message de confirmation dans la console
    std::cout << "Simulation réinitialisée à l'itération 0" << std::endl;
}

void InterfaceQt::onVitesseChanged(int valeur) {
    // Convertir 1-100 en délai (10ms - 1000ms)
    delaiMs = 1010 - (valeur * 10);
    
    QString vitesse;
    if (valeur < 25) vitesse = "Très lente";
    else if (valeur < 50) vitesse = "Lente";
    else if (valeur < 75) vitesse = "Moyenne";
    else vitesse = "Rapide";
    
    lblVitesse->setText(QString("Vitesse: %1 (%2 ms)").arg(vitesse).arg(delaiMs));
    
    if (!enPause) {
        timer->setInterval(delaiMs);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void InterfaceQt::onToriqueChanged(Qt::CheckState state) {
    jeu.definirModeTorique(state == Qt::Checked);
    std::cout << "Mode torique: " << (state == Qt::Checked ? "activé" : "désactivé") << std::endl;
}

void InterfaceQt::onParalleleChanged(Qt::CheckState state) {
    jeu.definirModeParallele(state == Qt::Checked);
    std::cout << "Mode parallèle: " << (state == Qt::Checked ? "activé" : "désactivé") << std::endl;
}
#else
void InterfaceQt::onToriqueChanged(int state) {
    jeu.definirModeTorique(state == Qt::Checked);
    std::cout << "Mode torique: " << (state == Qt::Checked ? "activé" : "désactivé") << std::endl;
}

void InterfaceQt::onParalleleChanged(int state) {
    jeu.definirModeParallele(state == Qt::Checked);
    std::cout << "Mode parallèle: " << (state == Qt::Checked ? "activé" : "désactivé") << std::endl;
}
#endif

void InterfaceQt::onIterationMaxChanged(int valeur) {
    iterationMax = valeur;
    jeu.definirIterationMax(valeur);
    
    if (valeur == 0) {
        lblIterationMax->setText("Itération max: Illimité");
    } else {
        lblIterationMax->setText(QString("Itération max: %1").arg(valeur));
    }
}

void InterfaceQt::onPlacerMotif() {
    QString motifSelectionne = cmbMotifs->currentText();
    std::string motif;
    
    if (motifSelectionne.contains("Planeur")) motif = "planeur";
    else if (motifSelectionne.contains("Bloc")) motif = "bloc";
    else if (motifSelectionne.contains("Clignotant")) motif = "clignotant";
    else if (motifSelectionne.contains("Ruche")) motif = "ruche";
    
    const Grille& grille = jeu.obtenirGrille();
    int ligne = grille.obtenirNbLignes() / 2;
    int colonne = grille.obtenirNbColonnes() / 2;
    
    if (jeu.placerMotif(motif, ligne, colonne)) {
        std::cout << "Motif '" << motif << "' placé au centre de la grille" << std::endl;
        mettreAJourAffichage();
    } else {
        std::cout << "Erreur: Impossible de placer le motif" << std::endl;
    }
}

void InterfaceQt::onTimerTick() {
    if (!enPause) {
        if (!jeu.executerIteration()) {
            // Le jeu est terminé (stable ou max atteint)
            enPause = true;
            timer->stop();
            btnPlayPause->setText("▶ Démarrer");
            std::cout << "Simulation terminée à l'itération " << jeu.obtenirIteration() << std::endl;
            return;
        }
        mettreAJourAffichage();
    }
}

void InterfaceQt::mettreAJourAffichage() {
    lblIteration->setText(QString("Itération: %1").arg(jeu.obtenirIteration()));
    lblCellulesVivantes->setText(QString("Cellules vivantes: %1").arg(compterCellulesVivantes()));
    canvasWidget->update();
}

void InterfaceQt::calculerTailleCellule() {
    const Grille& grille = jeu.obtenirGrille();
    int nbLignes = grille.obtenirNbLignes();
    int nbColonnes = grille.obtenirNbColonnes();
    
    // Taille de cellule cible pour un affichage grand et visible
    tailleCellule = 80;  // Par défaut : très grandes cellules
    
    // Ajuster si la grille est grande
    if (nbLignes > 20 || nbColonnes > 20) {
        tailleCellule = 30;
    }
    if (nbLignes > 40 || nbColonnes > 40) {
        tailleCellule = 20;
    }
    if (nbLignes > 60 || nbColonnes > 60) {
        tailleCellule = 15;
    }
    if (nbLignes > 100 || nbColonnes > 100) {
        tailleCellule = 10;
    }
    if (nbLignes > 200 || nbColonnes > 200) {
        tailleCellule = 5;
    }
    
    // Assurer une taille minimale
    if (tailleCellule < 2) tailleCellule = 2;
}

int InterfaceQt::compterCellulesVivantes() const {
    const Grille& grille = jeu.obtenirGrille();
    int count = 0;
    
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            if (grille.obtenirCellule(i, j).estVivante()) {
                count++;
            }
        }
    }
    
    return count;
}

void InterfaceQt::dessinerGrille(QPainter& painter) {
    calculerTailleCellule();
    
    const Grille& grille = jeu.obtenirGrille();
    
    for (int i = 0; i < grille.obtenirNbLignes(); ++i) {
        for (int j = 0; j < grille.obtenirNbColonnes(); ++j) {
            const Cellule& cellule = grille.obtenirCellule(i, j);
            
            QColor couleur;
            if (cellule.estObstacle()) {
                if (cellule.estVivante()) {
                    couleur = couleurObstacleVivant;  // Orange
                } else {
                    couleur = couleurObstacleMort;     // Rouge
                }
            } else if (cellule.estVivante()) {
                couleur = couleurVivante;
            } else {
                couleur = couleurMorte;
            }
            
            painter.fillRect(
                j * tailleCellule,
                i * tailleCellule,
                tailleCellule - 1,
                tailleCellule - 1,
                couleur
            );
        }
    }
}

void InterfaceQt::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Space:
            onPlayPause();
            break;
        case Qt::Key_Right:
            onStep();
            break;
        case Qt::Key_Left:
            onStepBack();
            break;
        case Qt::Key_Up:
            sliderVitesse->setValue(std::min(100, sliderVitesse->value() + 10));
            break;
        case Qt::Key_Down:
            sliderVitesse->setValue(std::max(1, sliderVitesse->value() - 10));
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void InterfaceQt::executer() {
    show();
}

// GrilleWidget implementation
GrilleWidget::GrilleWidget(InterfaceQt* p, QWidget* parentWidget)
    : QWidget(parentWidget), parent(p) {
}

void GrilleWidget::paintEvent(QPaintEvent* /*event*/) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    parent->dessinerGrille(painter);
}
