import bb.cascades 1.4

Container {
    property alias imageSource: imageView.imageSource

    topPadding: ui.sdu(1)
    leftPadding: ui.sdu(1)
    rightPadding: ui.sdu(1)
    bottomPadding: ui.sdu(1)

    signal clicked()
    layout: DockLayout {

    }
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    background: Color.Transparent

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        layout: DockLayout {
        }
        ImageView {
            id: imageView
            maxWidth: ui.sdu(14)
            maxHeight: ui.sdu(14)
            minWidth: ui.sdu(14)
            minHeight: ui.sdu(14)
            imageSource: "asset:///images/cloud/dropbox.png"
        }
    }

    onTouch: {
        if (event.touchType == TouchType.Down) {
            imageView.opacity = 0.5
        } else if (event.touchType == TouchType.Cancel) {
            imageView.opacity = 1.0
        } else if (event.touchType == TouchType.Up) {
            imageView.opacity = 1.0
            clicked()
        }
    }
}
