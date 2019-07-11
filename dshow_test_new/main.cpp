#include <dshow.h>
#include <windows.h>

#pragma comment(lib,"strmiids.lib")


IGraphBuilder *pGraph = NULL;
ICaptureGraphBuilder2 *pBuild = NULL;
IBaseFilter *pCap;
IMediaControl *pControl = NULL;
IMediaEvent   *pEvent = NULL;


HRESULT InitCaptureGraphBuilder(
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

HRESULT EnumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
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

void DisplayDeviceInformation(IEnumMoniker *pEnum)
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

HRESULT SelectDevices(void)
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

void main()
{
	HRESULT hr = CoInitialize(NULL);
	hr = InitCaptureGraphBuilder(&pGraph, &pBuild);
	if (SUCCEEDED(hr))
	{
		hr = SelectDevices();
		hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
		hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
		hr = pBuild->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
			pCap, NULL, NULL);
		if (SUCCEEDED(hr))
		{
			hr = pControl->Run();
			if (SUCCEEDED(hr))
			{
				long evCode;
				pEvent->WaitForCompletion(INFINITE, &evCode);
			}
		}
	}
	CoUninitialize();
}

//ICaptureGraphBuilder2 *pBuild; // Capture Graph Builder
//// Initialize pBuild (not shown).
//HRESULT hr = InitCaptureGraphBuilder(ppGraph, ppBuild);
//
//IBaseFilter *pCap; // Video capture filter.
//// Initialize pCap and add it to the filter graph (not shown).
//hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
//if (SUCCEEDED(hr))
//{
//	hr = pGraph->AddFilter(pCap, L"Capture Filter");
//}
//
//HRESULT hr = pBuild->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,
//	pCap, NULL, NULL);