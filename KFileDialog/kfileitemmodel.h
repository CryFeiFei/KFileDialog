#ifndef KFILEITEMMODEL_H
#define KFILEITEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QTimer>
#include "kfileitemnode.h"

//只加载当前目录的所有的文件以及文件夹
//由于性能问题，暂时还要把加载过程放到新的线程里。
class KFileItemModel : public QAbstractItemModel
{
public:
	explicit KFileItemModel(QObject* parent = nullptr, const QString& rootPath = QString());
	~KFileItemModel();

public:
	virtual int rowCount(const QModelIndex& parent) const;
	virtual int columnCount(const QModelIndex& parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QModelIndex parent(const QModelIndex& index) const;
	virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;

//	virtual bool canFetchMore(const QModelIndex &parent) const;
//	virtual void fetchMore(const QModelIndex &parent);

public slots:
	void addChildren();
	void addItems(QList<KFileItemNode*> fileInfo);

private:
	void _init();
	void _createTree();
	void _destroyTree();
//	void _createChildren();
	KFileItemNode* _nodeFromIndex(const QModelIndex& index) const;

private:
	QString m_rootPath;
	QFileInfo m_rootFileInfo;
	KFileItemNode* m_rootNode;

	//
	int m_fileCount;
	QFileInfoList m_fileInfoList;

	//定时器
	//定时添加
	QTimer* m_timer;

};

#endif // KFILEITEMMODEL_H
