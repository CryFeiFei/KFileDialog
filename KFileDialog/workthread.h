#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QMetaType>


class TimerThread : public QObject
{
	Q_OBJECT
public:
	TimerThread(const QString& path, QObject* parent = nullptr);
	~TimerThread();
public:
	void run();
	void doWork();
signals:
	void workStart();
	void workFinished();
	void working(QString& fileInfo);

private:
	QFileInfoList m_fileInfoList;
};

#endif // WORKTHREAD_H
