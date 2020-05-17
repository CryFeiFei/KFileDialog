#include "kdetailview.h"


#include <QFileInfo>
#include <QLineEdit>
#include <QDir>
#include <QHeaderView>

#include <QDebug>

#include "../kfileitemnode.h"
#include "../kfileitemmodel.h"

QWidget* KFileItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		KFileItemNode* node = reinterpret_cast<KFileItemNode*>(index.internalPointer());
		QLineEdit* lineEdit = new QLineEdit(parent);
		QFileInfo fileInfo(node->m_fileName);
		lineEdit->setPlaceholderText(fileInfo.fileName());
		return lineEdit;
	}
	else
		return NULL;
}

void KFileItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

	if (index.column() != 0)
		return;

	QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);

	KFileItemNode* node = reinterpret_cast<KFileItemNode*>(index.internalPointer());

	QFileInfo fileInfo(node->m_fileName);
	lineEdit->setText(fileInfo.fileName());
}

void KFileItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() != 0)
		return;

	QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
	KFileItemNode* node = reinterpret_cast<KFileItemNode*>(index.internalPointer());

	QFileInfo fileInfo(node->m_fileName);
	if (lineEdit->text() == fileInfo.fileName())
		return;

	QString dirPath = fileInfo.absolutePath() + QDir::separator() + lineEdit->text();

	qDebug()<<"----"<<dirPath<<endl;

	node->m_fileName = dirPath;
}

KDetailView::KDetailView(QWidget* parent) : QTableView(parent)
{
	m_model = new KFileItemModel(this);
//	QStringList filterListType;
//	filterListType<<"*.*";
//	m_model->Init(strDesktop, filterListType);

	setModel(m_model);
	KFileItemDelegate* fileItemDelegate = new KFileItemDelegate(this);
	setItemDelegate(fileItemDelegate);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setShowGrid(false); //不显示格子线
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setEnabled(false);
	horizontalHeader()->setCascadingSectionResizes(true);

	connect(m_model, SIGNAL(loadFinished()), this, SLOT(modelLoadFinished()));
//	connect(ui->tableView->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortHeader(int)));
}

void KDetailView::Init(const QString &loadPath, const QStringList &listFilter)
{
	if (m_model)
	{
		m_model->Init(loadPath, listFilter);
	}
}

void KDetailView::modelLoadFinished()
{
	horizontalHeader()->setEnabled(true);
}
