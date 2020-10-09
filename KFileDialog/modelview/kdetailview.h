#ifndef DETAILVIEW_H
#define DETAILVIEW_H

#include <QObject>
#include <QTableView>
#include <QStyledItemDelegate>

class KFileItemModel;

class KFileItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	KFileItemDelegate(QObject* parent = 0)
		: QStyledItemDelegate(parent) {}

	~KFileItemDelegate() {}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
	void paint(QPainter *painter,
			   const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class KFileFilterProxyModel;

class KDetailView : public QTableView
{
	Q_OBJECT
public:
	KDetailView(QWidget *parent = nullptr);
	~KDetailView(){}

public:
	void Init(const QString& loadPath, const QStringList& listFilter);

public slots:
	void modelLoadFinished();
	void sortHeader(int);


private:
	KFileItemModel* m_model;
	KFileFilterProxyModel* m_sortModel;
};

#endif // DETAILVIEW_H
