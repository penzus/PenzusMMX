import bb.cascades 1.4

Container {
    property alias text: label.text
    property alias data: extField.text
    property bool checkable: false
    property bool checked: labelCheckBox.checked
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }

    Label {
        minWidth: ui.sdu(28)
        maxWidth: ui.sdu(28)
        id: label
        text: "-"
        verticalAlignment: VerticalAlignment.Center
        visible: !checkable
    }
    CheckBox {
        visible: checkable
        minWidth: 280
        maxWidth: 280
        id: labelCheckBox
        text: label.text
        verticalAlignment: VerticalAlignment.Center
    }
    TextArea {
        id: extField
        enabled: !(labelCheckBox.visible && !labelCheckBox.checked)
        verticalAlignment: VerticalAlignment.Center
        maximumLength: 500
        textFormat: TextFormat.Plain
        inputMode: TextAreaInputMode.Text
        hintText: ""
    }
}
