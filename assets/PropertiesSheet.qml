import bb.cascades 1.4

Sheet { 
    id: propertiesSheet
    function initialize() {
        propertiesPage.initialize()
    }
    PropertiesPage {
        id: propertiesPage
        onCloseRequest: {
            propertiesSheet.close()
        }
    }
}
