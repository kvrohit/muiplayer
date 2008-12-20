#ifndef MUI_H
#define MUI_H

#include <QtGui>
#include "cfmod.hpp"
#include "cplaylist.hpp"
#include "ui_mui.h"

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
    void displayTime();
    void open();
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
    QStandardItemModel model;
    
    int ms, lenms;
    int currentRow;
    bool isPlaying, isPaused;
    
    void loadSettings();
    void saveSettings();
    int getSongLength(const QString &);
    QString getFilenameOnly(QString) const;
    void closeEvent(QCloseEvent *);
};

#endif // MUI_H
