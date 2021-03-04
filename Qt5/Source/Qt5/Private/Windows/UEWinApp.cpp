// Fill out your copyright notice in the Description page of Project Settings.


#include "Windows/UEWinApp.h"
#include "Misc/Build.h"
#include "Qt5.h"
#include "Qt5BlueprintFunctionLibrary.h"

#if PLATFORM_WINDOWS

#if QT_VERSION >= 0x050000
#define QT_WA(unicode, ansi) unicode
#endif

HHOOK hhook;

UEWinApp::UEWinApp(int argc, char **argv): QApplication(argc, argv)
{
	setQuitOnLastWindowClosed(false);
}

UEWinApp::~UEWinApp()
{
	if (hhook) {
		UnhookWindowsHookEx(hhook);
		hhook = 0;
	}
}

LRESULT CALLBACK QtFilterProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (qApp) {
		// don't process deferred-deletes while in a modal loop
		qApp->sendPostedEvents(0, -1);
	}

	return CallNextHookEx(hhook, nCode, wParam, lParam);
}

void plogQtMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	switch (type)
	{
	case QtDebugMsg:
		UE_LOG(Qt5Log,Log,TEXT("plogQtMessageHandlerMsg is %s"),*UQt5BlueprintFunctionLibrary::QStringToFString(msg));
		break;
	case QtInfoMsg:
		UE_LOG(Qt5Log,Display,TEXT("plogQtMessageHandlerMsg is %s"),*UQt5BlueprintFunctionLibrary::QStringToFString(msg));
		break;
	case QtWarningMsg:
		UE_LOG(Qt5Log,Warning,TEXT("plogQtMessageHandlerMsg is %s"),*UQt5BlueprintFunctionLibrary::QStringToFString(msg));
		break;
	case QtCriticalMsg:
		UE_LOG(Qt5Log,Error,TEXT("plogQtMessageHandlerMsg is %s"),*UQt5BlueprintFunctionLibrary::QStringToFString(msg));
		break;
	case QtFatalMsg:
		UE_LOG(Qt5Log,Fatal,TEXT("plogQtMessageHandlerMsg is %s"),*UQt5BlueprintFunctionLibrary::QStringToFString(msg));
		break;
	default:
		break;
	}
}
bool UEWinApp::pluginInstance()
{
	if (qApp)
		return false;
	QT_WA({
	hhook = SetWindowsHookExW(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
		}, {
		hhook = SetWindowsHookExA(WH_GETMESSAGE, QtFilterProc, 0, GetCurrentThreadId());
		});

	int argc = 0;
	(void)new QApplication(argc, 0);
	if (qApp)
	{
		qApp->setQuitOnLastWindowClosed(false);
	}
 #if (!UE_BUILD_SHIPPING)
	qInstallMessageHandler(plogQtMessageHandler);
#endif
	return true;
}

void UEWinApp::pluginExit()
{
#if (!UE_BUILD_SHIPPING)
	qInstallMessageHandler(nullptr);
#endif
	if (hhook) {
		UnhookWindowsHookEx(hhook);
		hhook = 0;
	}
	delete qApp;
}
#endif
