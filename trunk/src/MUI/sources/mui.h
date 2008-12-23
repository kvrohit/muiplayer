#ifndef MUI_H
#define MUI_H

#include <QtGui>
#include "cfmod.hpp"
#include "ui_mui.h"
#include "playlistmodel.h"

#define ROWHEIGHT 21

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
    
    void showAboutBox();

private:
    Ui::MUIClass ui;
    
    FMOD::Player *p;
    
    QTimer *timer;
    PlaylistModel model;
    
    int ms, lenms;
    int currentRow;
    bool isPlaying, isPaused;
    
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *);
	
	QString qWelcomeString;
};

#endif // MUI_H
