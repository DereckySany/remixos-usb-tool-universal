/*
 *  Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"

#if ENABLE(SVG)

#include "JSSVGElementWrapperFactory.h"

#include "JSSVGAElement.h"
#include "JSSVGAnimateColorElement.h"
#include "JSSVGAnimateElement.h"
#include "JSSVGAnimateTransformElement.h"
#include "JSSVGCircleElement.h"
#include "JSSVGClipPathElement.h"
#include "JSSVGCursorElement.h"
#include "JSSVGDefsElement.h"
#include "JSSVGDescElement.h"
#include "JSSVGEllipseElement.h"
#include "JSSVGFEBlendElement.h"
#include "JSSVGFEColorMatrixElement.h"
#include "JSSVGFEComponentTransferElement.h"
#include "JSSVGFECompositeElement.h"
#include "JSSVGFEDiffuseLightingElement.h"
#include "JSSVGFEDisplacementMapElement.h"
#include "JSSVGFEDistantLightElement.h"
#include "JSSVGFEFloodElement.h"
#include "JSSVGFEFuncAElement.h"
#include "JSSVGFEFuncBElement.h"
#include "JSSVGFEFuncGElement.h"
#include "JSSVGFEFuncRElement.h"
#include "JSSVGFEGaussianBlurElement.h"
#include "JSSVGFEImageElement.h"
#include "JSSVGFEMergeElement.h"
#include "JSSVGFEMergeNodeElement.h"
#include "JSSVGFEOffsetElement.h"
#include "JSSVGFEPointLightElement.h"
#include "JSSVGFESpecularLightingElement.h"
#include "JSSVGFESpotLightElement.h"
#include "JSSVGFETileElement.h"
#include "JSSVGFETurbulenceElement.h"
#include "JSSVGFilterElement.h"
#include "JSSVGForeignObjectElement.h"
#include "JSSVGGElement.h"
#include "JSSVGImageElement.h"
#include "JSSVGLinearGradientElement.h"
#include "JSSVGLineElement.h"
#include "JSSVGMarkerElement.h"
#include "JSSVGMaskElement.h"
#include "JSSVGMetadataElement.h"
#include "JSSVGPathElement.h"
#include "JSSVGPatternElement.h"
#include "JSSVGPolygonElement.h"
#include "JSSVGPolylineElement.h"
#include "JSSVGRadialGradientElement.h"
#include "JSSVGRectElement.h"
#include "JSSVGScriptElement.h"
#include "JSSVGSetElement.h"
#include "JSSVGStopElement.h"
#include "JSSVGStyleElement.h"
#include "JSSVGSVGElement.h"
#include "JSSVGSwitchElement.h"
#include "JSSVGSymbolElement.h"
#include "JSSVGTextElement.h"
#include "JSSVGTitleElement.h"
#include "JSSVGTRefElement.h"
#include "JSSVGTSpanElement.h"
#include "JSSVGUseElement.h"
#include "JSSVGViewElement.h"

#include "SVGNames.h"

#include "SVGAElement.h"
#include "SVGAnimateColorElement.h"
#include "SVGAnimateElement.h"
#include "SVGAnimateTransformElement.h"
#include "SVGCircleElement.h"
#include "SVGClipPathElement.h"
#include "SVGCursorElement.h"
#include "SVGDefsElement.h"
#include "SVGDescElement.h"
#include "SVGEllipseElement.h"
#include "SVGFEBlendElement.h"
#include "SVGFEColorMatrixElement.h"
#include "SVGFEComponentTransferElement.h"
#include "SVGFECompositeElement.h"
#include "SVGFEDiffuseLightingElement.h"
#include "SVGFEDisplacementMapElement.h"
#include "SVGFEDistantLightElement.h"
#include "SVGFEFloodElement.h"
#include "SVGFEFuncAElement.h"
#include "SVGFEFuncBElement.h"
#include "SVGFEFuncGElement.h"
#include "SVGFEFuncRElement.h"
#include "SVGFEGaussianBlurElement.h"
#include "SVGFEImageElement.h"
#include "SVGFEMergeElement.h"
#include "SVGFEMergeNodeElement.h"
#include "SVGFEOffsetElement.h"
#include "SVGFEPointLightElement.h"
#include "SVGFESpecularLightingElement.h"
#include "SVGFESpotLightElement.h"
#include "SVGFETileElement.h"
#include "SVGFETurbulenceElement.h"
#include "SVGFilterElement.h"
#include "SVGForeignObjectElement.h"
#include "SVGGElement.h"
#include "SVGImageElement.h"
#include "SVGLinearGradientElement.h"
#include "SVGLineElement.h"
#include "SVGMarkerElement.h"
#include "SVGMaskElement.h"
#include "SVGMetadataElement.h"
#include "SVGPathElement.h"
#include "SVGPatternElement.h"
#include "SVGPolygonElement.h"
#include "SVGPolylineElement.h"
#include "SVGRadialGradientElement.h"
#include "SVGRectElement.h"
#include "SVGScriptElement.h"
#include "SVGSetElement.h"
#include "SVGStopElement.h"
#include "SVGStyleElement.h"
#include "SVGSVGElement.h"
#include "SVGSwitchElement.h"
#include "SVGSymbolElement.h"
#include "SVGTextElement.h"
#include "SVGTitleElement.h"
#include "SVGTRefElement.h"
#include "SVGTSpanElement.h"
#include "SVGUseElement.h"
#include "SVGViewElement.h"

using namespace KJS;

// FIXME: Eventually this file should be autogenerated, just like SVGNames, SVGElementFactory, etc.

namespace WebCore {

using namespace SVGNames;

typedef JSNode* (*CreateSVGElementWrapperFunction)(ExecState*, PassRefPtr<SVGElement>);

#if ENABLE(SVG_EXPERIMENTAL_FEATURES)
#define FOR_EACH_TAG(macro) \
    macro(a, A) \
    macro(animateColor, AnimateColor) \
    macro(animate, Animate) \
    macro(animateTransform, AnimateTransform) \
    macro(circle, Circle) \
    macro(clipPath, ClipPath) \
    macro(cursor, Cursor) \
    macro(defs, Defs) \
    macro(desc, Desc) \
    macro(ellipse, Ellipse) \
    macro(feBlend, FEBlend) \
    macro(feColorMatrix, FEColorMatrix) \
    macro(feComponentTransfer, FEComponentTransfer) \
    macro(feComposite, FEComposite) \
    macro(feDiffuseLighting, FEDiffuseLighting) \
    macro(feDisplacementMap, FEDisplacementMap) \
    macro(feDistantLight, FEDistantLight) \
    macro(feFlood, FEFlood) \
    macro(feFuncA, FEFuncA) \
    macro(feFuncB, FEFuncB) \
    macro(feFuncG, FEFuncG) \
    macro(feFuncR, FEFuncR) \
    macro(feGaussianBlur, FEGaussianBlur) \
    macro(feImage, FEImage) \
    macro(feMerge, FEMerge) \
    macro(feMergeNode, FEMergeNode) \
    macro(feOffset, FEOffset) \
    macro(fePointLight, FEPointLight) \
    macro(feSpecularLighting, FESpecularLighting) \
    macro(feSpotLight, FESpotLight) \
    macro(feTile, FETile) \
    macro(feTurbulence, FETurbulence) \
    macro(filter, Filter) \
    macro(foreignObject, ForeignObject) \
    macro(g, G) \
    macro(image, Image) \
    macro(linearGradient, LinearGradient) \
    macro(line, Line) \
    macro(marker, Marker) \
    macro(mask, Mask) \
    macro(metadata, Metadata) \
    macro(path, Path) \
    macro(pattern, Pattern) \
    macro(polyline, Polyline) \
    macro(polygon, Polygon) \
    macro(radialGradient, RadialGradient) \
    macro(rect, Rect) \
    macro(script, Script) \
    macro(set, Set) \
    macro(stop, Stop) \
    macro(style, Style) \
    macro(svg, SVG) \
    macro(switch, Switch) \
    macro(symbol, Symbol) \
    macro(text, Text) \
    macro(title, Title) \
    macro(tref, TRef) \
    macro(tspan, TSpan) \
    macro(use, Use) \
    macro(view, View) \
    // end of macro

#else

#define FOR_EACH_TAG(macro) \
    macro(a, A) \
    macro(circle, Circle) \
    macro(clipPath, ClipPath) \
    macro(cursor, Cursor) \
    macro(defs, Defs) \
    macro(desc, Desc) \
    macro(ellipse, Ellipse) \
    macro(g, G) \
    macro(image, Image) \
    macro(linearGradient, LinearGradient) \
    macro(line, Line) \
    macro(marker, Marker) \
    macro(mask, Mask) \
    macro(metadata, Metadata) \
    macro(path, Path) \
    macro(pattern, Pattern) \
    macro(polyline, Polyline) \
    macro(polygon, Polygon) \
    macro(radialGradient, RadialGradient) \
    macro(rect, Rect) \
    macro(script, Script) \
    macro(set, Set) \
    macro(stop, Stop) \
    macro(style, Style) \
    macro(svg, SVG) \
    macro(switch, Switch) \
    macro(symbol, Symbol) \
    macro(text, Text) \
    macro(title, Title) \
    macro(tref, TRef) \
    macro(tspan, TSpan) \
    macro(use, Use) \
    macro(view, View) \
    // end of macro

#endif

#define CREATE_WRAPPER_FUNCTION(tag, name) \
static JSNode* create##name##Wrapper(ExecState* exec, PassRefPtr<SVGElement> element) \
{ \
    return new JSSVG##name##Element(exec, static_cast<SVG##name##Element*>(element.get())); \
}
FOR_EACH_TAG(CREATE_WRAPPER_FUNCTION)
#undef CREATE_WRAPPER_FUNCTION

JSNode* createJSSVGWrapper(ExecState* exec, PassRefPtr<SVGElement> element)
{
    static HashMap<WebCore::AtomicStringImpl*, CreateSVGElementWrapperFunction> map;
    if (map.isEmpty()) {
#define ADD_TO_HASH_MAP(tag, name) map.set(tag##Tag.localName().impl(), create##name##Wrapper);
FOR_EACH_TAG(ADD_TO_HASH_MAP)
#undef ADD_TO_HASH_MAP
    }
    CreateSVGElementWrapperFunction createWrapperFunction = map.get(element->localName().impl());
    if (createWrapperFunction)
        return createWrapperFunction(exec, element);
    return new JSSVGElement(exec, element.get());
}

}

#endif // ENABLE(SVG)
