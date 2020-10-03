
#include "kloadfileengine.h"
#include "../kfileitemnode.h"

KLoadFile::KLoadFile(QObject *parent, LoadType type/* = LoadType::LocalType*/) :
		QObject(parent), m_loadType(type)
{
	qRegisterMetaType<QList<KFileItemNode*>>("QList<KFileItemNode*>");
}
