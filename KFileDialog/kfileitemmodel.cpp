#include "kfileitemmodel.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>
#include <QFileIconProvider>
#include "workthread.h"
#include <QThread>
#include <QMetaType>

//#include <QtCu
//#include <Qt

//static void addChildren(QString m_rootPath, QStringList filter, KFileItemNode* rootNode)
//{
//	QDir dir(m_rootPath);
//	QFileInfoList infoList = dir.entryInfoList(filter, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
//	for (int i = 0; i < infoList.size(); i++)
//	{
//		QFileInfo fileInfo = infoList.at(i);
//		QString childFileName = fileInfo.absoluteFilePath();

//		KFileItemNode::FileType fileType;
//		if (fileInfo.isFile())
//		{
//			fileType = KFileItemNode::File;
//		}
//		else if (fileInfo.isDir())
//		{
//			fileType = KFileItemNode::Folder;
//		}
//		else
//		{
//			continue ;
//		}

//		KFileItemNode* childNode = new KFileItemNode(fileType, rootNode, childFileName);
//		if (childNode)
//			rootNode->m_children.append(childNode);

//	}
//}

KFileItemNode::KFileItemNode(FileType fileType, KFileItemNode *parent, const QString &fileName)
	: m_fileType(fileType)
	, m_parent(parent)
	, m_fileName(fileName)
{

}

KFileItemNode::~KFileItemNode()
{
	qDeleteAll(m_children);
}

KFileItemModel::KFileItemModel(QObject* parent/* = nullptr*/, const QString& rootPath)
	: QAbstractItemModel(parent)
	, m_rootNode(nullptr)
{
	m_rootPath = QDir::toNativeSeparators(rootPath);

	qRegisterMetaType<QString>("QString&");

	QThread* workerThread = new QThread();
	TimerThread* worker = new TimerThread(m_rootPath);
	worker->moveToThread(workerThread);

//	connect(ui->threadButton2, &QPushButton::clicked, fTimerThreadStart);
	QObject::connect(workerThread, &QThread::started, worker, &TimerThread::run);
	QObject::connect(worker, &TimerThread::workFinished, workerThread, &QThread::quit);
	QObject::connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);

	workerThread->start();
//	m_timer->start(2000);

	_createTree();

	connect(worker, &TimerThread::working, this, &KFileItemModel::addTenItem);
}

void KFileItemModel::addTenItem(QString& fileInfo)
{
	qDebug()<<fileInfo<<endl;
}

KFileItemModel::~KFileItemModel()
{

}

void KFileItemModel::_init()
{

}

void KFileItemModel::_createTree()
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

	_destroyTree();

	if (rootInfo.isDir())
	{
		m_rootNode = new KFileItemNode(KFileItemNode::Folder, nullptr, m_rootPath);
		if (m_rootNode)
		{
			_createChildren();
		}
	}
	resetInternalData();
}

void KFileItemModel::_destroyTree()
{
	if (m_rootNode)
	{
		m_rootNode->m_children.clear();
		delete m_rootNode;
		m_rootNode = nullptr;
	}
//#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	resetInternalData();
//#endif
}

// 这里放到子线程里做
void KFileItemModel::_createChildren()
{
	//todo 这里要写过滤条件
	QStringList listType;
	listType<<"*.*";


	QDir dir(m_rootPath);
	m_fileInfoList = dir.entryInfoList(listType, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
	m_fileCount = m_fileInfoList.size();

	for (int i = 0; i < 10; i++)
	{
		QFileInfo fileInfo = m_fileInfoList.at(i);
		QString childFileName = fileInfo.absoluteFilePath();

		KFileItemNode::FileType fileType;
		if (fileInfo.isFile())
		{
			fileType = KFileItemNode::File;
		}
		else if (fileInfo.isDir())
		{
			fileType = KFileItemNode::Folder;
		}
		else
		{
			continue ;
		}

		KFileItemNode* childNode = new KFileItemNode(fileType, m_rootNode, childFileName);
		if (childNode)
			m_rootNode->m_children.append(childNode);

	}
}

KFileItemNode* KFileItemModel::_nodeFromIndex(const QModelIndex &index) const
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
	KFileItemNode *itemNode = _nodeFromIndex(index);
	QFileInfo fileInfo(itemNode->m_fileName);

	//第一列文字
	if (role == Qt::DisplayRole && index.column() == 0)
	{
		return QVariant(fileInfo.baseName());
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
