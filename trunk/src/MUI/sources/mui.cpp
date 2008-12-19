#include "mui.h"
#include "about.h"
#include <string>

MUI::MUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    ui.labelNowPlaying->setText("<b>Welcome to MUI</b><br />Alpha v0.0.1");
    
    ui.tableView->setModel(&model);
    currentRow = 0;
    isPlaying = isPaused = false;
    
    timer = new QTimer(this);
    timer->setInterval(500);
    
    p = new FMOD::Player();
    
    ui.slider->setMinimum(0);
    ui.slider->setMaximum(0);
    
    connect(timer, SIGNAL(timeout()),
		this, SLOT(displayTime()));
        
    connect(ui.slider, SIGNAL(sliderReleased()),
        this, SLOT(sSeek()));
    connect(ui.slider, SIGNAL(sliderPressed()),
        this, SLOT(sFreeze()));
    connect(ui.slider, SIGNAL(sliderMoved(int)),
        this, SLOT(sMove(int)));

    connect(ui.actionExit, SIGNAL(triggered()),
        this, SLOT(close()));
    connect(ui.actionOpen, SIGNAL(triggered()),
        this, SLOT(open()));
    connect(ui.actionClear, SIGNAL(triggered()),
        this, SLOT(clear()));
    connect(ui.actionAbout, SIGNAL(triggered()),
        this, SLOT(showAboutBox()));
    
    connect(ui.buttonAdd, SIGNAL(clicked()),
        this, SLOT(addMusicFiles()));
    connect(ui.buttonNext, SIGNAL(clicked()),
        this, SLOT(next()));
    connect(ui.buttonPrevious, SIGNAL(clicked()),
        this, SLOT(previous()));
    connect(ui.buttonPlay, SIGNAL(clicked()),
        this, SLOT(play()));

    connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)),
        this, SLOT(handleDoubleClick(const QModelIndex &)));
}

void MUI::showAboutBox()
{
    About *a = new About();
    a->show();
}

void MUI::handleDoubleClick(const QModelIndex &index)
{
    QString totalTime;
    int row;
    
    row = index.row();
    currentRow = row;
    
    QString file = QVariant(index.sibling(row, 0).data()).toString();
    std::string filename = file.toStdString();
    
    try {
        stop();
        
        p->renderFile(filename);
        lenms = p->getLength();
        
        totalTime.sprintf("%02d:%02d", lenms / 1000 / 60, lenms / 1000 % 60);
        ui.labelTotalTime->setText(totalTime);
        
        ui.slider->setMaximum(lenms);
        p->play();
        
        ui.labelNowPlaying->setText(file.section('/', -1));
        ui.buttonPlay->setIcon(QIcon(":/images/images/pause.png"));
        isPlaying = true;
        timer->start();
    }
    catch(FMOD::FMODException &e) {
        qDebug() << "Render Error";
    }
}

void MUI::stop()
{
    p->stop();
    isPlaying = isPaused = false;
    ui.buttonPlay->setIcon(QIcon(":/images/images/play.png"));
    ui.slider->setValue(0);
    timer->stop();
    
    ui.labelTime->setText("00:00");
    ui.labelTotalTime->setText("00:00");
    ui.labelNowPlaying->setText("<b>Welcome to MUI</b><br />Alpha v0.0.1");
}

void MUI::play()
{
    if(isPlaying)
    {
        p->pause();
        ui.buttonPlay->setIcon(QIcon(":/images/images/play.png"));
        isPaused = true;
        isPlaying = false;
        return;
    }
    else if(isPaused)
    {
        p->resume();
        ui.buttonPlay->setIcon(QIcon(":/images/images/pause.png"));
        isPaused = false;
        isPlaying = true;
        return;
    }
        
    QModelIndex l = ui.tableView->currentIndex();
    qDebug() << l;
    if( !l.isValid() ) return;
    handleDoubleClick(l);
}

void MUI::next()
{
    int rc = model.rowCount();
    if(currentRow == (--rc))
    {
        stop();
        return;
    }
    
    int nextRow = currentRow + 1;
    ui.tableView->selectRow(nextRow);
    handleDoubleClick(ui.tableView->currentIndex());
}

void MUI::previous()
{
    if (currentRow == 0) return;
    int previousRow = currentRow - 1;
    ui.tableView->selectRow(previousRow);
    handleDoubleClick(ui.tableView->currentIndex());
}

void MUI::open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open", "/",
        "Playlist (*.m3u);;All Files (*.*)");
    std::string file = filename.toStdString();
    
    MUIPlaylist::Playlist p;
    int length;
    
    std::string title, filepath;
    p.renderPlaylist(file);
    while(p.getNextEntry(length, title, filepath))
        model.setItem(model.rowCount(), new QStandardItem(QString::fromStdString(filepath)));
}

void MUI::addMusicFiles()
{
    QString filename;    
    QStringList files = QFileDialog::getOpenFileNames(this, "Open", "/",
        "Audio (*.mp3 *.aac *.mp4 *.ogg);;All files (*.*)");
    for (int i=0; i<files.size(); ++i)
    {
        filename = files.at(i);
        model.setItem(model.rowCount(), new QStandardItem(filename));
    }
}

void MUI::clear()
{
    model.clear();
}

void MUI::displayTime()
{
    ms = p->getPosition();
    
    QString curTime;
    curTime.sprintf("%02d:%02d", ms / 1000 / 60, ms / 1000 % 60);
    
    ui.slider->setValue(ms);
    ui.labelTime->setText(curTime);
    
    if(ms == lenms) next();
}

void MUI::sSeek()
{
    unsigned int pos = ui.slider->value();
    p->setPosition(pos);
    timer->start();
}

void MUI::sFreeze()
{
    timer->stop();
}

void MUI::sMove(int value)
{
    QString curTime;
    curTime.sprintf("%02d:%02d", value / 1000 / 60, value / 1000 % 60);
    ui.labelTime->setText(curTime);
}

MUI::~MUI()
{

}
