#include "kfile.h"
#include <QFile>

KFile::KFile(QObject *parent) : QObject(parent)
{

}

bool KFile::rename(const QString& oldName, const QString& newName)
{
	return QFile::rename(oldName, newName);
}
