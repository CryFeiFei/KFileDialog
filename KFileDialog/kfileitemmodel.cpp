#include "kfileitemmodel.h"

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

KFileItemModel::KFileItemModel()
{

}
