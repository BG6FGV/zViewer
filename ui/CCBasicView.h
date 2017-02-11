#ifndef CCBASICVIEW_H
#define CCBASICVIEW_H

#include "globals/CCGlobalStructs.h"
#include "cells/Line.h"
#include "cells/Rectangle.h"
#include "cells/Ellipse.h"
//#include "cells/TextRenderer.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QList>


class CCBasicView : public QGLWidget
{
	Q_OBJECT

public:
	CCBasicView(QWidget* parent, QGLWidget* sharedWidget = 0);
	CCBasicView(QGLFormat& format, QWidget* parent, QGLWidget* sharedWidget = 0);
	~CCBasicView();

	virtual unsigned char* snap(int &w, int &h);//������ͼ�񣬷���ͼ�����ݣ�w, h Ϊ��������
	virtual QString snap2();//����ͼ�񣬷��ر���·��

protected:
	virtual void initializeGL();
	virtual void resizeGL(int width, int height);
	virtual void paintGL();

	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void wheelEvent(QWheelEvent *);
protected:
	virtual void draw();
	virtual void showFocus();
	virtual void createAreaObject(AreaToolStyle style);
	virtual QString screen_shot(QString path);
	virtual void write2file(int w, int h, unsigned char* pData, int bitdepth = 8);
protected:
	glm::mat4 P;
	glm::mat4 MV;
	float hw_ratio;
	int window_width;
	int window_height;
	float window_hw_ratio;
	float global_scale_ratio;

	float DeltaX;//ƫ��
	float DeltaY;
	glm::vec2 panOldPoint;
	QPoint dragStartPosition;
	glm::vec4 original_window_border;

	//����״̬
	bool DragMode;
	bool ZoomMode;
	bool BrightnessMode;
	bool RectangleMode;
	bool EllipseMode;
	bool LengthMode;
	bool AngleMode;
	bool ArrowMode;
	bool PencilMode;
	bool TextMode;

	//������ʾ����
	CLine* m_LineRender;
	CRectangle* m_RectRender;
	CEllipse* m_EllipseRender;
	//TextRenderer* m_TextRender;

	//�����б�
	AreaObject* m_ActiveAreaObject;
	QList<AreaObject*> m_AreaObjectList;

	DrawingMode m_DrawMode;//����״̬
	//�����ʾ״̬
	bool ShowAreaObjects;

	//others
	glm::vec3 m_CornerPoint[4];//
};

#endif // CCBASICVIEW_H
