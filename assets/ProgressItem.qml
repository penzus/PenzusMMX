import bb.cascades 1.4

Container {
    id: progressItem
    property alias titile: header.title
    property alias subtitile: header.subtitle
    
    signal clicked()
    opacity: 0.8
    
    onTouch: {
        if (event.touchType == TouchType.Down) {
            progressItem.opacity = 1.0
        } else if (event.touchType == TouchType.Cancel) {
            progressItem.opacity = 0.8
        } else if (event.touchType == TouchType.Up) {
            progressItem.opacity = 0.8
            progressItem.clicked()
        }
    }

    function setUsage(used, total, percent, text) {
        textLabel.text = text
        subtitile = percent + "%" 
        progressIndicator.toValue = total
        progressIndicator.value = used
    }

    Header {
        id: header
        title: "-"
        subtitle: "-"
        bottomMargin: 15.0
    }

    Label {
        id: textLabel
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Left
        text: "-"
    }
    
    ProgressIndicator {
        id: progressIndicator
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Fill
        value: 50.0
        toValue: 100.0
        state: ProgressIndicatorState.Progress
    }
    bottomPadding: 45.0
}
