#include "kfileitemnode.h"

KFileItemNode::KFileItemNode(FileType fileType, const QString &fileName, KFileItemNode *parent)
	: m_fileType(fileType)
	, m_parent(parent)
	, m_fileName(fileName)
{

}

KFileItemNode::~KFileItemNode()
{
	qDeleteAll(m_children);
}
