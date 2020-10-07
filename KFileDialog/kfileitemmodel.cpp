#include "kfileitemmodel.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QApplication>
#include <QFileIconProvider>
#include "thread/klocalloadthread.h"
#include <QThread>
#include <QLineEdit>
#include <QDateTime>
#include <QSortFilterProxyModel>

////
MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{
}



bool MySortFilterProxyModel::lessThan(const QModelIndex &left,
									  const QModelIndex &right) const
{
	QString strLeft = left.data(Qt::DisplayRole).toString();
	QString strRight = right.data(Qt::DisplayRole).toString();
	return strLeft.size() < strRight.size();
}


//------------------------------------------

KFileItemModel::KFileItemModel(QObject* parent/* = nullptr*/)
	: QAbstractItemModel(parent)
	, m_rootNode(nullptr)
{
	qRegisterMetaType<QList<KFileItemNode*>>("QList<KFileItemNode*>");

	m_loadThread = new QThread();
	m_kLocalLoadThread = new KLocalLoadThread();
	m_kLocalLoadThread->moveToThread(m_loadThread);

	//开始的时候启动线程。
	QObject::connect(m_loadThread, &QThread::started, m_kLocalLoadThread, &KLocalLoadThread::run);
	//读取结束之后线程暂停。
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::workDestory, m_kLocalLoadThread, &KLocalLoadThread::deleteLater);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::destroyed, m_loadThread, &QThread::quit);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::working, this, &KFileItemModel::addItems);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::workFinished, this, &KFileItemModel::loadFinished);
}

void KFileItemModel::clearCache()
{
	if (m_loadThread)
	{
		QObject::disconnect(m_loadThread, &QThread::started, m_kLocalLoadThread, &KLocalLoadThread::run);
	}

	if (m_kLocalLoadThread)
	{
		m_kLocalLoadThread->stopLoad();
		QObject::disconnect(m_kLocalLoadThread, &KLocalLoadThread::workDestory, m_kLocalLoadThread, &KLocalLoadThread::deleteLater);
		QObject::disconnect(m_kLocalLoadThread, &KLocalLoadThread::destroyed, m_loadThread, &QThread::quit);
		QObject::disconnect(m_kLocalLoadThread, &KLocalLoadThread::working, this, &KFileItemModel::addItems);
		QObject::disconnect(m_kLocalLoadThread, &KLocalLoadThread::workFinished, this, &KFileItemModel::loadFinished);
	}

	destroyTree();
}

void KFileItemModel::Init(const QString &path, const QStringList& listFilter)
{
	clearCache();

	m_rootPath = QDir::toNativeSeparators(path);
	createTree();

	//开始的时候启动线程。
	QObject::connect(m_loadThread, &QThread::started, m_kLocalLoadThread, &KLocalLoadThread::run);
	//读取结束之后线程暂停。
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::workDestory, m_kLocalLoadThread, &KLocalLoadThread::deleteLater);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::destroyed, m_loadThread, &QThread::quit);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::working, this, &KFileItemModel::addItems);
	QObject::connect(m_kLocalLoadThread, &KLocalLoadThread::workFinished, this, &KFileItemModel::loadFinished);

	m_kLocalLoadThread->Init(path, listFilter);

	if (m_loadThread->isRunning() && m_kLocalLoadThread)
	{
		m_kLocalLoadThread->run();
	}
	else
		m_loadThread->start();
}

void KFileItemModel::addItems(QList<KFileItemNode*> fileInfo)
{
	beginInsertRows(QModelIndex(), m_rootNode->m_children.size(), m_rootNode->m_children.size() + fileInfo.size() - 1);

//	while(1)
//	{
	for (int i = 0; i < fileInfo.size(); i++)
	{
		m_rootNode->m_children.append(fileInfo.at(i));
	}


	endInsertRows();
//	qDebug()<<"fileInfo"<<endl;
}

void KFileItemModel::createTree()
{
	QFileInfo rootInfo(m_rootPath);
	if (!rootInfo.exists())
	{
		Q_ASSERT("not exists");
		return;
	}

	if (rootInfo.isFile())
	{
		Q_ASSERT("is File");
		return;
	}

	if (rootInfo.isDir())
	{
		m_rootNode = new KFileItemNode(KFileItemNode::Folder, m_rootPath, nullptr);
	}
}

void KFileItemModel::destroyTree()
{
	if (m_rootNode)
	{
		m_rootNode->m_children.clear();
		m_rootNode->m_children.reserve(0);
		delete m_rootNode;
		m_rootNode = nullptr;
	}
//	resetInternalData();

	// 要刷新下，要不然model里的scrollarea一直得刷新，具体表现就是数据没变，滚轴增加了
	emit layoutChanged();
}


KFileItemNode* KFileItemModel::nodeFromIndex(const QModelIndex &index) const
{
	if (!index.isValid())
		return m_rootNode;

	if (!(m_rootNode && (index.row() < m_rootNode->m_children.count()) && index.column() < 4))
		return m_rootNode;

	return m_rootNode->m_children.at(index.row());
}

