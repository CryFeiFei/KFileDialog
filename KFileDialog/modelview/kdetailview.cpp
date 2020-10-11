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
		QString fileName = index.data(Qt::DisplayRole).toString();
		QLineEdit* lineEdit = new QLineEdit(parent);
		lineEdit->setPlaceholderText(fileName);
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
	QString strFileName = index.data(Qt::DisplayRole).toString();
	lineEdit->setText(strFileName);
}

void KFileItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	if (index.column() != 0)
		return;

	QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);

	QString beforName = index.data(KFileItemModel::FilePathRole).toString();
	QString beforBaseName = index.data(Qt::DisplayRole).toString();
	if (lineEdit->text() == beforBaseName)
		return;

	QFileInfo fileInfo(beforName);
	QString afterName = fileInfo.absolutePath() + QDir::separator() + lineEdit->text();
	qDebug()<<"----"<<afterName<<endl;
	model->setData(index, afterName, KFileItemModel::FilePathRole);

	//kit
	bool ret = KFile::rename(beforName, afterName);
}

void KFileItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem viewOption(option);
	initStyleOption(&viewOption, index);
	if (option.state.testFlag(QStyle::State_HasFocus))
	{
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
	}
	QStyledItemDelegate::paint(painter, viewOption, index);
}

//--------------------------------------
//--------------------------------------

KDetailView::KDetailView(QWidget* parent) : QTableView(parent)
{
	m_model = new KFileItemModel(this);
//	QStringList filterListType;
//	filterListType<<"*.*";
//	m_model->Init(strDesktop, filterListType);

	m_sortModel = new KFileFilterProxyModel(this);
	m_sortModel->setSourceModel(m_model);

	setModel(m_sortModel);
	KFileItemDelegate* fileItemDelegate = new KFileItemDelegate(this);
	setItemDelegate(fileItemDelegate);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setShowGrid(false); //不显示格子线
//	horizontalHeader()->setStretchLastSection(true);
//	horizontalHeader()->setEnabled(false);
//	horizontalHeader()->setCascadingSectionResizes(true);
	this->setSortingEnabled(true);
	this->setEditTriggers(EditTrigger::NoEditTriggers);

	connect(m_model, SIGNAL(loadFinished()), this, SLOT(modelLoadFinished()));
	connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortHeader(int)));
}

void KDetailView::Init(const QString &loadPath, const QStringList &listFilter)
{
	m_sortModel->clear();
	if (m_model)
	{
		m_model->Init(loadPath, listFilter);
	}
}

void KDetailView::modelLoadFinished()
{
	horizontalHeader()->setEnabled(true);
}


void KDetailView::sortHeader(int index)
{

}
