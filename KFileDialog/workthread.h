#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QMetaType>
#include <QVector>
#include <QList>

class KFileItemNode;

class WorkThread : public QObject
{
	Q_OBJECT
public:
	WorkThread(const QString& path, QObject* parent = nullptr);
	~WorkThread();
public:
	void run();
	void doWork();
	void timeEmit();
signals:
	void workStart();
	void workFinished();
	void working(QList<KFileItemNode*> fileInfo);

private:
	QFileInfoList m_fileInfoList;

	int m_emitIndex;
	QList<KFileItemNode> m_vecFileItem;
};

#endif // WORKTHREAD_H
