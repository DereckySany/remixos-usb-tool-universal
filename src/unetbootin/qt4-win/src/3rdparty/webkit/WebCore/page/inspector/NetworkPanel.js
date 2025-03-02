/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

WebInspector.NetworkPanel = function()
{
    WebInspector.Panel.call(this);

    this.timelineEntries = [];

    this.timelineElement = document.createElement("div");
    this.timelineElement.className = "network-timeline";
    this.element.appendChild(this.timelineElement);

    this.summaryElement = document.createElement("div");
    this.summaryElement.className = "network-summary";
    this.element.appendChild(this.summaryElement);

    this.dividersElement = document.createElement("div");
    this.dividersElement.className = "network-dividers";
    this.timelineElement.appendChild(this.dividersElement);

    this.resourcesElement = document.createElement("div");
    this.resourcesElement.className = "network-resources";
    var panel = this;
    this.resourcesElement.addEventListener("click", function(event) { return panel.onClick(event) }, false);
    this.timelineElement.appendChild(this.resourcesElement);

    var graphArea = document.createElement("div");
    graphArea.className = "network-graph-area";
    this.summaryElement.appendChild(graphArea);

    this.graphLabelElement = document.createElement("div");
    this.graphLabelElement.className = "network-graph-label";
    graphArea.appendChild(this.graphLabelElement);

    this.graphModeSelectElement = document.createElement("select");
    this.graphModeSelectElement.className = "network-graph-mode";
    this.graphModeSelectElement.addEventListener("change", function(event) { return panel.onModeChange(event) }, false);
    this.graphLabelElement.appendChild(this.graphModeSelectElement);
    this.graphLabelElement.appendChild(document.createElement("br"));

    var sizeOptionElement = document.createElement("option");
    sizeOptionElement.textContent = "Transfer Size";
    sizeOptionElement.calculator = new WebInspector.TransferSizeCalculator();
    this.graphModeSelectElement.appendChild(sizeOptionElement);

    var timeOptionElement = document.createElement("option");
    timeOptionElement.textContent = "Transfer Time";
    timeOptionElement.calculator = new WebInspector.TransferTimeCalculator();
    this.graphModeSelectElement.appendChild(timeOptionElement);

    var graphSideElement = document.createElement("div");
    graphSideElement.className = "network-graph-side";
    graphArea.appendChild(graphSideElement);

    this.summaryGraphElement = document.createElement("canvas");
    this.summaryGraphElement.setAttribute("width", "450");
    this.summaryGraphElement.setAttribute("height", "38");
    this.summaryGraphElement.className = "network-summary-graph";
    graphSideElement.appendChild(this.summaryGraphElement);

    this.legendElement = document.createElement("div");
    this.legendElement.className = "network-graph-legend";
    graphSideElement.appendChild(this.legendElement);

    var networkPanel = this;
    window.addEventListener("resize", function() { networkPanel.updateTimelineDividersIfNeeded() }, false);

    this.setup = true;
    this.drawSummaryGraph(); // draws an empty graph
}

