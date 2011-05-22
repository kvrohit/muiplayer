#include "mui.h"

MUI::MUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    aboutDialog = new About(this);

    ui.treeView->setModel(&model);
    ui.treeView->hideColumn(Mui::FILEPATH);

    // Dock Widget
    QDockWidget *dock = new QDockWidget(this);
    dock->setObjectName("dock");
    dock->setWindowTitle("Metadata");
    mdWidget = new MetaDataWidget(dock);
    dock->setWidget(mdWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QDockWidget *fsDock = new QDockWidget(this);
    fsDock->setObjectName("fsDock");
    fsDock->setWindowTitle("File System Browser");
    FileSystemBrowser *fsBrowser = new FileSystemBrowser(fsDock);
    fsDock->setWidget(fsBrowser);
    addDockWidget(Qt::RightDockWidgetArea, fsDock);
    // ui.menuView->addAction(dock->toggleViewAction());
    // End dock widget

    // Sliders and Icons
    QToolBar *artDataToolBar = new QToolBar("Media Info Bar", this);

    artDataWidget = new ArtData(artDataToolBar);
    artDataToolBar->addWidget(artDataWidget);
    artDataToolBar->setObjectName("MediaInfoBar");
    addToolBar(artDataToolBar);
    // End Sliders and Icons

    isPlaying = isPaused = false;

    timer = new QTimer(this);
    timer->setInterval(500);

    p = new FMOD::Player();

    artDataWidget->m_seekBar->setRange(0, 0);

    setupSignalsAndSlots();
    loadSettings();
}

void MUI::showAboutBox()
{
    aboutDialog->show();
}

void MUI::showErrorDialog()
{
    log.show();
}

void MUI::handleDoubleClick(const QModelIndex &index)
{
    int row = index.row();
    QString filepath = index.data(FILEPATHROLE).toString();
    std::string std_filepath = filepath.toStdString();

    try {
        if(nowPlayingIndex.isValid()) {
            model.updateIcon(nowPlayingIndex.row(), Mui::STOPPED_STATE);
        }
        QString msg = "";

        if(isPlaying) stop();

        p->renderFile(std_filepath);
        lenms = p->getLength();

        msg.append(filepath);

        try {
            tagreader.renderFile(std_filepath);
            tag = tagreader.getTag();
            mdWidget->setTag(tag, std_filepath);

            msg = "";
            msg.append(Mui::convertToUnicode(tag.title));
            msg.append(" <b>by</b> ");
            msg.append(Mui::convertToUnicode(tag.artist));
            msg.append(" <b>from the ablum</b> ");
            msg.append(Mui::convertToUnicode(tag.album));

            artDataWidget->setAlbumArt(filepath, tag.artfile.c_str());
        }
        catch(AudioTag::TagException &tex) {
            log.append(tex.what());
        }

        artDataWidget->setTotalTime(Mui::formatTimeToQString(lenms));
        artDataWidget->m_seekBar->setMaximum(lenms);
        artDataWidget->setSongTitle(msg);

        p->play();
        sVolume(volume);
        model.updateIcon(row, Mui::PLAY_STATE);

        ui.actionPlay->setIcon(QIcon(Mui::MediaPauseIcon));
        isPlaying = true;
        timer->start();
        nowPlayingIndex = index;
    }
    catch(FMOD::FMODException &e) {
        log.append(e.what());
    }
}

void MUI::stop()
{
    p->stop();
    isPlaying = isPaused = false;
    ui.actionPlay->setIcon(QIcon(Mui::MediaPlaybackIcon));
    artDataWidget->reset();
    mdWidget->reset();
    timer->stop();

    if(tag.artfile != "")
    {
        QDir().remove( tag.artfile.c_str() );
    }
}

void MUI::play()
{
    if(isPlaying) {
        p->pause();
        ui.actionPlay->setIcon(QIcon(Mui::MediaPlaybackIcon));
        model.updateIcon(nowPlayingIndex.row(), Mui::PAUSED_STATE);
        isPaused = true;
        isPlaying = false;
        return;
    }
    else if(isPaused) {
        p->resume();
        ui.actionPlay->setIcon(QIcon(Mui::MediaPauseIcon));
        model.updateIcon(nowPlayingIndex.row(), Mui::PLAY_STATE);
        isPaused = false;
        isPlaying = true;
        return;
    }

    QModelIndex l = ui.treeView->currentIndex();
    if(!l.isValid()) {
        return;
    }
    handleDoubleClick(l);
}

void MUI::next()
{
    int rc = model.rowCount();
    int currentRow = nowPlayingIndex.row();
    if(currentRow == (--rc)) {
        stop();
        return;
    }
    model.updateIcon(currentRow, Mui::STOPPED_STATE);

    ui.treeView->selectionModel()->setCurrentIndex(
            ui.treeView->indexBelow(nowPlayingIndex),
            QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);

    handleDoubleClick(ui.treeView->currentIndex());
}

void MUI::previous()
{
    int currentRow = nowPlayingIndex.row();
    if (currentRow == 0) return;
    model.updateIcon(currentRow, Mui::STOPPED_STATE);
    ui.treeView->selectionModel()->clearSelection();
    ui.treeView->selectionModel()->setCurrentIndex(
            ui.treeView->indexAbove(nowPlayingIndex),
            QItemSelectionModel::Rows | QItemSelectionModel::ClearAndSelect);
    handleDoubleClick(ui.treeView->currentIndex());
}

