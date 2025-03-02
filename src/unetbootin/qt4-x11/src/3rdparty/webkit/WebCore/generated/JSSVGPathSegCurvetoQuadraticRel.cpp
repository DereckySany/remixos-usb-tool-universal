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


#if ENABLE(SVG)

#include "SVGElement.h"
#include "JSSVGPathSegCurvetoQuadraticRel.h"

#include <wtf/GetPtr.h>

#include "SVGPathSegCurvetoQuadratic.h"

#include <runtime/JSNumberCell.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSSVGPathSegCurvetoQuadraticRel)

/* Hash table */

static const HashTableValue JSSVGPathSegCurvetoQuadraticRelTableValues[5] =
{
    { "x", DontDelete, (intptr_t)jsSVGPathSegCurvetoQuadraticRelX, (intptr_t)setJSSVGPathSegCurvetoQuadraticRelX },
    { "y", DontDelete, (intptr_t)jsSVGPathSegCurvetoQuadraticRelY, (intptr_t)setJSSVGPathSegCurvetoQuadraticRelY },
    { "x1", DontDelete, (intptr_t)jsSVGPathSegCurvetoQuadraticRelX1, (intptr_t)setJSSVGPathSegCurvetoQuadraticRelX1 },
    { "y1", DontDelete, (intptr_t)jsSVGPathSegCurvetoQuadraticRelY1, (intptr_t)setJSSVGPathSegCurvetoQuadraticRelY1 },
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegCurvetoQuadraticRelTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 3, JSSVGPathSegCurvetoQuadraticRelTableValues, 0 };
#else
    { 8, 7, JSSVGPathSegCurvetoQuadraticRelTableValues, 0 };
#endif

/* Hash table for prototype */

static const HashTableValue JSSVGPathSegCurvetoQuadraticRelPrototypeTableValues[1] =
{
    { 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegCurvetoQuadraticRelPrototypeTable =
#if ENABLE(PERFECT_HASH_SIZE)
    { 0, JSSVGPathSegCurvetoQuadraticRelPrototypeTableValues, 0 };
#else
    { 1, 0, JSSVGPathSegCurvetoQuadraticRelPrototypeTableValues, 0 };
#endif

const ClassInfo JSSVGPathSegCurvetoQuadraticRelPrototype::s_info = { "SVGPathSegCurvetoQuadraticRelPrototype", 0, &JSSVGPathSegCurvetoQuadraticRelPrototypeTable, 0 };

JSObject* JSSVGPathSegCurvetoQuadraticRelPrototype::self(ExecState* exec)
{
    return getDOMPrototype<JSSVGPathSegCurvetoQuadraticRel>(exec);
}

const ClassInfo JSSVGPathSegCurvetoQuadraticRel::s_info = { "SVGPathSegCurvetoQuadraticRel", &JSSVGPathSeg::s_info, &JSSVGPathSegCurvetoQuadraticRelTable, 0 };

JSSVGPathSegCurvetoQuadraticRel::JSSVGPathSegCurvetoQuadraticRel(PassRefPtr<Structure> structure, PassRefPtr<SVGPathSegCurvetoQuadraticRel> impl, SVGElement* context)
    : JSSVGPathSeg(structure, impl, context)
{
}

JSObject* JSSVGPathSegCurvetoQuadraticRel::createPrototype(ExecState* exec)
{
    return new (exec) JSSVGPathSegCurvetoQuadraticRelPrototype(JSSVGPathSegCurvetoQuadraticRelPrototype::createStructure(JSSVGPathSegPrototype::self(exec)));
}

bool JSSVGPathSegCurvetoQuadraticRel::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegCurvetoQuadraticRel, Base>(exec, &JSSVGPathSegCurvetoQuadraticRelTable, this, propertyName, slot);
}

JSValuePtr jsSVGPathSegCurvetoQuadraticRelX(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->x());
}

JSValuePtr jsSVGPathSegCurvetoQuadraticRelY(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->y());
}

JSValuePtr jsSVGPathSegCurvetoQuadraticRelX1(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->x1());
}

JSValuePtr jsSVGPathSegCurvetoQuadraticRelY1(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(asObject(slot.slotBase()))->impl());
    return jsNumber(exec, imp->y1());
}

void JSSVGPathSegCurvetoQuadraticRel::put(ExecState* exec, const Identifier& propertyName, JSValuePtr value, PutPropertySlot& slot)
{
    lookupPut<JSSVGPathSegCurvetoQuadraticRel, Base>(exec, propertyName, value, &JSSVGPathSegCurvetoQuadraticRelTable, this, slot);
}

void setJSSVGPathSegCurvetoQuadraticRelX(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl());
    imp->setX(value->toFloat(exec));
    if (static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context())
        static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegCurvetoQuadraticRelY(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl());
    imp->setY(value->toFloat(exec));
    if (static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context())
        static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegCurvetoQuadraticRelX1(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl());
    imp->setX1(value->toFloat(exec));
    if (static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context())
        static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl()->associatedAttributeName());
}

void setJSSVGPathSegCurvetoQuadraticRelY1(ExecState* exec, JSObject* thisObject, JSValuePtr value)
{
    SVGPathSegCurvetoQuadraticRel* imp = static_cast<SVGPathSegCurvetoQuadraticRel*>(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl());
    imp->setY1(value->toFloat(exec));
    if (static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context())
        static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->context()->svgAttributeChanged(static_cast<JSSVGPathSegCurvetoQuadraticRel*>(thisObject)->impl()->associatedAttributeName());
}


}

#endif // ENABLE(SVG)