WebInspector.NetworkPanel.prototype = {
    show: function()
    {
        WebInspector.networkListItem.select();
        WebInspector.Panel.prototype.show.call(this);
    },

    hide: function()
    {
        WebInspector.networkListItem.deselect();
        WebInspector.Panel.prototype.hide.call(this);
    },

    onClick: function(event)
    {
        var resourceElement = event.target.firstParentOrSelfWithClass("network-resource");
        if (!resourceElement)
            return;

        var balloon = event.target.firstParentOrSelfWithClass("tip-balloon");
        if (balloon) {
            resourceElement.timelineEntry.toggleTipBalloon(event);
            return;
        }

        resourceElement.timelineEntry.toggleShowingInfo();
    },

    onModeChange: function(event)
    {
        this.updateSummaryGraph();
    },

    get calculator()
    {
        return this.graphModeSelectElement.options[this.graphModeSelectElement.selectedIndex].calculator;
    },

    get totalDuration()
    {
        return this.latestEndTime - this.earliestStartTime;
    },

    refresh: function()
    {
        if (!this.setup)
            return;

        this.needsRefresh = false;

        // calling refresh will call updateTimelineBoundriesIfNeeded, which can clear needsRefresh for future entries,
        // so find all the entries that needs refresh first, then loop back trough them to call refresh
        var entriesNeedingRefresh = [];
        var entriesLength = this.timelineEntries.length;
        for (var i = 0; i < entriesLength; ++i) {
            var entry = this.timelineEntries[i];
            if (entry.needsRefresh || entry.infoNeedsRefresh)
                entriesNeedingRefresh.push(entry);
        }

        entriesLength = entriesNeedingRefresh.length;
        for (var i = 0; i < entriesLength; ++i)
            entriesNeedingRefresh[i].refresh(false, true, true);

        this.updateTimelineDividersIfNeeded();
        this.sortTimelineEntriesIfNeeded();
        this.updateSummaryGraph();
    },

    makeLegendElement: function(label, value, color)
    {
        var legendElement = document.createElement("label");
        legendElement.className = "network-graph-legend-item";

        if (color) {
            var swatch = document.createElement("canvas");
            swatch.className = "network-graph-legend-swatch";
            swatch.setAttribute("width", "13");
            swatch.setAttribute("height", "24");

            legendElement.appendChild(swatch);

            this.drawSwatch(swatch, color);
        }

        var labelElement = document.createElement("div");
        labelElement.className = "network-graph-legend-label";
        legendElement.appendChild(labelElement);

        var headerElement = document.createElement("div");
        var headerElement = document.createElement("div");
        headerElement.className = "network-graph-legend-header";
        headerElement.textContent = label;
        labelElement.appendChild(headerElement);

        var valueElement = document.createElement("div");
        valueElement.className = "network-graph-legend-value";
        valueElement.textContent = value;
        labelElement.appendChild(valueElement);

        return legendElement;
    },

    sortTimelineEntriesSoonIfNeeded: function()
    {
        if ("sortTimelineEntriesTimeout" in this)
            return;

        var _self = this;
        this.sortTimelineEntriesTimeout = setTimeout(function () { _self.sortTimelineEntriesIfNeeded() }, 500);
    },

    sortTimelineEntriesIfNeeded: function()
    {
        if ("sortTimelineEntriesTimeout" in this) {
            clearTimeout(this.sortTimelineEntriesTimeout);
            delete this.sortTimelineEntriesTimeout;
        }

        this.timelineEntries.sort(WebInspector.NetworkPanel.timelineEntryCompare);

        var nextSibling = null;
        for (var i = (this.timelineEntries.length - 1); i >= 0; --i) {
            var entry = this.timelineEntries[i];
            if (entry.resourceElement.nextSibling !== nextSibling)
                this.resourcesElement.insertBefore(entry.resourceElement, nextSibling);
            nextSibling = entry.resourceElement;
        }
    },

    updateTimelineBoundriesIfNeeded: function(resource, immediate)
    {
        var didUpdate = false;
        if (resource.startTime !== -1 && (this.earliestStartTime === undefined || resource.startTime < this.earliestStartTime)) {
            this.earliestStartTime = resource.startTime;
            didUpdate = true;
        }

        if (resource.endTime !== -1 && (this.latestEndTime === undefined || resource.endTime > this.latestEndTime)) {
            this.latestEndTime = resource.endTime;
            didUpdate = true;
        }

        if (didUpdate) {
            if (immediate) {
                this.refreshAllTimelineEntries(true, true, immediate);
                this.updateTimelineDividersIfNeeded();
            } else {
                this.refreshAllTimelineEntriesSoon(true, true, immediate);
                this.updateTimelineDividersSoonIfNeeded();
            }
        }

        return didUpdate;
    },

    updateTimelineDividersSoonIfNeeded: function()
    {
        if ("updateTimelineDividersTimeout" in this)
            return;

        var _self = this;
        this.updateTimelineDividersTimeout = setTimeout(function () { _self.updateTimelineDividersIfNeeded() }, 500);
    },

    updateTimelineDividersIfNeeded: function()
    {
        if ("updateTimelineDividersTimeout" in this) {
            clearTimeout(this.updateTimelineDividersTimeout);
            delete this.updateTimelineDividersTimeout;
        }

        if (!this.visible) {
            this.needsRefresh = true;
            return;
        }

        var dividerCount = Math.round(this.dividersElement.offsetWidth / 64);
        var timeSlice = this.totalDuration / dividerCount;
        
        if (this.lastDividerTimeSlice === timeSlice)
            return;

        this.lastDividerTimeSlice = timeSlice;

        this.dividersElement.removeChildren();

        for (var i = 1; i <= dividerCount; ++i) {
            var divider = document.createElement("div");
            divider.className = "network-divider";
            if (i === dividerCount)
                divider.addStyleClass("last");
            divider.style.left = ((i / dividerCount) * 100) + "%";

            var label = document.createElement("div");
            label.className = "network-divider-label";
            label.textContent = Number.secondsToString(timeSlice * i);
            divider.appendChild(label);

            this.dividersElement.appendChild(divider);
        }
    },

    refreshAllTimelineEntriesSoon: function(skipBoundryUpdate, skipTimelineSort, immediate)
    {
        if ("refreshAllTimelineEntriesTimeout" in this)
            return;

        var _self = this;
        this.refreshAllTimelineEntriesTimeout = setTimeout(function () { _self.refreshAllTimelineEntries() }, 500, skipBoundryUpdate, skipTimelineSort, immediate);
    },

    refreshAllTimelineEntries: function(skipBoundryUpdate, skipTimelineSort, immediate)
    {
        if ("refreshAllTimelineEntriesTimeout" in this) {
            clearTimeout(this.refreshAllTimelineEntriesTimeout);
            delete this.refreshAllTimelineEntriesTimeout;
        }

        var entriesLength = this.timelineEntries.length;
        for (var i = 0; i < entriesLength; ++i)
            this.timelineEntries[i].refresh(skipBoundryUpdate, skipTimelineSort, immediate);
    },

    drawSwatch: function(canvas, color) {
        var ctx = canvas.getContext('2d');

        ctx.save();

        drawSwatchSquare(ctx, color);

        ctx.save();

        ctx.translate(0, (13 * 2) - 1);
        ctx.scale(1, -1);

        drawSwatchSquare(ctx, color);

        ctx.restore();

        fadeOutRect(ctx, 0, 0 + 13, 13, 13, 0.5, 0.0);

        ctx.restore();
    },

    drawSummaryGraph: function(segments) {
        if (!this.summaryGraphElement)
            return;

        var ctx = this.summaryGraphElement.getContext('2d');
        var x = 0;
        var y = 0;
        var width = 450;
        var height = 19;

        if (!segments || !segments.length)
            segments = [{color: "white", value: 1}];

        ctx.save();

        ctx.clearRect(x, y, width, (height * 2));

        drawShadowLine(ctx, x, y + height, width, 2);
        drawPill(ctx, x, y, width, height, height / 2, segments);

        ctx.save();

        ctx.translate(0, (height * 2) + 1);
        ctx.scale(1, -1);

        drawPill(ctx, x, y, width, height, height / 2, segments);

        ctx.restore();

        fadeOutRect(ctx, x, y + height + 2, width, height, 0.5, 0.0);

        ctx.restore();
    },

    updateSummaryGraphSoon: function()
    {
        if ("updateSummaryGraphTimeout" in this)
            return;

        var _self = this;
        this.updateSummaryGraphTimeout = setTimeout(function () { _self.updateSummaryGraph() }, 500);
    },

    updateSummaryGraph: function()
    {
        if ("updateSummaryGraphTimeout" in this) {
            clearTimeout(this.updateSummaryGraphTimeout);
            delete this.updateSummaryGraphTimeout;
        }

        var graphInfo = this.calculator.computeValues(this.timelineEntries);

        var categoryOrder = ["documents", "stylesheets", "images", "scripts", "other"];
        var categoryColors = {documents: {r: 47, g: 102, b: 236}, stylesheets: {r: 157, g: 231, b: 119}, images: {r: 164, g: 60, b: 255}, scripts: {r: 255, g: 121, b: 0}, other: {r: 186, g: 186, b: 186}};
        var fillSegments = [];

        this.legendElement.removeChildren();

        if (this.totalLegendLabel)
            this.totalLegendLabel.parentNode.removeChild(this.totalLegendLabel);

        this.totalLegendLabel = this.makeLegendElement(this.calculator.title, this.calculator.formatValue(graphInfo.total));
        this.totalLegendLabel.addStyleClass("network-graph-legend-total");
        this.graphLabelElement.appendChild(this.totalLegendLabel);

        for (var i = 0; i < categoryOrder.length; ++i) {
            var category = categoryOrder[i];
            var size = graphInfo.categoryValues[category];
            if (!size)
                continue;

            var color = categoryColors[category];
            var colorString = "rgb(" + color.r + ", " + color.g + ", " + color.b + ")";

            var fillSegment = {color: colorString, value: size};
            fillSegments.push(fillSegment);

            var legendLabel = this.makeLegendElement(category, this.calculator.formatValue(size), colorString);
            this.legendElement.appendChild(legendLabel);
        }

        this.drawSummaryGraph(fillSegments);
    },

    clearTimeline: function()
    {
        delete this.earliestStartTime;
        delete this.latestEndTime;

        var entriesLength = this.timelineEntries.length;
        for (var i = 0; i < entriesLength; ++i)
            delete this.timelineEntries[i].resource.networkTimelineEntry;

        this.timelineEntries = [];
        this.resourcesElement.removeChildren();

        this.drawSummaryGraph(); // draws an empty graph
    },

    addResourceToTimeline: function(resource)
    {
        var timelineEntry = new WebInspector.NetworkTimelineEntry(this, resource);
        this.timelineEntries.push(timelineEntry);
        this.resourcesElement.appendChild(timelineEntry.resourceElement);

        timelineEntry.refresh();
        this.updateSummaryGraphSoon();
    }
}

