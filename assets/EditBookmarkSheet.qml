import bb.cascades 1.4

Sheet { 
    id: editBookmarksSheet
    signal bookmarkChanged(string name, string path, bool added)
    property variant addMode: true
    
    function initialize(name, path) {
        nameTextField.text = ""
        bookmarkDirectoryItem.absFileNamePath = ""
        bookmarkDirectoryItem.update()
        
        addMode = (name == "" && path == "")
        if (addMode) {
            titleBar.title = qsTr("Add bookmark")
            if (!mainListViewModel.currentPathIsTopLevelDir()) {
                bookmarkDirectoryItem.absFileNamePath = mainListViewModel.currentPath(false)
                bookmarkDirectoryItem.update()
                nameTextField.text = bookmarkDirectoryItem.fileName
            }
        } else {
            titleBar.title = qsTr("Edit bookmark")
            bookmarkDirectoryItem.absFileNamePath = path
            bookmarkDirectoryItem.update()
            nameTextField.text = name
        }
        
        nameTextField.updateEnteredData(nameTextField.text)
    }
    
    Page {
        id: editBookmarksPage
        titleBar: TitleBar {
            id: titleBar
            title: qsTr("Add bookmark")
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    editBookmarksSheet.close()
                }
            }
            acceptAction: ActionItem {
                id: acceptAction
                title: qsTr("OK")
                onTriggered: {
                    editBookmarksSheet.close()
                    editBookmarksSheet.bookmarkChanged(nameTextField.text, bookmarkDirectoryItem.absFileNamePath, addMode)
                }
            }
        }
        Container {
            topPadding: 20.0
            leftPadding: 20.0
            rightPadding: 20.0
            bottomPadding: 20.0
            InputItem {
                id: bookmarkDirectoryItem
                title: qsTr("Folder")
                bottomPadding: 20.0
                onAbsFileNamePathChanged: {
                    nameTextField.text = bookmarkDirectoryItem.fileName
                }
            }
            Header {
                id: nameHeader
                title: qsTr("Name")
            }
            TextField {
                id: nameTextField
                function updateEnteredData(text) {
                    acceptAction.enabled = text.length
                }
                onTextChanging: {
                    updateEnteredData(text)
                }
            }
        }
    }
}
