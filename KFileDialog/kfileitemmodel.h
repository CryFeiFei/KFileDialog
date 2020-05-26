#ifndef KFILEITEMMODEL_H
#define KFILEITEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QTimer>
#include "kfileitemnode.h"
#include <QObject>
#include <QStyledItemDelegate>

class KLocalLoadThread;

//只加载当前目录的所有的文件以及文件夹
//由于性能问题，暂时还要把加载过程放到新的线程里。
class KFileItemModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	KFileItemModel(QObject* parent = nullptr);
	~KFileItemModel(){}

public:
	void Init(const QString& path, const QStringList& listFilter);

public:
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QModelIndex parent(const QModelIndex& index) const;
	virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;


	virtual Qt::ItemFlags flags(const QModelIndex& index) const;
	//这个函数不写了，
//	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

//	virtual bool canFetchMore(const QModelIndex &parent) const;
//	virtual void fetchMore(const QModelIndex &parent);

signals:
	void loadFinished();

public slots:
	void addItems(QList<KFileItemNode*> fileInfo);

private:
	void clearCache();
	void createTree();
	void destroyTree();
//	void _createChildren();
	KFileItemNode* nodeFromIndex(const QModelIndex& index) const;

private:
	QString m_rootPath;
	KFileItemNode* m_rootNode;

	//定时器
	//定时添加
	QTimer* m_timer;

	// load Thread
	QThread* m_loadThread;
	KLocalLoadThread* m_kLocalLoadThread;

	// sort Thread
	QThread* m_sortThread;
};

#endif // KFILEITEMMODEL_H
