#include "mui.h"
#include "about.h"
#include <string>

MUI::MUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{	
	qWelcomeString = "<b>Welcome to MUI</b> <br/> Version 0.0.1";	
	ui.setupUi(this);
    ui.labelNowPlaying->setText(qWelcomeString);
    
    ui.tableView->setModel(&model);
	ui.tableView->hideColumn(2);
    ui.tableView->verticalHeader()->setDefaultSectionSize(21);
    
    // Dock Widget
    QDockWidget *dock = new QDockWidget(this);
    dock->setWindowTitle("Metadata");
    mdWidget = new MetaDataWidget(dock);
    dock->setWidget(mdWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    ui.menuView->addAction(dock->toggleViewAction());
    // End dock widget
	
    // Sliders and Icons
    widgetSlider = new QWidget(this);
    
    QHBoxLayout *layout = new QHBoxLayout;
    QSizePolicy sp;
    sp.setHorizontalPolicy(QSizePolicy::Expanding);
    sp.setHorizontalStretch(1);
    
    slider = new QSlider(Qt::Horizontal, this);
    slider->setSizePolicy(sp);
    sliderVolume = new VolumeSlider(Qt::Horizontal, this);
    
    QLabel *labelTimeIcon = new QLabel(this);
    labelTimeIcon->setPixmap(QPixmap(":/images/images/time.png"));
    QLabel *labelVolumeIcon = new QLabel(this);
    labelVolumeIcon->setPixmap(QPixmap(":/images/images/volume.png"));
    
    layout->addWidget(labelTimeIcon);
    layout->addWidget(slider);
    layout->addWidget(labelVolumeIcon);
    layout->addWidget(sliderVolume);
    widgetSlider->setLayout(layout);
    
    ui.toolBar->addWidget(widgetSlider);
    // End Sliders and Icons
    
    currentRow = 0;
    isPlaying = isPaused = false;
    
    timer = new QTimer(this);
    timer->setInterval(500);
    
    p = new FMOD::Player();
    
    slider->setRange(0, 0);
    loadSettings();
    setAcceptDrops(true);
    
    connect(timer, SIGNAL(timeout()),
		this, SLOT(displayTime()));
        
    connect(slider, SIGNAL(sliderReleased()),
        this, SLOT(sSeek()));
    connect(slider, SIGNAL(sliderPressed()),
        this, SLOT(sFreeze()));
    connect(slider, SIGNAL(sliderMoved(int)),
        this, SLOT(sMove(int)));
    connect(sliderVolume, SIGNAL(sliderMoved(int)),
        this, SLOT(sVolume(int)));

    connect(ui.actionExit, SIGNAL(triggered()),
        this, SLOT(close()));
    connect(ui.actionOpen, SIGNAL(triggered()),
        this, SLOT(openPlaylist()));
	connect(ui.actionSave, SIGNAL(triggered()),
        this, SLOT(savePlaylist()));		
    connect(ui.actionClear, SIGNAL(triggered()),
        this, SLOT(clear()));
    connect(ui.actionAbout, SIGNAL(triggered()),
        this, SLOT(showAboutBox()));
    
    connect(ui.buttonAdd, SIGNAL(clicked()),
        this, SLOT(addMusicFiles()));
    connect(ui.actionNext, SIGNAL(triggered()),
        this, SLOT(next()));
    connect(ui.actionPrevious, SIGNAL(triggered()),
        this, SLOT(previous()));
    connect(ui.actionPlay, SIGNAL(triggered()),
        this, SLOT(play()));

    connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)),
        this, SLOT(handleDoubleClick(const QModelIndex &)));
}

