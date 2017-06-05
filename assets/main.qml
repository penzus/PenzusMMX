import bb.cascades 1.4
import bb.system 1.2
import QtQuick 1.0

NavigationPane {
    id: navigationPane
    property variant mainViewPage: null
    
    onPopTransitionEnded: {
        Application.menuEnabled = true
        
        // EDITOR
        if ( page.objectName == "TextEditorSettingsPage") {
            page.save()
            if (navigationPane.top.objectName == "TextEditorPreviewerPage") {
                navigationPane.top.updateGuiFromSettings()
            }
        }
        
        if (page.objectName == "SettingsPage") {
            if (page.save()) {
                mainListViewModel.updateModel()
                if (mainViewPage) {
                    mainViewPage.updateShowActions()
                }
            }
        }
        if ((page.objectName == "BookmarksPage") || (page.objectName == "HistoryPage") ) {
            return
        }
        
        if  (page.objectName == "MainViewPage") {
            page.hideSearchBox()
            page.visible = false
            mainListViewModel.stopFileSystemWatcher() // stop file system watch
            return
        }
        
        page.destroy()
    }
    
    Menu.definition: MenuDefinition {
        actions: [
            ActionItem {
                title: qsTr("About")
                imageSource: "asset:///images/info.png"
                onTriggered: {
                    Application.menuEnabled = false
                    var page = aboutPageDefinition.createObject();
                    push(page);
                }
            },
            ActionItem {
                title: qsTr("Settings")
                imageSource: "asset:///images/settings.png"
                onTriggered: {
                    Application.menuEnabled = false
                    
                    if (navigationPane.top.objectName == "TextEditorPreviewerPage") {
                        invokeTextEditorSettingsPage()
                    } else {
                        var page = settingsPageDefinition.createObject();
                        push(page)
                    }
                }
            }
        ]
    }
    
    function invokeSpaceInfoPage(path) {
        var spaceInfoPage = spaceInfoPageDefinition.createObject()
        spaceInfoPage.initialize(path)
        push(spaceInfoPage)
    }
    
    function showMessage(message) {
        popupMessage.body = message
        popupMessage.show()
    }
    
    function invokeTextEditorSettingsPage() {
        var textEditorSettingsPage = textEditorSettingsPageDefinition.createObject()
        push(textEditorSettingsPage)
    }
    
    function invokeTextEditorPreviewerPage(fileNamePath) {
        var textEditorPreviewerPage = textEditorPreviewerPageDefinition.createObject()
        textEditorPreviewerPage.initialize(fileNamePath)
        push(textEditorPreviewerPage)
    }
    
    
    function createMainPage() {
        if (!mainViewPage) {
            mainViewPage = mainViewPageDefinition.createObject()
        }
    }
    
    function invokeMainViewPage(path) {
        createMainPage()
        mainViewPage.cd(path)
        push(mainViewPage)
    }
    Page {
        id: mainPage
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            layout: DockLayout {
            }
            Container {
                topPadding: 10.0
                bottomPadding: 10.0
                leftPadding: 10.0
                rightPadding: 10.0
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    
                    CircleComponent {
                        id: deviceCircleComponent
                        text: qsTr("Device")
                        onClicked: {
                            navigationPane.invokeMainViewPage(fileUtils.getPathByIndex(0))
                        }
                    }
                    CircleComponent {
                        id: cardCircleComponent
                        visible: false
                        text: qsTr("SD Card")
                        onClicked: {
                            navigationPane.invokeMainViewPage(fileUtils.getPathByIndex(1))
                        }
                        onCreationCompleted: {
                            visible = fsInfo.cardIsMounted()
                        }
                    }
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    CircleComponent {
                        text: qsTr("Bookmarks")
                        imageSource: "asset:///images/editbookmarks.png"
                        onClicked: {
                            createMainPage()
                            mainViewPage.invokeBookmarksPage()

                        }
                    }
                    CircleComponent {
                        text: qsTr("History")
                        imageSource: "asset:///images/history.png"
                        onClicked: {
                            createMainPage()
                            mainViewPage.invokeHistoryPage()
                        }
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    topMargin: ui.sdu(2)
                    CloudButton {
                        visible: settings.dropBoxBlackBerry
                        imageSource: "asset:///images/cloud/dropbox.png"
                        onClicked: {
                            navigationPane.invokeMainViewPage(fileUtils.getPathByIndex(2))
                        }
                    }
                    CloudButton {
                        visible: settings.boxBlackBerry
                        imageSource: "asset:///images/cloud/box.png"
                        onClicked: {
                            navigationPane.invokeMainViewPage(fileUtils.getPathByIndex(3))
                        }
                    }
                    CloudButton {
                        visible: settings.oneDriveBlackBerry
                        imageSource: "asset:///images/cloud/onedrive.png"
                        onClicked: {
                            navigationPane.invokeMainViewPage(fileUtils.getPathByIndex(4))
                        }
                    }
                }
            }
            Label {
                visible: false
                id: promoLabel
                text: myApp.officialAppDescription()
                verticalAlignment: VerticalAlignment.Bottom
                textStyle.color: Color.Red
                textStyle.fontSize: FontSize.Small
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
    }
    
    attachedObjects: [
        Connections {
            target: myApp
            onInvokedDirectory1: {
                if ( (!mainViewPage) || (!mainViewPage.visible)) {
                    navigationPane.invokeMainViewPage(path)
                }
            }
        },
        Connections {
            target: fsInfo
            onCardMounted: {
                cardCircleComponent.visible = mounted
            }
            onDataChanged: {
                if (index == 0) {
                    deviceCircleComponent.topText = percentValue + "%"
                    deviceCircleComponent.bottomText = addInfo
                }
                else if (index == 1) {
                    cardCircleComponent.topText = percentValue + "%"
                    cardCircleComponent.bottomText = addInfo
                }
            }
        },
        SystemToast {
            id: popupMessage
        },
        ComponentDefinition {
            id: settingsPageDefinition
            source: "SettingsPage.qml"
        },
        ComponentDefinition {
            id: aboutPageDefinition
            source: "AboutPage.qml"
        },
        ComponentDefinition {
            id: spaceInfoPageDefinition
            source: "SpaceInfoPage.qml"
        },
        ComponentDefinition {
            id: mainViewPageDefinition
            source: "MainViewPage.qml"
        },
        ComponentDefinition {
            id: textEditorSettingsPageDefinition
            source: "TextEditorSettingsPage.qml"
        },
        ComponentDefinition {
            id: textEditorPreviewerPageDefinition
            source: "TextEditorPreviewerPage.qml"
        },
        ComponentDefinition {
            id: appCover
            source: "AppCover.qml"
        },
        MultiCover {
            id: multi
            SceneCover {
                MultiCover.level: CoverDetailLevel.High
                content: AppCover {
                    id: appCoverHigh
                }
            }
            SceneCover {
                MultiCover.level: CoverDetailLevel.Medium
                content: AppCover {
                    id: appCoverMedium
                }
            }
        }
    ]
    
    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All
        Application.setCover(multi)
        if ((!myApp.invokedAsApplication()) && settings.openLastDirectoryOnStartup && (historyListViewModel.itemsCount() > 0)) {
            navigationPane.invokeMainViewPage( historyListViewModel.lastItemByTimestamp() )
        } else {
            createMainPage() // Pre create page, to fast access later
        }
        
        fsInfo.update()
    }
}
