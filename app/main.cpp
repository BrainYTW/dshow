#include "ScreenRecord.h"
#include <SRGui.h>
#include <QtWidgets/qapplication.h>


#pragma comment(lib,"SRGui.lib")


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SRGui *hehe = new SRGui();
	hehe->show();
	return a.exec();
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