/*
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2007-2008 Trolltech ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CSSComputedStyleDeclaration.h"
#include "CSSPropertyNames.h"
#include "FrameLoaderClientQt.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "DocumentLoader.h"
#include "MIMETypeRegistry.h"
#include "ResourceResponse.h"
#include "Page.h"
#include "PluginData.h"
#include "ProgressTracker.h"
#include "RenderPart.h"
#include "ResourceRequest.h"
#include "HistoryItem.h"
#include "HTMLFormElement.h"
#include "NotImplemented.h"
#include "QNetworkReplyHandler.h"
#include "ResourceHandleInternal.h"
#include "ResourceHandle.h"

#include "qwebpage.h"
#include "qwebframe.h"
#include "qwebframe_p.h"
#include "qwebhistoryinterface.h"
#include "qwebpluginfactory.h"

#include <qfileinfo.h>

#include <QCoreApplication>
#include <QDebug>
#if QT_VERSION >= 0x040400
#include <QNetworkRequest>
#include <QNetworkReply>
#else
#include "qwebnetworkinterface_p.h"
#endif

static bool dumpFrameLoaderCallbacks = false;
static bool dumpResourceLoadCallbacks = false;

static QMap<unsigned long, QString> dumpAssignedUrls;

void QWEBKIT_EXPORT qt_dump_frame_loader(bool b)
{
    dumpFrameLoaderCallbacks = b;
}

void QWEBKIT_EXPORT qt_dump_resource_load_callbacks(bool b)
{
    dumpResourceLoadCallbacks = b;
}

// Compare with WebKitTools/DumpRenderTree/mac/FrameLoadDelegate.mm
static QString drtDescriptionSuitableForTestResult(WebCore::Frame* _frame)
{
    QWebFrame* frame = QWebFramePrivate::kit(_frame);
    QString name = frame->frameName();

    bool isMainFrame = frame == frame->page()->mainFrame();
    if (isMainFrame) {
        if (!name.isEmpty())
            return QString::fromLatin1("main frame \"%1\"").arg(name);
        return QLatin1String("main frame");
    } else {
        if (!name.isEmpty())
            return QString::fromLatin1("frame \"%1\"").arg(name);
        return QLatin1String("frame (anonymous)");
    }
}

static QString drtDescriptionSuitableForTestResult(const WebCore::KURL& _url)
{
    QUrl url = _url;
    return url.toString();
}

static QString drtDescriptionSuitableForTestResult(const WebCore::ResourceError& error)
{
    QString failingURL = error.failingURL();
    return QString::fromLatin1("<NSError domain NSURLErrorDomain, code %1, failing URL \"%2\">").arg(error.errorCode()).arg(failingURL);
}

static QString drtDescriptionSuitableForTestResult(const WebCore::ResourceRequest& request)
{
    QString url = request.url().url();
    return QString::fromLatin1("<NSURLRequest %1>").arg(url);
}

static QString drtDescriptionSuitableForTestResult(const WebCore::ResourceResponse& response)
{
    QString text = response.httpStatusText();
    if (text.isEmpty())
        return QLatin1String("(null)");

    return text;
}


namespace WebCore
{

FrameLoaderClientQt::FrameLoaderClientQt()
    : m_frame(0)
    , m_webFrame(0)
    , m_firstData(false)
    , m_policyFunction(0)
    , m_loadSucceeded(false)
{
    connect(this, SIGNAL(sigCallPolicyFunction(int)), this, SLOT(slotCallPolicyFunction(int)), Qt::QueuedConnection);
}


FrameLoaderClientQt::~FrameLoaderClientQt()
{
}

void FrameLoaderClientQt::setFrame(QWebFrame* webFrame, Frame* frame)
{
    m_webFrame = webFrame;
    m_frame = frame;
    if (!m_webFrame || !m_webFrame->page()) {
        qWarning("FrameLoaderClientQt::setFrame frame without Page!");
        return;
    }

    connect(this, SIGNAL(loadStarted()),
            m_webFrame->page(), SIGNAL(loadStarted()));
    connect(this, SIGNAL(loadProgress(int)),
            m_webFrame->page(), SIGNAL(loadProgress(int)));
    connect(this, SIGNAL(loadFinished(bool)),
            m_webFrame->page(), SIGNAL(loadFinished(bool)));
    connect(this, SIGNAL(titleChanged(const QString&)),
            m_webFrame, SIGNAL(titleChanged(const QString&)));
}

QWebFrame* FrameLoaderClientQt::webFrame() const
{
    return m_webFrame;
}

void FrameLoaderClientQt::callPolicyFunction(FramePolicyFunction function, PolicyAction action)
{
    ASSERT(!m_policyFunction);
    ASSERT(function);

    m_policyFunction = function;
    emit sigCallPolicyFunction(action);
}

void FrameLoaderClientQt::slotCallPolicyFunction(int action)
{
    if (!m_frame || !m_policyFunction)
        return;
    FramePolicyFunction function = m_policyFunction;
    m_policyFunction = 0;
    (m_frame->loader()->*function)(WebCore::PolicyAction(action));
}

bool FrameLoaderClientQt::hasWebView() const
{
    //notImplemented();
    return true;
}


bool FrameLoaderClientQt::hasFrameView() const
{
    //notImplemented();
    return true;
}

bool FrameLoaderClientQt::privateBrowsingEnabled() const
{
    return false;
}


void FrameLoaderClientQt::savePlatformDataToCachedPage(CachedPage*)
{
    notImplemented();
}

void FrameLoaderClientQt::transitionToCommittedFromCachedPage(CachedPage*)
{
}

void FrameLoaderClientQt::transitionToCommittedForNewPage()
{
    ASSERT(m_frame);
    ASSERT(m_webFrame);

    Page* page = m_frame->page();
    ASSERT(page);

    bool isMainFrame = m_frame == page->mainFrame();

    m_frame->setView(0);

    FrameView* frameView;
    if (isMainFrame)
        frameView = new FrameView(m_frame, m_webFrame->page()->viewportSize());
    else
        frameView = new FrameView(m_frame);

    if (!m_webFrame->d->allowsScrolling)
        frameView->setScrollbarsMode(ScrollbarAlwaysOff);
    if (m_webFrame->d->marginWidth != -1)
        frameView->setMarginWidth(m_webFrame->d->marginWidth);
    if (m_webFrame->d->marginHeight != -1)
        frameView->setMarginHeight(m_webFrame->d->marginHeight);
    if (m_webFrame->d->horizontalScrollBarPolicy != Qt::ScrollBarAsNeeded)
        frameView->setHScrollbarMode((ScrollbarMode)m_webFrame->d->horizontalScrollBarPolicy);
    if (m_webFrame->d->verticalScrollBarPolicy != Qt::ScrollBarAsNeeded)
        frameView->setVScrollbarMode((ScrollbarMode)m_webFrame->d->verticalScrollBarPolicy);

    m_frame->setView(frameView);
    // FrameViews are created with a ref count of 1. Release this ref since we've assigned it to frame.
    frameView->deref();

    if (m_webFrame && m_webFrame->page())
        m_webFrame->d->updateBackground();

    if (m_frame->ownerRenderer())
        m_frame->ownerRenderer()->setWidget(frameView);
}


void FrameLoaderClientQt::makeRepresentation(DocumentLoader*)
{
    // don't need this for now I think.
}


void FrameLoaderClientQt::forceLayout()
{
    m_frame->forceLayout(true);
}


void FrameLoaderClientQt::forceLayoutForNonHTML()
{
}


void FrameLoaderClientQt::setCopiesOnScroll()
{
    // apparently mac specific
}


void FrameLoaderClientQt::detachedFromParent2()
{
}


void FrameLoaderClientQt::detachedFromParent3()
{
}


void FrameLoaderClientQt::detachedFromParent4()
{
}

void FrameLoaderClientQt::dispatchDidHandleOnloadEvents()
{
    // don't need this one
    if (dumpFrameLoaderCallbacks)
        printf("%s - didHandleOnloadEventsForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

}


void FrameLoaderClientQt::dispatchDidReceiveServerRedirectForProvisionalLoad()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didReceiveServerRedirectForProvisionalLoadForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    notImplemented();
}


void FrameLoaderClientQt::dispatchDidCancelClientRedirect()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didCancelClientRedirectForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    notImplemented();
}


void FrameLoaderClientQt::dispatchWillPerformClientRedirect(const KURL& url,
                                                            double interval,
                                                            double fireDate)
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - willPerformClientRedirectToURL: %s \n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)), qPrintable(drtDescriptionSuitableForTestResult(url)));

    notImplemented();
}


void FrameLoaderClientQt::dispatchDidChangeLocationWithinPage()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didChangeLocationWithinPageForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    notImplemented();
}


void FrameLoaderClientQt::dispatchWillClose()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - willCloseFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));
}


void FrameLoaderClientQt::dispatchDidStartProvisionalLoad()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didStartProvisionalLoadForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    if (m_webFrame)
        emit m_webFrame->provisionalLoad();
}


void FrameLoaderClientQt::dispatchDidReceiveTitle(const String& title)
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didReceiveTitle: %s\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)), qPrintable(QString(title)));

    if (!m_webFrame)
        return;



    // ### hack
    emit m_webFrame->urlChanged(m_webFrame->url());
    emit titleChanged(title);
}


void FrameLoaderClientQt::dispatchDidCommitLoad()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didCommitLoadForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    if (m_frame->tree()->parent() || !m_webFrame)
        return;

    m_webFrame->page()->d->updateNavigationActions();

    // We should assume first the frame has no title. If it has, then the above dispatchDidReceiveTitle()
    // will be called very soon with the correct title.
    // This properly resets the title when we navigate to a URI without a title.
    emit titleChanged(String());
}


void FrameLoaderClientQt::dispatchDidFinishDocumentLoad()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didFinishDocumentLoadForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    if (m_frame->tree()->parent() || !m_webFrame)
        return;

    m_webFrame->page()->d->updateNavigationActions();
}


void FrameLoaderClientQt::dispatchDidFinishLoad()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didFinishLoadForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    m_loadSucceeded = true;

    if (m_frame->tree()->parent() || !m_webFrame)
        return;
    m_webFrame->page()->d->updateNavigationActions();
}


void FrameLoaderClientQt::dispatchDidFirstLayout()
{
    if (m_webFrame)
        emit m_webFrame->initialLayoutCompleted();
}


void FrameLoaderClientQt::dispatchShow()
{
    notImplemented();
}


void FrameLoaderClientQt::cancelPolicyCheck()
{
//    qDebug() << "FrameLoaderClientQt::cancelPolicyCheck";
    m_policyFunction = 0;
}


void FrameLoaderClientQt::dispatchWillSubmitForm(FramePolicyFunction function,
                                                 PassRefPtr<FormState>)
{
    notImplemented();
    Q_ASSERT(!m_policyFunction);
    // FIXME: This is surely too simple
    callPolicyFunction(function, PolicyUse);
}


void FrameLoaderClientQt::dispatchDidLoadMainResource(DocumentLoader*)
{
}


void FrameLoaderClientQt::revertToProvisionalState(DocumentLoader*)
{
    notImplemented();
}


void FrameLoaderClientQt::clearUnarchivingState(DocumentLoader*)
{
    notImplemented();
}


void FrameLoaderClientQt::postProgressStartedNotification()
{
    if (m_webFrame && m_frame->page()) {
        emit loadStarted();
        postProgressEstimateChangedNotification();
    }
    if (m_frame->tree()->parent() || !m_webFrame)
        return;
    m_webFrame->page()->d->updateNavigationActions();
}

void FrameLoaderClientQt::postProgressEstimateChangedNotification()
{
    if (m_webFrame && m_frame->page())
        emit loadProgress(qRound(m_frame->page()->progress()->estimatedProgress() * 100));
}

void FrameLoaderClientQt::postProgressFinishedNotification()
{
    if (m_webFrame && m_frame->page())
        emit loadFinished(m_loadSucceeded);
}

void FrameLoaderClientQt::setMainFrameDocumentReady(bool b)
{
    // this is only interesting once we provide an external API for the DOM
}


void FrameLoaderClientQt::willChangeTitle(DocumentLoader*)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}


void FrameLoaderClientQt::didChangeTitle(DocumentLoader *)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}


void FrameLoaderClientQt::finishedLoading(DocumentLoader* loader)
{
    if (m_firstData) {
        FrameLoader *fl = loader->frameLoader();
        fl->setEncoding(m_response.textEncodingName(), false);
        m_firstData = false;
    }
}


void FrameLoaderClientQt::finalSetupForReplace(DocumentLoader*)
{
    notImplemented();
}


void FrameLoaderClientQt::setDefersLoading(bool)
{
    notImplemented();
}


bool FrameLoaderClientQt::isArchiveLoadPending(ResourceLoader*) const
{
    return false;
}


void FrameLoaderClientQt::cancelPendingArchiveLoad(ResourceLoader*)
{
    notImplemented();
}


void FrameLoaderClientQt::clearArchivedResources()
{
    // don't think we need to do anything here currently
}


bool FrameLoaderClientQt::canShowMIMEType(const String& MIMEType) const
{
    if (MIMETypeRegistry::isSupportedImageMIMEType(MIMEType))
        return true;

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(MIMEType))
        return true;

    return false;
}

bool FrameLoaderClientQt::representationExistsForURLScheme(const String& URLScheme) const
{
    return false;
}


String FrameLoaderClientQt::generatedMIMETypeForURLScheme(const String& URLScheme) const
{
    notImplemented();
    return String();
}


void FrameLoaderClientQt::frameLoadCompleted()
{
    // Note: Can be called multiple times.
    // Even if already complete, we might have set a previous item on a frame that
    // didn't do any data loading on the past transaction. Make sure to clear these out.
    m_frame->loader()->setPreviousHistoryItem(0);
}


void FrameLoaderClientQt::restoreViewState()
{
    notImplemented();
}


void FrameLoaderClientQt::provisionalLoadStarted()
{
    // don't need to do anything here
}


void FrameLoaderClientQt::didFinishLoad()
{
//     notImplemented();
}


void FrameLoaderClientQt::prepareForDataSourceReplacement()
{
    m_frame->loader()->detachChildren();
}

void FrameLoaderClientQt::setTitle(const String&, const KURL&)
{
    // no need for, dispatchDidReceiveTitle is the right callback
}


String FrameLoaderClientQt::userAgent(const KURL& url)
{
    if (m_webFrame) {
        return m_webFrame->page()->userAgentForUrl(url);
    }
    return String();
}

void FrameLoaderClientQt::dispatchDidReceiveIcon()
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didReceiveIconForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    if (m_webFrame) {
        emit m_webFrame->iconChanged();
    }
}

void FrameLoaderClientQt::frameLoaderDestroyed()
{
    delete m_webFrame;
    m_frame = 0;
    m_webFrame = 0;

    delete this;
}

bool FrameLoaderClientQt::canHandleRequest(const WebCore::ResourceRequest&) const
{
    return true;
}

void FrameLoaderClientQt::windowObjectCleared() const
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didClearWindowObjectForFrame\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    if (m_webFrame)
        emit m_webFrame->javaScriptWindowObjectCleared();
}

void FrameLoaderClientQt::didPerformFirstNavigation() const
{
    if (m_frame->tree()->parent() || !m_webFrame)
        return;
    m_webFrame->page()->d->updateNavigationActions();
}

void FrameLoaderClientQt::registerForIconNotification(bool)
{
    notImplemented();
}

void FrameLoaderClientQt::updateGlobalHistoryForStandardLoad(const WebCore::KURL& url)
{
    QWebHistoryInterface *history = QWebHistoryInterface::defaultInterface();
    if (history)
        history->addHistoryEntry(url.prettyURL());
}

void FrameLoaderClientQt::updateGlobalHistoryForReload(const WebCore::KURL&)
{
    notImplemented();
}

bool FrameLoaderClientQt::shouldGoToHistoryItem(WebCore::HistoryItem*) const
{
    return true;
}

void FrameLoaderClientQt::saveViewStateToItem(WebCore::HistoryItem*)
{
}

bool FrameLoaderClientQt::canCachePage() const
{
    return true;
}

void FrameLoaderClientQt::setMainDocumentError(WebCore::DocumentLoader* loader, const WebCore::ResourceError& error)
{
    if (m_firstData) {
        loader->frameLoader()->setEncoding(m_response.textEncodingName(), false);
        m_firstData = false;
    }
}

void FrameLoaderClientQt::committedLoad(WebCore::DocumentLoader* loader, const char* data, int length)
{
    if (!m_frame)
        return;
    FrameLoader *fl = loader->frameLoader();
    if (m_firstData) {
        fl->setEncoding(m_response.textEncodingName(), false);
        m_firstData = false;
    }
    fl->addData(data, length);
}

WebCore::ResourceError FrameLoaderClientQt::cancelledError(const WebCore::ResourceRequest& request)
{
    return ResourceError("Error", -999, request.url().prettyURL(),
            QCoreApplication::translate("QWebFrame", "Request cancelled", 0, QCoreApplication::UnicodeUTF8));
}

// copied from WebKit/Misc/WebKitErrors[Private].h
enum {
    WebKitErrorCannotShowMIMEType =                             100,
    WebKitErrorCannotShowURL =                                  101,
    WebKitErrorFrameLoadInterruptedByPolicyChange =             102,
    WebKitErrorCannotUseRestrictedPort = 103,
    WebKitErrorCannotFindPlugIn =                               200,
    WebKitErrorCannotLoadPlugIn =                               201,
    WebKitErrorJavaUnavailable =                                202,
};

WebCore::ResourceError FrameLoaderClientQt::blockedError(const WebCore::ResourceRequest& request)
{
    return ResourceError("Error", WebKitErrorCannotUseRestrictedPort, request.url().prettyURL(),
            QCoreApplication::translate("QWebFrame", "Request blocked", 0, QCoreApplication::UnicodeUTF8));
}


WebCore::ResourceError FrameLoaderClientQt::cannotShowURLError(const WebCore::ResourceRequest& request)
{
    return ResourceError("Error", WebKitErrorCannotShowURL, request.url().url(),
            QCoreApplication::translate("QWebFrame", "Cannot show URL", 0, QCoreApplication::UnicodeUTF8));
}

WebCore::ResourceError FrameLoaderClientQt::interruptForPolicyChangeError(const WebCore::ResourceRequest& request)
{
    return ResourceError("Error", WebKitErrorFrameLoadInterruptedByPolicyChange, request.url().url(),
            QCoreApplication::translate("QWebFrame", "Frame load interruped by policy change", 0, QCoreApplication::UnicodeUTF8));
}

WebCore::ResourceError FrameLoaderClientQt::cannotShowMIMETypeError(const WebCore::ResourceResponse& response)
{
    return ResourceError("Error", WebKitErrorCannotShowMIMEType, response.url().url(),
            QCoreApplication::translate("QWebFrame", "Cannot show mimetype", 0, QCoreApplication::UnicodeUTF8));
}

WebCore::ResourceError FrameLoaderClientQt::fileDoesNotExistError(const WebCore::ResourceResponse& response)
{
    return ResourceError("Error", -998 /* ### */, response.url().url(),
            QCoreApplication::translate("QWebFrame", "File does not exist", 0, QCoreApplication::UnicodeUTF8));
}

