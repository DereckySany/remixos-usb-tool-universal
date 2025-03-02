/*
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <wildfox@kde.org>
                  2004, 2005, 2006 Rob Buis <buis@kde.org>

    This file is part of the KDE project

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

#ifndef SVGLineElement_h
#define SVGLineElement_h

#if ENABLE(SVG)

#include "SVGExternalResourcesRequired.h"
#include "SVGLangSpace.h"
#include "SVGStyledTransformableElement.h"
#include "SVGTests.h"

namespace WebCore
{
    class SVGLength;
    class SVGLineElement : public SVGStyledTransformableElement,
                           public SVGTests,
                           public SVGLangSpace,
                           public SVGExternalResourcesRequired
    {
    public:
        SVGLineElement(const QualifiedName&, Document*);
        virtual ~SVGLineElement();
        
        virtual bool isValid() const { return SVGTests::isValid(); }

        // 'SVGLineElement' functions
        virtual void parseMappedAttribute(MappedAttribute* attr);
        virtual void notifyAttributeChange() const;

        virtual Path toPathData() const;

        virtual bool supportsMarkers() const { return true; }

    protected:
        virtual const SVGElement* contextElement() const { return this; }
        virtual bool hasRelativeValues() const;

    private:
        ANIMATED_PROPERTY_FORWARD_DECLARATIONS(SVGExternalResourcesRequired, bool, ExternalResourcesRequired, externalResourcesRequired)
 
        ANIMATED_PROPERTY_DECLARATIONS(SVGLineElement, SVGLength, SVGLength, X1, x1)
        ANIMATED_PROPERTY_DECLARATIONS(SVGLineElement, SVGLength, SVGLength, Y1, y1)
        ANIMATED_PROPERTY_DECLARATIONS(SVGLineElement, SVGLength, SVGLength, X2, x2)
        ANIMATED_PROPERTY_DECLARATIONS(SVGLineElement, SVGLength, SVGLength, Y2, y2)
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif

// vim:ts=4:noet
