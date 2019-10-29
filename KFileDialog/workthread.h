#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>

class TimerThread : public QObject
{
	Q_OBJECT
public:
	TimerThread(QObject* parent = nullptr);
	~TimerThread();
public:
	void run();
	void doWork();
signals:
	void workStart();
	void workFinished();
	void working();
};

#endif // WORKTHREAD_H
