#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include <QDialog>

namespace Ui {
class KFileDialog;
}

class KFileDialog : public QDialog
{
	Q_OBJECT

public:
	explicit KFileDialog(QWidget *parent = nullptr);
	~KFileDialog();

private:
	Ui::KFileDialog *ui;
};

#endif // KFILEDIALOG_H
