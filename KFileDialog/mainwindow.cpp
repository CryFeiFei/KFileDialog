#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kfileitemmodel.h"
#include <QStandardPaths>
#include <QSortFilterProxyModel>
#include <QDir>

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
}

MainWindow::~MainWindow()
{
	delete ui;
}
