import bb.cascades 1.4
import bb.system 1.2
import bb.cascades.pickers 1.0
import QtQuick 1.0

Page {
    id: textEditorPreviewerPage
    objectName: "TextEditorPreviewerPage"
    
    titleBar: TitleBar {
        id: titleBar
        title: myApp.officialAppName()
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
    }
    
    property string currentFileName
    property bool textChanged: false
    property int operationMode: 0 // 1 - new file, 2 - load file
    
    function initialize(fileNamePath) {
        clearTextArea()
        updateGuiFromSettings()
        loadFileName(fileNamePath)
        updateGuiFromCurrentState()
    }
    
    function updateSystemDialogBody() {
        systemDialog.body = (qsTr("Save current file \'") + ((currentFileName == "") ? "unnamed" : textEditor.fileBaseName(currentFileName)) + "\'?")
    }
    function updateGuiFromSettings() {
        if (textArea.textFormat != settings.textFormat)   textArea.textFormat = settings.textFormat
        if (textArea.textStyle.fontSize != settings.fontSize) textArea.textStyle.fontSize = settings.fontSize
        if (textArea.textStyle.fontStyle != settings.fontStyle) textArea.textStyle.fontStyle = settings.fontStyle
        if ( textArea.textStyle.FontWeight != settings.fontWeight)  textArea.textStyle.FontWeight = settings.fontWeight
    }
    function clearTextArea() {
        currentFileName = ""
        textArea.text = ""
        textChanged = false
    }
    
    function updateGuiFromCurrentState() {
        saveAction.enabled = textChanged
        
        if (currentFileName == "") {
            titleBar.title = myApp.officialAppName()
        } else {
            titleBar.title = textEditor.fileBaseName(currentFileName)
        }
        if (textChanged) {
            titleBar.title = " * " + titleBar.title
            propertiesAction.title = " * " + qsTr("Properties")
            propertiesDialog.title = propertiesAction.title
        } else {
            propertiesAction.title = qsTr("Properties")
            propertiesDialog.title = propertiesAction.title
        }
    }
    
    function loadFileName(selectedFile) {
        if (textEditor.loadFileName(selectedFile)) {
            textArea.text = textEditor.fileNameData();
            currentFileName = textEditor.currentFileNamePath()
            textChanged = false
            
            updateGuiFromCurrentState()
            
            //--textArea.requestFocus()
            textArea.editor.cursorPosition = 0;

        } else {
            popupMessage.body = textEditor.lastError()
            popupMessage.show()
        }
        updateShareUri()
    }
    
    function updateShareUri() {
        //
    }
    
    function saveTextToSelectedFile() {
        if (textEditor.saveToFileName(currentFileName, textArea.text)) {
            popupMessage.body = qsTr("Saved")
            textChanged = false
            updateGuiFromCurrentState()
            updateShareUri()
        } else {
            popupMessage.body = qsTr("Cannot save file!")
        }
        popupMessage.show()
    }

    Container {
        id: textAreaContainer
        background: Color.Transparent
        TextArea {
            id: textArea
            objectName: "textArea"
            inputMode: TextAreaInputMode.Text
            input {
                flags: TextInputFlag.SpellCheckOff | TextInputFlag.PredictionOff
            }
            onTextChanging: {
                if (! textEditorPreviewerPage.textChanged) {
                    textEditorPreviewerPage.textChanged = true;
                    updateGuiFromCurrentState()
                }
            }
        }
    }
    actions: [
        ActionItem {
            id: propertiesAction
            title: qsTr("Properties")
            imageSource: "asset:///images/info.png"
            onTriggered: {
                propertiesDialog.body = textEditor.docProperties(textArea.text)
                propertiesDialog.show()
            }
            shortcuts: [
                Shortcut {
                    key: "P"
                }
            ]
        },

        ActionItem {
            id: saveAction
            ActionBar.placement: ActionBarPlacement.OnBar
            title: qsTr("Save")
            imageSource: "asset:///images/save.png"
            onTriggered: {
                if (currentFileName == "") {
                } else {
                    saveTextToSelectedFile()
                }
            }
            shortcuts: [
                Shortcut {
                    key: "S"
                }
            ]
        }
    ]
    
    attachedObjects: [
        SystemToast {
            id: popupMessage
        },
        SystemDialog {
            id: propertiesDialog
            title: qsTr("Properties")
            cancelButton.label: objectName
            customButton.label: objectName

        },
        SystemDialog {
            id: systemDialog
            title: qsTr("Save")
            cancelButton.label: qsTr("Don't save")
            customButton.label: qsTr("Cancel")
            confirmButton.label: qsTr("Save")
            
            onFinished: {
                if (systemDialog.result == SystemUiResult.ConfirmButtonSelection) { // save
                    if (currentFileName == "") {
                        return
                    } else {
                        saveTextToSelectedFile()
                        if (operationMode == 1) {
                            textEditor.resetCurrentFile()
                            clearTextArea()
                        } else if (operationMode == 2) {
                            //
                        }
                        operationMode = 0
                        updateGuiFromCurrentState()
                    }

                } else if (systemDialog.result == SystemUiResult.CancelButtonSelection) { // Don't save
                    textEditor.resetCurrentFile()
                    clearTextArea()
                    
                    if (operationMode == 2) {
                        //
                    }
                    operationMode = 0
                    updateGuiFromCurrentState()

                } else { // cancel
                    // nothing to do
                }
            }
        },
        
        FilePicker {
            id: filePicker
            title: qsTr("Save")
            mode: FilePickerMode.Saver
            allowOverwrite: true
            defaultSaveFileNames: [ "file.txt" ]
            property string selectedFile
            type: FileType.Other
            filter: [ "*.*" ]
            onFileSelected: {
                selectedFile = selectedFiles[0];
                if (mode == FilePickerMode.Saver) {
                    currentFileName = selectedFile
                    saveTextToSelectedFile()
                    
                    if (operationMode == 1) {
                        operationMode = 0
                        clearTextArea()
                        updateGuiFromCurrentState()
                    }
                } else {
                    loadFileName(selectedFile);
                }
            }
        }
    ]
}
