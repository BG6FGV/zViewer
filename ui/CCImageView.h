#ifndef CCImageView_H
#define CCImageView_H

#include "logic/GLSLShader.h"
#include "globals/CCGlobalStructs.h"
#include "CCBasicView.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class CCImageView : public CCBasicView
{
	Q_OBJECT

public:
	CCImageView(QWidget *parent, int index=0);
	CCImageView(QGLFormat& format, QWidget* parent, int index = 0, QGLWidget* sharedWidget = 0);
	CCImageView(QWidget* parent, int index, QGLWidget* sharedWidget = 0);
	~CCImageView();
signals:
	void NotifyAcceptThumb(QString name, int index); //nameΪ����Ψһ��ʶ, indexΪ����Ψһ��ʶ
	void FocusChanged(int ind=0,QString str="");//������ǰ����index�������Ϣ
public:
	inline void setFocus(bool b){ inFocus = b; updateGL(); }
	inline void setDiagnosis(QString str) { diagnosis_string = str; }
	void recycle_texture();//1.���µ���texture��2.����ɾ������ִ�д˺���
public slots:
	void UpdateImageInfo(ImageInfo*,int index=0);
	void SetOperationMode(OPERATION_TYPE, OPERATION_MODE);
	void ShowCornerInfo();
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *);

	virtual void dragEnterEvent(QDragEnterEvent *);
	virtual void dropEvent(QDropEvent *);
	//void paintEvent(QPaintEvent*);
private:
	void draw();
	void init_image();
	void updateSlice(bool next, int step=1);//next=true,�·�ҳ��step��ҳ��Ŀ
private:
	int index;
	int current_image_index;//����ͼ��ĵ�ǰ���,0 start
	bool inFocus;
	QString diagnosis_string;//�����Ϣ
	bool textureExists;
	bool shaderExists;
	ImageInfo* pImage;
	// shader related
	GLSLShader shader;
	GLuint vaoID;
	GLuint vboVerticesID;
	GLuint vboIndicesID;
	GLuint vboTextureID;
	Texture2D* texture;
	GLuint textureID;

	//triangle vertices and indices
	glm::vec2 vertices[4];
	glm::vec2 tcoord[4];
	GLushort indices[6];

	glm::vec2 adjust_grayscale_point;

};

#endif // CCImageView_H
