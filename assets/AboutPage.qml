import bb.cascades 1.4

Page {
    id: aboutPage

    Container {
        id: container
        layout: DockLayout {
        }

        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Label {
                text: myApp.officialAppName()
                horizontalAlignment: HorizontalAlignment.Center
                textStyle.fontWeight: FontWeight.Bold
                textStyle.fontSize: FontSize.XXLarge
            }
            Label {
                text: myApp.officialAppDescription()
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                text: {
                    qsTr("Version")  + " " + myApp.versionNumber()
                }
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: myApp.officialUrlLink()
                textStyle.fontSize: FontSize.Large
            }
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: myApp.officialEMailLink()
                textStyle.fontSize: FontSize.Large
            }
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                text: myApp.officialBBMChannelLink()
                textStyle.fontSize: FontSize.Large
            }
        }
    }
    actions: [
        InvokeActionItem {
            title: "BlackBerry World"
            imageSource: "asset:///images/world.png"
            query {
                mimeType: "application/x-bb-appworld"
                uri: "appworld://content/" + myApp.contentNumber()
                invokeActionId: "bb.action.OPEN"
            }
        },
        
        InvokeActionItem {
            title: qsTr("More apps...")
            imageSource: "asset:///images/moreapps.png"
            query {
                mimeType: "application/x-bb-appworld"
                uri: "appworld://vendor/" + myApp.vendorNumber()
                invokeActionId: "bb.action.OPEN"
            }
        },
        InvokeActionItem {
            query {
                mimeType: "text/plain"
                invokeActionId: "bb.action.SHARE"
                data: myApp.officialAppName() + (". You can find it at ") + "http://appworld.blackberry.com/webstore/content/" + myApp.contentNumber()
            }
        },
        InvokeActionItem {
            title: qsTr("Feedback")
            query {
                mimeType: "text/plain"
                invokeTargetId: "sys.pim.uib.email.hybridcomposer"
                invokeActionId: "bb.action.SENDEMAIL"
                uri: myApp.officialEMailLink(false)
            }
        }
    ]
}
