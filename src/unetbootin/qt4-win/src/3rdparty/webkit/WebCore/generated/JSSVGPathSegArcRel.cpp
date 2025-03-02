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

#include "Document.h"
#include "Frame.h"
#include "SVGDocumentExtensions.h"
#include "SVGElement.h"
#include "SVGAnimatedTemplate.h"
#include "JSSVGPathSegArcRel.h"

#include <wtf/GetPtr.h>

#include "SVGPathSegArc.h"

using namespace KJS;

namespace WebCore {

/* Hash table */

static const HashEntry JSSVGPathSegArcRelTableEntries[] =
{
    { 0, 0, 0, 0, 0 },
    { "r2", JSSVGPathSegArcRel::R2AttrNum, DontDelete, 0, &JSSVGPathSegArcRelTableEntries[9] },
    { "x", JSSVGPathSegArcRel::XAttrNum, DontDelete, 0, &JSSVGPathSegArcRelTableEntries[7] },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { "y", JSSVGPathSegArcRel::YAttrNum, DontDelete, 0, &JSSVGPathSegArcRelTableEntries[8] },
    { 0, 0, 0, 0, 0 },
    { "r1", JSSVGPathSegArcRel::R1AttrNum, DontDelete, 0, 0 },
    { "angle", JSSVGPathSegArcRel::AngleAttrNum, DontDelete, 0, 0 },
    { "largeArcFlag", JSSVGPathSegArcRel::LargeArcFlagAttrNum, DontDelete, 0, &JSSVGPathSegArcRelTableEntries[10] },
    { "sweepFlag", JSSVGPathSegArcRel::SweepFlagAttrNum, DontDelete, 0, 0 }
};

static const HashTable JSSVGPathSegArcRelTable = 
{
    2, 11, JSSVGPathSegArcRelTableEntries, 7
};

/* Hash table for prototype */

static const HashEntry JSSVGPathSegArcRelPrototypeTableEntries[] =
{
    { 0, 0, 0, 0, 0 }
};

static const HashTable JSSVGPathSegArcRelPrototypeTable = 
{
    2, 1, JSSVGPathSegArcRelPrototypeTableEntries, 1
};

const ClassInfo JSSVGPathSegArcRelPrototype::info = { "SVGPathSegArcRelPrototype", 0, &JSSVGPathSegArcRelPrototypeTable, 0 };

JSObject* JSSVGPathSegArcRelPrototype::self(ExecState* exec)
{
    return KJS::cacheGlobalObject<JSSVGPathSegArcRelPrototype>(exec, "[[JSSVGPathSegArcRel.prototype]]");
}

const ClassInfo JSSVGPathSegArcRel::info = { "SVGPathSegArcRel", &JSSVGPathSeg::info, &JSSVGPathSegArcRelTable, 0 };

JSSVGPathSegArcRel::JSSVGPathSegArcRel(ExecState* exec, SVGPathSegArcRel* impl)
    : JSSVGPathSeg(exec, impl)
{
    setPrototype(JSSVGPathSegArcRelPrototype::self(exec));
}

bool JSSVGPathSegArcRel::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    return getStaticValueSlot<JSSVGPathSegArcRel, JSSVGPathSeg>(exec, &JSSVGPathSegArcRelTable, this, propertyName, slot);
}

JSValue* JSSVGPathSegArcRel::getValueProperty(ExecState* exec, int token) const
{
    switch (token) {
    case XAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsNumber(imp->x());
    }
    case YAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsNumber(imp->y());
    }
    case R1AttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsNumber(imp->r1());
    }
    case R2AttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsNumber(imp->r2());
    }
    case AngleAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsNumber(imp->angle());
    }
    case LargeArcFlagAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsBoolean(imp->largeArcFlag());
    }
    case SweepFlagAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        return jsBoolean(imp->sweepFlag());
    }
    }
    return 0;
}

void JSSVGPathSegArcRel::put(ExecState* exec, const Identifier& propertyName, JSValue* value, int attr)
{
    lookupPut<JSSVGPathSegArcRel, JSSVGPathSeg>(exec, propertyName, value, attr, &JSSVGPathSegArcRelTable, this);
}

void JSSVGPathSegArcRel::putValueProperty(ExecState* exec, int token, JSValue* value, int /*attr*/)
{
    switch (token) {
    case XAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setX(value->toFloat(exec));
        break;
    }
    case YAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setY(value->toFloat(exec));
        break;
    }
    case R1AttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setR1(value->toFloat(exec));
        break;
    }
    case R2AttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setR2(value->toFloat(exec));
        break;
    }
    case AngleAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setAngle(value->toFloat(exec));
        break;
    }
    case LargeArcFlagAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setLargeArcFlag(value->toBoolean(exec));
        break;
    }
    case SweepFlagAttrNum: {
        SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

        imp->setSweepFlag(value->toBoolean(exec));
        break;
    }
    }
    SVGPathSegArcRel* imp = static_cast<SVGPathSegArcRel*>(impl());

    ASSERT(exec && exec->dynamicInterpreter());
    Frame* activeFrame = static_cast<ScriptInterpreter*>(exec->dynamicInterpreter())->frame();
    if (!activeFrame)
        return;

    SVGDocumentExtensions* extensions = (activeFrame->document() ? activeFrame->document()->accessSVGExtensions() : 0);
    if (extensions && extensions->hasGenericContext<SVGPathSeg>(imp)) {
        const SVGElement* context = extensions->genericContext<SVGPathSeg>(imp);
        ASSERT(context);

        context->notifyAttributeChange();
    }

}


}

#endif // ENABLE(SVG)
