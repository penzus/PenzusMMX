import bb.cascades 1.4
import bb.system 1.2
import QtQuick 1.0
import TagEditor 1.0


Page {
    id: propertiesPage
    
    signal closeRequest()
    
    titleBar: TitleBar {
        id: titleBar
        title: qsTr("Properties")
        dismissAction: ActionItem {
            id: dismissAction
            title: qsTr("Cancel")
            onTriggered: {
                propertiesPage.closeSheet()
            }
        }
        acceptAction: ActionItem {
            id: acceptAction
            title: qsTr("OK")
            onTriggered: {
                // multimedia - .nomultimedia
                if (properties.isDir()) {
                    if (otherInfoContainer.visible) {
                        if (!properties.setNoMultimediaProperty(noMultimediaCheckBox.checked)) {
                            propertiesPage.showMessage(qsTr("Cannot set .nomedia property"))
                        }
                    }
                }
                
                // audio tag editor
                if (!properties.isAudioFileWithEditableTags()) {
                    propertiesPage.closeSheet()
                    return
                }
                
                var tagEditor = properties.tagEditor()
                var tagInfo = tagEditor.tagInfo()
                
                var dataWasChanged = false
                
                if (tagInfo.title != titleItem.data) {
                    dataWasChanged = true
                    tagInfo.title = titleItem.data
                }
                
                if ( tagInfo.artist != artistItem.data) {
                    dataWasChanged = true
                    tagInfo.artist = artistItem.data
                }
                
                if ( tagInfo.album != albumItem.data) {
                    dataWasChanged = true
                    tagInfo.album = albumItem.data
                }
                
                if (tagInfo.comment != commentItem.data) {
                    dataWasChanged = true
                    tagInfo.comment = commentItem.data
                }
                
                if (tagInfo.genre != genreItem.data) {
                    dataWasChanged = true
                    tagInfo.genre = genreItem.data
                }
                
                if ( tagInfo.year != Number(yearItem.data)) {
                    dataWasChanged = true
                    tagInfo.year = Number(yearItem.data)
                }
                
                if (tagInfo.track != Number(trackItem.data)) {
                    dataWasChanged = true
                    tagInfo.track = Number(trackItem.data)
                }
                
                if (dataWasChanged) {
                    if (!tagEditor.save()) {
                        propertiesPage.showMessage(qsTr("Error saving audio data"))
                        return
                    }
                }
                
                propertiesPage.closeSheet()
            }
        }
    }
    
    function closeSheet() {
        if(myApp.invokedAsCard()) {
            myApp.closeCard()
        }
        closeRequest()
    }
    
    function showMessage(message) {
        popupMessage.body = message
        popupMessage.show()
    }
    
    function updatePrivateInfo(directoriesCount,filesCount) {
        return qsTr("folder count") + ": " + directoriesCount + ", " + qsTr("file count" )  +  ": " + filesCount
    }
    
    attachedObjects: [
        Connections {
            target: properties
            onDirectoryContentInfoChanged: {
                sizeInfoItem.data = totalSize
                contentsInfoItem.data = updatePrivateInfo(directoriesCount,filesCount)//qsTr("Folder count") + ": " + directoriesCount + ", " + qsTr("file count" )  +  ": " + filesCount
            }
        },
        Connections {
            target: properties
            onSelectedItemsInfoChanged: {
                fileInfoItem.data =  updatePrivateInfo(directoriesCount,filesCount)//qsTr("Folder count") + ": " + directoriesCount + ", " + qsTr("file count" )  +  ": " + filesCount
            }
        },
        Connections {
            target: myApp
            onHandleInvokeFinished: {
                if (myApp.invokedAsCard()) {
                    tagEditor.setFileName( myApp.uri() )
                    propertiesPage.initialize()
                    segmentedControl.selectedIndex = 0
                    segmentedControl.updateUi()
                }
            }
        },
        SystemToast {
            id: popupMessage
        }
    ]
    
    function initialize() {
        contentsInfoItem.data = qsTr("calculating...")
        contentsInfoItem.visible = false
        segmentedControl.visible = false
        audioInfoContainer.visible = false
        imageInfoContainer.visible = false
        otherInfoContainer.visible = false
        
        rightsInfoItem.visible = true
        creationDateInfoItem.visible = true
        modDateInfoItem.visible = true
        
        titleBar.resetDismissAction()
        acceptAction.title = qsTr("Close")
        
        if (!properties.isAudioFileWithEditableTags()) {
            segmentedControl.selectedIndex = 0
            segmentedControl.updateUi()
        }
        
        if (properties.isFile()) {
            headerFileInfo.title = qsTr("File info")
            fileInfoItem.text = qsTr("File")
            
            fileInfoItem.data = properties.getFileName()
            pathInfoItem.data = properties.getHumanReadablePath()
            sizeInfoItem.data = properties.getHumanReadableFileSize() + "  " + properties.getHumanReadableFileSizeBytes()
            rightsInfoItem.data = properties.getRightsDescription()
            creationDateInfoItem.data = properties.getCreated()
            modDateInfoItem.data = properties.getModified()
            
            if (properties.isAudioFileWithEditableTags()) {
                segmentedControl.visible = true
                audioInfoContainer.visible = true
                
                var tagEditor = properties.tagEditor()
                
                typeInfoItem.data = properties.getType()
                bitrateInfoItem.data = tagEditor.getBitrate()
                sampleRateInfoItem.data = tagEditor.getSampleRate()
                channelsInfoItem.data  = tagEditor.getChannels()
                lengthInfoItem.data = tagEditor.getLength()
                
                var tagInfo = tagEditor.tagInfo()
                titleItem.data = tagInfo.title
                artistItem.data = tagInfo.artist
                albumItem.data = tagInfo.album
                genreItem.data = tagInfo.genre
                commentItem.data = tagInfo.comment
                
                trackItem.data = (tagInfo.track > 0) ? tagInfo.track : ""
                yearItem.data = (tagInfo.year > 0) ? tagInfo.year : ""
                
                coverContainer.initialize()
                
                titleBar.setDismissAction(dismissAction)
                acceptAction.title = qsTr("OK")

            } else if (properties.isImage()){
                imageInfoContainer.visible = true
                imageSizeInfoItem.data = properties.imageWidth() + "x" + properties.imageHeight()
                imageInfoContainer.loadPreviewImage(properties.getAbsoluteFilePathWithPrefixFile())
            } else {
                // Not audio file...
            }
        } else if (properties.isDir()){
            headerFileInfo.title = qsTr("Folder info")
            fileInfoItem.text = qsTr("Folder")
            contentsInfoItem.visible = true
            
            
            otherInfoContainer.visible = settings.allowEditNoMultimedia
            if (otherInfoContainer.visible) {
                noMultimediaCheckBox.checked = properties.hasNoMultimediaProperty()
                titleBar.setDismissAction(dismissAction)
                acceptAction.title = qsTr("OK")
            }
            
            fileInfoItem.data = properties.getFileName()
            pathInfoItem.data = properties.getHumanReadablePath()
            sizeInfoItem.data = properties.getHumanReadableFileSize()
            rightsInfoItem.data = properties.getRightsDescription()
            creationDateInfoItem.data = properties.getCreated()
            modDateInfoItem.data = properties.getModified()


        } else if (properties.isMultipleItems()) {
            headerFileInfo.title = qsTr("Selected items info")
            fileInfoItem.text = qsTr("Selected")
            fileInfoItem.data = qsTr("calculating...")
            contentsInfoItem.visible = true
            
            fileInfoItem.data = properties.getFileName()
            pathInfoItem.data = properties.getHumanReadablePath()
            sizeInfoItem.data = properties.getHumanReadableFileSize()
            
            rightsInfoItem.visible = false
            creationDateInfoItem.visible = false
            modDateInfoItem.visible = false


        } else {
            // Unknow type
        }
    }
    
    property string currentFileName
    
    ScrollView {
        Container {
            topPadding: 20.0
            bottomPadding: 20.0
            leftPadding: 20.0
            rightPadding: 20.0
            
            SegmentedControl {
                id: segmentedControl
                bottomMargin: 20
                Option {
                    id: tagOption
                    text: qsTr("General")
                    selected: true
                }
                Option {
                    id: infoOption
                    text: qsTr("Info")
                }
                Option {
                    id: coverOption
                    text: qsTr("Cover")
                }
                function updateUi() {
                    infoContainer.visible = (selectedIndex == 0)
                    tagContainer.visible = (selectedIndex == 1)
                    coverContainer.visible = (selectedIndex == 2)
                }
                onSelectedIndexChanged: {
                    updateUi()
                }
            }
            CoverContainer {
                id: coverContainer
                visible: false
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
            Container {
                id: infoContainer
                visible: true
                Container {
                    Header {
                        id: headerFileInfo
                        title: qsTr("File info")
                    }
                    InfoItem {
                        id: fileInfoItem
                        topPadding: 20.0
                        multiline: true
                        text: qsTr("File")
                    }
                    InfoItem {
                        id: pathInfoItem
                        topPadding: 20.0
                        multiline: true
                        text: qsTr("Path")
                    }
                    InfoItem {
                        id: contentsInfoItem
                        topPadding: 20.0
                        multiline: true
                        text: qsTr("Contents")
                    }
                    InfoItem {
                        id: sizeInfoItem
                        topPadding: 20.0
                        text: qsTr("Size")
                    }
                    InfoItem {
                        id: rightsInfoItem
                        topPadding: 20.0
                        text: qsTr("Permissions")
                    }
                    InfoItem {
                        id: creationDateInfoItem
                        topPadding: 20.0
                        text: qsTr("Created")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: modDateInfoItem
                        text: qsTr("Modified")
                    }
                }
                Container {
                    id: otherInfoContainer
                    visible: false
                    topPadding: 20.0
                    Header {
                        title: qsTr("Other")
                    }
                    CheckBox {
                        id: noMultimediaCheckBox
                        text: qsTr("No media")
                    }
                }
                Container {
                    id: imageInfoContainer
                    topPadding: 20.0
                    function loadPreviewImage(path) {
                        imageView.imageSource = path
                    }
                    Header {
                        title: qsTr("Image info")
                    }
                    InfoItem {
                        topPadding: 20.0
                        bottomPadding: 20.0
                        id: imageSizeInfoItem
                        text: qsTr("Size")
                    }
                    ImageView {
                        id: imageView
                        maxWidth:  ui.sdu(60)
                        maxHeight: ui.sdu(60)
                        imageSource: "asset:///images/image.png"
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        scalingMethod: ScalingMethod.AspectFit
                    }
                }
                Container {
                    id: audioInfoContainer
                    topPadding: 20.0
                    Header {
                        title: qsTr("Audio info")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: typeInfoItem
                        text: qsTr("Type")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: bitrateInfoItem
                        text: qsTr("Bitrate")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: sampleRateInfoItem
                        text: qsTr("Sample rate")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: channelsInfoItem
                        text: qsTr("Channels")
                    }
                    InfoItem {
                        topPadding: 20.0
                        id: lengthInfoItem
                        text: qsTr("Duration")
                    }
                }
            }
            
            Container {
                id: tagContainer
                visible: false
                EditableItem {
                    id: titleItem
                    text: qsTr("Title")
                }
                EditableItem {
                    topPadding: 20.0
                    id: artistItem
                    text: qsTr("Artist")
                }
                
                EditableItem {
                    topPadding: 20.0
                    id: albumItem
                    text: qsTr("Album")
                }
                
                EditableItem {
                    topPadding: 20.0
                    id: trackItem
                    text: qsTr("Track")
                }
                EditableItem {
                    topPadding: 20.0
                    id: yearItem
                    text: qsTr("Year")
                }
                EditableItem {
                    topPadding: 20.0
                    id: genreItem
                    text: qsTr("Genre")
                }
                BigEditableItem {
                    topPadding: 20.0
                    id: commentItem
                    text: qsTr("Comment")
                }
            }

        }
    }
}