bool FrameLoaderClientQt::shouldFallBack(const WebCore::ResourceError&)
{
    notImplemented();
    return false;
}

WTF::PassRefPtr<WebCore::DocumentLoader> FrameLoaderClientQt::createDocumentLoader(const WebCore::ResourceRequest& request, const SubstituteData& substituteData)
{
    RefPtr<DocumentLoader> loader = new DocumentLoader(request, substituteData);
    return loader.release();
}

void FrameLoaderClientQt::download(WebCore::ResourceHandle* handle, const WebCore::ResourceRequest&, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&)
{
#if QT_VERSION >= 0x040400
    if (!m_webFrame)
        return;

    QNetworkReplyHandler* handler = handle->getInternal()->m_job;
    QNetworkReply* reply = handler->release();
    if (reply) {
        QWebPage *page = m_webFrame->page();
        if (page->forwardUnsupportedContent())
            emit m_webFrame->page()->unsupportedContent(reply);
        else
            reply->abort();
    }
#endif
}

void FrameLoaderClientQt::assignIdentifierToInitialRequest(unsigned long identifier, WebCore::DocumentLoader* loader, const WebCore::ResourceRequest& request)
{
    if (dumpResourceLoadCallbacks)
        dumpAssignedUrls[identifier] = drtDescriptionSuitableForTestResult(request.url());
}

