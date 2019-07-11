#include "kfiledialog.h"
#include "ui_kfiledialog.h"

KFileDialog::KFileDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::KFileDialog)
{
	ui->setupUi(this);
}

KFileDialog::~KFileDialog()
{
	delete ui;
}
