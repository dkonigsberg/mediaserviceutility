import bb.cascades 1.0

Page {
    id: page
    signal testQuery()
    content: Container {
        horizontalAlignment: HorizontalAlignment.Fill
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            text: "Media Service Utility"
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        Divider { }
        Button {
            horizontalAlignment: HorizontalAlignment.Center
            text: "Test Query"
            onClicked: {
                page.testQuery()
            }
        }
    }
}
