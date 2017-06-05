import bb.cascades 1.4

Container {
    property alias text: textLabel.text
    property alias topText: textLabel1.text
    property alias bottomText: textLabel3.text
    property string imageSource
    property alias imageViewMaxWidth:  imageView.maxWidth
    property alias imageViewMaxHeight:  imageView.maxHeight
    property alias imageViewMinWidth:  imageView.minWidth
    property alias imageViewMinHeight:  imageView.minHeight
    
    topPadding: ui.sdu(1)
    leftPadding: ui.sdu(1)
    rightPadding: ui.sdu(1)
    bottomPadding: ui.sdu(1)
    
    signal clicked()
    
    id: appCoverContainer
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
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///images/circle-xxl.png"
            
            maxWidth: ui.sdu(30)
            maxHeight: ui.sdu(30)
            minWidth: ui.sdu(30)
            minHeight: ui.sdu(30)
        }
        Label {
            id: textLabel
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("History")
            textStyle.color: Color.Black
            textStyle.fontSize: FontSize.Medium
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            Label {
                id: textLabel1
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle.color: Color.DarkGray
            }
            Label {
                id: textLabel2
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                text: " "
                textStyle.color: Color.Black
            }
            Label {
                id: textLabel3
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                textStyle.color: Color.DarkGray
            }   
        }
    }
    onTouch: {
        if (event.touchType == TouchType.Down) {
            imageView.opacity = 0.2
        } else if (event.touchType == TouchType.Cancel) {
            imageView.opacity = 1.0
        } else if (event.touchType == TouchType.Up) {
            imageView.opacity = 1.0
            clicked()
        }
    }
}
