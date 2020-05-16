#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#include <QObject>
#include <QTableView>
#include <QStyledItemDelegate>


class KFileItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	KFileItemDelegate(QObject* parent = 0)
		: QStyledItemDelegate(parent) {}

	~KFileItemDelegate() {}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
};

class KDetailView : public QTableView
{
public:
	KDetailView(QWidget *parent = nullptr);
	~KDetailView(){}
};

#endif // DETAILVIEW_H
