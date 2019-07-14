#ifndef KFILEITEMMODEL_H
#define KFILEITEMMODEL_H

#include <QAbstractItemModel>

class KFileItemNode
{
public:
	enum FileType
	{
		File,
		Folder,
	};

	explicit KFileItemNode(FileType fileType, KFileItemNode* parent = nullptr, const QString& fileName = QString());
	~KFileItemNode();

	QString m_fileName;
	FileType m_fileType;
	KFileItemNode* m_parent;
	QList<KFileItemNode*> m_children;
};

class KFileItemModel
{
public:
	KFileItemModel();
};

#endif // KFILEITEMMODEL_H
