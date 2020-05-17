#ifndef KLocalLoadThread_H
#define KLocalLoadThread_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QMetaType>
#include <QVector>
#include <QList>
#include <QElapsedTimer>

class KFileItemNode;

class KLocalLoadThread : public QObject
{
	Q_OBJECT
public:
	KLocalLoadThread(QObject* parent = nullptr);
	~KLocalLoadThread();

public:
	void Init(const QString& path, const QStringList& filter);

public slots:
	void run();

public:

	void doWork();
	void timeEmit();
signals:
	void workStart();
	void workFinished();
	void workDestory();
	void working(QList<KFileItemNode*> fileInfo);

public slots:
	void stopLoad(bool stop = true);

private:
	//resource
	QFileInfoList m_fileInfoList;
	QStringList m_listFilter;
	QString m_rootPath;

	QElapsedTimer m_Timer;
	QList<KFileItemNode> m_vecFileItem; //用于统计是否读取完成

	bool m_stopLoad;
	int m_loadCount;
};

#endif // KLocalLoadThread_H
