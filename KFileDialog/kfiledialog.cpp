#include "kfiledialog.h"
#include "ui_kfiledialog.h"



#include <QStandardPaths>
#include <QDir>

#include <QFileDialog>


KFileDialog::KFileDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KFileDialog)
{
	ui->setupUi(this);

	chooseDir();

	connect(ui->pushButton, &QPushButton::clicked, this, &KFileDialog::chooseDir);
}

KFileDialog::~KFileDialog()
{
	delete ui;
}

void KFileDialog::chooseDir()
{
	QString loadPath;

	QStringList filterListType;
	filterListType<<"*.*";


	QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QString str = QDir::separator() + QString("123");
	strDesktop += str;

	loadPath = QFileDialog::getExistingDirectory(0, "test", strDesktop);

	if (!loadPath.isEmpty())
		ui->stackedWidget->Init(loadPath, filterListType);
}
