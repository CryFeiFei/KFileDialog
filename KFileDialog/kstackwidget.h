#ifndef KSTACKWIDGET_H
#define KSTACKWIDGET_H

#include <QObject>
#include <QStackedWidget>


class KDetailView;

class KStackWidget : public QStackedWidget
{
	Q_OBJECT
public:
	explicit KStackWidget(QWidget *parent = nullptr);

public:
	void Init(const QString& loadPath, const QStringList& listFilter);

signals:


private:
	QString m_loadPath;
	QStringList m_listFilter;
	KDetailView* m_detailView;
};

#endif // KSTACKWIDGET_H
