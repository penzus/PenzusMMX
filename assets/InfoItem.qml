import bb.cascades 1.4

Container {
    property alias text: label.text
    property alias data: extField.text
    property alias multiline: extField.multiline
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    Label {
        minWidth: ui.sdu(28)
        maxWidth: ui.sdu(28)
        id: label
        text: "-"
        verticalAlignment: VerticalAlignment.Top
    }
    Label {
        id: extField
        verticalAlignment: VerticalAlignment.Center
        textFormat: TextFormat.Plain
    }
}
