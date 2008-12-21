#include "about.h"

About::About()
{
	QString qtVer = "Using Qt ";
	
    ui.setupUi(this);	
	
	QStringList componentHeaderLabels;
	componentHeaderLabels << "Component" << "Version";
	model.setHorizontalHeaderLabels(componentHeaderLabels);
    
    ui.tableComponents->setModel(&model);
	ui.tableComponents->horizontalHeader()->setStretchLastSection( true );
	// base
	model.setItem(0, 0, new QStandardItem("MUI base"));
    model.setItem(0, 1, new QStandardItem("0.0.1"));
	ui.tableComponents->setRowHeight(0, ROWHEIGHT);	
	
	// cfmod
    model.setItem(1, 0, new QStandardItem("cfmod"));
    model.setItem(1, 1, new QStandardItem(QString::fromStdString(FMOD::libversion)));
	ui.tableComponents->setRowHeight(1, ROWHEIGHT);	
	
	// cplaylist
	model.setItem(2, 0, new QStandardItem("cplaylist"));
    model.setItem(2, 1, new QStandardItem(QString::fromStdString(MUIPlaylist::libversion)));
	ui.tableComponents->setRowHeight(2, ROWHEIGHT);	
	
    
    ui.labelQtVersion->setText(qtVer.append(qVersion()));
}
