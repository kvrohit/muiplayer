#ifndef MUI_H
#define MUI_H

#include <QtWidgets>

#include "ui_mui.h"
#include "volumeslider.h"
#include "metadatawidget.h"
#include "filesystembrowser.h"
#include "artdata.h"
#include "errorlog.h"
#include "about.h"
#include "editstyle.h"

#include "musicdata.h"
#include "musicdatamodel.h"

#include "cfmod.hpp"
#include "caudiotags.hpp"

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
    void displayTime();
    void stop();
    void play();
    void next();
    void previous();

    // Slider related slots
    void sSeek();
    void sFreeze();
    void sMove(int);
    void sVolume(int);

    void addMusicFiles();
    void showErrorDialog();
    void openPlaylist();
    void savePlaylist();
    void clear();
    void editStyleSheet();
    void showAboutBox();

private:
    Ui::MUIClass ui;
    ErrorLog log;
    QModelIndex nowPlayingIndex;
    MusicDataModel model;

    FMOD::Player *p;
    QTimer *timer;
    MetaDataWidget *mdWidget;
    About *aboutDialog;
    ArtData *artDataWidget;

    int ms, lenms;
    int volume;
    bool isPlaying, isPaused;

    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *);
    void setupSignalsAndSlots();
    void doSelect(const QModelIndex&, const QModelIndex&);

    AudioTag::TagReader tagreader;
    AudioTag::GenericTag tag;
};
#endif // MUI_H
