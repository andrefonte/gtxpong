#include "Scene.h"

#include "Ball.h"
#include "Racquet.h"
#include "GraphicsTimer.h"
#include "GraphicsCounter.h"

#include <QtCore/QTime>
#include <QtGui/QKeyEvent>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    _init_();
}

Scene::Scene(const QRectF &sceneRect, QObject *parent) :
    QGraphicsScene(sceneRect, parent)
{
    _init_();
}

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x, y, width, height, parent)
{
    _init_();
}

void Scene::newGame(Scene::GameType gameType)
{
    _gameType = gameType;

    // Center all items, restart all score count.
    this->_restartGame();

    this->_showAllItems();

    // Start game after one second
    this->_start();
}

void Scene::pauseGame()
{
    _toStart->stop();

    _timer->stop();

    _gTimePlayed->stop();

    _isPaused = true;
}

void Scene::continueGame()
{
    // Start game immediately
    this->start();
}

void Scene::restartGame()
{
    // Center all items, restart all score count.
    this->_restartGame();

    // Start the game after one second
    this->_start();
}

void Scene::quitGame()
{
    this->_hideAllItems();

    _hasGame = false;
}

bool Scene::isPaused() const
{
    return _isPaused;
}

bool Scene::hasGame() const
{
    return _hasGame;
}

void Scene::setSettingsGame(const Scene::Settings &settings)
{
    _settings = settings;

    this->_updateSettings();
}

Scene::Settings Scene::settingsGame() const
{
    return _settings;
}

Scene::GameType Scene::runningGameType() const
{
    return _gameType;
}

void Scene::moveBall()
{
    /**
      * Verify if ball is in a valid playable zone.
      * Inverts direction if is necesary.
      */

    // Top
    if (_ball1->y() < _nonPlayableZone) {
        _ball1->setYDirection(1);
    }

    // Botton
    if ((_ball1->y() + _ball1->rect().height())
            > (this->height() - _nonPlayableZone)) {
        _ball1->setYDirection(-1);
    }

    if (_gameType == Scene::FOREVER_ALONE) {

        // Right
        if ((_ball1->x() + _ball1->rect().width())
                > (this->width() - _nonPlayableZone)) {
            _ball1->setXDirection(-1);
        }
    }
    else {
        // Collides with racquet 2?
        if (_ball1->collidesWithItem(_player2) == true) {
            _ball1->setXDirection(-1);
        }
    }

    // Collides with racquet 1?
    if (_ball1->collidesWithItem(_player1) == true) {
        _ball1->setXDirection(1);
    }

    /**
      * Increases respective score if ball1 leaves playable zone.
      */
    this->_increaseScores();
}

void Scene::moveIA()
{
    if (_gameType == Scene::VS_IA) {
        // To implement IA movements...
    }
}

void Scene::start()
{
    // Start game immediately

    _timer->start(1000 / 33);

    _gTimePlayed->start();

    _hasGame = true;

    _isPaused = false;
}

