import bb.cascades 1.4
import bb.system 1.2
import QtQuick 1.0

Sheet { 
    id: searchFilterSheet
    function initialize() {
        serachFilterPage.initialize()
    }
    
    signal searchFilterChanged()
    
    Page {
        id: serachFilterPage
        
        signal closeRequest()
        
        titleBar: TitleBar {
            title: qsTr("Filter")
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    searchFilterSheet.close()
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Search")
                onTriggered: {
                    settings.searchPath = inputPathItem.absFileNamePath
                    
                    var tagInfo = settings.searchTagInfo()
                    
                    tagInfo.useFileName = fileNameItem.checked
                    tagInfo.fileName = fileNameItem.data
                    
                    tagInfo.useTitle = titleItem.checked
                    tagInfo.title = titleItem.data
                    
                    tagInfo.useArtist = artistItem.checked
                    tagInfo.artist = artistItem.data
                    
                    tagInfo.useAlbum = albumItem.checked
                    tagInfo.album = albumItem.data
                    
                    tagInfo.useComment = commentItem.checked
                    tagInfo.comment = commentItem.data
                    
                    tagInfo.useGenre = genreItem.checked
                    tagInfo.genre = genreItem.data
                    
                    tagInfo.useYear = yearItem.checked
                    tagInfo.year = Number(yearItem.data)
                    
                    tagInfo.useTrack = trackItem.checked
                    tagInfo.track = Number(trackItem.data)
                    
                    settings.saveSearchTagInfo()
                    settings.save()

                    searchFilterSheet.close()
                    searchFilterSheet.searchFilterChanged()
                }
            }
        }
        
        function initialize() {
            inputPathItem.absFileNamePath = settings.searchPath
            inputPathItem.update()

            var tagInfo = settings.searchTagInfo()
            
            fileNameItem.checked = tagInfo.useFileName
            fileNameItem.data = tagInfo.fileName
            
            titleItem.checked = tagInfo.useTitle
            titleItem.data = tagInfo.title
            
            artistItem.checked = tagInfo.useArtist
            artistItem.data = tagInfo.artist
            
            albumItem.checked = tagInfo.useAlbum
            albumItem.data = tagInfo.album
            
            commentItem.checked = tagInfo.useComment
            commentItem.data = tagInfo.comment
            
            genreItem.checked = tagInfo.useGenre
            genreItem.data = tagInfo.genre
            
            yearItem.checked = tagInfo.useYear
            yearItem.data = ( tagInfo.year > 0) ? tagInfo.year : ""
            
            trackItem.checked = tagInfo.useTrack
            trackItem.data = ( tagInfo.track > 0) ?  tagInfo.track : ""
        }
        
        ScrollView {
            Container {
                topPadding: 20.0
                bottomPadding: 20.0
                leftPadding: 20.0
                rightPadding: 20.0

                CoverItem {
                    id: coverContainer
                    visible: false
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Container {
                    id: advancedContainer
                    visible: false
                    AdvancedSearchContainer {
                        id: advancedSearchContainer

                    }
                }

                Container {
                    id: tagContainer
                    InputItem {
                        id: inputPathItem
                    }
                    Header {
                        bottomMargin: 20.0
                    }
                    EditableItem {
                        id: fileNameItem
                        checkable: true
                        text: qsTr("File name")
                        bottomPadding: 20.0
                    }
                    Header {
                        title: qsTr("Tags")
                    }
                    EditableItem {
                        topPadding: 20.0
                        id: titleItem
                        checkable: true
                        text: qsTr("Title")
                    }
                    EditableItem {
                        topPadding: 20.0
                        id: artistItem
                        checkable: true
                        text: qsTr("Artist")
                    }

                    EditableItem {
                        topPadding: 20.0
                        id: albumItem
                        checkable: true
                        text: qsTr("Album")
                    }

                    EditableItem {
                        topPadding: 20.0
                        id: trackItem
                        checkable: true
                        text: qsTr("Track")
                    }
                    EditableItem {
                        topPadding: 20.0
                        id: yearItem
                        checkable: true
                        text: qsTr("Year")
                    }
                    EditableItem {
                        topPadding: 20.0
                        id: genreItem
                        checkable: true
                        text: qsTr("Genre")
                    }
                    BigEditableItem {
                        topPadding: 20.0
                        id: commentItem
                        checkable: true
                        text: qsTr("Comment")
                    }
                }
            }
        }
    }
}
