import bb.cascades 1.4
import bb.system 1.2

Page {
    id: bookmarksPage
    objectName: "BookmarksPage"
    signal pathSelected(string path)
    
    property variant editBookmarkSheet: null
    property string savedCurrentBookmarkName
    property string savedCurrentBookmarkPath
    
    titleBar: TitleBar {
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
        title: qsTr("Bookmarks")
        appearance: TitleBarAppearance.Branded
    }
    function updateGui() {
        clearAction.enabled = !bookmarksListViewModel.isEmpty()
        bookmarksItemsListView.visible = clearAction.enabled
    }
    
    onCreationCompleted: {
        bookmarksListViewModel.updateModel()
    }
    
    function onBookmarkChanged(name, path, added) {
        if (added) {
            bookmarksListViewModel.addItem(name,path)
            updateGui()
        } else {
            bookmarksListViewModel.updateItem(savedCurrentBookmarkName, savedCurrentBookmarkPath, name,path)
        }
    }

    function invokeEditBookmarkSheet(name, path) {
        savedCurrentBookmarkName = name
        savedCurrentBookmarkPath = path
        
        if (!editBookmarkSheet) {
            editBookmarkSheet = editBookmarkSheetDefinition.createObject()
            editBookmarkSheet.bookmarkChanged.connect(bookmarksPage.onBookmarkChanged)

        }
        editBookmarkSheet.open()
        editBookmarkSheet.initialize(name,path)
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: editBookmarkSheetDefinition
            source: "EditBookmarkSheet.qml"
        },
        SystemDialog {
            id: deleteConfirmDialog
            title: qsTr("Delete")
            cancelButton.label: qsTr("Cancel")
            confirmButton.label: qsTr("Delete")
            
            property string errorText
            property string deleteTheSelectedFile: qsTr("Delete selected bookmark")
            property string cannonDeleteTheSelectedFile: qsTr("Cannot delete selected bookmark")
            property string  savedPath
            
            function invoke(path,name) {
                savedPath = path
                popupMessage.cancel()
                body = deleteTheSelectedFile + " '" + name + "'?"
                errorText = cannonDeleteTheSelectedFile
                show()
            }
            onFinished: {
                if (result == SystemUiResult.ConfirmButtonSelection) {
                    bookmarksListViewModel.deleteItem(savedPath)
                }
            }
        }
    ]
    
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        ListView {
            id: bookmarksItemsListView
            visible: true
            dataModel: bookmarksListViewModel.model
            scrollRole: ScrollRole.Main
            signal editBookmark(string path, string name)
            signal deleteBookmark(string path, string name)
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: standardItem
                        title: ListItemData.name
                        description: ListItemData.humanReadablePath
                        contextActions: [
                            ActionSet {
                                title: ListItemData.title
                                subtitle: qsTr("Actions")
                                ActionItem {
                                    title: qsTr("Edit")
                                    imageSource: "asset:///images/edit.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.editBookmark(ListItemData.absoluteFilePath, ListItemData.name)
                                    }
                                }
                                DeleteActionItem {
                                    title: qsTr("Delete")
                                    imageSource: "asset:///images/delete.png"
                                    onTriggered: {
                                        standardItem.ListItem.view.deleteBookmark(ListItemData.absoluteFilePath, ListItemData.name)
                                    }
                                }
                            }
                        ]
                    }
                }
            ]
            onDeleteBookmark: {
                deleteConfirmDialog.invoke(path,name)
            }
            onTriggered: {
                bookmarksPage.pathSelected(bookmarksListViewModel.data(indexPath, "absoluteFilePath"))
                navigationPane.pop()
                navigationPane.push(mainViewPage)
            }
            onEditBookmark: {
                invokeEditBookmarkSheet(name, path)
            }
        }
        
        NoItemsToDisplayContainer {
            id: noItemsToDisplayContainer
            visible: !bookmarksItemsListView.visible
        }
    }
    actions: [
        ActionItem {
            id: addBookmarkAction
            title: qsTr("Add bookmark")
            imageSource: "asset:///images/addbookmark.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                invokeEditBookmarkSheet("","")
            }
        },
        ActionItem {
            id: clearAction
            title: qsTr("Clear")
            imageSource: "asset:///images/clearlist.png"
            onTriggered: {
                bookmarksListViewModel.clear()
                bookmarksPage.updateGui()
            }
        }
    ]
}
