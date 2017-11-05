#ifndef MUI_H
#define MUI_H

#include <QtWidgets>
#include <QMediaPlayer>
#include <QMediaMetaData>

#include "ui_mui.h"
#include "volumeslider.h"
#include "metadatawidget.h"
#include "artdata.h"
#include "about.h"

#include "metadata.h"
#include "musicdata.h"
#include "musicdatamodel.h"

#include "globals.h"
#include <string>

class MUI : public QMainWindow
{
    Q_OBJECT
public:
    MUI(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~MUI();

private slots:
    // Media playback related slots
    void handleDoubleClick(const QModelIndex &);
    void positionChanged(qint64);
    void durationChanged(qint64);
    void metaDataAvailable(bool);
    void stateChanged(QMediaPlayer::State);
    void mediaStatusChanged(QMediaPlayer::MediaStatus);
    void stop();
    void play();
    void next();
    void previous();

    void addMusicFiles();
    void clear();
    void showAboutBox();
    void toggleMenuBar(bool);

private:
    Ui::MUIClass ui;
    QModelIndex nowPlayingIndex;
    MusicDataModel model;

    QMediaPlayer *player;
    MetaDataWidget *mdWidget;
    About *aboutDialog;
    ArtData *artDataWidget;

    qint64 ms, lenms;
    int volume;
    bool isPlaying, isPaused;
    bool isMenuBarVisible;

    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *);
    void setupSignalsAndSlots();
    void setupKeyboardShortcuts();
    void doSelect(const QModelIndex&, const QModelIndex&);

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
};
#endif // MUI_H