void MUI::openPlaylist()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Playlist", "/",
        "Playlist (*.m3u);;All Files (*.*)");

    if(filename.isEmpty()) {
        return;
    }

    model.appendPlaylist(filename);
}

void MUI::savePlaylist()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Playlist", "/",
        "Playlist (*.m3u);;All Files (*.*)");

    if(filename.isEmpty()) {
        return;
    }

    model.savePlaylist(filename);
}

void MUI::addMusicFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Open", "/",
        "Audio (*.mp3 *.aac *.mp4 *.ogg);;All files (*.*)");

    foreach(QString filename, files) {
        model.appendData(filename);
    }
}

void MUI::clear()
{
    model.resetData();
}

void MUI::displayTime()
{
    ms = p->getPosition();

    if(ms == lenms) {
        next();
        return;
    }

    artDataWidget->m_seekBar->setValue(ms);
    artDataWidget->setCurrentTime(Mui::formatTimeToQString(ms));
}

void MUI::sSeek()
{
    unsigned int pos = artDataWidget->m_seekBar->value();
    p->setPosition(pos);
    timer->start();
}

void MUI::sFreeze()
{
    timer->stop();
}

void MUI::sMove(int value)
{
    artDataWidget->setCurrentTime(Mui::formatTimeToQString(value));
}

void MUI::sVolume(int value)
{
    try {
        float v = value / 100.00;
        volume = value;
        p->setVolume(v);
    }
    catch(FMOD::FMODException &e) {
        log.append(e.what());
    }
}

void MUI::loadSettings()
{
    QSettings settings("BurningMedia", "MUI");

    settings.beginGroup("MainWindow");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    volume = settings.value("vol", 100).toInt();
    // sliderVolume->setValue(volume);
    settings.endGroup();

    settings.beginGroup("ColumnWidth");
    ui.treeView->setColumnWidth(Mui::STATEICON,
                                settings.value("col0", "120").toInt());
    ui.treeView->setColumnWidth(Mui::SONGTITLE,
                                settings.value("col1", "120").toInt());
    ui.treeView->setColumnWidth(Mui::ARTIST,
                                settings.value("col2", "120").toInt());
    ui.treeView->setColumnWidth(Mui::ALBUM,
                                settings.value("col3", "120").toInt());
    ui.treeView->setColumnWidth(Mui::DURATION,
                                settings.value("col4", "120").toInt());
    settings.endGroup();

    /*
    settings.beginGroup("SplitterStates");
    ui.splitterMain->restoreState(settings.value("splitterMain").toByteArray());
    settings.endGroup();
    */

    model.load();
}

void MUI::closeEvent(QCloseEvent *event)
{
    if(tag.artfile != "") {
        QDir().remove( tag.artfile.c_str() );
    }
    saveSettings();
    event->accept();
}

void MUI::saveSettings()
{
    QSettings settings("BurningMedia", "MUI");

    settings.beginGroup("MainWindow");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    // settings.setValue("vol", sliderVolume->value());
    settings.endGroup();

    settings.beginGroup("ColumnWidth");
    settings.setValue("col0", ui.treeView->columnWidth(Mui::STATEICON));
    settings.setValue("col1", ui.treeView->columnWidth(Mui::SONGTITLE));
    settings.setValue("col2", ui.treeView->columnWidth(Mui::ARTIST));
    settings.setValue("col3", ui.treeView->columnWidth(Mui::ALBUM));
    settings.setValue("col4", ui.treeView->columnWidth(Mui::DURATION));
    settings.endGroup();

    /*
    settings.beginGroup("SplitterStates");
    settings.setValue("splitterMain", ui.splitterMain->saveState());
    settings.endGroup();
    */

    model.save();
}

void MUI::setupSignalsAndSlots()
{
    connect(timer, SIGNAL(timeout()),
        this, SLOT(displayTime()));

    connect(artDataWidget->m_seekBar, SIGNAL(sliderReleased()),
        this, SLOT(sSeek()));
    connect(artDataWidget->m_seekBar, SIGNAL(sliderPressed()),
        this, SLOT(sFreeze()));
    connect(artDataWidget->m_seekBar, SIGNAL(sliderMoved(int)),
        this, SLOT(sMove(int)));
    //connect(sliderVolume, SIGNAL(sliderMoved(int)),
    //    this, SLOT(sVolume(int)));

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
    connect(ui.actionErrorLog, SIGNAL(triggered()),
        this, SLOT(showErrorDialog()));
    connect(ui.actionAddMusicFiles, SIGNAL(triggered()),
            this, SLOT(addMusicFiles()));

    /*connect(ui.buttonAdd, SIGNAL(clicked()),
        this, SLOT(addMusicFiles()));*/
    connect(ui.actionNext, SIGNAL(triggered()),
        this, SLOT(next()));
    connect(ui.actionPrevious, SIGNAL(triggered()),
        this, SLOT(previous()));
    connect(ui.actionPlay, SIGNAL(triggered()),
        this, SLOT(play()));

    connect(ui.treeView, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(handleDoubleClick(const QModelIndex &)));

    connect(ui.actionEditStyleSheet, SIGNAL(triggered()),
        this, SLOT(editStyleSheet()));
}

void MUI::editStyleSheet()
{
    EditStyle *editStyleDialog = new EditStyle;
    editStyleDialog->show();
}

MUI::~MUI()
{
    // Do cleanup here...
    delete p;
    delete timer;
    delete artDataWidget;
    delete mdWidget;
    delete aboutDialog;
}
