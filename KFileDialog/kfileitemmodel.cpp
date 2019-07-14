#include "kfileitemmodel.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QApplication>

//#include <QtCu
//#include <Qt

static void addChildren(QString m_rootPath, QStringList filter, KFileItemNode* rootNode)
{
	QDir dir(m_rootPath);
	QFileInfoList infoList = dir.entryInfoList(filter, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
	for (int i = 0; i < infoList.size(); i++)
	{
		QFileInfo fileInfo = infoList.at(i);
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

		KFileItemNode* childNode = new KFileItemNode(fileType, rootNode, childFileName);
		if (childNode)
			rootNode->m_children.append(childNode);

	}
}

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
	m_strRootPath = QDir::toNativeSeparators(rootPath);
}

KFileItemModel::~KFileItemModel()
{

}

void KFileItemModel::_init()
{

}

void KFileItemModel::_createTree()
{
	QFileInfo rootInfo(m_strRootPath);
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
		m_rootNode = new KFileItemNode(KFileItemNode::Folder, nullptr, m_strRootPath);
		if (m_rootNode)
		{
			_createChildren();
		}
	}
}

void KFileItemModel::_destroyTree()
{
	if (m_rootNode)
	{
		m_rootNode->m_children.clear();
		delete m_rootNode;
		m_rootNode = nullptr;
	}
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	resetInternalData();
#endif
}

// 这里放到子线程里做
void KFileItemModel::_createChildren()
{
	//todo 这里要写过滤条件
	QStringList listType;
	listType<<"*.*";

	QFuture<void> future = QtConcurrent::run(addChildren, m_strRootPath, listType, m_rootNode);
	while(!future.isFinished())
	{
		QApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
