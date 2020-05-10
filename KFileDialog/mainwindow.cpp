#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kfileitemmodel.h"
#include <QStandardPaths>
#include <QSortFilterProxyModel>
#include <QDir>
#include <QFileSystemModel>

#if _DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QString str = QDir::separator() + QString("123");
	strDesktop += str;

	KFileItemModel* model = new KFileItemModel(ui->tableView, strDesktop);

//	QSortFilterProxyModel *pProxyModel = new QSortFilterProxyModel(this);
//	pProxyModel->setSourceModel(model);

//	ui->listView->setRootIsDecorated(false);
//	ui->listView->setItemsExpandable(false);
//	ui->listView->header()->setSortIndicatorShown(false);
//	ui->treeView->setItemDelegate(new UnixTempTreeDelegate(treeView));
//	ui->listView->setSortingEnabled(true);
	ui->tableView->setModel(model);
//	ui->tableView->setSortingEnabled(true);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setShowGrid(false); //不显示格子线
	ui->tableView->horizontalHeader()->setStretchLastSection(true);
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->horizontalHeader()->setEnabled(false);

	connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortHeader(int)));
	//----------------------


	QFileSystemModel* sysModel = new QFileSystemModel(this);
	QModelIndex index = sysModel->setRootPath(strDesktop);
	ui->tableView_2->setModel(sysModel);
	ui->tableView_2->setRootIndex(index);
	ui->tableView_2->setVisible(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::sortHeader(int nHeaderIndex)
{

	//文件多的话，这里得重写

}

//-----------------------------------------------


