import bb.cascades 1.4
import bb.cascades.pickers 1.0
import QtQuick 1.0

Container {
    id: inputItem
    
    property alias title: header.title
    property alias fileName: dataLabel.text
    property string absFileNamePath
    
    signal absFileNamePathChanged(string newAbsFileNamePath)
    
    
    function reset() {
        dataLabel.text = ""
        pathContainer.visible = false
    }
    
    function setTypeMusic(extension) {
        filePicker.type = FileType.Music
        filePicker.filter = extension
    }
    
    function update() {
        dataLabel.text = extractFileNameFromPath(absFileNamePath)
        dataLabel.visible = true
        if (absFileNamePath.length > 0 ) {
            pathLabel.text = qsTr("Path:" ) + " " + fileUtils.getHumanReadablePath(fileUtils.absolutePath(absFileNamePath))
            pathContainer.visible = true
        } else {
            pathContainer.visible = false
        }
    }
    
    function extractFileNameFromPath(fileName) {
        return fileName.substring(fileName.lastIndexOf("/") + 1, fileName.length)
    }
    Header {
        id: header
        title: qsTr("Input folder")
        mode: HeaderMode.Plain
    }
    Container {
        id: topContainer
        topPadding: 20.0
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Fill
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            id: notSetLabel
            visible: dataLabel.text.length == 0
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10.0
            }
            verticalAlignment: VerticalAlignment.Center
            text: qsTr("<Not selected>")
            textStyle.color: Color.Red
        }
        Label {
            id: dataLabel
            visible: false
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10.0
            }
            verticalAlignment: VerticalAlignment.Center
        }
        
        Button {
            maxWidth: 250
            id: selectFileButton
            text: qsTr("Select")
            verticalAlignment: VerticalAlignment.Center
            function selectDir() {
                filePicker.title = qsTr("Select folder")
                filePicker.mode = FilePickerMode.SaverMultiple
                filePicker.directories = [ absFileNamePath ]
                filePicker.open()
            }
            
            onClicked: {
                selectDir()
            }
        }
        bottomPadding: 20.0
    }
    
    Container {
        id: pathContainer
        visible: dataLabel.visible
        Label {
            multiline: true
            id: pathLabel
            text: "Path: "
            textStyle.fontSize: FontSize.XSmall
            textStyle.color: Color.Gray
            verticalAlignment: VerticalAlignment.Center
        }
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Center
    }
    
    attachedObjects: [
        FilePicker {
            id: filePicker
            title: qsTr("Select")
            mode: FilePickerMode.SaverMultiple
            allowOverwrite: false
            type: FileType.Music
            filter: [ "*.*" ]
            property variant lastSelectedFiles
            onFileSelected: {
                absFileNamePath = selectedFiles[0]
                lastSelectedFiles = selectedFiles.slice()
                inputItem.update()
                absFileNamePathChanged(absFileNamePath)
            }
        }
    ]
}
