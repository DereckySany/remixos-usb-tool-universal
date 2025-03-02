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

#include "JSHTMLFontElement.h"

#include <wtf/GetPtr.h>

#include "HTMLFontElement.h"
#include "KURL.h"

#include <runtime/JSNumberCell.h>
#include <runtime/JSString.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSHTMLFontElement)

/* Hash table */

static const HashTableValue JSHTMLFontElementTableValues[5] =
{
    { "color", DontDelete, (intptr_t)jsHTMLFontElementColor, (intptr_t)setJSHTMLFontElementColor },
    { "face", DontDelete, (intptr_t)jsHTMLFontElementFace, (intptr_t)setJSHTMLFontElementFace },
    { "size", DontDelete, (intptr_t)jsHTMLFontElementSize, (intptr_t)setJSHTMLFontElementSize },
    { "constructor", DontEnum|ReadOnly, (intptr_t)jsHTMLFontElementConstructor, (intptr_t)0 },
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLFontElementTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 15, JSHTMLFontElementTableValues, 0 };
#else
    { 9, 7, JSHTMLFontElementTableValues, 0 };
#endif

/* Hash table for constructor */

static const HashTableValue JSHTMLFontElementConstructorTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLFontElementConstructorTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLFontElementConstructorTableValues, 0 };
#else
    { 1, 0, JSHTMLFontElementConstructorTableValues, 0 };
#endif

class JSHTMLFontElementConstructor : public DOMObject {
public:
    JSHTMLFontElementConstructor(ExecState* exec)
        : DOMObject(JSHTMLFontElementConstructor::createStructure(exec->lexicalGlobalObject()->objectPrototype()))
    {
        putDirect(exec->propertyNames().prototype, JSHTMLFontElementPrototype::self(exec), None);
    }
    virtual bool getOwnPropertySlot(ExecState*, const Identifier&, PropertySlot&);
    virtual const ClassInfo* classInfo() const { return &s_info; }
    static const ClassInfo s_info;

    static PassRefPtr<Structure> createStructure(JSValuePtr proto) 
    { 
        return Structure::create(proto, TypeInfo(ObjectType, ImplementsHasInstance)); 
    }
};

const ClassInfo JSHTMLFontElementConstructor::s_info = { "HTMLFontElementConstructor", 0, &JSHTMLFontElementConstructorTable, 0 };

bool JSHTMLFontElementConstructor::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLFontElementConstructor, DOMObject>(exec, &JSHTMLFontElementConstructorTable, this, propertyName, slot);
}

/* Hash table for prototype */

static const HashTableValue JSHTMLFontElementPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSHTMLFontElementPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSHTMLFontElementPrototypeTableValues, 0 };
#else
    { 1, 0, JSHTMLFontElementPrototypeTableValues, 0 };
#endif

const ClassInfo JSHTMLFontElementPrototype::s_info = { "HTMLFontElementPrototype", 0, &JSHTMLFontElementPrototypeTable, 0 };

JSObject* JSHTMLFontElementPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSHTMLFontElement>(exec);
}

const ClassInfo JSHTMLFontElement::s_info = { "HTMLFontElement", &JSHTMLElement::s_info, &JSHTMLFontElementTable, 0 };

JSHTMLFontElement::JSHTMLFontElement(PassRefPtr<Structure> structure, PassRefPtr<HTMLFontElement> impl)
    : JSHTMLElement(structure, impl)
{
}

JSObject* JSHTMLFontElement::createPrototype(ExecState* exec)
{
    return new (exec) JSHTMLFontElementPrototype(JSHTMLFontElementPrototype::createStructure(JSHTMLElementPrototype::self(exec)));
}

bool JSHTMLFontElement::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSHTMLFontElement, Base>(exec, &JSHTMLFontElementTable, this, propertyName, slot);
}

JSValuePtr jsHTMLFontElementColor(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->color());
}

JSValuePtr jsHTMLFontElementFace(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->face());
}

JSValuePtr jsHTMLFontElementSize(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(asObject(slot.slotBase()))->impl());
    return jsString(exec, imp->size());
}

JSValuePtr jsHTMLFontElementConstructor(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    return static_cast<JSHTMLFontElement*>(asObject(slot.slotBase()))->getConstructor(exec);
}
void JSHTMLFontElement::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    lookupPut<JSHTMLFontElement, Base>(exec, propertyName, value, &JSHTMLFontElementTable, this, slot);
}

void setJSHTMLFontElementColor(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(thisObject)->impl());
    imp->setColor(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLFontElementFace(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(thisObject)->impl());
    imp->setFace(valueToStringWithNullCheck(exec, value));
}

void setJSHTMLFontElementSize(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    HTMLFontElement* imp = static_cast<HTMLFontElement*>(static_cast<JSHTMLFontElement*>(thisObject)->impl());
    imp->setSize(valueToStringWithNullCheck(exec, value));
}

JSValuePtr JSHTMLFontElement::getConstructor(ExecState* exec)
{
    return getDOMConstructor<JSHTMLFontElementConstructor>(exec);
}


}
