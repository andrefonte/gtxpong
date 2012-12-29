#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore/QFile>
#include <QtWidgets/QShortcut>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

const char *MainWindow::SETTINGSFILENAME = "settings.gtxp";

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize
    _visibleWidget = 0;
    _oldWidget = 0;

    _scene = new Scene(0, 0, 550, 300, this);

    // Load initial settings
    this->_loadSettings();
    this->_adjustSceneGameSettings();

    // Adjust size of al widgets
    this->_hideAllWidgets();

    ui->graphicsView->setGeometry(0, 0, 0, 0);
    ui->graphicsView->setScene(_scene);
    ui->graphicsView->adjustSize();

    this->adjustSize();

    this->setMaximumSize(this->width(), this->height());
    this->setMinimumSize(this->width(), this->height());

    // Centers all menus
    this->_centerWidget(ui->mainMenu);
    this->_centerWidget(ui->gameTypeMenu);
    this->_centerWidget(ui->pausedMenu);
    this->_centerWidget(ui->aboutWidget);
    this->_centerWidget(ui->settingsWidget);
    this->_centerWidget(ui->winWidget);
    this->_centerWidget(ui->helpWidget);

    // Principal menu is shown
    this->_setVisibleWidget(ui->mainMenu);

    // Establish all connections
    this->_makeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewGameClicked()
{
    this->_setVisibleWidget(ui->gameTypeMenu);
}

void MainWindow::on2PlayersClicked()
{
    this->_enableScene();
    _scene->newGame(Scene::VS_HUMAN);
}

void MainWindow::onForeverAloneClicked()
{
    this->_enableScene();
    _scene->newGame(Scene::FOREVER_ALONE);
}

void MainWindow::onSettingsClicked()
{
    this->_loadSettings();
    this->_setVisibleWidget(ui->settingsWidget);
}

void MainWindow::onDefaultClicked()
{
    ui->rc1ComboBox->setCurrentIndex(0);
    ui->rc2ComboBox->setCurrentIndex(3);
    ui->bcComboBox->setCurrentIndex(1);
    ui->rcvSlider->setValue(10);
    ui->bvSlider->setValue(2);
    ui->ptwSpin->setValue(5);
}

void MainWindow::onAcceptClicked()
{
    this->_adjustSceneGameSettings();
    this->_saveSettings();
    this->onBack();
}

void MainWindow::onAboutClicked()
{
    this->_setVisibleWidget(ui->aboutWidget);
}

void MainWindow::onHelpClicked()
{
    this->_setVisibleWidget(ui->helpWidget);
}

void MainWindow::onPauseClicked()
{
    if (_scene->hasGame() == true) {
        if (_scene->isPaused() == false) {
            this->_setVisibleWidget(ui->pausedMenu);
            _scene->pauseGame();
        }
        else {
            this->onContinueClicked();
        }
    }
}

void MainWindow::onContinueClicked()
{
    this->_enableScene();
    _scene->continueGame();
}

void MainWindow::onRestartClicked()
{
    this->_enableScene();
    _scene->restartGame();
}

void MainWindow::onQuitClicked()
{
    _scene->quitGame();
    this->_setVisibleWidget(ui->mainMenu);
}

void MainWindow::onBack()
{
    this->_setVisibleWidget(_oldWidget);
}

void MainWindow::onWin(Scene::Player winner)
{
    QString msg;

    if (_scene->runningGameType() == Scene::FOREVER_ALONE) {
        msg = trUtf8("¡ ¡ ¡ <b>YOU</b> LOSE ! ! !");
    }
    else {
        if (winner == Scene::Player1) {
            msg = trUtf8("¡ ¡ ¡ <b>PLAYER 1</b> WINS ! ! !");
        }
        else {
            msg = trUtf8("¡ ¡ ¡ <b>PLAYER 2</b> WINS ! ! !");
        }
    }

    ui->winLabel->setText(msg);

    ui->winWidget->adjustSize();
    this->_centerWidget(ui->winWidget);

    this->_setVisibleWidget(ui->winWidget);
}

void MainWindow::onMainMenuClicked()
{
    _scene->quitGame();

    this->_setVisibleWidget(ui->mainMenu);
}

bool MainWindow::_saveSettings()
{
    QFile file(SETTINGSFILENAME);

    if (file.open(QIODevice::WriteOnly) == false) {
        return false;
    }

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("settings");
    stream.writeAttribute("RC1", QString().setNum(ui->rc1ComboBox->currentIndex()));
    stream.writeAttribute("RC2", QString().setNum(ui->rc2ComboBox->currentIndex()));
    stream.writeAttribute("BC", QString().setNum(ui->bcComboBox->currentIndex()));
    stream.writeAttribute("RCV", QString().setNum(ui->rcvSlider->value()));
    stream.writeAttribute("BV", QString().setNum(ui->bvSlider->value()));
    stream.writeAttribute("PTW", QString().setNum(ui->ptwSpin->value()));
    stream.writeEndElement();
    stream.writeEndDocument();

    file.close();

    return true;
}

