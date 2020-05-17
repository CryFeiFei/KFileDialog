#include <QTimer>
#include <QDebug>
#include <QTime>
#include "klocalloadthread.h"

#include "../kfileitemnode.h"

KLocalLoadThread::KLocalLoadThread(QObject* parent):
	QObject (parent)
	, m_stopLoad(false)
	, m_loadCount(0)
{
	qRegisterMetaType<QList<KFileItemNode*>>("QList<KFileItemNode*>");
}
KLocalLoadThread::~KLocalLoadThread()
{
	qDebug()<<"KLocalLoadThread Destory"<<endl;
}

void KLocalLoadThread::Init(const QString &path, const QStringList &filter)
{
	m_fileInfoList.clear();
	m_loadCount = 0;
	m_vecFileItem.clear();

	m_rootPath = path;
	m_listFilter = filter;

	QDir dir(path);
//	QStringList filterListType;
//	filterListType<<"*.*";
	m_fileInfoList = dir.entryInfoList(filter, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
}

void KLocalLoadThread::run()
{
	emit workStart();

//	QTimer *timer = new QTimer(this);
//	connect(timer, &QTimer::timeout, this, &KLocalLoadThread::timeEmit);
//	timer->start(2000);
	doWork();
	qDebug()<<"emit workFinished"<<endl;
	emit workFinished();
}

void KLocalLoadThread::timeEmit()
{
	qDebug()<<QTime::currentTime()<<endl;
}

void KLocalLoadThread::stopLoad(bool stop/* = true*/)
{
	m_stopLoad = stop;
}

void KLocalLoadThread::doWork()
{
	QList<KFileItemNode*> nodeList;
	nodeList.clear();
	m_Timer.start();
	for (int i = 0; i < m_fileInfoList.size(); i++)
	{
		if (m_stopLoad)
			goto KS_EXIT;

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
			m_loadCount ++;
			m_vecFileItem.append(*childNode);
			nodeList.append(childNode);
		}
//		sleep(2);
		if (m_loadCount > 10000 || m_Timer.elapsed() > 2000|| m_vecFileItem.count() == m_fileInfoList.count())
		{
			qDebug()<<"!!!!!!!!!!!!!!!!!!!1"<<endl;
			emit working(nodeList);
			m_Timer.start();
			nodeList.clear();
			m_loadCount = 0;
		}

//		m_vecFileItem.push_back(KFileItemNode(m_fileInfoList));
	}

KS_EXIT:
	m_stopLoad = false;
	m_loadCount = 0;
	return;
}
