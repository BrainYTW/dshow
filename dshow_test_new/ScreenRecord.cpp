#include "ScreenRecord.h"

HRESULT SCREENRECORD::InitCaptureGraphBuilder(
	IGraphBuilder **ppGraph,  // Receives the pointer.
	ICaptureGraphBuilder2 **ppBuild  // Receives the pointer.
)
{
	if (!ppGraph || !ppBuild)
	{
		return E_POINTER;
	}

	// Create the Capture Graph Builder.
	HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL,
		CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pBuild);
	if (FAILED(hr))
	{
		return hr;
	}
	if (SUCCEEDED(hr))
	{
		// Create the Filter Graph Manager.
		hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder, (void**)&pGraph);
		if (FAILED(hr))
		{
			return hr;
		}
		if (SUCCEEDED(hr))
		{
			// Initialize the Capture Graph Builder.
			pBuild->SetFiltergraph(pGraph);

			// Return both interface pointers to the caller.
			*ppBuild = pBuild;
			*ppGraph = pGraph; // The caller must release both interfaces.
			return S_OK;
		}
		else
		{
			pBuild->Release();
		}
	}
	return hr; // Failed
}

HRESULT SCREENRECORD::EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
	// Create the System Device Enumerator.
	ICreateDevEnum *pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
		pDevEnum->Release();
	}
	return hr;
}

void SCREENRECORD::DisplayDeviceInformation(IEnumMoniker *pEnum)
{
	IMoniker *pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		IPropertyBag *pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;
		}

		VARIANT var;
		VariantInit(&var);

		// Get description or friendly name.
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			printf("%S\n", var.bstrVal);
			VariantClear(&var);
		}

		hr = pPropBag->Write(L"FriendlyName", &var);

		// WaveInID applies only to audio capture devices.
		hr = pPropBag->Read(L"WaveInID", &var, 0);
		if (SUCCEEDED(hr))
		{
			printf("WaveIn ID: %d\n", var.lVal);
			VariantClear(&var);
		}

		hr = pPropBag->Read(L"DevicePath", &var, 0);
		if (SUCCEEDED(hr))
		{
			// The device path is not intended for display.
			printf("Device path: %S\n", var.bstrVal);
			VariantClear(&var);
		}

		hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
		if (SUCCEEDED(hr))
		{
			hr = pGraph->AddFilter(pCap, L"Capture Filter");
		}

		pPropBag->Release();
		pMoniker->Release();
	}
}

HRESULT SCREENRECORD::SelectDevices(void)
{
	IEnumMoniker *pEnum;
	HRESULT hr;
	hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);//funtion 1
	if (SUCCEEDED(hr))
	{
		DisplayDeviceInformation(pEnum);//funtion 2
		pEnum->Release();
		return hr;
	}

	hr = EnumerateDevices(CLSID_AudioInputDeviceCategory, &pEnum);//funtion 1
	if (SUCCEEDED(hr))
	{
		DisplayDeviceInformation(pEnum);//funtion 2
		pEnum->Release();
		return hr;
	}
}

void SCREENRECORD::StartRecord()
{
	HRESULT hr = CoInitialize(NULL);
	hr = InitCaptureGraphBuilder(&pGraph, &pBuild);
	if (SUCCEEDED(hr))
	{
		hr = SelectDevices();
		hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
		hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
		hr = pBuild->SetOutputFileName(
			&MEDIASUBTYPE_Avi,  // Specifies AVI for the target file.
			L"C:\\Users\\qf\\Desktop\\test\\Example.avi", // File name.
			&pMux,              // Receives a pointer to the mux.
			NULL);              // (Optional) Receives a pointer to the file sink.
		//hr = pBuild->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
		//	pCap, NULL, NULL);
		hr = pBuild->RenderStream(
			&PIN_CATEGORY_CAPTURE, // Pin category.
			&MEDIATYPE_Video,      // Media type.
			pCap,                  // Capture filter.
			NULL,                  // Intermediate filter (optional).
			pMux);                 // Mux or file sink filter.

		// Release the mux filter.
		pMux->Release();
		if (SUCCEEDED(hr))
		{
			hr = pControl->Run();
		}
	}
}

void SCREENRECORD::CloseInterface()
{
	pControl->Release();
	pGraph->Release();
	pBuild->Release();
}

void SCREENRECORD::FinishRecord()
{
	pControl->Stop();
	CloseInterface();
	CoUninitialize();
}

void SCREENRECORD::FtpUpload()
{
	BOOL dRes, pRes;
	HINTERNET hInternet;
	HINTERNET hConnect;
	hInternet = InternetOpen("A3GS Sample", INTERNET_OPEN_TYPE_DIRECT,
		NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE);
	if (NULL == hInternet)
	{
		printf("InternetOpen Error:%d\n", GetLastError());
	}
	hConnect = InternetConnect(hInternet, "10.4.64.11"/*FTP服务器地址*/, INTERNET_DEFAULT_FTP_PORT/*FTP端口号，此为默认值---21*/,
		"vsuser"/*用户名*/, "654321"/*密码*/, INTERNET_SERVICE_FTP,
		INTERNET_FLAG_EXISTING_CONNECT || INTERNET_FLAG_PASSIVE, 0);
	if (NULL == hInternet)
	{
		printf("InternetConnect Error:%d\n", GetLastError());
		InternetCloseHandle(hInternet);
	}
	pRes = FtpPutFile(hConnect, "E:\\Example.avi", "mycamera0717.avi", FTP_TRANSFER_TYPE_ASCII, 0);
	if (pRes == 0)
	{
		printf("上传文件失败！\n");
	}
	else {
		printf("上传文件成功！\n");
	}
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
}

int main()
{
	SCREENRECORD *s = new SCREENRECORD;
	char cmd;
	printf("s - start\nf - finish\nq - quit\n\n");

	while (true)
	{
		std::cin >> cmd;
		switch (cmd)
		{
		case 's':
			{
				printf("start\n");
				s->StartRecord();
			}
			break;
		case 'f':
			{
				printf("finish\n");
				s->FinishRecord();
			}
			break;
		case 'q':
			return 0;
			break;
		default: printf("Unknown command!\n");
			break;
		}
	}
}