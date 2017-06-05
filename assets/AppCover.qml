import bb.cascades 1.4

Container {
    id: appCoverContainer
    layout: DockLayout {
        
    }

    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    background: Color.Black
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            imageSource: "asset:///images/logo.png"
        }
        Label {
            text: myApp.officialAppDescriptionUpper()
            textStyle.fontSize: FontSize.XXSmall
            textStyle.color: Color.Gray
        }
    }
}
