#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets/QGraphicsScene>

#include "Setting.h"

class Ball;
class Racquet;
class GraphicsTimer;
class GraphicsCounter;

class Scene : public QGraphicsScene
{
    Q_OBJECT

public:
    Scene(QObject *parent = 0);
    Scene(const QRectF &sceneRect, QObject *parent = 0);
    Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent = 0);

    enum GameType { VS_IA, VS_HUMAN, FOREVER_ALONE };

    struct Settings
    {
        Settings()
        {
            credits = 1;
        }

        Settings(const Setting &player1, const Setting &player2, const Setting &ball1)
        {
            this->player1 = player1;
            this->player2 = player2;
            this->ball1 = ball1;

            credits = 1;
        }

        Setting player1;
        Setting player2;
        Setting ball1;

        unsigned int credits;
    };

    enum Player {
        Player1,
        Player2
    };

    /**
      * \brief Retrun true if pause is on.
      * \see newGame(GameType)
      * \see pauseGame()
      * \see continueGame()
      * \see restartGame()
      */
    bool isPaused() const;

    /**
      * \brief Return true if there is a game on.
      * \see newGame(GameType)
      * \see quitGame()
      */
    bool hasGame() const;

    void setSettingsGame(const Settings &settings);
    Settings settingsGame() const;

    GameType runningGameType() const;

public slots:
    void newGame(GameType gameType);
    void pauseGame();
    void continueGame();
    void restartGame();
    void quitGame();

signals:

    /**
      * Emited when a player wins the game.
      */
    void won(Scene::Player winner);

private slots:
    void moveBall();

    /**
      * \todo Impelemnt an IA for single player.
      */
    void moveIA();

    /**
      * \brief Start the game.
      * \see _start()
      */
    void start();

    /**
      * \brief Called when a player wins.
      * \see _emitWhoWins()
      */
    void endGame();

protected:

    /**
      * \brief Moves racquets depending of game type.
      */
    void keyPressEvent(QKeyEvent *event);

private:
    Ball *_ball1;
    Racquet *_player1;
    Racquet *_player2;

    QGraphicsRectItem *_boardBounds;

    /**
      * Global font for change size and font values,
      * affects to time played and score text items.
      */
    QFont _globalFont;

    GraphicsTimer *_gTimePlayed;
    QGraphicsSimpleTextItem *_timePlayed;

    GraphicsCounter *_gScore1;
    QGraphicsSimpleTextItem *_score1;

    GraphicsCounter *_gScore2;
    QGraphicsSimpleTextItem *_score2;

    /**
      * Timer used for move ball and move IA step by step.
      */
    QTimer *_timer;

    /**
      * Timer used for wait one second, then start the game.
      */
    QTimer *_toStart;

    qreal _nonPlayableZone;

    Settings _settings;

    bool _isPaused;
    bool _hasGame;

    GameType _gameType;

    /**
      * \brief Start the game after one sec.
      * \see start()
      */
    void _start();

    /**
      * Set default positions to all items
      * and restart graphical counters and tiemers.
      */
    void _restartGame();

    void _restartBall1();

    void _upRacquet(Racquet *racquet);
    void _downRacquet(Racquet *racquet);

    void _showAllItems();
    void _hideAllItems();

    void _updateSettings();

    /**
      * Increase a score if ball1 leaves playable zone.
      */
    void _increaseScores();

    /**
      * \see won(Player)
      */
    void _emitWhoWins();

    void __initVars();
    void __configureVars();
    void __addAllItems();
    void __hideAllItems();

    void _init_();

    static const qreal FONTSIZE  = 35;
    static const qreal TOPSPACE = 10;
};

#endif // GRAPHICSSCENE_H
