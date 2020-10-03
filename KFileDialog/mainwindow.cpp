#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kfileitemmodel.h"
#include <QStandardPaths>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QFileSystemModel>

#include "modelview/kdetailview.h"
#include "kfiledialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	showFileDialog();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::showFileDialog()
{
	KFileDialog dlg;
	dlg.exec();
}

//-----------------------------------------------


