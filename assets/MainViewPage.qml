import bb.cascades 1.4
import bb.system 1.2
import bb.cascades.pickers 1.0
import QtQuick 1.0
import FileUtils 1.0


Page {
    id: mainViewPage
    objectName: "MainViewPage"
    property bool visible: false
    property variant searchFilterSheet: null
    property variant propertiesSheet: null
    property variant historyPage: null
    property variant bookmarksPage: null
    
    titleBar: TitleBar {
        id: titleBar
        visibility: (settings.showTitleBar && !searchBox.visible) ? ChromeVisibility.Visible : ChromeVisibility.Hidden
        title: myApp.officialAppName()
        scrollBehavior: TitleBarScrollBehavior.Sticky
    }
    
    function goToMainPage() {
        navigationPane.pop()
    }
    
    function hideSearchBox() {
        if (searchBox.visible) {
            searchBox.closeSearchBox(false)
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            onTriggered: {
                hideSearchBox()
                
                if (!mainListViewModel.currentPathIsTopLevelDir()) {
                    mainViewPage.cd("..")
                } else {
                    mainViewPage.goToMainPage()
                }
            }
        }
    }

    function invokeCreateNewFileDialog() {
        invokeCreateNewFileDialog.show()

    }
    
    function invokePropertiesSheet(array) {
        if (!propertiesSheet) {
            propertiesSheet = propertiesSheetDefinition.createObject()
        }
        properties.setArray(array)
        propertiesSheet.open()
        propertiesSheet.initialize()
    }
    
    function invokeHistoryPage() {
        if (!historyPage) {
            historyPage = historyPageDefinition.createObject()
            historyPage.pathSelected.connect(mainViewPage.cd)
        }
        historyPage.updateGui()
        push(historyPage)
    }
    
    function invokeBookmarksPage() {
        if (!bookmarksPage) {
            bookmarksPage = bookmarksPageDefinition.createObject()
            bookmarksPage.pathSelected.connect(mainViewPage.cd)
        }
        bookmarksPage.updateGui()
        push(bookmarksPage)
    }
    
    
    function prepareFileOperationMultiple(arrayList, mode)
    {
        if (!fileUtils.initializeToStartOperation(arrayList, mode)) {
            navigationPane.showMessage(qsTr("Please, wait when this operation will be finished"))
        } else {
            fileOperationContainer.initialize(arrayList.length, mode)
        }
        updateFileOperationEnabled()
    }
    
    function updateFileOperationEnabled() {
        fileOperationContainer.setEnableForFileOperation(mainListViewModel.currentPath(false))
    }
    
    function prepareFileOperation(absolutePath, mode)
    {
        var arrayList = new Array()
        arrayList.push(absolutePath)
        prepareFileOperationMultiple(arrayList, mode)
    }
    
    function updateShowActions() {
        var newPathIsDeviceRoot = fileUtils.isTopLevelDeviceDir(mainListViewModel.currentPath(false))
        
        mainViewPage.removeAction(bookmarksAction)
        mainViewPage.removeAction(historyAction)
        mainViewPage.removeAction(searchAction)
        mainViewPage.removeAction(addDirectoryAction)
        mainViewPage.removeAction(addFileAction)
        mainViewPage.removeAction(dirPropertiesAction)
        mainViewPage.removeAction(refreshAction)
        mainViewPage.removeAction(toTopAction)
        mainViewPage.removeAction(toBottomAction)
        mainViewPage.removeAction(selectAction)
        
        // update order
        if (settings.showScrollActions) {
            mainViewPage.addAction(toTopAction)
            mainViewPage.addAction(toBottomAction)
        }
        
        mainViewPage.addAction(bookmarksAction)
        mainViewPage.addAction(historyAction)
        mainViewPage.addAction(searchAction)
        mainViewPage.addAction(dirPropertiesAction)
        
        if (!newPathIsDeviceRoot) {
            mainViewPage.addAction(addDirectoryAction)
            mainViewPage.addAction(addFileAction)
        }
        mainViewPage.addAction(refreshAction)
        mainViewPage.addAction(selectAction)
    }
    
    function cd(path) {
        visible = true

        var oldPathIsDeviceRoot = fileUtils.isTopLevelDeviceDir(mainListViewModel.currentPath(false))
        if (!mainListViewModel.cd(path)) {
            navigationPane.showMessage(qsTr("Cannot change folder to") + " " + path)
            return
        }
        updateCurrentView()
        updateShowActions()
        
        // History
        historyListViewModel.addItem(mainListViewModel.currentPath(false))
        
        // titleBar
        titleBar.title = mainListViewModel.currentDirectoryName()
        
        // update file operation item (enable/disable)
        if (fileOperationContainer.visible) {
            updateFileOperationEnabled()
        }
    }
    
    function updateCurrentView() {
        mainListViewModel.updateModel()
        pathHeader.title = mainListViewModel.currentPath(true)
    }
    
    function invokeSearchFilterSheet() {
        if (!searchFilterSheet) {
            searchFilterSheet = searchFilterSheetDefinition.createObject()
            searchFilterSheet.searchFilterChanged.connect(mainViewPage.updateCurrentView)
        }
        searchFilterSheet.open()
        searchFilterSheet.initialize()
    }
    
    
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        
        SearchBox {
            topPadding: 10.0
            bottomPadding: 10.0
            id: searchBox
            visible: false
        }
        
        Header {
            id: pathHeader
            visible: !searchBox.visible
        }
        FileOperationContainer {
            id: fileOperationContainer
            visible: false
            topPadding: 10.0
            bottomPadding: 10.0
            leftPadding: 10.0
            rightPadding: 10.0
            onStartOperationClicked: {
                fileUtils.startOperation(mainListViewModel.currentPath(false))
            }
            onCancelOperationClicked: {
                fileUtils.cancelOperation()
            }
            attachedObjects: [
                Connections {
                    target: fileUtils
                    onOperationError: {
                        fileOperationContainer.setFinished()
                        infoDialog.showMessage(qsTr("Warning"), message)
                    }
                    onOperationStateChanged: {
                        fileOperationContainer.setProgress(progressPercent, message)
                    }
                    onOperationThreadFinished: {
                        fileOperationContainer.setFinished()
                    }
                }
            ]
        }
        
        ListView {
            id: mainItemsListView
            visible: true
            dataModel: mainListViewModel.model
            scrollRole: ScrollRole.Main
            
            function listViewPrivate() {
                return mainListViewModel
            }
            
            function selectAllItems() {
                mainItemsListView.selectAll()
                mainItemsListView.multiSelectHandler.active = true
            }
            
            signal renameItem(string absolutePath, int type)  // rename file/directory
            signal shareItem(string absolutePath, int type) // share to system
            signal propertiesItem(string absolutePath, int type)  //
            signal deleteItem(string absolutePath)
            signal cutItem(string absolutePath,int type)
            signal copyItem(string absolutePath, int type)
            signal openItem(string absolutePath, int type)
            
            signal deleteMultipleItems(variant array)
            signal propertiesMultipleItems(variant array)
            signal createShortCut(string absolutePath, string imageSource)
            
            signal toTop()
            signal toBottom()
            
            signal copyMultipleItems(variant array)
            signal moveMultipleItems(variant array)
            
            
            multiSelectAction: MultiSelectActionItem {

            }
            
            multiSelectHandler {
                onActiveChanged: {
                    if (active == true) {
                         // Multiple selection is activated
                    }
                    else {
                        mainItemsListView.clearSelection()
                    }
                }
                onCanceled: {
                    mainItemsListView.clearSelection()
                }
                
                actions: [
                    ActionItem {
                        id: multiSelectCutAction
                        title: qsTr("Cut")
                        imageSource: "asset:///images/cut.png"
                        enabled: false
                        onTriggered: {
                            var selectedItemsList = mainItemsListView.selectionList()
                            var arrayList = new Array()
                            for (var i = 0; i < selectedItemsList.length; i ++) {
                                arrayList.push( mainListViewModel.dataAsAbsoluteFilePath(selectedItemsList[i]))
                            }
                            prepareFileOperationMultiple(arrayList, 2)
                        }
                        shortcuts: [
                            Shortcut {
                                key: "X"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectCopyAction
                        title: qsTr("Copy")
                        imageSource: "asset:///images/copy.png"
                        enabled: false
                        onTriggered: {
                            var selectedItemsList = mainItemsListView.selectionList()
                            var arrayList = new Array()
                            for (var i = 0; i < selectedItemsList.length; i ++) {
                                arrayList.push( mainListViewModel.dataAsAbsoluteFilePath(selectedItemsList[i]))
                            }
                            prepareFileOperationMultiple(arrayList, 1)
                        }
                        shortcuts: [
                            Shortcut {
                                key: "C"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectShareAction
                        title: qsTr("Share")
                        imageSource: "asset:///images/share.png"
                        enabled: false
                        onTriggered: {
                            var selectedItemsList = mainItemsListView.selectionList()
                            var propertiesItemsArray = new Array()
                            for (var i = 0; i < selectedItemsList.length; i ++) {
                                propertiesItemsArray.push( mainListViewModel.dataAsAbsoluteFilePath(selectedItemsList[i]))
                            }
                            myApp.shareFiles(propertiesItemsArray)
                        }
                        shortcuts: [
                            Shortcut {
                                key: "F"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectPropertiesAction
                        title: qsTr("Properties")
                        imageSource: "asset:///images/properties.png"
                        onTriggered: {
                            var selectedItemsList = mainItemsListView.selectionList()
                            var propertiesItemsArray = new Array()
                            for (var i = 0; i < selectedItemsList.length; i ++) {
                                propertiesItemsArray.push( mainListViewModel.dataAsAbsoluteFilePath(selectedItemsList[i]))
                            }
                            mainItemsListView.propertiesMultipleItems(propertiesItemsArray)
                        }
                        shortcuts: [
                            Shortcut {
                                key: "P"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectSelectAllAction
                        title: qsTr("Select all")
                        imageSource: "asset:///images/selectall.png"
                        onTriggered: {
                            mainItemsListView.multiSelectHandler.active = false
                            mainItemsListView.selectAllItems()
                        }
                        shortcuts: [
                            Shortcut {
                                key: "S"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectToggleSelectionAction
                        title: qsTr("Toggle selection")
                        imageSource: "asset:///images/toggleselection.png"
                        onTriggered: {
                            var selectedList = mainItemsListView.selectionList()
                            mainItemsListView.multiSelectHandler.active = false
                            mainItemsListView.clearSelection()
                            mainItemsListView.selectAllItems()
                            for (var i = 0; i < selectedList.length; i++) {
                                mainItemsListView.toggleSelection(selectedList[i])
                            }
                            mainItemsListView.multiSelectHandler.active = true
                        }
                        shortcuts: [
                            Shortcut {
                                key: "I"
                            }
                        ]
                    },
                    ActionItem {
                        id: multiSelectClearSelectionAction
                        title: qsTr("Clear selection")
                        imageSource: "asset:///images/deselectall.png"
                        onTriggered: {
                            mainItemsListView.multiSelectHandler.active = false
                            mainItemsListView.clearSelection()
                            mainItemsListView.multiSelectHandler.active = true
                        }
                        shortcuts: [
                            Shortcut {
                                key: "R"
                            }
                        ]
                    },
                    DeleteActionItem {
                        id: multiSelectDeleteAction
                        title: qsTr("Delete")
                        imageSource: "asset:///images/delete.png"
                        enabled: false
                        onTriggered: {
                            var selectedItemsList = mainItemsListView.selectionList()
                            var deletedItemsArray = new Array()
                            for (var i = 0; i < selectedItemsList.length; i ++) {
                                deletedItemsArray.push( mainListViewModel.data(selectedItemsList[i],  "absoluteFilePath"))
                            }
                            mainItemsListView.deleteMultipleItems(deletedItemsArray)
                        }
                    }
                ]
            }
            
            onSelectionChanged: {
                var multipleEnabled = (selectionList().length >= 1)
                multiSelectDeleteAction.enabled = multipleEnabled
                multiSelectCutAction.enabled = multipleEnabled
                multiSelectCopyAction.enabled = multipleEnabled
                multiSelectShareAction.enabled = multipleEnabled
                multiSelectPropertiesAction.enabled = multipleEnabled
                
                if (mainListViewModel.currentPathIsTopLevelDeviceDir()) {
                    multiSelectShareAction.enabled = false
                    multiSelectDeleteAction.enabled = false
                    multiSelectCutAction.enabled  = false
                }
                
                var selectedItemsList = mainItemsListView.selectionList()
                for (var i = 0; i < selectedItemsList.length; i ++) {
                    if ( mainListViewModel.data(selectedItemsList[i],  "type") == 1) {
                        multiSelectShareAction.enabled = false
                        return
                    }
                }
            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: standardItem
                        title: ListItemData.title
                        description: ListItemData.description
                        status: ListItemData.status
                        imageSource: ListItemData.imageSource
                        selectionIndicator: ListItemSelectionIndicator.None
                        
                        contextMenuHandler: ContextMenuHandler {
                            onPopulating: {
                                var isTopLevelDeviceDir = standardItem.ListItem.view.listViewPrivate().currentPathIsTopLevelDeviceDir()
                                
                                fileActionSet.remove(cutAction)
                                fileActionSet.remove(copyAction)
                                fileActionSet.remove(openAction)
                                fileActionSet.remove(shareAction)
                                fileActionSet.remove(addToHomeScreenAction)
                                fileActionSet.remove(propertiesAction)
                                fileActionSet.remove(renameAction)
                                fileActionSet.remove(multiSelectAction)
                                
                                fileActionSet.remove(deleteAction)
                                
                                if (!isTopLevelDeviceDir) {
                                    fileActionSet.add(cutAction)
                                }
                                fileActionSet.add(copyAction)
                                if (ListItemData.type == 2) {
                                    fileActionSet.add(openAction)
                                    fileActionSet.add(shareAction)
                                }
                                fileActionSet.add(addToHomeScreenAction)
                                fileActionSet.add(propertiesAction)
                                
                                if (!isTopLevelDeviceDir) {
                                    fileActionSet.add(renameAction)
                                }
                                
                                fileActionSet.add(multiSelectAction)
                                
                                if (!isTopLevelDeviceDir) {
                                    fileActionSet.add(deleteAction)
                                }
                            }
                        }
                        
                        contextActions: [
                            ActionSet {
                                id: fileActionSet
                                title: ListItemData.title
                                subtitle:  ListItemData.status
                                ActionItem {
                                    id: cutAction
                                    title: qsTr("Cut")
                                    imageSource: "asset:///images/cut.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.cutItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "X"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: copyAction
                                    title: qsTr("Copy")
                                    imageSource: "asset:///images/copy.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.copyItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "C"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: openAction
                                    title: qsTr("Open with")
                                    imageSource: "asset:///images/open.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.openItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "O"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: shareAction
                                    title: qsTr("Share")
                                    imageSource: "asset:///images/share.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.shareItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "F"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: addToHomeScreenAction
                                    title: qsTr("Add to home screen")
                                    imageSource: "asset:///images/addtohomescreen.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.createShortCut(ListItemData.absoluteFilePath, ListItemData.imageSource)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "W"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: propertiesAction
                                    title: qsTr("Properties")
                                    imageSource: "asset:///images/properties.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.propertiesItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "P"
                                        }
                                    ]
                                }
                                ActionItem {
                                    id: renameAction
                                    title: qsTr("Rename")
                                    imageSource: "asset:///images/rename.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.renameItem(ListItemData.absoluteFilePath, ListItemData.type)
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "R"
                                        }
                                    ]
                                }
                                MultiSelectActionItem {
                                    id: multiSelectAction
                                    title: qsTr("Select more")
                                    imageSource: "asset:///images/selectmore.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.multiSelectHandler.active = true
                                    }
                                    shortcuts: [
                                        Shortcut {
                                            key: "M"
                                        }
                                    ]
                                }
                                
                                
                                
                                
                                DeleteActionItem {
                                    id: deleteAction
                                    title: qsTr("Delete")
                                    imageSource: "asset:///images/delete.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.deleteItem(ListItemData.absoluteFilePath)
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            onDeleteMultipleItems: {
                deleteConfirmDialog.invokeMultipleItems(array)
            }
            onPropertiesMultipleItems: {
                mainViewPage.invokePropertiesSheet(array)
            }
            onPropertiesItem: {
                var array = new Array()
                array.push(absolutePath)
                mainViewPage.invokePropertiesSheet(array)
            }
            
            onDeleteItem: {
                deleteConfirmDialog.invoke(absolutePath)
            }
            
            onCreateShortCut: {
                if (!myApp.createShortCut(absolutePath, imageSource)) {
                    navigationPane.showMessage(qsTr("Cannot create shortcut"))
                } else {
                    navigationPane.showMessage(qsTr("Added to home screen"))
                }
            }
            
            onRenameItem: {
                renamePrompt.invoke(absolutePath, type)
            }
            
            onToTop: {
                mainItemsListView.scrollToPosition(ScrollPosition.Beginning,ScrollAnimation.None)
            }
            
            onToBottom: {
                mainItemsListView.scrollToPosition(ScrollPosition.End,ScrollAnimation.None)
            }
            
            ////////////////////////////////////////////////////////////
            onCopyItem: {
                mainViewPage.prepareFileOperation(absolutePath, 1)
            }
            
            onCutItem: {
                mainViewPage.prepareFileOperation(absolutePath, 2)
            }
            
            ////////////////////////////////////////////////////////////
            
            onOpenItem: {
                invokeSelectedAppDialog.fileName = absolutePath
                myApp.queryTargets(absolutePath)
            }
            
            onShareItem: {
                if (type==1) {
                    return
                }
                myApp.shareFile(absolutePath)
            }
            
            
            
            onTriggered: {
                if ( mainListViewModel.isDir(indexPath) ) {
                    mainViewPage.hideSearchBox()
                    mainViewPage.cd(mainListViewModel.path(indexPath))
                } else {
                    var absolutePath =  mainListViewModel.data(indexPath, "absoluteFilePath")
                    if (settings.propertiesAsDefaultAction) {
                        mainViewPage.invokePropertiesSheet(absolutePath)
                    } else {
                        myApp.invokeCard(absolutePath)
                    }
                }
            }
        }
        StatusBar {
            id: statusBar
            text: mainListViewModel.statusBarText
        }
    }
    actions: [
        ActionItem {
            id: bookmarksAction
            title: qsTr("Bookmarks")
            imageSource: "asset:///images/editbookmarks.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                mainViewPage.invokeBookmarksPage()
            }
            shortcuts: [
                Shortcut {
                    key: "K"
                }
            ]
        },
        ActionItem {
            id: historyAction
            title: qsTr("History")
            imageSource: "asset:///images/history.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                mainViewPage.invokeHistoryPage()
            }
            shortcuts: [
                Shortcut {
                    key: "H"
                }
            ]
        },
        ActionItem {
            id: searchAction
            title: qsTr("Search")
            imageSource: "asset:///images/search.png"
            onTriggered: {
                searchBox.invoke()
            }
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: [
                Shortcut {
                    key: "S"
                }
            ]
        },
        ActionItem {
            id: dirPropertiesAction
            title: qsTr("Properties")
            imageSource: "asset:///images/properties.png"
            onTriggered: {
                if (fileUtils.isTopLevelDir(mainListViewModel.currentPath(false))) {
                    navigationPane.invokeSpaceInfoPage(mainListViewModel.currentPath(false))
                } else {
                    var array = new Array()
                    array.push(mainListViewModel.currentPath(false))
                    mainViewPage.invokePropertiesSheet(array)
                }
            }
            shortcuts: [
                Shortcut {
                    key: "P"
                }
            ]
        },
        ActionItem {
            id: addDirectoryAction
            title: qsTr("Add folder")
            imageSource: "asset:///images/adddirectory.png"
            attachedObjects: [
                SystemPrompt {
                    id: addDirectoryPrompt
                    title: qsTr("Add folder")
                    property string baseErrorString: ("  " + qsTr("already exists. Try again."))
                    //inputField.defaultText: "abc123456"
                    inputField.emptyText: qsTr("Enter new name")
                    onFinished: {
                        if (result == SystemUiResult.ConfirmButtonSelection) {
                            var newPath = fileUtils.mergePathWithName(mainListViewModel.currentPath(false),inputFieldTextEntry())
                            if (fileUtils.directoryOrFileExists(newPath)) {
                                addDirectoryPrompt.body = inputFieldTextEntry() + baseErrorString
                                addDirectoryPrompt.show()
                                return
                            } else {
                                if (fileUtils.mkDir(newPath)) {
                                    mainListViewModel.addItem(newPath)
                                } else {
                                    navigationPane.showMessage(qsTr("Cannot create folder."))
                                }
                            }
                        }
                    }

                }
            ]
            onTriggered: {
                addDirectoryPrompt.body = ""
                addDirectoryPrompt.show()
            }
            shortcuts: [
                Shortcut {
                    key: "A"
                }
            ]

        },
        ActionItem {
            id: addFileAction
            title: qsTr("Add file")
            imageSource: "asset:///images/addfile.png"
            attachedObjects: [
                SystemListDialog {
                    id: invokeCreateNewFileDialog
                    title: qsTr("Select type")
                    //customButton.label: qsTr("Choose a filename")
                    cancelButton.label: qsTr("Cancel")
                    confirmButton.label: objectName
                    selectionMode: ListSelectionMode.Single
                    selectionIndicator: ListSelectionIndicator.Highlight
                    dismissOnSelection: true
                    onFinished: {
                        if (result != SystemUiResult.CancelButtonSelection) {
                            var index = -1
                            if ( selectedIndices.length == 1) {
                                index = selectedIndices[0]
                            }

                            if (index != 4) {
                                if (!fileUtils.addFile(mainListViewModel.currentPath(false), index)) {
                                    navigationPane.showMessage(qsTr("Cannot create file."))
                                } else {
                                    navigationPane.showMessage(qsTr("File was added."))
                                }
                            } else {
                                addFilePrompt.show()
                            }
                        }
                    }
                    defaultButton: null
                },
                SystemPrompt {
                    id: addFilePrompt
                    title: qsTr("Create file")
                    property string baseErrorString: ("  " + qsTr("already exists. Try again."))
                    inputField.emptyText: qsTr("Enter a filename")
                    onFinished: {
                        if (result == SystemUiResult.ConfirmButtonSelection) {
                            var newPath = fileUtils.mergePathWithName(mainListViewModel.currentPath(false),inputFieldTextEntry())
                            if (fileUtils.directoryOrFileExists(newPath)) {
                                addFilePrompt.body = inputFieldTextEntry() + baseErrorString
                                addFilePrompt.show()
                                return
                            } else {
                                if (fileUtils.addOtherFile(newPath)) {
                                    mainListViewModel.addItem(newPath)
                                    navigationPane.showMessage(qsTr("File was added."))
                                } else {
                                    navigationPane.showMessage(qsTr("Cannot create file."))
                                }
                            }
                        }
                    }
                }
            ]
            onTriggered: {
                invokeCreateNewFileDialog.clearList()
                invokeCreateNewFileDialog.appendItem("MS-Word (DOC)")
                invokeCreateNewFileDialog.appendItem("MS-Excel (XLS)")
                invokeCreateNewFileDialog.appendItem("MS-PowerPoint (PPT)")
                invokeCreateNewFileDialog.appendItem("Text (TXT)")
                invokeCreateNewFileDialog.appendItem(qsTr("Other"))
                invokeCreateNewFileDialog.show()
                /*
                 addFilePrompt.body = ""
                 addFilePrompt.show()
                 */
            }
            shortcuts: [
                Shortcut {
                    key: "F"
                }
            ]

        },
        ActionItem {
            id: refreshAction
            title: qsTr("Refresh")
            imageSource: "asset:///images/reload.png"
            onTriggered: {
                mainViewPage.updateCurrentView()
            }
            shortcuts: [
                Shortcut {
                    key: "L"
                }
            ]
        },
        ActionItem {
            id: selectAction
            title: qsTr("Select")
            imageSource: "asset:///images/select.png"
            onTriggered: {
                mainItemsListView.multiSelectHandler.active = true
            }
            shortcuts: [
                Shortcut {
                    key: "M"
                }
            ]
        },
        ActionItem {
            id: toTopAction
            title: qsTr("To top")
            imageSource: "asset:///images/totop.png"
            onTriggered: {
                mainItemsListView.toTop()
            }
            shortcuts: [
                Shortcut {
                    key: "T"
                }
            ]
        },
        
        ActionItem {
            id: toBottomAction
            title: qsTr("To bottom")
            imageSource: "asset:///images/tobottom.png"
            onTriggered: {
                mainItemsListView.toBottom()
            }
            shortcuts: [
                Shortcut {
                    key: "B"
                }
            ]
        }

    ]
    shortcuts: [
        Shortcut {
            key: "D"
            onTriggered: {
                mainViewPage.goToMainPage()
            }
        }
    ]
    attachedObjects: [
        SystemDialog {
            id: infoDialog
            cancelButton.label: objectName
            confirmButton.label: qsTr("Close")
            function showMessage(title, message) {
                title = title
                body = message
                show()
            }
        },
        Connections {
            target: myApp
            onProcessQueryReplyFinished: {
                var queryResults = myApp.queryReplyResults()
                if (queryResults.length == 1) {
                    myApp.invokeApp(invokeSelectedAppDialog.fileName, 0)
                    return;
                }
                invokeSelectedAppDialog.clearList()
                
                if (queryResults.length == 0) {
                    if ( textEditor.fileCanLoaded(lastFileNameForInvoke) ) {
                        navigationPane.invokeTextEditorPreviewerPage(lastFileNameForInvoke)
                    } else {
                        navigationPane.showMessage(qsTr("Cannot open selected file") + " (" + qsTr("No target app") + ")" )
                    }
                } else {
                    for (var i = 0; i < queryResults.length; i++) {
                        invokeSelectedAppDialog.appendItem( queryResults[i] )
                    }
                    invokeSelectedAppDialog.show()
                }
            }
            onExternalInvokeError: {
                if ((message == "No target app") && (textEditor.fileCanLoaded(lastFileNameForInvoke) ) ) {
                    navigationPane.invokeTextEditorPreviewerPage(lastFileNameForInvoke)
                } else {
                    navigationPane.showMessage(qsTr("Cannot open selected file") + " (" + message + ")" )
                }
            }
        },
        SystemListDialog {
            id: invokeSelectedAppDialog
            title: qsTr("Select application")
            cancelButton.label: qsTr("Cancel")
            confirmButton.label: objectName
            dismissOnSelection: true
            selectionMode: ListSelectionMode.Single
            selectionIndicator: ListSelectionIndicator.Highlight
            property string fileName
            onFinished: {
                if (result != SystemUiResult.CancelButtonSelection) {
                    var index = -1
                    if ( selectedIndices.length == 1) {
                        index = selectedIndices[0]
                    }
                    myApp.invokeApp(fileName, index)
                }
            }
            defaultButton: null
        },
        SystemPrompt {
            id: renamePrompt
            cancelButton.label: qsTr("Cancel")
            confirmButton.label: qsTr("Rename")
            title: qsTr("Enter a new name")
            dismissAutomatically: true
            inputField.emptyText: qsTr("Enter a new name")
            includeRememberMe: false
            
            property string  savedPath
            property int   savedType
            
            function invoke(absolutePath, type) {
                savedPath = absolutePath
                savedType = type
                
                if (type == 1) {
                    inputField.defaultText = fileUtils.fileName(absolutePath)
                }
                if (type == 2) { //file
                    if (settings.showFileNameExtensionWhenRename) {
                        inputField.defaultText = fileUtils.fileName(absolutePath)
                    } else {
                        inputField.defaultText = fileUtils.completeBaseName(absolutePath)
                    }
                }
                show()
            }
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    var newPath = fileUtils.renameFileItem(savedPath, inputFieldTextEntry(), settings.showFileNameExtensionWhenRename)
                    if ( newPath.length > 0 ){
                        mainListViewModel.renameItem(savedPath,newPath)
                    }  else {
                        navigationPane.showMessage(qsTr("Cannot rename selected item"))
                    }
                }
            }
        },
        SystemDialog {
            id: deleteConfirmDialog
            title: qsTr("Delete")
            cancelButton.label: qsTr("Cancel")
            confirmButton.label: qsTr("Delete")
            property bool multipleDeletion: false
            property string errorText
            
            property string deleteTheSelectedDirectory: qsTr("Delete selected folder")
            property string cannotDeleteTheSelectedDirectory: qsTr("Cannot delete selected folder.")
            
            property string deleteTheSelectedFile: qsTr("Delete selected file")
            property string cannonDeleteTheSelectedFile: qsTr("Cannot delete selected file")
            
            
            property string deleteTheSelectedItems: qsTr("Delete selected items?")
            property string cannotDeleteTheSelectedItems: qsTr("Cannot delete selected items.")
            
            property string  savedPath
            property variant savedArray
            
            function invokeMultipleItems(array) {
                multipleDeletion = true
                savedArray = array.slice()
                popupMessage.cancel()
                body = deleteTheSelectedItems
                errorText = cannotDeleteTheSelectedItems
                show()
            }
            
            function invoke(path) {
                multipleDeletion = false
                savedPath = path
                popupMessage.cancel()
                if (fileUtils.isDir(path)) {
                    body = deleteTheSelectedDirectory + " '" + fileUtils.fileName(path) + "'?"
                    errorText = cannotDeleteTheSelectedDirectory
                } else {
                    body = deleteTheSelectedFile + " '" + fileUtils.fileName(path) + "'?"
                    errorText = cannonDeleteTheSelectedFile
                }
                show()
            }
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    if (multipleDeletion) {
                        var failItems = fileUtils.deleteFileItems(savedArray)
                        mainListViewModel.deleteMultipleItems(savedArray,failItems)
                        if (failItems.length > 0){
                            navigationPane.showMessage(errorText)
                        }
                    } else {
                        if (fileUtils.deleteFileItem(savedPath)){
                            mainListViewModel.deleteItem(savedPath)
                        } else {
                            navigationPane.showMessage(errorText)
                        }
                    }
                    mainListViewModel.postDeleteItemsProccessing()
                }
            }
        },
        ComponentDefinition {
            id: propertiesSheetDefinition
            source: "PropertiesSheet.qml"
        },
        ComponentDefinition {
            id: historyPageDefinition
            source: "HistoryPage.qml"
        },
        ComponentDefinition {
            id: bookmarksPageDefinition
            source: "BookmarksPage.qml"
        },
        ComponentDefinition {
            id: searchFilterSheetDefinition
            source: "SearchFilterSheet.qml"
        }
    ]
}
