/*
 * Copyright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>
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

#if ENABLE(SVG)
#include "SVGResourceClipper.h"

#include "SVGRenderTreeAsText.h"
#include "TextStream.h"

namespace WebCore {

SVGResourceClipper::SVGResourceClipper()
    : SVGResource()
{
}

SVGResourceClipper::~SVGResourceClipper()
{
}

void SVGResourceClipper::resetClipData()
{
    m_clipData.clear();
}

void SVGResourceClipper::addClipData(const Path& path, WindRule rule, bool bboxUnits)
{
    m_clipData.addPath(path, rule, bboxUnits);
}

const ClipDataList& SVGResourceClipper::clipData() const
{
    return m_clipData;
}

TextStream& SVGResourceClipper::externalRepresentation(TextStream& ts) const
{
    ts << "[type=CLIPPER]";
    ts << " [clip data=" << clipData().clipData() << "]";
    return ts;
}

TextStream& operator<<(TextStream& ts, WindRule rule)
{
    switch (rule) {
        case RULE_NONZERO:
            ts << "NON-ZERO"; break;
        case RULE_EVENODD:
            ts << "EVEN-ODD"; break;
    }

    return ts;
}

TextStream& operator<<(TextStream& ts, const ClipData& d)
{
    ts << "[winding=" << d.windRule << "]";

    if (d.bboxUnits)
        ts << " [bounding box mode=" << d.bboxUnits << "]";

    ts << " [path=" << d.path.debugString() << "]";
    return ts;
}

SVGResourceClipper* getClipperById(Document* document, const AtomicString& id)
{
    SVGResource* resource = getResourceById(document, id);
    if (resource && resource->isClipper())
        return static_cast<SVGResourceClipper*>(resource);

    return 0;
}

} // namespace WebCore

#endif
