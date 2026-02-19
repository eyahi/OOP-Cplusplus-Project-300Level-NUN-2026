import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 400
    height: 600
    visible: true
    title: qsTr("Haly Chat App")
    color: "#FFFFFF" // Blue background

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // ListView displays all messages from the C++ model
        ListView {
            id: messageList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: msgModel // MODEL (MVVM/MVC)
            clip: true

            // Delegate: each message object
            delegate: Rectangle {
                width: parent.width
                height: textItem.implicitHeight + 10
                color: display.startsWith("YOU") ? "#779ECB" : "#FFFFFF" // Polymorphism: color depends on sender
                radius: 5

                Text {
                    id: textItem
                    text: display
                    color: display.startsWith("YOU") ? "#FFFFFF" : "#000000"
                    anchors.margins: 5
                    anchors.fill: parent
                    wrapMode: Text.Wrap
                }
            }
        }

        // Row containing the input field and send button
        RowLayout {
            Layout.fillWidth: true
            spacing: 5

            // User input
            TextField {
                id: messageInput
                Layout.fillWidth: true
                placeholderText: "Type a message..."
                text: msgModel ? msgModel.currentMessage : "" // Data binding: QML property <-> C++ property

                // OOP: ENCAPSULATION
                // Function: onTextChanged()
                // Updates the C++ model's currentMessage property whenever user types
                onTextChanged: if(msgModel) msgModel.currentMessage = text

                // OOP: COMMAND PATTERN
                // Function: Keys.onReleased()
                // Enter key triggers sendMessage() from C++ model
                Keys.onReleased: {
                    if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                        if (msgModel) {
                            msgModel.sendMessage() // OOP: Abstraction & Encapsulation
                            messageInput.text = "" // Clear input after sending
                        }
                    }
                }
            }

            // Send button
            Button {
                text: "➡"
                Layout.preferredWidth: 40
                // OOP: COMMAND PATTERN
                // Function: onClicked()
                // Calls C++ function sendMessage() without QML knowing the internal logic
                onClicked: {
                    if (msgModel && messageInput.text.length > 0) {
                        msgModel.sendMessage() // Encapsulation
                        messageInput.text = ""
                    }
                }
            }
        }
    }
}
