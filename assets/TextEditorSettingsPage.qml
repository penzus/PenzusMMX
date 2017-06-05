import bb.cascades 1.4
import bb.system 1.2

Page {
    id: textEditorSettingsPage
    objectName: "TextEditorSettingsPage"
    titleBar: TitleBar {
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
        title: qsTr("Settings")
    }
    function save() {
        settings.textFormat = textFormatDropDown.selectedOption.value
        settings.fontWeight = fontWeightDropDown.selectedOption.value
        settings.fontStyle = fontStyleDropDown.selectedOption.value
        settings.fontSize = fontSizeDropDown.selectedOption.value
        settings.save()

    }
    onCreationCompleted: {
        textFormatDropDown.selectedIndex = settings.textFormat - 1
        setData(fontWeightDropDown, settings.fontWeight)
        setData(fontStyleDropDown, settings.fontStyle)
        setData(fontSizeDropDown, settings.fontSize)
    }
    
    function setData(dropDown, item) {
        dropDown.selectedIndex = 0;
        for (var i = 0; i < dropDown.count(); i ++) {
            var option = dropDown.at(i)
            if (option.value == item) {
                dropDown.selectedIndex = i
                break
            }
        }
    }
    
    
    Container {
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                leftPadding: 20.0
                rightPadding: 20.0
                topPadding: 20.0
                bottomPadding: 20.0
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                
                
                DropDown {
                    visible: false
                    id: fontWeightDropDown
                    title: qsTr("Font weight")
                    horizontalAlignment: HorizontalAlignment.Fill
                    Option {
                        text: qsTr("Default")
                        value: FontWeight.Default
                        selected: true
                    }
                    Option {
                        text: qsTr("Normal")
                        value: FontWeight.Normal
                    }
                    Option {
                        text: qsTr("Bold")
                        value: FontWeight.Bold
                    }
                    Option {
                        text: "W100"
                        value: FontWeight.W100
                    }
                    Option {
                        text: "W200"
                        value: FontWeight.W200
                    }
                    Option {
                        text: "W300"
                        value: FontWeight.W300
                    }
                    Option {
                        text: "W400"
                        value: FontWeight.W400
                    }
                    Option {
                        text: "W500"
                        value: FontWeight.W500
                    }
                    Option {
                        text: "W600"
                        value: FontWeight.W600
                    }
                    Option {
                        text: "W700"
                        value: FontWeight.W700
                    }
                    Option {
                        text: "W800"
                        value: FontWeight.W800
                    }
                    Option {
                        text: "W900"
                        value: FontWeight.W900
                    }
                    
                    onSelectedValueChanged: {
                        textArea.textStyle.FontWeight = selectedValue
                    }
                }
                Divider {

                }
                DropDown {
                    id: textFormatDropDown
                    title: qsTr("Text format")
                    horizontalAlignment: HorizontalAlignment.Fill
                    Option {
                        text: qsTr("Plain")
                        value: "1"
                    }
                    Option {
                        text: qsTr("HTML")
                        value: "2"
                    }
                    Option {
                        selected: true
                        text: qsTr("Auto")
                        value: "3"
                    }
                }
                
                Divider {

                }
                DropDown {
                    id: fontStyleDropDown
                    title: qsTr("Font style")
                    horizontalAlignment: HorizontalAlignment.Fill
                    Option {
                        text: qsTr("Default")
                        selected: true
                        value: FontStyle.Default
                    }
                    Option {
                        text: qsTr("Normal")
                        value: FontStyle.Normal
                    }
                    Option {
                        text: qsTr("Italic")
                        value: FontStyle.Italic
                    }
                    onSelectedValueChanged: {
                        textArea.textStyle.fontStyle = selectedValue
                    }
                }
                Divider {

                }
                DropDown {
                    id: fontSizeDropDown
                    title: qsTr("Font size")
                    horizontalAlignment: HorizontalAlignment.Fill
                    Option {
                        selected: true
                        text: qsTr("Default")
                        value: FontSize.Default
                    }
                    Option {
                        text: "XXSmall"
                        value: FontSize.XXSmall
                    }
                    Option {
                        text: "XSmall"
                        value: FontSize.XSmall
                    }
                    Option {
                        text: "Small"
                        value: FontSize.Small
                    }
                    Option {
                        text: "Medium"
                        value: FontSize.Medium
                    }
                    Option {
                        text: "Large"
                        value: FontSize.Large
                    }
                    Option {
                        text: "XLarge"
                        value: FontSize.XLarge
                    }
                    Option {
                        text: "XXLarge"
                        value: FontSize.XXLarge
                    }
                    onSelectedValueChanged: {
                        textArea.textStyle.fontSize = selectedValue
                    }
                }
                Divider {

                }
                TextArea {
                    id: textArea
                    text: qsTr("The quick brown fox jumps over the lazy dog.")
                    editable: false

                }
            }
        }
    }
}