void FrameLoaderClientQt::dispatchWillSendRequest(WebCore::DocumentLoader*, unsigned long identifier, WebCore::ResourceRequest& newRequest, const WebCore::ResourceResponse& redirectResponse)
{
    if (dumpResourceLoadCallbacks)
        printf("%s - willSendRequest %s redirectResponse %s\n",
               qPrintable(dumpAssignedUrls[identifier]),
               qPrintable(drtDescriptionSuitableForTestResult(newRequest)),
               qPrintable(drtDescriptionSuitableForTestResult(redirectResponse)));

    // seems like the Mac code doesn't do anything here by default neither
    //qDebug() << "FrameLoaderClientQt::dispatchWillSendRequest" << request.isNull() << request.url().url();
}

void FrameLoaderClientQt::dispatchDidReceiveAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&)
{
    notImplemented();
}

void FrameLoaderClientQt::dispatchDidCancelAuthenticationChallenge(DocumentLoader*, unsigned long, const AuthenticationChallenge&)
{
    notImplemented();
}

void FrameLoaderClientQt::dispatchDidReceiveResponse(WebCore::DocumentLoader*, unsigned long, const WebCore::ResourceResponse& response)
{

    m_response = response;
    m_firstData = true;
    //qDebug() << "    got response from" << response.url().url();
}

