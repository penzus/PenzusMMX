import bb.cascades 1.4

DropDown {
    id: myDropDown

    function selectOptionByValue(value) {
        for (var i = 0; i < myDropDown.count(); i ++) {
            var dropDownOption = myDropDown.at(i);
            if (dropDownOption.value == value) {
                myDropDown.setSelectedIndex(i);
                return true;
            }
        }
        return false;
    }
    
    function appendOption(text, value ) {
        var option = optionControlDefinition.createObject();
        option.text = text
        option.value = value
        myDropDown.add(option)
    }
    
    function selectOptionByName(optionName) {
        for (var i = 0; i < myDropDown.count(); i ++) {
            var dropDownOption = myDropDown.at(i);
            if (dropDownOption.value == optionName) {
                myDropDown.setSelectedIndex(i);
                return true;
            }
        }
        return false;
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: optionControlDefinition
            Option {
            }
        }
    ]
}
