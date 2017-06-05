import bb.cascades 1.4

Container {
    id: searchBoxContainer
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    horizontalAlignment: HorizontalAlignment.Center
    
    function invoke() {
        searchBoxContainer.visible = true
        mainListViewModel.prepareFilter()
        searchBoxTextField.requestFocus();
    }
    function closeSearchBox(restoreFromCache) {
        mainListViewModel.clearFilter(restoreFromCache)
        searchBoxTextField.text = ""
        searchBoxContainer.visible = false
    }
    
    Container {
        verticalAlignment: VerticalAlignment.Center
        TextField {
            id: searchBoxTextField
            hintText: qsTr("Search...")
            onFocusedChanged: {
                if (focused) {

                }
            }
            onTextChanging: {
                mainListViewModel.setFilter(text)
            }
        }
    }
    Container {
        leftMargin: 20.0
        verticalAlignment: VerticalAlignment.Center
        Button {
            id: cancelButton
            text: qsTr("Cancel")
            preferredWidth: 150.0
            onClicked: {
                searchBoxContainer.closeSearchBox(true)
            }
        }
    }
} 
