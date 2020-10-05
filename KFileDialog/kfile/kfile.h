#ifndef KFILE_H
#define KFILE_H

#include <QObject>

class KFile : public QObject
{
	Q_OBJECT
public:
	explicit KFile(QObject *parent = nullptr);

public:
	static bool rename(const QString& oldName, const QString& newName);

signals:

};

#endif // KFILE_H