WebInspector.NetworkPanel.prototype.__proto__ = WebInspector.Panel.prototype;

WebInspector.NetworkPanel.timelineEntryCompare = function(a, b)
{
    if (a.resource.startTime < b.resource.startTime)
        return -1;
    if (a.resource.startTime > b.resource.startTime)
        return 1;
    if (a.resource.endTime < b.resource.endTime)
        return -1;
    if (a.resource.endTime > b.resource.endTime)
        return 1;
    return 0;
}

WebInspector.NetworkTimelineEntry = function(panel, resource)
{
    this.panel = panel;
    this.resource = resource;
    resource.networkTimelineEntry = this;

    this.resourceElement = document.createElement("div");
    this.resourceElement.className = "network-resource";
    this.resourceElement.timelineEntry = this;

    this.titleElement = document.createElement("div");
    this.titleElement.className = "network-title";
    this.resourceElement.appendChild(this.titleElement);

    this.fileElement = document.createElement("div");
    this.fileElement.className = "network-file";
    this.fileElement.innerHTML = WebInspector.linkifyURL(resource.url, resource.displayName);
    this.titleElement.appendChild(this.fileElement);

    this.tipButtonElement = document.createElement("button");
    this.tipButtonElement.className = "tip-button";
    this.showingTipButton = this.resource.tips.length;
    this.fileElement.insertBefore(this.tipButtonElement, this.fileElement.firstChild);

    var entry = this;
    this.tipButtonElement.addEventListener("click", function(event) { entry.toggleTipBalloon(event) }, false );

    this.areaElement = document.createElement("div");
    this.areaElement.className = "network-area";
    this.titleElement.appendChild(this.areaElement);

    this.barElement = document.createElement("div");
    this.areaElement.appendChild(this.barElement);

    this.infoElement = document.createElement("div");
    this.infoElement.className = "network-info hidden";
    this.resourceElement.appendChild(this.infoElement);
}

