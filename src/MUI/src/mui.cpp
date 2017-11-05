#include "mui.h"

MUI::MUI(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
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

    ui.menuView->addAction(dock->toggleViewAction());
    // End dock widget

    // Sliders and Icons
    QToolBar *artDataToolBar = new QToolBar("Media Info Bar", this);
    artDataWidget = new ArtData(artDataToolBar);
    artDataToolBar->addWidget(artDataWidget);
    artDataToolBar->setObjectName("MediaInfoBar");
    addToolBar(artDataToolBar);
    // End Sliders and Icons

    isPlaying = isPaused = false;
    player = new QMediaPlayer();

    artDataWidget->m_seekBar->setRange(0, 0);

    setupKeyboardShortcuts();
    setupSignalsAndSlots();
    loadSettings();

    toggleMenuBar(isMenuBarVisible);
}

void MUI::showAboutBox() {
    aboutDialog->show();
}

void MUI::handleDoubleClick(const QModelIndex &index) {
    QString filepath = index.data(FILEPATHROLE).toString();

    if (isPlaying) {
        stop();
    }

    player->setMedia(QUrl::fromLocalFile(filepath));
    player->play();

    Meta::AudioTag tag = model.audioTagAtIndex(index);
    QString songTitle = tag.title;
    songTitle.append("<b> by </b>");
    songTitle.append(tag.artist);

    artDataWidget->setSongTitle(songTitle);
    artDataWidget->setAlbumArt(tag.albumArt);
    mdWidget->setTag(tag);

    ui.actionPlay->setIcon(QIcon(Mui::MediaPauseIcon));
    isPlaying = true;
    nowPlayingIndex = index;
}

void MUI::stop() {
    player->stop();
    player->setMedia(QMediaContent());
    isPlaying = isPaused = false;
    ui.actionPlay->setIcon(QIcon(Mui::MediaPlaybackIcon));
    artDataWidget->reset();
    mdWidget->reset();
}

void MUI::play() {
    switch (player->state()) {
    case QMediaPlayer::PlayingState:
        player->pause();
        return;
    case QMediaPlayer::PausedState:
        player->play();
        return;
    default:
        return;
    }

    QModelIndex l = ui.treeView->currentIndex();
    if (!l.isValid()) {
        return;
    }

    handleDoubleClick(l);
}

void MUI::next() {
    doSelect(nowPlayingIndex, ui.treeView->indexBelow(nowPlayingIndex));
}

void MUI::previous() {
    doSelect(nowPlayingIndex, ui.treeView->indexAbove(nowPlayingIndex));
}

void MUI::doSelect(const QModelIndex &/*currentIndex*/, const QModelIndex &newIndex) {
    if (!newIndex.isValid()) {
        return;
    }

    QItemSelectionModel *selectionModel = ui.treeView->selectionModel();
    QItemSelection rowSelection;

    ui.treeView->setCurrentIndex(newIndex);
    rowSelection.select(newIndex, newIndex);
    selectionModel->select(rowSelection,
                           QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    handleDoubleClick(newIndex);
}

void MUI::addMusicFiles() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Open", "/",
                                                      Mui::AudioFilter);

    foreach(QString filename, files) {
        model.appendData(filename);
    }
}

void MUI::clear() {
    model.resetData();
}

void MUI::positionChanged(qint64 pos) {
    artDataWidget->m_seekBar->setValue(pos);
    artDataWidget->setCurrentTime(Mui::formatTimeToQString(pos));
}

void MUI::durationChanged(qint64 duration) {
    artDataWidget->m_seekBar->setRange(0, duration);
    artDataWidget->setTotalTime(Mui::formatTimeToQString(duration));
}

void MUI::loadSettings() {
    QSettings settings("BurningMedia", "MUI");

    settings.beginGroup("MainWindow");
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
    volume = settings.value("vol", 100).toInt();
    // sliderVolume->setValue(volume);
    isMenuBarVisible = settings.value("menuBarVisible", true).toBool();
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

void MUI::saveSettings() {
    QSettings settings("BurningMedia", "MUI");

    settings.beginGroup("MainWindow");
    settings.setValue("state", saveState());
    settings.setValue("geometry", saveGeometry());
    // settings.setValue("vol", sliderVolume->value());
    settings.setValue("menuBarVisible", isMenuBarVisible);
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

void MUI::setupSignalsAndSlots() {
    connect(ui.actionExit, SIGNAL(triggered()),
        this, SLOT(close()));
    connect(ui.actionClear, SIGNAL(triggered()),
        this, SLOT(clear()));
    connect(ui.actionAbout, SIGNAL(triggered()),
        this, SLOT(showAboutBox()));
    connect(ui.actionAddMusicFiles, SIGNAL(triggered()),
            this, SLOT(addMusicFiles()));

    connect(ui.actionNext, SIGNAL(triggered()),
        this, SLOT(next()));
    connect(ui.actionPrevious, SIGNAL(triggered()),
        this, SLOT(previous()));
    connect(ui.actionPlay, SIGNAL(triggered()),
        this, SLOT(play()));

    connect(ui.treeView, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(handleDoubleClick(const QModelIndex &)));

    connect(ui.actionShowMenuBar, SIGNAL(toggled(bool)),
        this, SLOT(toggleMenuBar(bool)));

    connect(player, SIGNAL(positionChanged(qint64)),
            this, SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(durationChanged(qint64)),
            this, SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(stateChanged(QMediaPlayer::State)));
}

void MUI::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Alt) {
        if (!isMenuBarVisible) {
            ui.menuBar->setVisible(true);
        }
    }
}

void MUI::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Alt) {
        if (!isMenuBarVisible) {
            ui.menuBar->setVisible(false);
        }
    }
}

void MUI::toggleMenuBar(bool visible) {
    ui.actionShowMenuBar->setChecked(visible);
    ui.menuBar->setVisible(visible);
    isMenuBarVisible = visible;
}

void MUI::setupKeyboardShortcuts() {
    ui.actionExit->setShortcut(QKeySequence::Quit);
    ui.actionSave->setShortcut(QKeySequence::Save);
    ui.actionOpen->setShortcut(QKeySequence::Open);
    ui.actionShowMenuBar->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_M));
}

void MUI::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch (status) {
    case QMediaPlayer::EndOfMedia:
        next();
        break;
    default:
        break;
    }
}

void MUI::stateChanged(QMediaPlayer::State state) {
    switch (state) {
    case QMediaPlayer::PlayingState:
        ui.actionPlay->setIcon(QIcon(Mui::MediaPauseIcon));
        break;
    case QMediaPlayer::PausedState:
        ui.actionPlay->setIcon(QIcon(Mui::MediaPlaybackIcon));
        break;
    case QMediaPlayer::StoppedState:
        break;
    default:
        break;
    }
}

void MUI::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}

MUI::~MUI() {
    delete player;
    delete artDataWidget;
    delete mdWidget;
    delete aboutDialog;
}
