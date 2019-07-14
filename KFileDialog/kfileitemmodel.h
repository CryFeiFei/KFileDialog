#ifndef KFILEITEMMODEL_H
#define KFILEITEMMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>

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

//只加载当前目录的所有的文件以及文件夹
//由于性能问题，暂时还要把加载过程放到新的线程里。
class KFileItemModel : public QAbstractItemModel
{
public:
	explicit KFileItemModel(QObject* parent = nullptr, const QString& rootPath = QString());
	~KFileItemModel();

public:

private:
	void _init();
	void _createTree();
	void _destroyTree();
	void _createChildren();

private:
	QString m_strRootPath;
	QFileInfo m_rootFileInfo;
	KFileItemNode* m_rootNode;

};

#endif // KFILEITEMMODEL_H