WebInspector.NetworkTimelineEntry.prototype = {
    refresh: function(skipBoundryUpdate, skipTimelineSort, immediate)
    {
        if (!this.panel.visible) {
            this.needsRefresh = true;
            this.panel.needsRefresh = true;
            return;
        }

        delete this.needsRefresh;

        if (!skipBoundryUpdate) {
            if (this.panel.updateTimelineBoundriesIfNeeded(this.resource, immediate))
                return; // updateTimelineBoundriesIfNeeded calls refresh() on all entries, so we can just return
        }

        if (!skipTimelineSort) {
            if (immediate)
                this.panel.sortTimelineEntriesIfNeeded();
            else
                this.panel.sortTimelineEntriesSoonIfNeeded();
        }

        if (this.resource.startTime !== -1) {
            var percentStart = ((this.resource.startTime - this.panel.earliestStartTime) / this.panel.totalDuration) * 100;
            this.barElement.style.left = percentStart + "%";
        } else {
            this.barElement.style.left = null;
        }

        if (this.resource.endTime !== -1) {
            var percentEnd = ((this.panel.latestEndTime - this.resource.endTime) / this.panel.totalDuration) * 100;
            this.barElement.style.right = percentEnd + "%";
        } else {
            this.barElement.style.right = "0px";
        }

        this.barElement.className = "network-bar network-category-" + this.resource.category.title;

        if (this.infoNeedsRefresh)
            this.refreshInfo();
    },

    refreshInfo: function()
    {
        if (!this.showingInfo) {
            this.infoNeedsRefresh = true;
            return;
        }

        if (!this.panel.visible) {
            this.panel.needsRefresh = true;
            this.infoNeedsRefresh = true;
            return;
        }

        this.infoNeedsRefresh = false;

        this.infoElement.removeChildren();

        var sections = [
            {title: "Request", info: this.resource.sortedRequestHeaders},
            {title: "Response", info: this.resource.sortedResponseHeaders}
        ];

        function createSectionTable(section) {
            if (!section.info.length)
                return;

            var table = document.createElement("table");
            this.infoElement.appendChild(table);

            var heading = document.createElement("th");
            heading.innerText = section.title;
            var row = table.createTHead().insertRow(-1).appendChild(heading);
            var body = document.createElement("tbody");
            table.appendChild(body);

            section.info.forEach(function(header) {
                var row = body.insertRow(-1);
                var th = document.createElement("th");
                th.innerText = header.header;
                row.appendChild(th);
                row.insertCell(-1).innerText = header.value;
            });
        }
        sections.forEach(createSectionTable, this);
    },

    refreshInfoIfNeeded: function()
    {
        if (this.infoNeedsRefresh === false)
            return;

        this.refreshInfo();
    },

    toggleShowingInfo: function()
    {
        this.showingInfo = !this.showingInfo;
    },

    get showingInfo()
    {
        return this._showingInfo;
    },

    set showingInfo(x)
    {
        if (this._showingInfo === x)
            return;

        this._showingInfo = x;

        var element = this.infoElement;
        if (x) {
            element.removeStyleClass("hidden");
            element.style.setProperty("overflow", "hidden");
            this.refreshInfoIfNeeded();
            WebInspector.animateStyle([{element: element, start: {height: 0}, end: {height: element.offsetHeight}}], 250, function() { element.style.removeProperty("height"); element.style.removeProperty("overflow") });
        } else {
            element.style.setProperty("overflow", "hidden");
            WebInspector.animateStyle([{element: element, end: {height: 0}}], 250, function() { element.addStyleClass("hidden"); element.style.removeProperty("height") });
        }
    },

    get showingTipButton()
    {
        return !this.tipButtonElement.hasStyleClass("hidden");
    },

    set showingTipButton(x)
    {
        if (x)
            this.tipButtonElement.removeStyleClass("hidden");
        else
            this.tipButtonElement.addStyleClass("hidden");
    },

    toggleTipBalloon: function(event)
    {
        this.showingTipBalloon = !this.showingTipBalloon;
        event.stopPropagation();
    },

    get showingTipBalloon()
    {
        return this._showingTipBalloon;
    },

    set showingTipBalloon(x)
    {
        if (this._showingTipBalloon === x)
            return;

        this._showingTipBalloon = x;

        if (x) {
            if (!this.tipBalloonElement) {
                this.tipBalloonElement = document.createElement("div");
                this.tipBalloonElement.className = "tip-balloon";
                this.titleElement.appendChild(this.tipBalloonElement);

                this.tipBalloonContentElement = document.createElement("div");
                this.tipBalloonContentElement.className = "tip-balloon-content";
                this.tipBalloonElement.appendChild(this.tipBalloonContentElement);
                var tipText = '';
                for (var id in this.resource.tips)
                    tipText += this.resource.tips[id].message + "\n";
                this.tipBalloonContentElement.textContent = tipText;
            }

            this.tipBalloonElement.removeStyleClass("hidden");
            WebInspector.animateStyle([{element: this.tipBalloonElement, start: {left: 160, opacity: 0}, end: {left: 145, opacity: 1}}], 250);
        } else {
            var element = this.tipBalloonElement;
            WebInspector.animateStyle([{element: this.tipBalloonElement, start: {left: 145, opacity: 1}, end: {left: 160, opacity: 0}}], 250, function() { element.addStyleClass("hidden") });
        }
    }
}