void MUI::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MUI::dropEvent(QDropEvent *event)
{
    addMusicFiles(event->mimeData()->urls());
    event->acceptProposedAction();
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
        
    QString file = QVariant(index.sibling(row, 2).data()).toString();
    QString title = QVariant(index.sibling(row, 0).data()).toString();
    std::string filename = file.toStdString();
    	
    try 
	{
		QString msg = "<b>Welcome to MUI</b><br/>Playing: ";
		
        if(isPlaying) stop();
        
        p->renderFile(filename);		
        lenms = p->getLength();
		
		msg.append(title);
		
        try {
			tagreader.renderFile(filename);
			tag = tagreader.getTag();
            mdWidget->setTag(tag);
			
            msg="";
			msg.append("<font size=6>");
			msg.append(convertToUnicode(tag.artist));
			msg.append(" / ");
			msg.append(convertToUnicode(tag.title));
			msg.append("</font><br/><font size = 4>");
			msg.append(convertToUnicode(tag.album));
			msg.append("</font>");
            
			qDebug() << "Title: "<<tag.title.c_str();
			qDebug() << "Artist: "<<tag.artist.c_str();
			qDebug() << "Album: "<<tag.album.c_str();
            qDebug() << "Artfile" << tag.artfile.c_str();
		}
		catch(AudioTag::TagException &tex) {
		}
        
        totalTime.sprintf("%02d:%02d", lenms / 1000 / 60, lenms / 1000 % 60);
        ui.labelTotalTime->setText(totalTime);
        
        slider->setMaximum(lenms);
        p->play();
        sVolume(volume);
        
        ui.labelNowPlaying->setText(msg);
        ui.actionPlay->setIcon(QIcon(":/images/images/pause.png"));
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
    ui.actionPlay->setIcon(QIcon(":/images/images/play.png"));
    slider->setValue(0);
    timer->stop();
    
    ui.labelTime->setText("00:00");
    ui.labelTotalTime->setText("00:00");
    ui.labelNowPlaying->setText(qWelcomeString);
	
	if(tag.artfile != "")
	{
		QDir().remove( tag.artfile.c_str() );		
	}
}

void MUI::play()
{
	static QString prev;
    if(isPlaying)
    {
        p->pause();
		prev = ui.labelNowPlaying->text();
		QString msg = "<b>Welcome to MUI</b><br/>Paused";
        ui.labelNowPlaying->setText(msg);
        ui.actionPlay->setIcon(QIcon(":/images/images/play.png"));
        isPaused = true;
        isPlaying = false;
        return;
    }
    else if(isPaused)
    {
        p->resume();
		ui.labelNowPlaying->setText(prev);
        ui.actionPlay->setIcon(QIcon(":/images/images/pause.png"));
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

void MUI::openPlaylist()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Playlist", "/",
        "Playlist (*.m3u);;All Files (*.*)");
        
    if(filename.isEmpty()) return;
    model.appendPlaylist(filename);
}

void MUI::savePlaylist()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save Playlist", "/",
		"Playlist (*.m3u);;All Files (*.*)");
	
    if(filename.isEmpty()) return;
    model.savePlaylist(filename);
}

void MUI::addMusicFiles()
{
    QString filename;
    QStringList files = QFileDialog::getOpenFileNames(this, "Open", "/",
        "Audio (*.mp3 *.aac *.mp4 *.ogg);;All files (*.*)");
    
    foreach(QString filename, files)
        model.append(filename, p->getLengthFromName(filename.toStdString()));
}

void MUI::addMusicFiles(QList<QUrl> urls)
{
    foreach(QUrl url, urls)
    {
        QString filename = url.path();
        #ifdef WIN32
        filename = filename.right(filename.length() - 1);
        #endif
        qDebug() << filename;
        
        QFileInfo f(filename);
        if(f.isDir())
        {
            QDirIterator *it = new QDirIterator(filename, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot,
                QDirIterator::Subdirectories);
            while(it->hasNext()) {
                QString f = it->next();
                model.append(f, p->getLengthFromName(f.toStdString()));
                qApp->processEvents();
            }
        }
        else
        {
            model.append(filename, p->getLengthFromName(filename.toStdString()));
        }
    }
}

void MUI::clear()
{
    model.clear();
    currentRow = 0;
}

void MUI::displayTime()
{
    ms = p->getPosition();
    
    QString curTime;
    curTime.sprintf("%02d:%02d", ms / 1000 / 60, ms / 1000 % 60);
    
    slider->setValue(ms);
    ui.labelTime->setText(curTime);
    
    if(ms == lenms) next();
}

void MUI::sSeek()
{
    unsigned int pos = slider->value();
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

void MUI::sVolume(int value)
{
    qDebug() << value;
    try {
        float v = value / 100.00;
        volume = value;
        p->setVolume(v);
    }
    catch(FMOD::FMODException &e) {
        qDebug() << "Volume Set Error";
    }
}

void MUI::loadSettings()
{
    // Load default playlist
    QString file = QCoreApplication::applicationDirPath();
    file.append("/default.m3u");
    QFileInfo f(file);
    if(f.exists()) model.appendPlaylist(file);
    
    QSettings settings("BurningMedia", "MUI");
    
    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    volume = settings.value("vol", 100).toInt();
    sliderVolume->setValue(volume);
    settings.endGroup();
    
    settings.beginGroup("ColumnWidth");
    ui.tableView->setColumnWidth(0, settings.value("col0", "120").toInt());
    ui.tableView->setColumnWidth(1, settings.value("col1", "120").toInt());
    ui.tableView->setColumnWidth(2, settings.value("col2", "120").toInt());
    settings.endGroup();
}

void MUI::closeEvent(QCloseEvent *event)
{
	if(tag.artfile != "")
	{
		QDir().remove( tag.artfile.c_str() );	
	}
    saveSettings();
    event->accept();
}

void MUI::saveSettings()
{
    QSettings settings("BurningMedia", "MUI");
    
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("vol", sliderVolume->value());
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