void FrameLoaderClientQt::dispatchDidReceiveContentLength(WebCore::DocumentLoader*, unsigned long, int)
{
}

void FrameLoaderClientQt::dispatchDidFinishLoading(WebCore::DocumentLoader* loader, unsigned long)
{
}

void FrameLoaderClientQt::dispatchDidFailLoading(WebCore::DocumentLoader* loader, unsigned long identifier, const WebCore::ResourceError& error)
{
    if (dumpResourceLoadCallbacks)
        printf("%s - didFailLoadingWithError: %s\n", qPrintable(dumpAssignedUrls[identifier]), qPrintable(drtDescriptionSuitableForTestResult(error)));

    if (m_firstData) {
        FrameLoader *fl = loader->frameLoader();
        fl->setEncoding(m_response.textEncodingName(), false);
        m_firstData = false;
    }
}

bool FrameLoaderClientQt::dispatchDidLoadResourceFromMemoryCache(WebCore::DocumentLoader*, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, int)
{
    notImplemented();
    return false;
}

void FrameLoaderClientQt::dispatchDidFailProvisionalLoad(const WebCore::ResourceError&)
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didFailProvisionalLoadWithError\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    m_loadSucceeded = false;
}

void FrameLoaderClientQt::dispatchDidFailLoad(const WebCore::ResourceError&)
{
    if (dumpFrameLoaderCallbacks)
        printf("%s - didFailLoadWithError\n", qPrintable(drtDescriptionSuitableForTestResult(m_frame)));

    m_loadSucceeded = false;
}

