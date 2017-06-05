import bb.cascades 1.4

Page {
    id: historyPage
    objectName: "HistoryPage"
    
    signal pathSelected(string path)
    
    titleBar: TitleBar {
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
        title: qsTr("History")
        appearance: TitleBarAppearance.Branded
    }
    
    function updateGui() {
        clearAction.enabled = !historyListViewModel.isEmpty()
        historyItemsListView.visible = clearAction.enabled
    }
    
    onCreationCompleted: {
        historyListViewModel.updateModel()
    }
    
    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        ListView {
            id: historyItemsListView
            visible: true
            dataModel: historyListViewModel.model
            scrollRole: ScrollRole.Main
            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        id: standardItem
                        title: ListItemData.name
                        description: ListItemData.humanReadablePath
                    }
                }
            ]
            
            onTriggered: {
                historyPage.pathSelected(historyListViewModel.data(indexPath, "absoluteFilePath"))
                navigationPane.pop()
                navigationPane.push(mainViewPage)
            }
        }
        NoItemsToDisplayContainer {
            id: noItemsToDisplayContainer
            visible: !historyItemsListView.visible
        }
    }
    actions: [
        ActionItem {
            id: clearAction
            title: qsTr("Clear")
            imageSource: "asset:///images/clearlist.png"
            onTriggered: {
                historyListViewModel.clear()
                historyPage.updateGui()
            }
        }
    ]
}
