#ifndef MUI_H
#define MUI_H

#include <QtGui>
#include "ui_mui.h"
#include "playlistmodel.h"
#include "volumeslider.h"
#include "metadatawidget.h"
#include "errorlog.h"

#include "cfmod.hpp"
#include "caudiotags.hpp"

#include "support.h"

const QString muiversion = "0.0.2";

class MUI : public QMainWindow
{
    Q_OBJECT

public:
    MUI(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MUI();

private slots:
    void handleDoubleClick(const QModelIndex &);
    void addMusicFiles();
    void addMusicFiles(QList<QUrl>);
    void showErrorDialog();
    void displayTime();
    void openPlaylist();
    void savePlaylist();
    void clear();
    void stop();
    void play();
    void next();	
    void previous();
    void sSeek();
    void sFreeze();
    void sMove(int);
    void sVolume(int);
    
    void showAboutBox();

private:
    Ui::MUIClass ui;
    
    FMOD::Player *p;
    
    QTimer *timer;
    QSlider *slider;
    VolumeSlider *sliderVolume;
    QWidget *widgetSlider;
    MetaDataWidget *mdWidget;
    PlaylistModel model;
    ErrorLog log;
    
    int ms, lenms;
    int currentRow;
    int volume;
    bool isPlaying, isPaused;
    
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *);
    
    QString qWelcomeString;
            
    AudioTag::TagReader tagreader;
    AudioTag::GenericTag tag;
};
#endif // MUI_H
