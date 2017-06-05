import bb.cascades 1.4

Page {
    id: settingsPage
    objectName: "SettingsPage"
    
    titleBar: TitleBar {
        visibility: ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
        title: qsTr("Settings")
        appearance: TitleBarAppearance.Branded
    }
    
    function save() {
        var saved = false 
        myApp.setCurrentCascadesThemeIndex(cascadesThemeDropDown.selectedIndex)

        if (settings.allowEditNoMultimedia != allowEditNoMultimediaCheckBox.checked) {
            settings.allowEditNoMultimedia = allowEditNoMultimediaCheckBox.checked
            saved = true
        }
        
        if (settings.localizedDateTimeFormat != Boolean(dateTimeFormatDropDown.selectedIndex)) {
            settings.localizedDateTimeFormat = dateTimeFormatDropDown.selectedIndex
            saved = true
        }
        
        if ( settings.showScrollActions != showScrollActionsCheckBox.checked ) {
            settings.showScrollActions = showScrollActionsCheckBox.checked  
            saved = true
        }
        
        if ( settings.showTitleBar != showTitlebarCheckBox.checked ) {
            settings.showTitleBar = showTitlebarCheckBox.checked  
            saved = true
        } 
        
        if ( settings.showFileNameExtensionWhenRename != showFileNameExtensionWhenRenameCheckBox.checked) {
            settings.showFileNameExtensionWhenRename = showFileNameExtensionWhenRenameCheckBox.checked
            saved = true
        } 
        
        if ( settings.showDescriptionInMainViewForDirs != showDescriptionInMainViewForDirsCheckBox.checked) {
            settings.showDescriptionInMainViewForDirs = showDescriptionInMainViewForDirsCheckBox.checked
            saved = true
        } 
        
        if ( settings.showDescriptionInMainViewForFiles != showDescriptionInMainViewForFilesCheckBox.checked) {
            settings.showDescriptionInMainViewForFiles = showDescriptionInMainViewForFilesCheckBox.checked
            saved = true
        }
        
        
        if ( settings.showStatusInMainView != showStatusInMainViewCheckBox.checked) {
            settings.showStatusInMainView = showStatusInMainViewCheckBox.checked
            saved = true
        } 
        if ( settings.showFileNameExtensionInMainView != showFileNameExtensionInMainViewCheckBox.checked) {
            settings.showFileNameExtensionInMainView = showFileNameExtensionInMainViewCheckBox.checked
            saved = true
        }
        
        if ( settings.openLastDirectoryOnStartup != Boolean(onStartupDropDown.selectedIndex)) {
            settings.openLastDirectoryOnStartup = onStartupDropDown.selectedIndex
            saved = true
        }
        
        if (saved) { 
            settings.save()
            return true
        }
        return false
    
    }
    onCreationCompleted: {
        cascadesThemeDropDown.selectedIndex = myApp.currentCascadesThemeIndex()
        dateTimeFormatDropDown.selectedIndex = settings.localizedDateTimeFormat
        onStartupDropDown.selectedIndex = settings.openLastDirectoryOnStartup
        showScrollActionsCheckBox.checked = settings.showScrollActions
        showTitlebarCheckBox.checked = settings.showTitleBar
        showFileNameExtensionWhenRenameCheckBox.checked = settings.showFileNameExtensionWhenRename
        showDescriptionInMainViewForFilesCheckBox.checked = settings.showDescriptionInMainViewForFiles
        showDescriptionInMainViewForDirsCheckBox.checked = settings.showDescriptionInMainViewForDirs
        showStatusInMainViewCheckBox.checked = settings.showStatusInMainView
        showFileNameExtensionInMainViewCheckBox.checked = settings.showFileNameExtensionInMainView
        
        allowEditNoMultimediaCheckBox.checked = settings.allowEditNoMultimedia
        //dropBoxBlackBerryCheckBox.checked = settings.dropBoxBlackBerry
        //boxBlackBerryCheckBox.checked = settings.boxBlackBerry
        //oneDriveBlackBerryCheckBox.checked = settings.oneDriveBlackBerry
    }
    
    Container {
        leftPadding: 20.0
        rightPadding: 20.0
        topPadding: 20.0
        bottomPadding: 20.0
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        SegmentedControl {
            id: segmentedControl
            bottomMargin: 20
            Option {
                id: generalOption
                text: qsTr("General")
                selected: true
            }
            Option {
                id: networkOption
                text: qsTr("Misc")
            }
        }
        ScrollView {
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                Container {
                    visible: networkOption.selected
                    Header {
                        title: qsTr("Expert settings")
                    }
                    Label {
                        text: qsTr("Expert settings are intended for editing only by experienced users.")
                        multiline: true
                    }
                    Divider {
                        
                    }
                    Label {
                        text: qsTr("This option allows for creating and deleting the system file .nomedia. In BackBerry 10 this file is used for hiding multimedia content from system applications, e.g. Music. The original files themselves remain untouched in their places. For using this function, select the folder, press Properties and check the No Media checkbox. Be aware that switching off the No Media setting does not always lead to immediate inclusion of multimedia content into the corresponding system apps. Device reboot might be required in some cases.") 
                        multiline: true
                    }
                    CustomCheckBox {
                        id: allowEditNoMultimediaCheckBox
                        topPadding: 20.0
                        text: qsTr("Enable creation/deletion of .nomedia file")
                    }


                    Header {
                        visible: false
                        title: qsTr("Clouds (via BlackBerry apps)")
                    }
                    CustomCheckBox {
                        visible: false
                        id: dropBoxBlackBerryCheckBox
                        topPadding: 20.0
                        text: ("DropBox")
                    }
                    Divider {
                        visible: false
                    }
                    CustomCheckBox {
                        visible: false
                        id: boxBlackBerryCheckBox
                        text: ("Box")
                    }
                    Divider {
                        visible: false
                    }
                    CustomCheckBox {
                        visible: false
                        id: oneDriveBlackBerryCheckBox
                        text: ("One Drive")
                    }
                }
                
                Container {
                    id: generalContainer
                    visible: generalOption.selected
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Header {
                        title: qsTr("Main view")
                    }
                    
                    
                    CustomCheckBox {
                        topPadding: 20.0
                        id: showTitlebarCheckBox
                        text: qsTr("Show title bar")
                    }
                    Divider {
                    
                    }                                
                    CustomCheckBox {
                        id: showDescriptionInMainViewForDirsCheckBox
                        text: qsTr("Show description for local folders")
                    }
                    
                    Divider {
                    
                    }                                
                    CustomCheckBox {
                        id: showDescriptionInMainViewForFilesCheckBox
                        text: qsTr("Show description for files")
                    }
                    Divider {
                    
                    }
                    CustomCheckBox {
                        id: showStatusInMainViewCheckBox
                        text: qsTr("Show status")
                    }
                    
                    Divider {
                    
                    }

                    CustomCheckBox {
                        id: showFileNameExtensionInMainViewCheckBox
                        text: qsTr("Show file extensions")
                    }
                    Divider {
                    
                    }
                    
                    CustomCheckBox {
                        id: showFileNameExtensionWhenRenameCheckBox
                        text: qsTr("Edit file extension when renaming file")
                    
                    }
                    Divider {
                    
                    }
                    CustomCheckBox {
                        id: showScrollActionsCheckBox
                        text: qsTr("Show scroll actions")
                        bottomPadding: 20.0
                    }
                    
                    Header {
                        title: qsTr("Misc")
                    }
                    DropDown {
                        id: onStartupDropDown
                        title: qsTr("At launch")
                        horizontalAlignment: HorizontalAlignment.Fill
                        Option {
                            text: qsTr("Open main page (default)")
                            value: 0
                        }
                        Option {
                            text: qsTr("Go to last folder")
                            value: 1
                        }
                    }
                    
                    Divider {
                    }
                    DropDown {
                        id: dateTimeFormatDropDown
                        title: qsTr("Time format")
                        horizontalAlignment: HorizontalAlignment.Fill
                        Option {
                            text: ("dd.MM.yyyy hh:mm")
                            value: 0
                        }
                        Option {
                            text: qsTr("Localized") 
                            value: 1
                        }
                    }
                    Divider {
                    
                    }
                    
                    DropDown {
                        id: cascadesThemeDropDown
                        title: qsTr("Visual theme")
                        horizontalAlignment: HorizontalAlignment.Fill
                        Option {
                            text: qsTr("Light")
                            value: "bright"
                        }
                        Option {
                            text: qsTr("Dark")
                            value: "dark"
                        }
                        onSelectedIndexChanged: {
                            if (selectedIndex == 0) {
                                Application.themeSupport.setVisualStyle(VisualStyle.Bright)
                            } else {
                                Application.themeSupport.setVisualStyle(VisualStyle.Dark)
                            }
                        }
                    }
                }
            }
        }
    }
}
