import bb.cascades 1.4
import bb.cascades.pickers 1.0
import bb.system 1.2
import QtQuick 1.0

Container {
    id: coverItem
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    property int imageSize: ui.sdu(60)
    property int operationType: 0
    // 0 - no changes
    // 1 - delete
    // 2 - update
    
    property string  originalImageSource
    property string newImageSource
    property bool  waitingForChildCard: false
    
    function reset() {
        operationType = 0
        originalImageSource = ""
        newImageSource = ""
    }
    
    function load( imageFileName) {
        operationType = 0
        originalImageSource = imageFileName
        imageView.imageSource = (imageFileName.length > 0) ? fileUtils.pathToUrl(imageFileName) : ""
        updateGui()
    }
    
    function updateGui() {
        deleteCoverButton.enabled = (imageView.imageSource == "") ? false : true
    }
    
    function setDelete() {
        operationType = 1
        imageView.imageSource = ""
        newImageSource = ""
        updateGui()
    }
    
    function setCover(imageFileName) {
        operationType = 2
        newImageSource =  imageFileName
        imageView.imageSource = fileUtils.pathToUrl(imageFileName)//"file://" + imageFileName
        updateGui()
    }
    

    Container {
        Container {
            topMargin: 20.0
            topPadding: 5.0
            bottomPadding: 5.0
            leftPadding: 5.0
            rightPadding: 5.0
            background: Color.Gray
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Container {
                id: noCoverContainer
                visible: !imageView.visible
                layout: DockLayout {
                }
                minWidth: imageSize
                minHeight: imageSize
                maxWidth: imageSize
                maxHeight: imageSize
                Label {
                    id: noCoverLabel
                    text: qsTr("Not set")
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            ImageView {
                id: imageView
                visible: (imageSource == "") ? false : true
                minWidth: imageSize
                minHeight: imageSize
                maxWidth: imageSize
                maxHeight: imageSize
            }
            gestureHandlers: [
                TapHandler {
                    onTapped: {
                        filePicker.open()
                    }
                }
            ]
        }
        Container {
            topPadding: 20.0
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            maxWidth: imageSize + 10
            Button {
                id: selectCoverButton
                text: qsTr("Select")
                onClicked: {
                    filePicker.open()
                }
            }
            Button {
                id: deleteCoverButton
                text: qsTr("Delete")
                onClicked: {
                    coverItem.setDelete()
                }
            }
        }
    }
    
    attachedObjects: [
        SystemDialog {
            id: askDialog
            body: qsTr("Do you want to set original file?")
            cancelButton.label: qsTr("Cancel")
            confirmButton.label: qsTr("Edit")
            customButton.label: qsTr("Set original")
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    coverItem.waitingForChildCard = true
                    myApp.openEditPictureFile( filePicker.selectedFileName )
                } else if (result == SystemUiResult.CustomButtonSelection) {
                    coverItem.setCover( filePicker.selectedFileName )
                }
            }
        },
        Connections {
            target: myApp
            onFileEditingDone: {
                if (!coverItem.waitingForChildCard) {
                    return
                }
                coverItem.waitingForChildCard = false
                coverItem.setCover( filePath )
            }
        },
        FilePicker {
            id: filePicker
            title: qsTr("Select")
            mode: FilePickerMode.Picker
            type: FileType.Picture
            filter: [ "*.*" ]
            property string selectedFileName
            onFileSelected: {
                //coverItem.waitingForChildCard = true
                //myApp.openEditPictureFile( selectedFiles[0] )
                selectedFileName = selectedFiles[0]
                askDialog.show()
            }
        }
    ]
}
