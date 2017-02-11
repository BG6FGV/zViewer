#include "ThumbWidget.h"
#include <QApplication>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QDate>
#include <QMessageBox>
//#include <QPushButton>


ThumbWidget::ThumbWidget(QWidget *parent)
	: QGLWidget(parent)
	//: QScrollArea(parent)
	, m_WindowMode(WindowMode::SingleWindow)
	, pWorkingWidgetHandle(NULL)
	, pDicomUtilsHandle(NULL)
	, pLoader(new CCDataLoader())
	, pLoaderThread(new QThread())
	, textureUploaded(false)
	, m_NewImagePath("")
	, report_hide(false)
{
	setAcceptDrops(true);
	connect(this, SIGNAL(BeginLoading(QList<QUrl>)), pLoader, SLOT(loadDataFromUrls(QList<QUrl>)));
	connect(pLoader, SIGNAL(NewDicomUtils(CCDicomUtils*)), this, SLOT(NewSeriesWindow(CCDicomUtils*)));
	connect(pLoader, SIGNAL(UpdateLoadProgress(QString, int, int, int)), this, SLOT(UpdateProgress(QString, int, int, int)));
	connect(pLoader, SIGNAL(FinishLoading(QString)), this, SLOT(FinishLoading(QString)));
	pLoader->moveToThread(pLoaderThread);
	pLoaderThread->start();

	contextAction = new QAction(QString::fromLocal8Bit("���ر���༭"),this);
	connect(contextAction, SIGNAL(triggered()), this, SLOT(relay_window()));

	setAutoFillBackground(true);
}

ThumbWidget::ThumbWidget(QGLFormat &format, QWidget *parent)
	:QGLWidget(format,parent)
	//: QScrollArea(parent)
	, m_WindowMode(WindowMode::SingleWindow)
	, pWorkingWidgetHandle(NULL)
	, pDicomUtilsHandle(NULL)
	, pLoader(new CCDataLoader())
	, pLoaderThread(new QThread())
	, textureUploaded(false)
	, m_NewImagePath("")
	, report_hide(false)
{
	setAcceptDrops(true);
	connect(this, SIGNAL(BeginLoading(QList<QUrl>)), pLoader, SLOT(loadDataFromUrls(QList<QUrl>)));
	connect(this, SIGNAL(BeginLoading2(QList<QString>)), pLoader, SLOT(loadDataFromUrls2(QList<QString>)));
	connect(pLoader, SIGNAL(NewDicomUtils(CCDicomUtils*)), this, SLOT(NewSeriesWindow(CCDicomUtils*)));
	connect(pLoader, SIGNAL(UpdateLoadProgress(QString, int, int, int)), this, SLOT(UpdateProgress(QString, int, int, int)));
	connect(pLoader, SIGNAL(FinishLoading(QString)), this, SLOT(FinishLoading(QString)));
	pLoader->moveToThread(pLoaderThread);
	pLoaderThread->start();

	contextAction = new QAction(QString::fromLocal8Bit("���ر���༭"), this);
	connect(contextAction, SIGNAL(triggered()), this, SLOT(relay_window()));

	setAutoFillBackground(true);
}

ThumbWidget::~ThumbWidget()
{

}

void ThumbWidget::init_glew()
{
}

void ThumbWidget::init_window()
{
	mainLayout = new QVBoxLayout;
	mainLayout->setMargin(0);
	this->setLayout(mainLayout);
}

void ThumbWidget::initializeGL()
{
	//init_glew();
	init_window();
}

void ThumbWidget::resizeGL(int w, int h)
{

}

void ThumbWidget::paintGL()
{
	//glClearColor(0.0, 22.0/255.0, 44.0/255.0, 1.0);
	glClearColor(0.071, 0.071, 0.071, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//do not forget
}

void ThumbWidget::mousePressEvent(QMouseEvent *event)
{
	
}

void ThumbWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
		
}

void ThumbWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;

	if (m_seriesWidgetMap.size() >= 3)
	{
		QMessageBox::information(NULL, QString::fromStdWString(L"��ʾ"),
			QString::fromStdWString(L"�ѳ�����ǰ����ӵ����������Ŀ��"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
		
	emit BeginLoading(urls);

	event->acceptProposedAction();
}

void ThumbWidget::NewSeriesWindow(CCDicomUtils* pUtils)
{
	m_dicomUtilsMap[/*pUtils->seriesInstanceId()*/pUtils->getIndex()] = pUtils;//����map
	add_series(/*pUtils->seriesInstanceId()*/pUtils->getIndex());//��id��ΪMap���
	m_NewImagePath = pUtils->getAbsoluteImagePath();
	emit NewImagePathUpdate(m_NewImagePath);
}

void ThumbWidget::UpdateProgress(QString name, int r0, int r1, int pos)
{
	m_seriesWidgetMap[name]->setLoadProgress(r0, r1, pos);
}

void ThumbWidget::FinishLoading(QString name)
{
	
}

void ThumbWidget::CheckReadyForUpdate(QList<QUrl> list)
{
	if (m_seriesWidgetMap.size() >= 3)
	{
		QMessageBox::information(NULL, QString::fromStdWString(L"��ʾ"),
			QString::fromStdWString(L"�ѳ�����ǰ����ӵ����������Ŀ��"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
		
	emit BeginLoading(list);
}

void ThumbWidget::CheckReadyForUpdate2(QList<QString> list)
{
	if (m_seriesWidgetMap.size() >= 3)
	{
		QMessageBox::information(NULL, QString::fromStdWString(L"��ʾ"),
			QString::fromStdWString(L"�ѳ�����ǰ����ӵ����������Ŀ��"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}

	emit BeginLoading2(list);
}

void ThumbWidget::update_texture(QString name, int index)
{
	//����Info
	ImageInfo* imageInfo = new ImageInfo;
	imageInfo->pData = m_dicomUtilsMap[name]->data();
	imageInfo->width = m_dicomUtilsMap[name]->width();
	imageInfo->height = m_dicomUtilsMap[name]->height();
	imageInfo->depth = m_dicomUtilsMap[name]->depth();
	imageInfo->window_center = m_dicomUtilsMap[name]->windowCenter();
	imageInfo->window_width = m_dicomUtilsMap[name]->windowWidth();
	imageInfo->mingray = m_dicomUtilsMap[name]->minPixelValue();
	imageInfo->maxgray = m_dicomUtilsMap[name]->maxPixelValue();

	//�浽map////////////////////////////////////////////////////////////////////
	//���Ƚ��index���Ӧ��series�İ�
	QMultiMap<QString, int>::iterator it = m_ModelViewMap.begin();
	bool flag = false;
	QString key_str = "";
	for (it; it != m_ModelViewMap.end(); ++it)
	{
		if (it.value() == index)
		{
			key_str = it.key();
			break;
		}
	}
	if (key_str != "")
		m_ModelViewMap.remove(key_str, index);
	m_ModelViewMap.insert(name, index);	//�浽map////////

	emit UpdateImageInfo(imageInfo,index);
}

void ThumbWidget::get_petinfo(QString name)
{
	BasicInfo* bi = new BasicInfo;
	bi->id = m_dicomUtilsMap[name]->patientId();
	bi->name = m_dicomUtilsMap[name]->patientName();
	bi->sex = m_dicomUtilsMap[name]->patientSex();
	QString birth = m_dicomUtilsMap[name]->patientBirthday();
	int thisyear = QDate::currentDate().year();
	bi->age = (thisyear - birth.left(4).toInt() <= 0) ? 1 : (thisyear - birth.left(4).toInt());
	bi->body = m_dicomUtilsMap[name]->studyBodyPart();
	bi->date = m_dicomUtilsMap[name]->studyDate();
	bi->modality = m_dicomUtilsMap[name]->seriesModality();
	emit PostBasicInfo(bi);

}

void ThumbWidget::PrepareImageInfo(QString para1, int para2)
{
	if (!textureUploaded)
	{
		textureUploaded = true;//��־����image view������
		emit EnableToolBar(true);
	}
	update_texture(para1, para2);
	get_petinfo(para1);
	//do something
	//......
}

void ThumbWidget::add_series(QString name)
{
	CCSeriesWidget* widget = new CCSeriesWidget(this, name, m_WindowMode);
	widget->setSeriesInfo("", m_dicomUtilsMap[name]->studyDateTime(),
							m_dicomUtilsMap[name]->seriesModality()+": "+
							m_dicomUtilsMap[name]->seriesDescription());
	widget->setImageData(m_dicomUtilsMap[name]->thumbData(), 
						m_dicomUtilsMap[name]->alignWidth(),
						m_dicomUtilsMap[name]->height());
	connect(widget, SIGNAL(SetCheckedState(QString)), this, SLOT(UpdateCheckedSeries(QString)));
	connect(widget, SIGNAL(DeleteSeries(QString)), this, SLOT(DeleteSeries(QString)));

	if (m_seriesWidgetMap.empty())
	{
		mainLayout->insertWidget(0,widget);
		mainLayout->addStretch(5000);
	}
	else
	{
		mainLayout->insertWidget(m_seriesWidgetMap.size(), widget);
	}
	m_seriesWidgetMap[name] = widget;
	mainLayout->setSpacing(0);
	this->setLayout(mainLayout);
}

void ThumbWidget::DeleteSeries(QString name)
{
	CCSeriesWidget* dying_widget = m_seriesWidgetMap[name];
	mainLayout->removeWidget(dying_widget);
	dying_widget->setGeometry(0, 0, 0, 0);
	delete dying_widget; dying_widget = NULL;
	m_seriesWidgetMap.remove(name);

	//��ѯmap////////////////////////////////////////////////////////////////////
	QList<int> list = m_ModelViewMap.values(name);
	//��ѯmap////////////////////////////////////////////////////////////////////
	emit ClearGLImageView(list);//���ImageView

	//ɾ��DicomUtilsʵ��
	CCDicomUtils* pUtils = m_dicomUtilsMap[name];
	delete pUtils; pUtils = NULL;
	m_dicomUtilsMap.remove(name);

	if (m_seriesWidgetMap.empty())
	{
		textureUploaded = false;
		emit EnableToolBar(false);
	}
}

//ֻ����SeriesWIdget��������źţ��жϴ���ģʽ�Ĳ�����SeriesWidget����ɣ��˴����ж�
void ThumbWidget::UpdateCheckedSeries(QString name)
{
	QMap<QString, CCSeriesWidget*>::iterator it = m_seriesWidgetMap.begin();
	for (it; it != m_seriesWidgetMap.end(); ++it)
	{
		if (it.key() == name)
			continue;
		it.value()->cancelCheckState();
	}
}

void ThumbWidget::SetWindowMode(WindowMode mode)
{
	//�л�����ģʽʱ��ͬ�����Model(Series��id)��View(ImageView��id)����
	//���map////////////////////////////////////////////////////////////////////
	if (!m_ModelViewMap.empty())
		m_ModelViewMap.clear();
	//���map////////////////////////////////////////////////////////////////////
	
	textureUploaded = false;
	emit EnableToolBar(false);

	this->m_WindowMode = mode;
	QMap<QString, CCSeriesWidget*>::iterator it = m_seriesWidgetMap.begin();
	for (it; it != m_seriesWidgetMap.end(); ++it)
	{
		it.value()->resetWindowMode(mode);
	}
	
}

void ThumbWidget::relayout()
{
	if (m_seriesWidgetMap.isEmpty())
		return;
	QMap<QString, CCSeriesWidget*>::iterator it = m_seriesWidgetMap.begin();
	for (it; it != m_seriesWidgetMap.end(); ++it)
	{
		mainLayout->addWidget(it.value());
	}
	mainLayout->setSpacing(0);
	mainLayout->addStretch();
}

void ThumbWidget::contextMenuEvent(QContextMenuEvent *) //�Ҽ��˵���༭
{
	QCursor cur = this->cursor();
	QMenu *menu = new QMenu(this);
	report_hide = !report_hide;
	if (!report_hide)
	{
		contextAction->setText(QString::fromLocal8Bit("��ʾ����༭"));
	}
	else
	{
		contextAction->setText(QString::fromLocal8Bit("���ر���༭"));
	}
	menu->setStyleSheet("QMenu{background-color:#999900;}");
	menu->addAction(contextAction); //��Ӳ˵���1
	menu->exec(cur.pos()); //���������
	delete menu;
}

void ThumbWidget::relay_window()
{
	
	
	emit RelayWindow();
	
}