#include "zviewer.h"

#include <windows.h>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QHBoxLayout>

#define TOOLBUTTON_SIZE 45

zViewer::zViewer(int w, int h, int l, int t, QWidget* parent, Qt::WindowFlags)
:dragMode(false)
{
	this->setAttribute(Qt::WA_QuitOnClose, true);
	screenWidth = w;
	screenHeight = h;
	clientLeft = l;
	clientTop = t;
	maxsizeRect.setWidth(screenWidth);
	maxsizeRect.setHeight(screenHeight);
	maxsizeRect.setLeft(clientLeft);
	maxsizeRect.setTop(clientTop);
	normalRect.setWidth(screenWidth / 3 * 2);
	normalRect.setHeight(screenHeight / 3 * 2);
	normalRect.setLeft((screenWidth) / 6);
	normalRect.setTop((screenHeight) / 6);
	windowSizeNormal = false;//�����Ƿ�������С
	windowIsMin = false;
	dragMode = false;

	QStatusBar *statusbar = statusBar();
	statusbar->setMaximumHeight(20);
	statusbar->setStyleSheet("QStatusBar{background:#313131;}");
	statusbar->show();

	setWindowIcon(QIcon(":/images/icon.png"));
	this->setWindowTitle("zViewer");

	m_mainWidget = new CCMainWidget(this);
	setCentralWidget(m_mainWidget);

	watcher = new QFileSystemWatcher;
	watcher->addPath("./style.qss");
	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(ResetApplicationSkin(QString)));
	connect(m_mainWidget, SIGNAL(CloseWindow()), this, SLOT(close()));

}

zViewer::~zViewer()
{

}

