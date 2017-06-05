import bb.cascades 1.4

Container {
    id: noItemsToDisplay
    property alias text1: textLabel1.text
    
    Container {
        id: spaceContainer1
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10.0
        }
    }
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    Container {
        property int containerSize: ui.sdu(82)//590.0
        
        minWidth: containerSize
        minHeight: containerSize
        preferredWidth: containerSize
        preferredHeight: containerSize
        maxWidth: containerSize
        maxHeight: containerSize
        
        id: container
        layout: DockLayout {
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10.0
        }
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Label {
                id: textLabel1
                text: qsTr("No items to display")
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                bottomMargin: 50.0
            }
        }
    }
    Container {
        id: spaceContainer2
        layoutProperties: StackLayoutProperties {
            spaceQuota: 10.0
        }
    }
}
