import bb.cascades 1.4

Dialog {
    id: createNewFileDialog
    Container {
        id: createNewFileDialogContainer
        
        property int dividerSize: 2
        property string dividerColorString: "#e5e5e5"
        
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        background: Color.create(0.0, 0.0, 0.0, 0.5)
        
        layout: DockLayout {
        }
        
        Container {
            // Set the size of the dialog relative to
            // the size of the screen that the app
            // is running on.
            maxWidth: 716
            maxHeight: 600
            minHeight: 716
            minWidth: 600
            
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout {
            }
            Container {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                Container {
                    topPadding: 20.0
                    bottomPadding: 20.0
                    background: Color.White
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                    Container {
                        leftPadding: 20.0
                        NativeLabel {
                            text: qsTr("Select type")
                            textStyle.fontWeight: FontWeight.Normal
                            textStyle.fontSize: FontSize.Large

                        }
                        bottomPadding: 20.0
                    }
                    ScrollView {
                        verticalAlignment: VerticalAlignment.Fill
                        horizontalAlignment: HorizontalAlignment.Fill
                        Container {
                            verticalAlignment: VerticalAlignment.Fill
                            horizontalAlignment: HorizontalAlignment.Fill
                            NativeRadioButton {
                                text: "MS-Word (DOC)"
                                background: Color.White
                            }
                            NativeHorizontalDivider {

                            }
                            NativeRadioButton {
                                text: "MS-Excel (XLS)"
                            }
                            NativeHorizontalDivider {

                            }
                            
                            NativeRadioButton {
                                text: "MS-PowerPoint  (PPT)"
                            }
                            NativeHorizontalDivider {

                            }
                            NativeRadioButton {
                                text: "Text (TXT)"
                            }
                            NativeHorizontalDivider {

                            }
                            NativeRadioButton {
                                text: qsTr("Other")
                            }
                            NativeHorizontalDivider {

                            }
                            TextField {
                                hintText: qsTr("Enter a filename")
                            }
                        }
                    }
                }
                
                Container {
                    NativeHorizontalDivider {

                    }
                    Container {
                        id: buttonsContainer
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        minHeight: 100
                        horizontalAlignment: HorizontalAlignment.Fill
                        NativeButton {
                            text: qsTr("Cancel")
                            onClicked: {
                                createNewFileDialog.close()
                            }
                        }
                        NativeVerticalDivider {

                        }
                        NativeButton {
                            text: qsTr("Create")
                        }
                    }
                }
            }
        }
    }
}
