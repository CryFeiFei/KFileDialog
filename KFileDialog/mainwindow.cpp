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

	KFileItemModel* model = new KFileItemModel(ui->treeView, strDesktop);

	QSortFilterProxyModel *pProxyModel = new QSortFilterProxyModel(this);
	pProxyModel->setSourceModel(model);

	ui->treeView->setRootIsDecorated(false);
	ui->treeView->setItemsExpandable(false);
	ui->treeView->header()->setSortIndicatorShown(false);
//	ui->treeView->setItemDelegate(new UnixTempTreeDelegate(treeView));
	ui->treeView->setSortingEnabled(true);
	ui->treeView->setModel(pProxyModel);
	ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

MainWindow::~MainWindow()
{
	delete ui;
}
