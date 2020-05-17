#include "kstackwidget.h"

#include "./modelview/kdetailview.h"

#include <QVBoxLayout>

KStackWidget::KStackWidget(QWidget *parent) :
							QStackedWidget(parent)
							,m_detailView(nullptr)
{
	setObjectName("KStackWidget");

	m_detailView = new KDetailView(this);
	addWidget(m_detailView);
}

void KStackWidget::Init(const QString &loadPath, const QStringList &listFilter)
{
	m_loadPath = loadPath;
	m_listFilter = listFilter;

	m_detailView->Init(loadPath, listFilter);
}
