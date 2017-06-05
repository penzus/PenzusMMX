import bb.cascades 1.4

Container {
    property alias text: textLabel.text
    signal clicked()
    
    minHeight: 108
    layout: DockLayout {

    }
    id: myContainer
    background: Color.White
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    Container {
        verticalAlignment: VerticalAlignment.Center
        leftPadding: 20.0
        NativeLabel {
            id: textLabel
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            textStyle.fontWeight: FontWeight.Normal
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
