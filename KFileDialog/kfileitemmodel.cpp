#include "kfileitemmodel.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

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

void KFileItemModel::_createChildren()
{
	//todo 这里要写过滤条件
	QStringList m_listType;
	m_listType<<"*.*";
	QDir dir(m_strRootPath);
	QFileInfoList infoList = dir.entryInfoList(m_listType, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
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

		KFileItemNode* childNode = new KFileItemNode(fileType, m_rootNode, childFileName);
		if (childNode)
			m_rootNode->m_children.append(childNode);

	}
}
