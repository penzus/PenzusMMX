import bb.cascades 1.4

Container {
    id: spaceInfoContainer
    leftPadding: 20.0
    topPadding: 20.0
    bottomPadding: 20.0
    rightPadding: 20.0
    objectName: "SpaceInfo"
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    function initialize(path) {
        deviceProgressItem.titile = mainListViewModel.currentPath(true)
        fsInfo.getUsage(path)
        deviceProgressItem.setUsage(fsInfo.usedMemory(), fsInfo.totalMemory(),fsInfo.percent(), fsInfo.text())
    }
    
    Container {
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Fill
        Container {
            ProgressItem {
                id: deviceProgressItem
                titile: qsTr("Device")
            }
        }
    }
}
