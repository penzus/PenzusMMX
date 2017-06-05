import bb.cascades 1.4
import bb.system 1.2

import QtQuick 1.0
import TagEditor 1.0

Container {
    bottomPadding: 20.0
    leftPadding: 20.0
    rightPadding: 20.0
    
    property alias dirCover: directoryCover
    property alias intCover: internalCover
    
    verticalAlignment: VerticalAlignment.Center
    horizontalAlignment: HorizontalAlignment.Center
    
    function initialize() {
        directoryCover.reset()
        internalCover.reset()
        directoryCover.load( properties.directoryImageCover() )
    }
    
    Container {
        Header {
            title: qsTr("Internal cover")
            id: internalCoverHeader
            visible: false
        }
        CoverItem {
            topPadding: 20.0
            bottomPadding: 20.0
            id: internalCover
            visible: false
        }
        
        Header {
            title: qsTr("Album cover")
        }
        CoverItem {
            topPadding: 20.0
            id: directoryCover
        }
    }
}
