#include "workthread.h"
#include <QTimer>
#include <QDebug>
#include <QTime>
#include "kfileitemnode.h"
#include <unistd.h>

static int timerCount = 100000;
WorkThread::WorkThread(const QString& path, QObject* parent): QObject (parent), m_emitIndex(0)
{
	QDir dir(path);
	QStringList listType;
	listType<<"*.*";

	qRegisterMetaType<QList<KFileItemNode*>>("QList<KFileItemNode*>");
	m_fileInfoList = dir.entryInfoList(listType, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
	m_Timer.start();
}
WorkThread::~WorkThread()
{
	qDebug()<<"workThread Destory"<<endl;
}
void WorkThread::run()
{
	emit workStart();

//	QTimer *timer = new QTimer(this);
//	connect(timer, &QTimer::timeout, this, &WorkThread::timeEmit);
//	timer->start(2000);
	doWork();
	emit workFinished();
}

void WorkThread::timeEmit()
{
	qDebug()<<QTime::currentTime()<<endl;
}

void WorkThread::doWork()
{
	static int emitCount = 0;
	QList<KFileItemNode*> nodeList;
	nodeList.clear();
	m_Timer.start();
	for (int i = 0; i < m_fileInfoList.size(); i++)
	{
		QFileInfo fileInfo = m_fileInfoList.at(i);
		QString childFileName = fileInfo.absoluteFilePath();

		KFileItemNode::FileType fileType;
		if (fileInfo.isFile())
		{
			fileType = KFileItemNode::File;
		}
		else if (fileInfo.isDir())
		{
			fileType = KFileItemNode::Folder;
		}
		else
		{
			continue ;
		}

		KFileItemNode* childNode = new KFileItemNode(fileType, childFileName);
		if (childNode)
		{
			emitCount ++;
			m_vecFileItem.append(*childNode);
			nodeList.append(childNode);
		}
//		sleep(2);
		if (emitCount > 10000 || m_Timer.elapsed() > 2000|| m_vecFileItem.count() == m_fileInfoList.count())
		{
			qDebug()<<"!!!!!!!!!!!!!!!!!!!1"<<endl;
			emit working(nodeList);
			m_Timer.start();
			nodeList.clear();
			emitCount = 0;
		}

//		m_vecFileItem.push_back(KFileItemNode(m_fileInfoList));
	}

//	emit workFinished();
//	timerCount ++;
//	if (timerCount > 100)
//		emit workFinished();
////	qDebug()<<QTime::currentTime()<<endl;
//	QFileInfo fileInfo = m_fileInfoList.at(timerCount);
//	QString strFileName = fileInfo.absoluteFilePath();
//	emit working(strFileName);
//	timerCount ++;
}
