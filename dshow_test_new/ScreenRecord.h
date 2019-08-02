#pragma once

#include <afxinet.h>
#include <dshow.h>
#include <iostream>

#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"Quartz.lib")

class SCREENRECORD
{
public:
	IGraphBuilder *pGraph = NULL;
	ICaptureGraphBuilder2 *pBuild = NULL;
	IBaseFilter *pCap;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;

	IBaseFilter *pMux;
	IConfigAviMux *pConfigMux = NULL;

	HRESULT InitCaptureGraphBuilder(
		IGraphBuilder **ppGraph,  // Receives the pointer.
		ICaptureGraphBuilder2 **ppBuild  // Receives the pointer.
	);

	HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum);

	void DisplayDeviceInformation(IEnumMoniker *pEnum);

	HRESULT SelectDevices(void);

	void StartRecord();

	void CloseInterface();

	void FinishRecord();

	void FtpUpload();
};