#ifndef KFILEITEMNODE_H
#define KFILEITEMNODE_H

#include <QObject>

class KFileItemNode
{
public:
	enum FileType
	{
		File,
		Folder,
	};

	KFileItemNode(FileType fileType, const QString& fileName = QString(), KFileItemNode* parent = nullptr);
	~KFileItemNode();

	QString m_fileName;
	FileType m_fileType;
	KFileItemNode* m_parent;
	QList<KFileItemNode*> m_children;
};

#endif // KFILEITEMNODE_H
