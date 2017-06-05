import bb.cascades 1.4
import bb.system 1.2
import QtQuick 1.0

Page {
    id: spaceInfoPage
    objectName: "SpaceInfoPage"
    
    titleBar: TitleBar {
        id: titleBar0
        title: myApp.officialAppName()
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
    }
    
    function initialize(path) {
        titleBar0.title = mainListViewModel.currentPath(true)
        spaceInfo.initialize(path)
    }
    
    Container {
        topPadding: 20.0
        bottomPadding: 20.0
        leftPadding: 20.0
        rightPadding: 20.0
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        layout: DockLayout {
        }
        SpaceInfo {
            id: spaceInfo
        }
    }
}