//vitrual
int KFileItemModel::rowCount(const QModelIndex &parent) const
{
	if (m_rootNode)
	{
		return m_rootNode->m_children.size();
	}

	return 0;
}

int KFileItemModel::columnCount(const QModelIndex &parent) const
{
	return 4;
}

QVariant KFileItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role != Qt::DisplayRole)
			return QVariant();

		switch (section)
		{
		case 0:
			return QVariant(QObject::tr("Name"));
		case 1:
			return QVariant(QObject::tr("Size"));
		case 2:
			return QVariant(QObject::tr("Type"));
		case 3:
			return QVariant(QObject::tr("ModifiedData"));
		}
	}
	return QVariant("");
}

QVariant KFileItemModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.model() != this)
		return QVariant();

	KFileItemNode *itemNode = nodeFromIndex(index);
	QFileInfo fileInfo(itemNode->m_fileName);

	//第一列文字
	if (role == Qt::DisplayRole && index.column() == 0)
	{
		return QVariant(fileInfo.fileName());
	}


	if (role == FilePathRole && index.column() == 0)
	{
		return QVariant(fileInfo.absoluteFilePath());
	}

	//第一列图标
	if (role == Qt::DecorationRole && index.column() == 0)
	{
		QFileIconProvider fileIconProvider;
		QIcon ico = fileIconProvider.icon(fileInfo);
		return ico;
	}

	//第二列大小
	if (index.column() == 1 && role == Qt::DisplayRole)
	{
		if (fileInfo.isDir())
			return QVariant();

		if (!fileInfo.isDir()/* && fileInfo.fileName() != QObject::tr("Size")*/)
		{
			qint64 nSize = fileInfo.size();
			return QVariant(nSize);
		}
	}

	//第三列类型 类型
	if (index.column() == 2 && role == Qt::DisplayRole)
	{
		if (fileInfo.isDir())
			return QVariant(tr("File Folder"));
		else
			return QVariant(fileInfo.suffix());
	}

	// 第四列 修改时间
	if (index.column() == 3 && role == Qt::DisplayRole)
	{
		QString modifiedData = fileInfo.lastModified().toString(Qt::SystemLocaleDate);
		return QVariant(modifiedData);
	}

	return QVariant();
}

QModelIndex KFileItemModel::parent(const QModelIndex& index) const
{
	return QModelIndex();
}

QModelIndex KFileItemModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!parent.isValid() && m_rootNode && row < m_rootNode->m_children.count() && column < 4)
	{
		KFileItemNode *childNode = m_rootNode->m_children.value(row);
		if (!childNode)
			return QModelIndex();

		return createIndex(row, column, childNode);
	}
	return QModelIndex();
}


Qt::ItemFlags KFileItemModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	if (index.column() == 0)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool KFileItemModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid())
		return false;

	if (index.column() == 0 && role == FilePathRole)
	{
		QString strFilePath = value.toString();
		KFileItemNode *itemNode = nodeFromIndex(index);
		itemNode->m_fileName = strFilePath;

		return true;
	}

	return false;
}

//bool KFileItemModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
//{
//	QString strLeft =  left.data(Qt::DisplayRole).toString();
//	QString strRight = right.data(Qt::DisplayRole).toString();
//	return strLeft > strRight;
//}

//bool KFileItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//	if (index.isValid() && role == Qt::EditRole)
//	{
//		KFileItemNode* node = static_cast<KFileItemNode*>(index.internalPointer());
//		node->m_fileName = value.toString();
//		emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
//		return true;
//	}
//	return false;
//}

//bool KFileItemModel::canFetchMore(const QModelIndex &parent) const
//{
//	if (parent.isValid())
//		return false;

//	return m_rootNode->m_children.size() < m_fileInfoList.size();
//}

//void KFileItemModel::fetchMore(const QModelIndex &parent)
//{
//	if (parent.isValid())
//		return;
//	int remainder = m_fileInfoList.size() - m_rootNode->m_children.size();
//	int itemsToFetch = qMin(1, remainder);

//	if (itemsToFetch <= 0)
//		return;

//	beginInsertRows(parent, m_rootNode->m_children.size(), m_rootNode->m_children.size() + itemsToFetch - 1);

////	fileCount += itemsToFetch;
////	for (int i = m_rootNode->m_children.size(); i < m_rootNode->m_children.size() + itemsToFetch; i++)
////	{
////		QFileInfo fileInfo = m_fileInfoList.at(i);
////		QString childFileName = fileInfo.absoluteFilePath();

////		KFileItemNode::FileType fileType;
////		if (fileInfo.isFile())
////		{
////			fileType = KFileItemNode::File;
////		}
////		else if (fileInfo.isDir())
////		{
////			fileType = KFileItemNode::Folder;
////		}
////		else
////		{
////			continue ;
////		}

////		KFileItemNode* childNode = new KFileItemNode(fileType, m_rootNode, childFileName);
////		if (childNode)
////			m_rootNode->m_children.append(childNode);

////	}

//	endInsertRows();
//}

// ------------------------------------------------------------------------------------------------
