#ifndef INTERFACE_QT_HPP
#define INTERFACE_QT_HPP

#include "JeuDeLaVie.hpp"
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPainter>
#include <QKeyEvent>

/**
 * @brief Interface graphique moderne avec Qt
 * 
 * Interface professionnelle avec widgets natifs pour le Jeu de la Vie.
 * Offre des contrôles riches et un design moderne.
 */
class InterfaceQt : public QMainWindow {
    Q_OBJECT

private:
    JeuDeLaVie& jeu;
    
    // Widget central et zone de dessin
    QWidget* centralWidget;
    QWidget* canvasWidget;
    
    // Contrôles
    QPushButton* btnPlayPause;
    QPushButton* btnStep;
    QPushButton* btnStepBack;
    QPushButton* btnReset;
    QSlider* sliderVitesse;
    QLabel* lblIteration;
    QLabel* lblVitesse;
    QLabel* lblCellulesVivantes;
    QCheckBox* chkTorique;
    QCheckBox* chkParallele;
    QComboBox* cmbMotifs;
    QPushButton* btnPlacerMotif;
    QSpinBox* spinIterationMax;
    QLabel* lblIterationMax;
    
    // État
    QTimer* timer;
    bool enPause;
    int delaiMs;
    int tailleCellule;
    int iterationMax;
    
    // Outils de dessin
    QComboBox* cmbDrawType; // Sélection du type de cellule à dessiner
    int drawMode; // 0=vivante,1=morte,2=obstacle mort,3=obstacle vivant
    
    // Couleurs
    QColor couleurVivante;
    QColor couleurMorte;
    QColor couleurObstacleMort;
    QColor couleurObstacleVivant;
    QColor couleurGrille;
    
public:
    /**
     * @brief Constructeur de l'interface Qt
     * @param jeuRef Référence vers le jeu
     */
    explicit InterfaceQt(JeuDeLaVie& jeuRef);
    
    /**
     * @brief Destructeur
     */
    ~InterfaceQt() override;
    
    /**
     * @brief Lance l'interface
     */
    void executer();
    
protected:
    /**
     * @brief Gestion des événements clavier
     */
    void keyPressEvent(QKeyEvent* event) override;
    
private slots:
    /**
     * @brief Gestion du bouton Play/Pause
     */
    void onPlayPause();
    
    /**
     * @brief Avance d'une itération
     */
    void onStep();
    
    /**
     * @brief Recule d'une itération
     */
    void onStepBack();
    
    /**
     * @brief Réinitialise la simulation
     */
    void onReset();
    
    /**
     * @brief Change la vitesse
     */
    void onVitesseChanged(int valeur);
    
    /**
     * @brief Toggle mode torique
     */
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        void onToriqueChanged(Qt::CheckState state);
    #else
        void onToriqueChanged(int state);
    #endif
    
    /**
     * @brief Toggle parallélisation
     */
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        void onParalleleChanged(Qt::CheckState state);
    #else
        void onParalleleChanged(int state);
    #endif
    
    /**
     * @brief Place un motif
     */
    void onPlacerMotif();
    
    /**
     * @brief Change le nombre d'itérations maximum
     */
    void onIterationMaxChanged(int valeur);
    
    /**
     * @brief Mise à jour automatique (timer)
     */
    void onTimerTick();
    
public:
    /**
     * @brief Dessine la grille (appelé par paintEvent de GrilleWidget)
     */
    void dessinerGrille(QPainter& painter);

    /**
     * @brief Méthode appelée par le widget de grille lors d'événements souris
     * @param x position x en pixels
     * @param y position y en pixels
     * @param buttons masques des boutons souris
     */
    void canvasMouseEvent(int x, int y, Qt::MouseButtons buttons);
    
private:
    /**
     * @brief Initialise les widgets
     */
    void initialiserWidgets();
    
    /**
     * @brief Initialise la mise en page
     */
    void initialiserLayout();
    
    /**
     * @brief Met à jour l'affichage
     */
    void mettreAJourAffichage();
    
    /**
     * @brief Calcule la taille des cellules
     */
    void calculerTailleCellule();
    
    /**
     * @brief Compte les cellules vivantes
     */
    int compterCellulesVivantes() const;
};

/**
 * @brief Widget personnalisé pour dessiner la grille
 */
class GrilleWidget : public QWidget {
private:
    InterfaceQt* parent;
    
public:
    explicit GrilleWidget(InterfaceQt* p, QWidget* parentWidget = nullptr);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    
    friend class InterfaceQt;
};

#endif // INTERFACE_QT_HPP
