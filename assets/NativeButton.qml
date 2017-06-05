import bb.cascades 1.4

Container {
    property alias text: textLabel.text

    layoutProperties: StackLayoutProperties {
        spaceQuota: 1.0
    }
    signal clicked()

    layout: DockLayout {

    }
    
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    background: Color.Transparent
    Container {
        id: myContainer
        background: Color.create("#f0f0f0")
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        layout: DockLayout {
        }
        NativeLabel {
            id: textLabel
        }
    }
    onTouch: {
        if (event.touchType == TouchType.Down) {
            myContainer.background = Color.create("#e5e5e5")
        } else if (event.touchType == TouchType.Cancel) {
            myContainer.background = Color.create("#f0f0f0")
        } else if (event.touchType == TouchType.Up) {
            myContainer.background = Color.create("#f0f0f0")
            clicked()
        }
    }
}
