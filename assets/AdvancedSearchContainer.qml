import bb.cascades 1.4
import CustomTimer 1.0

Container {
    id: avdancedContainer

    property alias dateSearchEnabled: checkBox.checked
    property alias startDate: startDateTimePicker
    property alias endDate: endDateTimePicker

    onCreationCompleted: {
        timer.start()
    }

    attachedObjects: [
        QTimer {
            id: timer
            singleShot: false
            interval: 10
            property int counter: 0
            onTimeout: {
                checkBox.checked = !checkBox.checked
                if (++counter == 3) {
                    timer.stop()
                }
            }
        }
    ]

    Header {
        title: qsTr("Date limits")
    }
    Container {
        topPadding: 20.0
        bottomPadding: 20.0
        horizontalAlignment: HorizontalAlignment.Fill
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        CheckBox {
            id: checkBox
            checked: true
            text: qsTr("")
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            onCheckedChanged: {
                startDateTimePicker.enabled = checked
                endDateTimePicker.enabled = checked
            }
        }

        DateTimePicker {
            id: startDateTimePicker
            verticalAlignment: VerticalAlignment.Center
        }
        DateTimePicker {
            id: endDateTimePicker
            verticalAlignment: VerticalAlignment.Center
            enabled: false
        }
    }
}