bool MainWindow::_loadSettings()
{
    QFile file(SETTINGSFILENAME);

    if (file.open(QIODevice::ReadOnly) == false) {
        return false;
    }

    QXmlStreamReader stream(&file);

    while (stream.atEnd() == false) {
        if (stream.isStartElement() == true) {
            if (stream.name() == "settings") {
                this->_adjustWidgetSettings(stream.attributes());
            }
            else {
                stream.raiseError(QString("Unrecognised element '%1'").arg(stream.name().toString()));
            }
        }
        stream.readNext();
    }

    if (stream.hasError() == true) {
        file.close();

        return false;
    }

    file.close();

    return true;
}

void MainWindow::_setVisibleWidget(QWidget *widget)
{
    if (_visibleWidget != 0) {
        _visibleWidget->setVisible(false);
        _oldWidget = _visibleWidget;
    }

    widget->setVisible(true);
    _visibleWidget = widget;

    ui->graphicsView->setEnabled(false);
}

void MainWindow::_hideAllWidgets()
{
    ui->mainMenu->setVisible(false);
    ui->gameTypeMenu->setVisible(false);
    ui->pausedMenu->setVisible(false);
    ui->aboutWidget->setVisible(false);
    ui->settingsWidget->setVisible(false);
    ui->winWidget->setVisible(false);
    ui->helpWidget->setVisible(false);

    _visibleWidget = 0;
}

void MainWindow::_centerWidget(QWidget *widget)
{
    qreal centerX = (this->width() / 2) - (widget->geometry().width() / 2);
    qreal centerY = (this->height() / 2) - (widget->geometry().height() / 2);

    widget->setGeometry(centerX, centerY,
                        widget->geometry().width(), widget->geometry().height());
}

void MainWindow::_adjustSceneGameSettings()
{
    Scene::Settings settings;

    settings.ball1.setColor(QColor(ui->bcComboBox->currentText()));
    settings.player1.setColor(QColor(ui->rc1ComboBox->currentText()));
    settings.player2.setColor(QColor(ui->rc2ComboBox->currentText()));

    settings.ball1.setSpeed(ui->bvSlider->value());
    settings.player1.setSpeed(ui->rcvSlider->value());
    settings.player2.setSpeed(ui->rcvSlider->value());

    settings.credits = ui->ptwSpin->value();

    _scene->setSettingsGame(settings);
}

void MainWindow::_adjustWidgetSettings(const QXmlStreamAttributes &attributes)
{
    foreach (QXmlStreamAttribute attribute, attributes) {
        QStringRef name = attribute.name();
        if (name == "RC1") {
            ui->rc1ComboBox->setCurrentIndex(attribute.value().toString().toInt());
        }
        if (name == "RC2") {
            ui->rc2ComboBox->setCurrentIndex(attribute.value().toString().toInt());
        }
        if (name == "BC") {
            ui->bcComboBox->setCurrentIndex(attribute.value().toString().toInt());
        }
        if (name == "RCV") {
            ui->rcvSlider->setValue(attribute.value().toString().toInt());
        }
        if (name == "BV") {
            ui->bvSlider->setValue(attribute.value().toString().toInt());
        }
        if (name == "PTW") {
            ui->ptwSpin->setValue(attribute.value().toString().toInt());
        }
    }
}

void MainWindow::_enableScene()
{
    this->_hideAllWidgets();
    ui->graphicsView->setEnabled(true);
    ui->graphicsView->setFocus();
}

void MainWindow::_makeConnections()
{
    // Temporal pointer
    QShortcut *pause = new QShortcut(Qt::Key_P, this);

    QObject::connect(ui->newGameButton, SIGNAL(clicked()), this, SLOT(onNewGameClicked()));
    QObject::connect(ui->twoPlayersButton, SIGNAL(clicked()), this, SLOT(on2PlayersClicked()));
    QObject::connect(ui->foreverAloneButton, SIGNAL(clicked()), this, SLOT(onForeverAloneClicked()));
    QObject::connect(ui->backButton, SIGNAL(clicked()), this, SLOT(onBack()));

    QObject::connect(ui->settingsButton, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
    QObject::connect(ui->defaultButton, SIGNAL(clicked()), this, SLOT(onDefaultClicked()));
    QObject::connect(ui->acceptButton, SIGNAL(clicked()), this, SLOT(onAcceptClicked()));
    QObject::connect(ui->backButton_2, SIGNAL(clicked()), this, SLOT(onBack()));

    QObject::connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(onHelpClicked()));
    QObject::connect(ui->backButton_4, SIGNAL(clicked()), this, SLOT(onBack()));

    QObject::connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(onAboutClicked()));
    QObject::connect(ui->backButton_3, SIGNAL(clicked()), this, SLOT(onBack()));

    QObject::connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(pause, SIGNAL(activated()), this, SLOT(onPauseClicked()));
    QObject::connect(ui->continueButton, SIGNAL(clicked()), this, SLOT(onContinueClicked()));
    QObject::connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(onRestartClicked()));
    QObject::connect(ui->settingsButton_2, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
    QObject::connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(onQuitClicked()));

    QObject::connect(_scene, SIGNAL(won(Scene::Player)), this, SLOT(onWin(Scene::Player)));
    QObject::connect(ui->restartButton_2, SIGNAL(clicked()), this, SLOT(onRestartClicked()));
    QObject::connect(ui->mainMenuButton, SIGNAL(clicked()), this, SLOT(onMainMenuClicked()));
}