WebCore::Frame* FrameLoaderClientQt::dispatchCreatePage()
{
    if (!m_webFrame)
        return 0;
    QWebPage *newPage = m_webFrame->page()->createWindow(QWebPage::WebBrowserWindow);
    if (!newPage)
        return 0;
    return newPage->mainFrame()->d->frame;
}

void FrameLoaderClientQt::dispatchDecidePolicyForMIMEType(FramePolicyFunction function, const WebCore::String& MIMEType, const WebCore::ResourceRequest&)
{
    // we need to call directly here
    Q_ASSERT(!m_policyFunction);
    m_policyFunction = function;
    if (canShowMIMEType(MIMEType))
        slotCallPolicyFunction(PolicyUse);
    else
        slotCallPolicyFunction(PolicyDownload);
}

void FrameLoaderClientQt::dispatchDecidePolicyForNewWindowAction(FramePolicyFunction function, const WebCore::NavigationAction&, const WebCore::ResourceRequest&, const WebCore::String&)
{
    Q_ASSERT(!m_policyFunction);
    m_policyFunction = function;
    slotCallPolicyFunction(PolicyUse);
}

void FrameLoaderClientQt::dispatchDecidePolicyForNavigationAction(FramePolicyFunction function, const WebCore::NavigationAction& action, const WebCore::ResourceRequest& request)
{
    Q_ASSERT(!m_policyFunction);
    m_policyFunction = function;
    if (m_webFrame) {
#if QT_VERSION < 0x040400
        QWebNetworkRequest r(request);
#else
        QNetworkRequest r(request.toNetworkRequest());
#endif
        QWebPage *page = m_webFrame->page();

        if (!page->d->acceptNavigationRequest(m_webFrame, r, QWebPage::NavigationType(action.type()))) {
            if (action.type() == NavigationTypeFormSubmitted || action.type() == NavigationTypeFormResubmitted)
                m_frame->loader()->resetMultipleFormSubmissionProtection();
            slotCallPolicyFunction(PolicyIgnore);
            return;
        }
    }
    slotCallPolicyFunction(PolicyUse);
    return;
}