void Scene::endGame()
{
    _hasGame = false;

    this->pauseGame();

    this->_emitWhoWins();
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    int keyu = Qt::Key_Up;
    int keyd = Qt::Key_Down;
    Racquet *player = _player1;

    if (_gameType == Scene::VS_HUMAN) {
        if (_ball1->xDirection() == -1) {
            keyu = Qt::Key_A;
            keyd = Qt::Key_Z;
            player = _player1;
        }
        else {
            keyu = Qt::Key_Up;
            keyd = Qt::Key_Down;
            player = _player2;
        }
    }

    if (event->key() == keyu) {
        this->_upRacquet(player);
    }
    else if (event->key() == keyd) {
        this->_downRacquet(player);
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::_start()
{
    // Start game after one second
    _toStart->start(1000);
}

void Scene::_restartGame()
{
    this->_restartBall1();

    // Center racquet 1
    qreal sceneCenterY = this->height() / 2;
    qreal player1CenterY = _player1->boundingRect().height() / 2;
    qreal centerHeight = sceneCenterY - player1CenterY;

    _player1->setPos(_nonPlayableZone + 5, centerHeight);

    // Center racquet 2
    qreal sceneWidth = this->width() - _nonPlayableZone;
    qreal playerWidth = _player2->boundingRect().width();
    qreal finalWidth = sceneWidth - playerWidth;

    _player2->setPos(finalWidth - 5, _player1->y());

    _gTimePlayed->restart();

    _gScore1->setValue(0);
    _gScore2->setValue(0);
}

void Scene::_restartBall1()
{
    qreal combinations[2] = {1, -1};

    // Center ball1
    _ball1->setPos((this->width() / 2)
                   - (_ball1->boundingRect().width() / 2),

                   (this->height() / 2)
                   - (_ball1->boundingRect().height() / 2));

    // Random directions
    _ball1->setXDirection(combinations[qrand() % 2]);
    _ball1->setYDirection(combinations[qrand() % 2]);
}

void Scene::_upRacquet(Racquet *racquet)
{
    qreal rcSpeed = racquet->setting().speed();
    qreal rcNewPos = racquet->y() - rcSpeed;

    if (rcNewPos > _nonPlayableZone) {
        racquet->setY(rcNewPos);
    }
    else {
        racquet->setY(_nonPlayableZone);
    }
}

void Scene::_downRacquet(Racquet *racquet)
{
    qreal rcSpeed = racquet->setting().speed();
    qreal rcNewPos = racquet->y() + rcSpeed;
    qreal rcHeight = racquet->rect().height();
    qreal scHeight = this->height() - _nonPlayableZone;

    if ((rcNewPos + rcHeight) < scHeight) {
        racquet->setY(rcNewPos);
    }
    else {
        racquet->setY(scHeight - rcHeight);
    }
}

void Scene::_showAllItems()
{
    _player1->setVisible(true);

    if (_gameType != Scene::FOREVER_ALONE) {
        _player2->setVisible(true);
        _score1->setVisible(true);
    }

    _ball1->setVisible(true);

    _score2->setVisible(true);

    _timePlayed->setVisible(true);

    _boardBounds->setVisible(true);
}

void Scene::_hideAllItems()
{
    _player1->setVisible(false);

    if (_gameType != Scene::FOREVER_ALONE) {
        _player2->setVisible(false);
        _score1->setVisible(false);
    }

    _ball1->setVisible(false);

    _score2->setVisible(false);

    _timePlayed->setVisible(false);

    _boardBounds->setVisible(false);
}

void Scene::_updateSettings()
{
    _player1->setSettng(this->settingsGame().player1);
    _score1->setBrush(QBrush(_player1->setting().color()));

    _player2->setSettng(this->settingsGame().player2);
    _score2->setBrush(QBrush(_player2->setting().color()));

    _ball1->setSettng(this->settingsGame().ball1);

    _gScore1->setEndValue(this->settingsGame().credits);
    _gScore2->setEndValue(this->settingsGame().credits);
}

void Scene::_increaseScores()
{
    if (_ball1->x() < _nonPlayableZone) {
        this->_restartBall1();

        // Stop ball movements
        _timer->stop();

        // Start ball movements after one second
        this->_start();

        _gScore2->increase();
    }

    if (_gameType != Scene::FOREVER_ALONE) {
        if ((_ball1->x() + _ball1->rect().width())
                > (this->width() - _nonPlayableZone)) {
            this->_restartBall1();

            // Stop ball movements
            _timer->stop();

            // Start ball movements after one second
            this->_start();

            _gScore1->increase();
        }
    }
}

void Scene::_emitWhoWins()
{
    if (_gScore1->isEnd() == true) {
        emit won(Scene::Player1);
    }
    else {
        emit won(Scene::Player2);
    }
}

void Scene::__initVars()
{
    _ball1 = new Ball(0, 0, 15, 15);
    _player1 = new Racquet(0, 0, 10, 60);
    _player2 = new Racquet(0, 0, 10, 60);

    _boardBounds = new QGraphicsRectItem;

    _globalFont = QFont("Courier New");

    _gTimePlayed = new GraphicsTimer(this);
    _timePlayed = new QGraphicsSimpleTextItem;

    _gScore1 = new GraphicsCounter(0, 0, this);
    _score1 = new QGraphicsSimpleTextItem;

    _gScore2 = new GraphicsCounter(0, 0, this);
    _score2 = new QGraphicsSimpleTextItem;

    _timer = new QTimer(this);

    _toStart = new QTimer(this);

    _isPaused = false;
    _hasGame = false;

    _gameType = Scene::FOREVER_ALONE;
}

void Scene::__configureVars()
{
    _globalFont.setPixelSize(FONTSIZE);

    _gTimePlayed->setTextItem(_timePlayed);
    _gTimePlayed->setFomat(true, true, false);

    _timePlayed->setFont(_globalFont);
    _timePlayed->setBrush(QBrush(Qt::gray));
    _timePlayed->setPen(QPen(Qt::black, 1));
    _timePlayed->setText("00:00");
    _timePlayed->setPos(
                (this->width() / 2)
                - (_timePlayed->boundingRect().width() / 2),
                TOPSPACE);

    _nonPlayableZone =
            (_timePlayed->pos().y() * 2)
            + _timePlayed->boundingRect().height();

    _boardBounds->setBrush(Qt::NoBrush);
    _boardBounds->setPen(QPen(Qt::darkYellow, 1));
    _boardBounds->setRect(_nonPlayableZone,
                          _nonPlayableZone,
                          this->width() - (_nonPlayableZone * 2),
                          this->height() - (_nonPlayableZone * 2));

    _gScore1->setTextItem(_score1);
    _score1->setFont(_globalFont);
    _score1->setBrush(QBrush(_player1->setting().color()));
    _score1->setPen(QPen(Qt::black, 1));
    _score1->setText("0");
    _score1->setPos(_nonPlayableZone
                    - (_score1->boundingRect().width() / 2),
                    (this->height()
                     - _nonPlayableZone) + 10);

    _gScore2->setTextItem(_score2);
    _score2->setFont(_score1->font());
    _score2->setBrush(QBrush(_player2->setting().color()));
    _score2->setPen(_score1->pen());
    _score2->setText(_score1->text());
    _score2->setPos(
                (this->width() -_nonPlayableZone)
                - (_score2->boundingRect().width() / 2),
                _score1->y());

    _gScore1->setEndValue(this->settingsGame().credits);
    _gScore2->setEndValue(this->settingsGame().credits);

    _toStart->setSingleShot(true);
}

void Scene::__addAllItems()
{
    this->addItem(_player1);
    this->addItem(_player2);
    this->addItem(_ball1);
    this->addItem(_boardBounds);
    this->addItem(_score1);
    this->addItem(_score2);
    this->addItem(_timePlayed);
}

void Scene::__hideAllItems()
{
    _player1->setVisible(false);
    _player2->setVisible(false);
    _ball1->setVisible(false);
    _boardBounds->setVisible(false);
    _score1->setVisible(false);
    _score2->setVisible(false);
    _timePlayed->setVisible(false);
}

void Scene::_init_()
{
    this->__initVars();

    this->__addAllItems();

    this->__hideAllItems();

    this->__configureVars();

    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(advance()));
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(moveBall()));
    QObject::connect(_timer, SIGNAL(timeout()), this, SLOT(moveIA()));
    QObject::connect(_toStart, SIGNAL(timeout()), this, SLOT(start()));
    QObject::connect(_gScore1, SIGNAL(gotEndValue()), this, SLOT(endGame()));
    QObject::connect(_gScore2, SIGNAL(gotEndValue()), this, SLOT(endGame()));

    qsrand(QTime::currentTime().msec());
}
