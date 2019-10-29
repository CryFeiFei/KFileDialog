#include "workthread.h"
#include <QTimer>
#include <QDebug>
#include <QTime>


static int timerCount = 11;
TimerThread::TimerThread(const QString& path, QObject* parent): QObject (parent)
{
	QDir dir(path);
	QStringList listType;
	listType<<"*.*";

	m_fileInfoList = dir.entryInfoList(listType, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);

	qRegisterMetaType<QString>("QString");
//	m_fileCount = m_fileInfoList.size();
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
//	timerCount ++;
	if (timerCount > 100)
		emit workFinished();
//	qDebug()<<QTime::currentTime()<<endl;
	QFileInfo fileInfo = m_fileInfoList.at(timerCount);
	QString strFileName = fileInfo.absoluteFilePath();
	emit working(strFileName);
	timerCount ++;
}