void FrameLoaderClientQt::dispatchUnableToImplementPolicy(const WebCore::ResourceError&)
{
    notImplemented();
}

void FrameLoaderClientQt::startDownload(const WebCore::ResourceRequest& request)
{
#if QT_VERSION >= 0x040400
    if (!m_webFrame)
        return;

    QWebPage *page = m_webFrame->page();
    emit m_webFrame->page()->downloadRequested(request.toNetworkRequest());
#endif
}

bool FrameLoaderClientQt::willUseArchive(WebCore::ResourceLoader*, const WebCore::ResourceRequest&, const WebCore::KURL&) const
{
    return false;
}

PassRefPtr<Frame> FrameLoaderClientQt::createFrame(const KURL& url, const String& name, HTMLFrameOwnerElement* ownerElement,
                                        const String& referrer, bool allowsScrolling, int marginWidth, int marginHeight)
{
    if (!m_webFrame)
        return 0;

    QWebFrameData frameData;
    frameData.url = url;
    frameData.name = name;
    frameData.ownerElement = ownerElement;
    frameData.referrer = referrer;
    frameData.allowsScrolling = allowsScrolling;
    frameData.marginWidth = marginWidth;
    frameData.marginHeight = marginHeight;

    QWebFrame* webFrame = new QWebFrame(m_webFrame, &frameData);
    emit m_webFrame->page()->frameCreated(webFrame);

    RefPtr<Frame> childFrame = adoptRef(webFrame->d->frame);

    // FIXME: All of the below should probably be moved over into WebCore
    childFrame->tree()->setName(name);
    m_frame->tree()->appendChild(childFrame);
    // ### set override encoding if we have one

    FrameLoadType loadType = m_frame->loader()->loadType();
    FrameLoadType childLoadType = FrameLoadTypeRedirectWithLockedHistory;

    childFrame->loader()->load(frameData.url, frameData.referrer, childLoadType,
                             String(), 0, 0);

    // The frame's onload handler may have removed it from the document.
    if (!childFrame->tree()->parent())
        return 0;

    return childFrame.release();
}

