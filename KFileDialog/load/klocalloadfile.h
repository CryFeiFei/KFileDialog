/*
CopyRight 2020 CryFeiFei Inc

Author : CryFeiFei

Date :

本地一个Local的应该就够了。要不要搞成danli，我觉得我想多了

*/

#ifndef KLOCALLOADFILE_H
#define KLOCALLOADFILE_H

#include <QObject>
#include "kloadfileengine.h"

class KLocalLoadFile : public KLoadFileEngine
{
	Q_OBJECT
public:
	explicit KLocalLoadFile(QObject *parent = nullptr);


signals:

};

#endif // KLOCALLOADFILE_H
