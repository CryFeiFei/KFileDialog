#ifndef KFILESYSTEMWATCHER_H
#define KFILESYSTEMWATCHER_H

#include <QObject>

class KFileSystemWatcher : public QObject
{
	Q_OBJECT
public:
	explicit KFileSystemWatcher(QObject *parent = nullptr);

signals:

public slots:
};

#endif // KFILESYSTEMWATCHER_H