WebInspector.TimelineValueCalculator = function()
{
}

WebInspector.TimelineValueCalculator.prototype = {
    computeValues: function(entries)
    {
        var total = 0;
        var categoryValues = {};

        function compute(entry)
        {
            var value = this._value(entry);
            if (value === undefined)
                return;

            if (!(entry.resource.category in categoryValues))
                categoryValues[entry.resource.category] = 0;
            categoryValues[entry.resource.category] += value;
            total += value;
        }
        entries.forEach(compute, this);

        return {categoryValues: categoryValues, total: total};
    },

    _value: function(entry)
    {
        return 0;
    },

    get title()
    {
        return "";
    },

    formatValue: function(value)
    {
        return value.toString();
    }
}

WebInspector.TransferTimeCalculator = function()
{
    WebInspector.TimelineValueCalculator.call(this);
}

WebInspector.TransferTimeCalculator.prototype = {
    computeValues: function(entries)
    {
        var entriesByCategory = {};
        entries.forEach(function(entry) {
            if (!(entry.resource.category in entriesByCategory))
                entriesByCategory[entry.resource.category] = [];
            entriesByCategory[entry.resource.category].push(entry);
        });

        var earliestStart;
        var latestEnd;
        var categoryValues = {};
        for (var category in entriesByCategory) {
            entriesByCategory[category].sort(WebInspector.NetworkPanel.timelineEntryCompare);
            categoryValues[category] = 0;

            var segment = {start: -1, end: -1};
            entriesByCategory[category].forEach(function(entry) {
                if (entry.resource.startTime == -1 || entry.resource.endTime == -1)
                    return;

                if (earliestStart === undefined)
                    earliestStart = entry.resource.startTime;
                else
                    earliestStart = Math.min(earliestStart, entry.resource.startTime);

                if (latestEnd === undefined)
                    latestEnd = entry.resource.endTime;
                else
                    latestEnd = Math.max(latestEnd, entry.resource.endTime);

                if (entry.resource.startTime <= segment.end) {
                    segment.end = Math.max(segment.end, entry.resource.endTime);
                    return;
                }

                categoryValues[category] += segment.end - segment.start;

                segment.start = entry.resource.startTime;
                segment.end = entry.resource.endTime;
            });

            // Add the last segment
            categoryValues[category] += segment.end - segment.start;
        }

        return {categoryValues: categoryValues, total: latestEnd - earliestStart};
    },

    get title()
    {
        return "Transfer Time";
    },

    formatValue: function(value)
    {
        return Number.secondsToString(value);
    }
}

