#include "kdetailview.h"


#include <QFileInfo>
#include <QLineEdit>
#include <QDir>
#include <QHeaderView>
#include "kfile/kfile.h"

#include <QDebug>

#include "../kfileitemnode.h"
#include "../kfileitemmodel.h"

//这里打个备注，这样搞不太好
//KFileItemNode* node = reinterpret_cast<KFileItemNode*>(index.internalPointer());
//自定义一个userrole专门储存fillname

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

	QString beforName = node->m_fileName;
	QFileInfo fileInfo(node->m_fileName);
	if (lineEdit->text() == fileInfo.fileName())
		return;

	QString afterName = fileInfo.absolutePath() + QDir::separator() + lineEdit->text();

	qDebug()<<"----"<<afterName<<endl;
	node->m_fileName = afterName;

	//kit
	bool ret = KFile::rename(beforName, afterName);
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