void zViewer::createTools()
{
	/*file related*/
	file_open_btn = new QToolButton(this);
	file_open_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	file_open_btn->setObjectName("file_open_btn");
	file_open_btn->setToolTip(QString::fromStdWString(L"��"));

	file_close_btn = new QToolButton(this);
	file_close_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	file_close_btn->setObjectName("file_close_btn");
	file_close_btn->setToolTip(QString::fromStdWString(L"�ر�"));

	file_save_btn = new QToolButton(this);
	file_save_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	file_save_btn->setObjectName("file_save_btn");
	file_save_btn->setToolTip(QString::fromStdWString(L"����"));

	file_print_btn = new QToolButton(this);
	file_print_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	file_print_btn->setObjectName("file_print_btn");
	file_print_btn->setToolTip(QString::fromStdWString(L"��ӡ"));

	file_report_btn = new QToolButton(this);
	file_report_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	file_report_btn->setObjectName("file_report_btn");
	file_report_btn->setToolTip(QString::fromStdWString(L"���ɱ���"));

	/*adjust related*/
	adjust_zoomin_btn = new QToolButton(this);
	adjust_zoomin_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	adjust_zoomin_btn->setObjectName("adjust_zoomin_btn");
	adjust_zoomin_btn->setToolTip(QString::fromStdWString(L"�Ŵ�"));

	adjust_zoomout_btn = new QToolButton(this);
	adjust_zoomout_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	adjust_zoomout_btn->setObjectName("adjust_zoomout_btn");
	adjust_zoomout_btn->setToolTip(QString::fromStdWString(L"��С"));

	adjust_window_btn = new QToolButton(this);
	adjust_window_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	adjust_window_btn->setObjectName("adjust_window_btn");
	adjust_window_btn->setToolTip(QString::fromStdWString(L"����/��λ"));

	adjust_undo_btn = new QToolButton(this);
	adjust_undo_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	adjust_undo_btn->setObjectName("adjust_undo_btn");
	adjust_undo_btn->setToolTip(QString::fromStdWString(L"�ָ���ʼ"));

	/*measure related*/
	measure_length_btn = new QToolButton(this);
	measure_length_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	measure_length_btn->setObjectName("measure_length_btn");
	measure_length_btn->setToolTip(QString::fromStdWString(L"���Ȳ���"));

	measure_angle_btn = new QToolButton(this);
	measure_angle_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	measure_angle_btn->setObjectName("measure_angle_btn");
	measure_angle_btn->setToolTip(QString::fromStdWString(L"�ǶȲ���"));

	measure_area_btn = new QToolButton(this);
	measure_area_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	measure_area_btn->setObjectName("measure_area_btn");
	measure_area_btn->setToolTip(QString::fromStdWString(L"�������"));

	measure_area2_btn = new QToolButton(this);
	measure_area2_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	measure_area2_btn->setObjectName("measure_area2_btn");
	measure_area2_btn->setToolTip(QString::fromStdWString(L"�������"));

	/*label related*/
	text_pencil_btn = new QToolButton(this);
	text_pencil_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	text_pencil_btn->setObjectName("text_pencil_btn");
	text_pencil_btn->setToolTip(QString::fromStdWString(L"�ֶ�����"));

	text_arrow_btn = new QToolButton(this);
	text_arrow_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	text_arrow_btn->setObjectName("text_arrow_btn");
	text_arrow_btn->setToolTip(QString::fromStdWString(L"��ͷ���"));

	text_showhide_btn = new QToolButton(this);
	text_showhide_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	text_showhide_btn->setObjectName("text_showhide_btn");
	text_showhide_btn->setToolTip(QString::fromStdWString(L"���ر��"));

	/*tools related*/
	tools_screenshot_btn = new QToolButton(this);
	tools_screenshot_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	tools_screenshot_btn->setObjectName("tools_screenshot_btn");
	tools_screenshot_btn->setToolTip(QString::fromStdWString(L"��ͼ"));

	tools_connect_btn = new QToolButton(this);
	tools_connect_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	tools_connect_btn->setObjectName("tools_connect_btn");
	tools_connect_btn->setToolTip(QString::fromStdWString(L"����PACS������"));

	tools_user_btn = new QToolButton(this);
	tools_user_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	tools_user_btn->setObjectName("tools_user_btn");
	tools_user_btn->setToolTip(QString::fromStdWString(L"ҽ��"));

	/*software related*/
	others_about_btn = new QToolButton(this);
	others_about_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	others_about_btn->setObjectName("others_about_btn");
	others_about_btn->setToolTip(QString::fromStdWString(L"����"));

	others_setting_btn = new QToolButton(this);
	others_setting_btn->setFixedSize(TOOLBUTTON_SIZE, TOOLBUTTON_SIZE);
	others_setting_btn->setObjectName("others_setting_btn");
	others_setting_btn->setToolTip(QString::fromStdWString(L"����"));

	////���
	file_tools = addToolBar("file");
	file_tools->addWidget(file_open_btn);
	file_tools->addWidget(file_close_btn);
	file_tools->addWidget(file_save_btn);
	file_tools->addWidget(file_print_btn);
	//file_tools->addWidget(file_report_btn);

	measure_tools = addToolBar("measure");
	measure_tools->addWidget(measure_length_btn);
	measure_tools->addWidget(measure_angle_btn);
	measure_tools->addWidget(measure_area_btn);
	measure_tools->addWidget(measure_area2_btn);

	adjust_tools = addToolBar("adjust");
	adjust_tools->addWidget(adjust_zoomin_btn);
	adjust_tools->addWidget(adjust_zoomout_btn);
	adjust_tools->addWidget(adjust_window_btn);
	adjust_tools->addWidget(adjust_undo_btn);

	text_tools = addToolBar("label");
	text_tools->addWidget(text_pencil_btn);
	text_tools->addWidget(text_arrow_btn);
	text_tools->addWidget(text_showhide_btn);

	tool_tools = addToolBar("tool");
	tool_tools->addWidget(tools_connect_btn);
	tool_tools->addWidget(tools_screenshot_btn);
	tool_tools->addWidget(file_report_btn);//�Ƶ��˴�
	tool_tools->addWidget(tools_user_btn);

	software_tools = addToolBar("software");
	software_tools->addWidget(others_about_btn);
	software_tools->addWidget(others_setting_btn);

}

void zViewer::ResetApplicationSkin(QString path)
{
	QFile styleFile("./style.qss");
	styleFile.open(QIODevice::ReadOnly);
	QString setStyleSheet(styleFile.readAll());
	qApp->setStyleSheet(setStyleSheet);
}