WebInspector.TransferTimeCalculator.prototype.__proto__ = WebInspector.TimelineValueCalculator.prototype;

WebInspector.TransferSizeCalculator = function()
{
    WebInspector.TimelineValueCalculator.call(this);
}

WebInspector.TransferSizeCalculator.prototype = {
    _value: function(entry)
    {
        return entry.resource.contentLength;
    },

    get title()
    {
        return "Transfer Size";
    },

    formatValue: function(value)
    {
        return Number.bytesToString(value);
    }
}

WebInspector.TransferSizeCalculator.prototype.__proto__ = WebInspector.TimelineValueCalculator.prototype;

function makeRoundedRectPath(ctx, x, y, w, h, r) {
    ctx.beginPath();
    ctx.moveTo(x, y + r);
    ctx.lineTo(x, y + h - r);
    ctx.quadraticCurveTo(x, y + h, x + r, y + h);
    ctx.lineTo(x + w - r, y + h);
    ctx.quadraticCurveTo(x + w, y + h, x + w, y + h - r);
    ctx.lineTo(x + w, y + r);
    ctx.quadraticCurveTo(x + w, y, x + w - r, y);
    ctx.lineTo(x + r, y);
    ctx.quadraticCurveTo(x, y, x, y + r);
    ctx.closePath();
}

function drawPillShading(ctx, x, y, w, h) {
    var lightGradient = ctx.createLinearGradient(x, y, x, y + (h / 1.5));
    lightGradient.addColorStop(0.0, 'rgba(220, 220, 220, 0.6)');
    lightGradient.addColorStop(0.4, 'rgba(220, 220, 220, 0.2)');
    lightGradient.addColorStop(1.0, 'rgba(255, 255, 255, 0.0)');

    var darkGradient = ctx.createLinearGradient(x, y + (h / 3), x, y + h);
    darkGradient.addColorStop(0.0, 'rgba(0, 0, 0, 0.0)');
    darkGradient.addColorStop(0.8, 'rgba(0, 0, 0, 0.2)');
    darkGradient.addColorStop(1.0, 'rgba(0, 0, 0, 0.5)');

    ctx.fillStyle = darkGradient;
    ctx.fillRect(x, y, w, h);

    ctx.fillStyle = lightGradient;
    ctx.fillRect(x, y, w, h);
}

