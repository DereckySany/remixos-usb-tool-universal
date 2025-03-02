/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"


#if ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGFEImageElement.h"

#include <wtf/GetPtr.h>

#include "CSSMutableStyleDeclaration.h"
#include "CSSStyleDeclaration.h"
#include "JSCSSStyleDeclaration.h"
#include "JSSVGAnimatedBoolean.h"
#include "JSSVGAnimatedLength.h"
#include "JSSVGAnimatedString.h"
#include "PlatformString.h"
#include "SVGFEImageElement.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGFEImageElementTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "x", JSSVGFEImageElement::XAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "xmllang", JSSVGFEImageElement::XmllangAttrNum, DontDelete, 0, 0 },
    { "height", JSSVGFEImageElement::HeightAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "xmlspace", JSSVGFEImageElement::XmlspaceAttrNum, DontDelete, 0, 0 },
    { "externalResourcesRequired", JSSVGFEImageElement::ExternalResourcesRequiredAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEImageElementTableEntries[12] },
    { "href", JSSVGFEImageElement::HrefAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEImageElementTableEntries[11] },
    { "width", JSSVGFEImageElement::WidthAttrNum, DontDelete|ReadOnly, 0, &JSSVGFEImageElementTableEntries[13] },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "result", JSSVGFEImageElement::ResultAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "y", JSSVGFEImageElement::YAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "className", JSSVGFEImageElement::ClassNameAttrNum, DontDelete|ReadOnly, 0, 0 },
    { "style", JSSVGFEImageElement::StyleAttrNum, DontDelete|ReadOnly, 0, 0 }
};

static const HashTable JSSVGFEImageElementTable = 
{
    2, 14, JSSVGFEImageElementTableEntries, 11
};

/* Hash table for prototype */

static const HashEntry JSSVGFEImageElementPrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSSVGFEImageElementPrototypeTable = 
{
    2, 1, JSSVGFEImageElementPrototypeTableEntries, 1
};

const ClassInfo JSSVGFEImageElementPrototype::info = { "SVGFEImageElementPrototype", 0, &JSSVGFEImageElementPrototypeTable, 0 };

JSObject* JSSVGFEImageElementPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGFEImageElementPrototype>(exec, "[[JSSVGFEImageElement.prototype]]");
}

const ClassInfo JSSVGFEImageElement::info = { "SVGFEImageElement", &JSSVGElement::info, &JSSVGFEImageElementTable, 0 };

JSSVGFEImageElement::JSSVGFEImageElement(ExecState* exec, SVGFEImageElement* impl)
    : JSSVGElement(exec, impl)
{
    setPrototype(JSSVGFEImageElementPrototype::self(exec));
}

bool JSSVGFEImageElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGFEImageElement, JSSVGElement>(exec, &JSSVGFEImageElementTable, this, propertyName, slot);
}

JSValue* JSSVGFEImageElement::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case HrefAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedString> obj = imp->hrefAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedString>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedString>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedString>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case XmllangAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        return jsString(imp->xmllang());
    }
    case XmlspaceAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        return jsString(imp->xmlspace());
    }
    case ExternalResourcesRequiredAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedBoolean> obj = imp->externalResourcesRequiredAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedBoolean>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedBoolean>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedBoolean>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case XAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedLength> obj = imp->xAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedLength>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedLength>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedLength>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case YAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedLength> obj = imp->yAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedLength>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedLength>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedLength>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case WidthAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedLength> obj = imp->widthAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedLength>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedLength>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedLength>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case HeightAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedLength> obj = imp->heightAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedLength>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedLength>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedLength>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case ResultAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedString> obj = imp->resultAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedString>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedString>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedString>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case ClassNameAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        ASSERT(exec && exec->dynamicInterpreter());

        RefPtr<SVGAnimatedString> obj = imp->classNameAnimated();
        Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
        if (activeFrame) {
            SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
            if (extensions) {
                if (extensions->hasGenericContext<SVGAnimatedString>(obj.get()))
                    ASSERT(extensions->genericContext<SVGAnimatedString>(obj.get()) == imp);
                else
                    extensions->setGenericContext<SVGAnimatedString>(obj.get(), imp);
            }
        }

        return toJS(exec, obj.get());
    }
    case StyleAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        return toJS(exec, WTF::getPtr(imp->style()));
    }
    }
    return 0;
}

void JSSVGFEImageElement::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSSVGFEImageElement, JSSVGElement>(exec, propertyName, value, attr, &JSSVGFEImageElementTable, this);
}

void JSSVGFEImageElement::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case XmllangAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        imp->setXmllang(value->toString(exec));
        break;
    }
    case XmlspaceAttrNum: {
        SVGFEImageElement* imp = static_cast<SVGFEImageElement*>(impl());

        imp->setXmlspace(value->toString(exec));
        break;
    }
    }
}


}

#endif // ENABLE(SVG) && ENABLE(SVG_EXPERIMENTAL_FEATURES)
