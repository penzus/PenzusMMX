import bb.cascades 1.4

Container {
    horizontalAlignment: HorizontalAlignment.Left
    property bool initialized: false
    property alias labelMinWidth: label.minWidth
    property alias labelMaxWidth: label.maxWidth
    property alias text: label.text
    property alias checked: checkBox.checked

    signal myCheckedChanged(bool checked)

    layout: StackLayout {
        orientation: LayoutOrientation.RightToLeft
    }
    ToggleButton {
        id: checkBox
        checked: false
        verticalAlignment: VerticalAlignment.Center
        onCheckedChanged: {
            if (initialized) {
                myCheckedChanged(checked)
            }
        }
    }
    Label {
        id: label
        verticalAlignment: VerticalAlignment.Center
        layoutProperties: StackLayoutProperties {
            spaceQuota: 0.1
        }
    }
}
