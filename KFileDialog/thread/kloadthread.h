#ifndef KloadThread_H
#define KloadThread_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QMetaType>
#include <QVector>
#include <QList>
#include <QElapsedTimer>

class KFileItemNode;

class KloadThread : public QObject
{
	Q_OBJECT
public:
	KloadThread(const QString& path, QObject* parent = nullptr);
	~KloadThread();
public:
	void run();
	void doWork();
	void timeEmit();
signals:
	void workStart();
	void workFinished();
	void workDestory();
	void working(QList<KFileItemNode*> fileInfo);

public slots:
	void stopLoad();

private:
	QElapsedTimer m_Timer;
	QFileInfoList m_fileInfoList;

	int m_emitIndex;
	QList<KFileItemNode> m_vecFileItem;

	bool m_stopLoad;
	int m_loadCount;
};

#endif // KloadThread_H