function drawDividerLines(ctx, x, y, w, h) {
    ctx.save();

    ctx.lineWidth = 1;

    for (var i = 1; i < 20; ++i) {
        ctx.beginPath();
        ctx.moveTo(x + (i * Math.round(w / 20)) + 0.5, y);
        ctx.lineTo(x + (i * Math.round(w / 20)) + 0.5, y + h);
        ctx.closePath();

        ctx.strokeStyle = 'rgba(0, 0, 0, 0.2)';
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(x + (i * Math.round(w / 20)) + 1.5, y);
        ctx.lineTo(x + (i * Math.round(w / 20)) + 1.5, y + h);
        ctx.closePath();

        ctx.strokeStyle = 'rgba(255, 255, 255, 0.2)';
        ctx.stroke();
    }

    ctx.restore();
}

function fadeOutRect(ctx, x, y, w, h, a1, a2) {
    ctx.save();

    var gradient = ctx.createLinearGradient(x, y, x, y + h);
    gradient.addColorStop(0.0, 'rgba(0, 0, 0, ' + (1.0 - a1) + ')');
    gradient.addColorStop(0.8, 'rgba(0, 0, 0, ' + (1.0 - a2) + ')');
    gradient.addColorStop(1.0, 'rgba(0, 0, 0, 1.0)');

    ctx.globalCompositeOperation = 'destination-out';

    ctx.fillStyle = gradient;
    ctx.fillRect(x, y, w, h);

    ctx.restore();
}

function drawPillSegments(ctx, x, y, w, h, segments) {
    var total = 0;
    for (var i = 0; i < segments.length; ++i)
        total += segments[i].value;

    var percents = segments.map(function(s) { return Math.max(Math.round(100 * s.value / total), 1) });
    var percentTotal = 0;
    for (var i = 0; i < percents.length; ++i)
        percentTotal += percents[i];

    while (percentTotal > 100) {
        var difference = percentTotal - 100;
        for (var i = 0; i < percents.length && percentTotal > 100; ++i) {
            if (percents[i] > 1) {
                --percents[i];
                --percentTotal;
            }
        }
    }

    var previousWidth = 0;
    for (var i = 0; i < segments.length; ++i) {
        var width = Math.round(w * percents[i] / 100);
        ctx.fillStyle = segments[i].color;
        ctx.fillRect(x + previousWidth, y, width, h);
        previousWidth += width;
    }
}

function drawPill(ctx, x, y, w, h, r, segments) {
    ctx.save();

    makeRoundedRectPath(ctx, x, y, w, h, r);
    ctx.clip();

    drawPillSegments(ctx, x, y, w, h, segments);
    drawDividerLines(ctx, x, y, w, h);
    drawPillShading(ctx, x, y, w, h);

    ctx.restore();
}

function drawShadowLine(ctx, x, y, w, h) {
    ctx.beginPath();
    ctx.moveTo(x + 1.5, y + 0.5);
    ctx.lineTo(x + w - 1.5, y + 0.5);
    ctx.closePath();

    ctx.strokeStyle = 'rgba(0, 0, 0, 0.2)';
    ctx.lineWidth = h;
    ctx.stroke();
}

function drawSwatchSquare(ctx, color) {
    ctx.save();

    ctx.fillStyle = color;
    ctx.fillRect(0, 0, 13, 13);

    var gradient = ctx.createLinearGradient(0, 0, 13, 13);
    gradient.addColorStop(0.0, 'rgba(255, 255, 255, 0.2)');
    gradient.addColorStop(1.0, 'rgba(255, 255, 255, 0.0)');

    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, 13, 13);

    gradient = ctx.createLinearGradient(13, 13, 0, 0);
    gradient.addColorStop(0.0, 'rgba(0, 0, 0, 0.2)');
    gradient.addColorStop(1.0, 'rgba(0, 0, 0, 0.0)');

    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, 13, 13);

    ctx.strokeStyle = "rgba(0, 0, 0, 0.6)";
    ctx.strokeRect(0.5, 0.5, 12, 12);

    ctx.restore();
}
