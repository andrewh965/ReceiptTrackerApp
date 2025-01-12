import QtQuick 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15


ApplicationWindow{
    visible: true
    width: 640
    height: 480
    title: "Receipt Tracker"

    property string tmpImg : "";
    property int uyear;
    property int umonth;
    property double total;
    property var monthList : ["January","February","March","April","May","June","July","August","September","October","November","December"];


    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        flash.mode: Camera.FlashOn
        imageCapture {
            onImageSaved: {
                imageHandler.printImage(requestId) //request id alr made by camera
                buttonBackground.color = "#c7cdd6"
            }
            onImageCaptured: {
                camera.unlock()
            }

        }
    }

    SwipeView{
        id: swipeView

        anchors.fill:parent
        width: parent.width
        height: parent.height
        Page{
            background: Rectangle{
                color: "#7676d6"
            }
            Rectangle {
                id: dimmingOverlay
                visible:false
                color: "black"
                opacity: .5
                anchors.fill:parent
                z:1
            }

            GroupBox{
                id: streamContainer
                height: parent.height * 3/4 + 10
                width : parent.width * 8/10 + 25
                anchors.horizontalCenter:  parent.horizontalCenter
                anchors.verticalCenter:  parent.verticalCenter
                anchors.verticalCenterOffset: -25

                padding:5
                topPadding:padding


                background: Rectangle {
                    color: "transparent"
                    border.color: "#000000"
                    radius: parent.padding * 2
                    border.width: parent.padding
                    z:1
                }
                contentItem: VideoOutput{
                    id: stream
                    source: camera
                    orientation: -90
                    fillMode: VideoOutput.PreserveAspectCrop
                }
            }


            Button{
                id: roundButton
                width: 50
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 40

                background: Rectangle{
                    id: buttonBackground
                    color: "#FFFFFFFF"
                    border.color: "#000000"
                    border.width: 4
                    radius: width/2
                    z:1
                }

                onClicked: {
                    buttonBackground.color = "#9197a1"
                    camera.imageCapture.captureToLocation('')
                }
            }

            Connections{
                target: imageHandler
                function onNotifyUser(store,total){
                    storeIn.text = store
                    totalIn.text = total
                    popup.open()
                }
            }

            Popup{
                id: popup
                width: parent.width * 3/4
                height: parent.height * 1/4
                anchors.centerIn:parent
                onOpened: dimmingOverlay.visible = true
                onClosed:{
                    dimmingOverlay.visible = false
                    imageHandler.submitForm(storeIn.text,totalIn.text)
                    storeIn.text = ""
                    totalIn.text = ""
                }
                bottomMargin: 50
                closePolicy: Popup.CloseOnPressOutside

                background: Rectangle {
                    id: popupBackground
                    color: "#FFFFFFFF"
                    border.color: "#000000"
                    radius: parent.padding * 2
                    border.width: 5
                }

                Column{
                    spacing: 20
                    anchors.centerIn: parent
                    Rectangle{
                        id: storeRect
                        width: popup.width * 3/4
                        height: 30
                        border.color: "#000000"
                        border.width: 1

                        TextArea{
                            id: storeIn
                            width:parent.width
                            height:parent.height
                            activeFocusOnPress: true
                            anchors.fill:parent
                            wrapMode: TextArea.NoWrap
                            placeholderText: qsTr("Store")
                            Keys.onReturnPressed: {
                                popup.focus = true
                                storeIn.focus = false
                            }
                        }
                        z: 1
                    }

                    Rectangle{
                        id: totalRect
                        width: popup.width * 3/4
                        height: 30
                        border.color: "#000000"
                        border.width: 1

                        TextArea{
                            id: totalIn
                            width:parent.width
                            height:parent.height
                            activeFocusOnPress: true
                            anchors.fill:parent
                            wrapMode: TextArea.NoWrap
                            placeholderText: qsTr("Total")
                            Keys.onReturnPressed: {
                                popup.focus = true
                                totalIn.focus = false
                            }
                        }
                        z: 1
                    }

                    Button{
                        id: submitButton
                        width: popup.width*1/4
                        height: 30
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text{
                            text: "Submit"
                            anchors.centerIn: parent
                            z:1
                        }

                        background: Rectangle{
                            id: submitBackgound
                            color: "#FFFFFFFF"
                            border.color: "#000000"
                            border.width: 2
                            radius: 10
                            z:1
                        }

                        onClicked: {
                            popup.close()

                        }

                    }

                }
            }
        }//end page
        /*******************************************************************************/

        Page{
            id: reportPage


            background: Rectangle{
                color:  "#7676d6"
            }


            ListView{ //year view
               id: yearView
               spacing: 10
               anchors.fill: parent
               width: parent.width
               anchors.topMargin: 30
               anchors.bottomMargin: 15
               model: yearModel
               delegate: Column{
                   anchors.horizontalCenter: parent.horizontalCenter
                   Button{
                       height: reportPage.height * 1/10
                       width : reportPage.width * 3/5
                       background: Rectangle{
                           id: yearButton
                           color:  "#416dc4"
                           border.color: "#000000"
                           border.width: 2
                           radius: 10
                        }
                       onPressed: {
                           yearButton.color = "#203661"
                       }onReleased: {
                           yearView.visible = false
                           yearButton.color ="#416dc4"
                           uyear = model.calendarVal
                           imageHandler.setCalendarModel(monthModel, uyear)
                           monthView.visible = true
                       }

                       Text{
                           text: model.calendarVal
                           font.bold: true
                           font.pointSize: 30
                           anchors.horizontalCenter:parent.horizontalCenter
                           anchors.verticalCenter:parent.verticalCenter
                       }
                    }


               }
            }


            ListView{

                id: monthView
                anchors.fill: parent
                width:parent.width
                model: monthModel
                visible:false
                spacing: 10
                anchors.topMargin: 30
                anchors.bottomMargin: 15

                Button{
                    width: 50
                    height: 25
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    background: Rectangle{
                        color:  "#416dc4"
                        border.color: "#000000"
                        border.width: 2
                        radius: 15
                     }
                    onClicked: {
                        yearView.visible = true
                        monthView.visible = false
                    }

                    Text{
                        text: "BACK"
                        font.bold: true
                        font.pointSize: 15
                        anchors.horizontalCenter:parent.horizontalCenter
                        anchors.verticalCenter:parent.verticalCenter
                    }
                }

                delegate: Column{
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button{
                        height: reportPage.height * 1/10
                        width : reportPage.width * 3/5
                        spacing: 25
                        visible:true
                        background:Rectangle{
                            id:monthButton
                            color:  "#4de8b4"
                            border.color: "#000000"
                            border.width: 2
                            radius: 10

                        }
                        onPressed: {
                            monthButton.color = "#26785d"

                        }onReleased: {
                            monthButton.color = "#4de8b4"
                            monthView.visible=false
                            umonth = Number(model.calendarVal)
                            imageHandler.setReceiptModel(uyear,umonth,receiptModel)
                            receiptView.visible = true
                            total = imageHandler.getTotal(receiptModel)
                        }

                        text: monthList[model.calendarVal-1]
                        font.bold: true
                        font.pointSize: 20

                    }

                }



            }


            //receipt tab
            ScrollView{
                id: receiptView
                anchors.fill: parent
                clip:false
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.horizontal.interactive: false
                ScrollBar.vertical.interactive: true
                visible:false
                ListView{

                   visible:true
                   spacing: 10
                   anchors.fill: parent
                   anchors.topMargin: 30
                   model: receiptModel

                   Button{
                       id: backButton
                       width: 50
                       height: 25
                       anchors.top: parent.top
                       anchors.topMargin: 5
                       anchors.left: parent.left
                       anchors.leftMargin: 5
                       background: Rectangle{
                           color:  "#416dc4"
                           border.color: "#000000"
                           border.width: 2
                           radius: 15
                        }
                       onClicked: {
                           monthView.visible = true
                           receiptView.visible = false
                       }

                       Text{
                           text: "BACK"
                           font.bold: true
                           font.pointSize: 15
                           anchors.horizontalCenter:parent.horizontalCenter
                           anchors.verticalCenter:parent.verticalCenter
                       }
                   }

                   Rectangle{
                       width: 60
                       height: 55
                       anchors.top: backButton.bottom
                       anchors.topMargin: 5
                       anchors.left: parent.left
                       anchors.leftMargin: 5
                       color:  "#4fff23"
                       border.color: "#000000"
                       border.width: 2
                       radius: 15



                       Text{
                           text: "<b><u>Total</u></b><br>"+total.toFixed(2)
                           font.pointSize: 15
                           textFormat: Text.RichText
                           anchors.horizontalCenter:parent.horizontalCenter
                           anchors.verticalCenter:parent.verticalCenter
                       }
                   }

                   delegate : Column{
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.leftMargin: 30
                       spacing: 10

                       Button{
                           height: reportPage.height * 1/10
                           width : reportPage.width * 3/5
                           background:Rectangle{
                               height: parent.height
                               width : parent.width
                               color:  "#4fff23"
                               border.color: "#000000"
                               border.width: 2
                               radius: 10
                               z:1
                               Column{

                                   anchors.centerIn: parent
                                   anchors.topMargin: 20
                                   Text{
                                       text: model.store
                                       font.bold: true
                                       font.pointSize: 20
                                       font.underline:true
                                       anchors.horizontalCenter:parent.horizontalCenter
                                   }
                                   Text{
                                       text: "$"+model.total

                                       anchors.horizontalCenter: parent.horizontalCenter
                                   }
                                   Text{
                                       text: model.date
                                       anchors.horizontalCenter: parent.horizontalCenter
                                   }

                               }

                            }
                           onClicked:{
                              imageHandler.getPix(model.ID)
                              tmpImg = "file://"+imageHandler.getTmp()+"/tmp.jpg"
                              picture.open()
                           }

                       }
                    }
                }
                Popup{
                    id: picture
                    width: 300
                    height: 300
                    anchors.centerIn: parent

                    background: Image{
                        source: tmpImg
                        rotation:90
                        fillMode: Image.PreserveAspectFit
                    }
                    closePolicy: Popup.CloseOnPressOutside
                    onClosed:{
                        imageHandler.deleteTmp()
                        tmpImg=""

                    }

                }

            }


        }//end page
    }//end swipe view

    PageIndicator {
        id: indicator
        count: swipeView.count
        currentIndex: swipeView.currentIndex
        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }


}
