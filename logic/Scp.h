#pragma once

#include <QObject>
#include "dcmtk\dcmnet\dstorscp.h"

class CCScp : public QObject, public DcmStorageSCP// ע��̳е�˳��
{
	Q_OBJECT
public:
	struct DefaultAssociationParameter
	{
		OFString myAETitle;
		OFString peerAETitle;
		OFString myHostName;
		UINT16 listeningPort;
		DefaultAssociationParameter()
		{
			myHostName = "127.0.0.1";
			listeningPort = 2015;
			myAETitle = "VET";
			peerAETitle = "COMMON";
		}
	}m_DefaultAssociationParams;
public:
	CCScp();
	~CCScp();
	void init_association_parameter(OFString& title,UINT16 port);

	void SetRoot(const OFString& dir) { rootDir = dir; }//DICOM��Ÿ�Ŀ¼
	void SetSubDir(const OFString& dir);//����DICOM�Ӽ�Ŀ¼
	OFString getRoot(){ return rootDir; }
	OFString getSubDir(){ return subDir; }
	OFString getAbsoluteDir(){ return absoluteDir; }

signals:
	void OneInstanceStored() const;
	void AssociationReleased();

protected:
	void notifyRECEIVEProgress(const unsigned long byteCount);
	//OFBool stopAfterCurrentAssociation();
	void notifyReleaseRequest();
	void notifyInstanceStored(const OFString &filename,
		const OFString &sopClassUID,
		const OFString &sopInstanceUID,
		DcmDataset *dataset = NULL) const;
private:
	void init();

private:
	OFList< OFString > transferSyntaxes;

	OFString rootDir;
	OFString subDir;
	OFString absoluteDir;
};