ObjectContentType FrameLoaderClientQt::objectContentType(const KURL& url, const String& _mimeType)
{
//    qDebug()<<" ++++++++++++++++ url is "<<url.prettyURL()<<", mime = "<<_mimeType;
    if (_mimeType == "application/x-qt-plugin" || _mimeType == "application/x-qt-styled-widget")
        return ObjectContentOtherPlugin;

    if (url.isEmpty())
        return ObjectContentNone;

    String mimeType = _mimeType;
    if (!mimeType.length()) {
        QFileInfo fi(url.path());
        mimeType = MIMETypeRegistry::getMIMETypeForExtension(fi.suffix());
    }

    if (!mimeType.length())
        return ObjectContentFrame;

    if (MIMETypeRegistry::isSupportedImageMIMEType(mimeType))
        return ObjectContentImage;

    if (m_frame->page() && m_frame->page()->pluginData()->supportsMimeType(mimeType))
        return ObjectContentOtherPlugin;

    if (MIMETypeRegistry::isSupportedNonImageMIMEType(mimeType))
        return ObjectContentFrame;

    if (url.protocol() == "about")
        return ObjectContentFrame;

    return ObjectContentNone;
}

static const CSSPropertyID qstyleSheetProperties[] = {
    CSS_PROP_COLOR,
    CSS_PROP_FONT_FAMILY,
    CSS_PROP_FONT_SIZE,
    CSS_PROP_FONT_STYLE,
    CSS_PROP_FONT_WEIGHT
};

