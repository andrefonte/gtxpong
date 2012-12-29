#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QWidget>

#include "Scene.h"

namespace Ui {
    class MainWindow;
}

class QXmlStreamAttributes;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onNewGameClicked();
//    void on1PlayerClicked();
    void on2PlayersClicked();
    void onForeverAloneClicked();

    void onSettingsClicked();
    void onDefaultClicked();
    void onAcceptClicked();

    void onAboutClicked();

    void onHelpClicked();

    void onPauseClicked();
    void onContinueClicked();
    void onRestartClicked();
    void onQuitClicked();

    void onBack();

    void onWin(Scene::Player winner);
    void onMainMenuClicked();

private:
    Ui::MainWindow *ui;

    Scene *_scene;

    QWidget *_visibleWidget;
    QWidget *_oldWidget;

    bool _saveSettings();
    bool _loadSettings();

    void _setVisibleWidget(QWidget *widget);
    void _hideAllWidgets();

    void _centerWidget(QWidget *widget);

    /**
      * Set settings from widgets to scene.
      */
    void _adjustSceneGameSettings();

    /**
      * Set settings from xml to widgets.
      */
    void _adjustWidgetSettings(const QXmlStreamAttributes &attributes);

    void _enableScene();

    void _makeConnections();

    static const char *SETTINGSFILENAME;
};

#endif // MAINWINDOW_H
