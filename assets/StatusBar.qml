import bb.cascades 1.4

Container {
    id: statusBarContainer
    property alias text: statusBarLabel.text
    property alias text2: statusBarLabel2.text
    Container {
        Divider {
        }
    }
    Container {
        leftMargin: 20.0
        leftPadding: 20.0
        bottomPadding: 10.0
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                id: statusBarLabel
                text: "statusbar"
                textStyle.color: Color.Gray
                textStyle.fontSize: FontSize.XSmall
                textStyle.fontWeight: FontWeight.Bold
                verticalAlignment: VerticalAlignment.Center
            }
            Label {
                visible: false
                id: statusBarLabel2
                text: "statusbar2"
                textStyle.color: Color.Gray
                textStyle.fontSize: FontSize.XSmall
                textStyle.fontWeight: FontWeight.Bold
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }
}
