import bb.cascades 1.4

Container {
    id: fileOperationContainer
    
    property int buttonWidth: 220
    property bool operationInProgress: false
    property bool operationAllowed: true
    property int operationType: 0
    
    
    signal startOperationClicked()
    signal cancelOperationClicked()
    
    
    function setEnableForFileOperation(path) {
        if (operationInProgress) {
            return
        }
        // check directory is writable
        var directoryIsWritable = fileUtils.directoryIsWritable(path)
        
        // input path = ??
        // path - is output path
        var directoryIsNested = fileUtils.directoryIsNested(path)
        
        operationAllowed = (directoryIsWritable && !directoryIsNested)
        alertContainer.visible = !operationAllowed//!directoryIsWritable
        secondButton.enabled = operationAllowed
        if (!directoryIsWritable) {
            if (operationType == 1) alertLabel.text = qsTr("Can't copy files to a read-only folder")
            else if (operationType == 2) alertLabel.text = qsTr("Can't move files to a read-only folder")
        } else {
            if (directoryIsNested) {
                alertLabel.text = qsTr("Source and destination is the same")
            }
        }
    }
    
    function setProgress(percent, message) {
        progressIndicator.value = percent
        percentValueLabel.text = percent + "%"
        progressInfoLabel.text = message
    }
    
    function setFinished() {
        operationInProgress = false
        fileOperationContainer.visible = false
    }
    
    function initialize(itemsCount, opType) {
        operationType = opType
        firstButton.text = qsTr("Clear")
        secondButton.text = qsTr("Paste")
        infoLabel.text = itemsCount + " " + qsTr("items selected")
        operationNameLabel.text = fileUtils.fileOperationDescriptionByType(operationType)
        operationInProgress = false
        fileOperationContainer.visible = true
        secondButton.visible = true
        infoLabel.visible = true
        progressIndicator.reset()
        progressInfoLabel.text = ""
        alertContainer.visible = false
    }
    Container {
        id: alertContainer
        visible: false
        topPadding: 20.0
        horizontalAlignment: HorizontalAlignment.Center
        Label {
            id: alertLabel
            text: "Copying"
            textStyle.color: Color.Red
            textStyle.fontSize: FontSize.XSmall
            textStyle.fontWeight: FontWeight.Bold
            multiline: true
        }
    }
    Container {
        id: statusContainer
        topPadding: 20.0
        visible: progressIndicator.visible && operationAllowed
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            id: operationNameLabel
            text: "Copying"
            textStyle.color: Color.Gray
            textStyle.fontSize: FontSize.XSmall
            textStyle.fontWeight: FontWeight.Bold
        }
        Label {
            id: spacelabel
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10
            }
        }
        Label {
            id: progressInfoLabel
            text: qsTr("Calculating size")
            textStyle.color: Color.Gray
            textStyle.fontSize: FontSize.XSmall
            textStyle.fontWeight: FontWeight.Bold
        }

    }
    Container {
        topPadding: 20.0
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        Button {
            id: firstButton
            text: "Clear"
            verticalAlignment: VerticalAlignment.Center
            maxWidth: buttonWidth
            minWidth: buttonWidth
            onClicked: {
                fileOperationContainer.visible = false
                if (fileOperationContainer.operationInProgress) {
                    fileOperationContainer.cancelOperationClicked()
                }
            }
        }
        Container {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10.0
            }
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            
            Label {
                id: infoLabel
                text: "1 items selected"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
        Button {
            id: secondButton
            text: "Paste"
            verticalAlignment: VerticalAlignment.Center
            maxWidth: buttonWidth
            minWidth: buttonWidth
            onClicked: {
                firstButton.text = qsTr("Cancel")
                secondButton.visible = false
                infoLabel.visible = false
                progressIndicator.start()
                fileOperationContainer.operationInProgress = true
                fileOperationContainer.startOperationClicked()
            }
        }
        Label {
            id: percentValueLabel
            verticalAlignment: VerticalAlignment.Center
            visible: progressIndicator.visible
            textStyle.color: Color.Gray
            textStyle.fontSize: FontSize.XSmall
            textStyle.fontWeight: FontWeight.Bold
            text: "0%"
            minWidth: 100
            horizontalAlignment: HorizontalAlignment.Right
        }
        
        ProgressIndicator {
            id: progressIndicator
            visible: false
            verticalAlignment: VerticalAlignment.Center
            function reset() {
                progressIndicator.visible = false
                value = 0
                toValue = 100
            }
            function start() {
                progressIndicator.visible = true
            }
        }
    }
    
    Divider {

    }
}
