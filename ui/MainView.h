#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "globals/CCGlobalStructs.h"
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include "ui/CCMultiView.h"
#include "ui/CCToolBar.h"
#include "ui/CCDictionary.h"
#include <QToolButton>

class CustomLabel;

class MainView : public QWidget
{
	Q_OBJECT

public:
	MainView(QWidget *parent);
	MainView(QGLFormat& format, QWidget* parent, QGLWidget* sharedWidget);
	~MainView();
signals:
	void Operation(OPERATION_TYPE, OPERATION_MODE);
	void LayoutChanged(int r, int c);
	void SetWindowMode(WindowMode);//���ô���ģʽ,�������ź��е��ظ����Ժ��ٸ�
	void UpdateImageInfo(ImageInfo*,int index=0);//index��ʾ���͵���ʾ�������
	void NotifyAcceptThumb(QString, int);
	void Cache4Report(SnapCache*);//���ͽ�������
	void DiagnosisOK(int,QString);//��RetrieveDiagnosis�е���,���ͻ�MultiVIew
	void ClearGLImageView(QList<int>);
	void EnableToolBar(bool);

	void PostImage(QString, CustomLabel*);//���ͽ�ͼλ��

public slots:
	void AcceptSnapshot(QString);
	void change_window_layout();
	void RetrieveDiagnosis(int ind=0,QString str="");//��ȡ�����Ϣ,������MultiView
	void AppendEntries(QString);//��ѡ�д�����ӵ������Ϣ
	void GetImageIndex(QString, CustomLabel*);//��ý�ͼ���
protected:
	void paintEvent(QPaintEvent *e);
	//void resizeEvent(QResizeEvent *);
private:
	void init_window();
private:
	CCToolBar* m_ToolBar;
	CCMultiView* m_MuliView;
	QTextEdit* reportEdit;
	CCDictionary* m_Dictionary;

	QToolButton* m_WindwowChange;
	QToolButton* m_ToolsDropdown;

	WindowMode m_WindowMode;
};

#endif // MAINVIEW_H