const unsigned numqStyleSheetProperties = sizeof(qstyleSheetProperties) / sizeof(qstyleSheetProperties[0]);

Widget* FrameLoaderClientQt::createPlugin(const IntSize&, Element* element, const KURL& url, const Vector<String>& paramNames,
                                          const Vector<String>& paramValues, const String& mimeType, bool loadManually)
{
//     qDebug()<<"------ Creating plugin in FrameLoaderClientQt::createPlugin for "<<url.prettyURL() << mimeType;
//     qDebug()<<"------\t url = "<<url.prettyURL();

    if (!m_webFrame)
        return 0;

    QStringList params;
    QStringList values;
    for (int i = 0; i < paramNames.size(); ++i)
        params.append(paramNames[i]);
    for (int i = 0; i < paramValues.size(); ++i)
        values.append(paramValues[i]);

    QString urlStr(url.url());
    QUrl qurl = urlStr;

    QObject *object = 0;

    if (mimeType == "application/x-qt-plugin" || mimeType == "application/x-qt-styled-widget") {
        object = m_webFrame->page()->createPlugin(element->getAttribute("classid"), qurl, params, values);
        QWidget *widget = qobject_cast<QWidget *>(object);
        if (widget && mimeType == "application/x-qt-styled-widget") {
            CSSComputedStyleDeclaration cssDecl(element);

            QString styleSheet = element->getAttribute("style");
            if (!styleSheet.isEmpty())
                styleSheet += QLatin1Char(';');

            for (int i = 0; i < numqStyleSheetProperties; ++i) {
                CSSPropertyID property = qstyleSheetProperties[i];

                styleSheet += QString::fromLatin1(::getPropertyName(property));
                styleSheet += QLatin1Char(':');
                styleSheet += cssDecl.getPropertyValue(property);
                styleSheet += QLatin1Char(';');
            }

            widget->setStyleSheet(styleSheet);
        }
    }

    if (!object) {
        QWebPluginFactory* factory = m_webFrame->page()->pluginFactory();
        if (factory)
            object = factory->create(mimeType, qurl, params, values);
    }

    if (object) {
        QWidget *widget = qobject_cast<QWidget *>(object);
        QWidget *view = m_webFrame->page()->view();
        if (widget && view) {
            widget->setParent(view);
            Widget* w= new Widget();
            w->setNativeWidget(widget);
            return w;
        }
        // FIXME: make things work for widgetless plugins as well
        delete object;
    }

    return 0;
}

void FrameLoaderClientQt::redirectDataToPlugin(Widget* pluginWidget)
{
    notImplemented();
    return;
}

Widget* FrameLoaderClientQt::createJavaAppletWidget(const IntSize&, Element*, const KURL& baseURL,
                                                    const Vector<String>& paramNames, const Vector<String>& paramValues)
{
    notImplemented();
    return 0;
}

String FrameLoaderClientQt::overrideMediaType() const
{
    return String();
}

QString FrameLoaderClientQt::chooseFile(const QString& oldFile)
{
    return webFrame()->page()->chooseFile(webFrame(), oldFile);
}

}

#include "moc_FrameLoaderClientQt.cpp"
