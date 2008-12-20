#include "mui.h"
#include "about.h"
#include <string>

MUI::MUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    ui.labelNowPlaying->setText("<b>Welcome to MUI</b><br />Alpha v0.0.1");
    
    QStringList headerLabels;
    headerLabels << "Songtitle" << "Duration" << "Filepath";
    model.setHorizontalHeaderLabels(headerLabels);
    
    ui.tableView->setModel(&model);
    ui.tableView->hideColumn(2);
    currentRow = 0;
    isPlaying = isPaused = false;
    
    timer = new QTimer(this);
    timer->setInterval(500);
    
    p = new FMOD::Player();
    
    ui.slider->setMinimum(0);
    ui.slider->setMaximum(0);
    
    loadSettings();
    
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

QString MUI::getFilenameOnly(QString filepath) const
{
    filepath = filepath.section('/', -1);
    return (filepath.left(filepath.lastIndexOf(".")));
}

void MUI::handleDoubleClick(const QModelIndex &index)
{
    QString totalTime;
    int row;
    
    row = index.row();
    currentRow = row;
    
    QString file = QVariant(index.sibling(row, 2).data()).toString();
    QString title = QVariant(index.sibling(row, 0).data()).toString();
    std::string filename = file.toStdString();
    
    try {
        stop();
        
        p->renderFile(filename);
        lenms = p->getLength();
        
        totalTime.sprintf("%02d:%02d", lenms / 1000 / 60, lenms / 1000 % 60);
        ui.labelTotalTime->setText(totalTime);
        
        QStandardItem *item = new QStandardItem(totalTime);
        item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        model.setItem(row, 1, item);
        
        ui.slider->setMaximum(lenms);
        p->play();
        
        ui.labelNowPlaying->setText(title);
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
    if(filename.isEmpty()) return;
    std::string file = filename.toStdString();
    
    MUIPlaylist::Playlist p;
    int length;
    
    std::string title, filepath;
    p.renderPlaylist(file);
    
    while(p.getNextEntry(length, title, filepath))
    {
        int r = model.rowCount();
        model.setItem(r, 0, new QStandardItem(QString::fromStdString(title)));
        model.setItem(r, 2, new QStandardItem(QString::fromStdString(filepath)));
        ui.tableView->setRowHeight(r, ROWHEIGHT);
    }
}

void MUI::addMusicFiles()
{
    QString filename;
    //QString totalTime;
    //int lenms;
    QStringList files = QFileDialog::getOpenFileNames(this, "Open", "/",
        "Audio (*.mp3 *.aac *.mp4 *.ogg);;All files (*.*)");
    for (int i=0; i<files.size(); ++i)
    {
        filename = files.at(i);
        //lenms = getSongLength(filename);
        //totalTime.sprintf("%02d:%02d", lenms / 1000 / 60, lenms / 1000 % 60);
        
        int r = model.rowCount();
        model.setItem(r, 0, new QStandardItem(getFilenameOnly(filename)));
        //model.setItem(r, 1, new QStandardItem(totalTime));
        model.setItem(r, 2, new QStandardItem(filename));
        ui.tableView->setRowHeight(r, ROWHEIGHT);
    }
}

// Fixme : serious bug
int MUI::getSongLength(const QString &filename)
{
    std::string file = filename.toStdString();
    p->renderFile(file);
    return (p->getLength());
}

void MUI::clear()
{
    model.removeRows(0, model.rowCount());
    currentRow = 0;
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

void MUI::loadSettings()
{
    QSettings settings("BurningMedia", "MUI");
    
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    
    settings.beginGroup("ColumnWidth");
    ui.tableView->setColumnWidth(0, settings.value("col0", "120").toInt());
    ui.tableView->setColumnWidth(1, settings.value("col1", "120").toInt());
    ui.tableView->setColumnWidth(2, settings.value("col2", "120").toInt());
    settings.endGroup();
}

void MUI::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MUI::saveSettings()
{
    QSettings settings("BurningMedia", "MUI");
    
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    
    settings.beginGroup("ColumnWidth");
    settings.setValue("col0", ui.tableView->columnWidth(0));
    settings.setValue("col1", ui.tableView->columnWidth(1));
    settings.setValue("col2", ui.tableView->columnWidth(2));
    settings.endGroup();
}

MUI::~MUI()
{

}