void zViewer::mousePressEvent(QMouseEvent *e)
{
	last = e->globalPos();
	dragMode = true;
}

void zViewer::mouseReleaseEvent(QMouseEvent *e)
{
	dragMode = false;
}

void zViewer::mouseMoveEvent(QMouseEvent *e)
{
	if (dragMode)
	{
		int dx = e->globalX() - last.x();
		int dy = e->globalY() - last.y();
		last = e->globalPos();
		move(x() + dx, y() + dy);
	}
}

bool zViewer::eventFilter(QObject *obj, QEvent *ev)
{
	POINT p;
	QPoint pp;
	QMouseEvent *event;
	switch (ev->type())
	{
	case QEvent::MouseButtonPress:
		setMouseTracking(true);//ʮ�ֱ�Ҫ����������ƶ���Ϣ
		GetCursorPos(&p);
		pp.setX(p.x);
		pp.setY(p.y);
		event = new QMouseEvent(QEvent::MouseButtonPress, pp, Qt::LeftButton, 0, 0);
		QApplication::sendEvent(this, event);
		break;
	case QEvent::MouseButtonRelease:
		setMouseTracking(false);
		GetCursorPos(&p);
		pp.setX(p.x);
		pp.setY(p.y);
		event = new QMouseEvent(QEvent::MouseButtonRelease, pp, Qt::LeftButton, 0, 0);
		QApplication::sendEvent(this, event);
		break;
	case QEvent::MouseMove:
		//if (obj->objectName() == "CCTabHeaderWidget")
		//{
		GetCursorPos(&p);
		pp.setX(p.x);
		pp.setY(p.y);
		event = new QMouseEvent(QEvent::MouseMove, pp, Qt::NoButton, 0, 0);//�ο�API������3������0����Qt::NoButton
		QApplication::sendEvent(this, event);
		//}
		break;
	default:
		break;
	}
	return false;
}

void zViewer::close()
{
	QMessageBox message(QMessageBox::Warning, QString::fromStdWString(L"ע��"),
		QString::fromStdWString(L"�رճ�����ջ���ͼ��ȷ���رգ�"),
		QMessageBox::Yes | QMessageBox::No, NULL);
	if (message.exec() == QMessageBox::No)
	{
		return;
	}
	//��ͼ����ɾ��
	QDir dir("snapshot");
	QStringList list = dir.entryList(QDir::Files);
	for (int i = 0; i < list.size(); ++i)
	{
		QString full_path = dir.absoluteFilePath(list.at(i));
		QFile::remove(full_path);
	}

	//����Dataɱ��
	removeFolderContent("Data");
	removeFolderContent("Upload");
	removeFolderContent("cache");

	QMainWindow::close();
}

bool zViewer::removeFolderContent(const QString &folderDir)
{
	QDir dir(folderDir);
	QFileInfoList fileList;
	QFileInfo curFile;
	if (!dir.exists())  { return false; }//�ļ����棬�򷵻�false
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
		| QDir::Readable | QDir::Writable
		| QDir::Hidden | QDir::NoDotAndDotDot
		, QDir::Name);
	while (fileList.size() > 0)//��������
	{
		int infoNum = fileList.size();
		for (int i = infoNum - 1; i >= 0; i--)
		{
			curFile = fileList[i];
			if (curFile.isFile())//������ļ���ɾ���ļ�
			{
				QFile fileTemp(curFile.filePath());
				fileTemp.remove();
				fileList.removeAt(i);
			}
			if (curFile.isDir())//������ļ���
			{
				QDir dirTemp(curFile.filePath());
				QFileInfoList fileList1 = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
					| QDir::Readable | QDir::Writable
					| QDir::Hidden | QDir::NoDotAndDotDot
					, QDir::Name);
				if (fileList1.size() == 0)//�²�û���ļ����ļ���
				{
					dirTemp.rmdir(".");
					fileList.removeAt(i);
				}
				else//�²����ļ��л��ļ�
				{
					for (int j = 0; j < fileList1.size(); j++)
					{
						if (!(fileList.contains(fileList1[j])))
							fileList.append(fileList1[j]);
					}
				}
			}
		}
	}
	return true;
}