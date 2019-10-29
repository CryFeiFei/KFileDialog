#include "workthread.h"
#include <QTimer>
#include <QDebug>
#include <QTime>


static int timerCount = 0;
TimerThread::TimerThread(QObject* parent): QObject (parent)
{
}
TimerThread::~TimerThread()
{
}
void TimerThread::run()
{
	emit workStart();
	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &TimerThread::doWork);
	timer->start(2000);
}
void TimerThread::doWork()
{
	timerCount ++;
	if (timerCount > 100)
		emit workFinished();
	qDebug()<<QTime::currentTime()<<endl;
	emit working();
}
