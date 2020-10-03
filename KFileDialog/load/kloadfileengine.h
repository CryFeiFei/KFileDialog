#ifndef KLOADFILE_H
#define KLOADFILE_H

#include <QObject>
#include <QList>

class KFileItemNode;

enum LoadEngineType
{
	LocalType = 0,
	SmbType = 1,
};

class KLoadFileEngine : public QObject
{
	Q_OBJECT
public:
	explicit KLoadFileEngine(QObject *parent = nullptr, LoadEngineType type = LoadEngineType::LocalType);

signals:
	void startLoad();
	void loading(QList<KFileItemNode*> items);
	void loadFinished();

protected:
	LoadEngineType m_loadType;
};

#endif // KLOADFILE_H
