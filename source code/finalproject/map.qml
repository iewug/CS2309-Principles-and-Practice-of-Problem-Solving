import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

Rectangle {
    Map {
        id: myMap
        objectName: "myMap"
        anchors.fill: parent
        plugin: Plugin {name: "esri"} //"mapboxgl", "esri", "osm"
        center: QtPositioning.coordinate(31.02201, 121.42509)
        zoomLevel: 5

        MapItemView {
            id: markBlue
            model: ListModel {
                id: markBlueList
                // @disable-check M16
                objectName: "markBlueList"
            }

            delegate: MapItemGroup {
                MapQuickItem{
                    id:wawawa
                    coordinate:QtPositioning.coordinate(location.latitude,location.longitude)
                    sourceItem: Image{
                        id: imageB
                        source: "markerBlue.png"
                    }
                    anchorPoint.x: imageB.width / 2
                    anchorPoint.y: imageB.height
                }
            }
        }

        MapItemView {
            id: markRed
            model: ListModel {
                id: markRedList
                // @disable-check M16
                objectName: "markRedList"
            }

            delegate: MapItemGroup {
                MapQuickItem{
                    id:hahaha
                    coordinate:QtPositioning.coordinate(location.latitude,location.longitude)
                    sourceItem: Image{
                        id: imageR
                        source: "markerRed.png"
                    }
                    anchorPoint.x: imageR.width / 2
                    anchorPoint.y: imageR.height
                }
            }
        }

        MapItemView {
            id: mapLine
            model: ListModel {
                id: mapLinesList
                // @disable-check M16
                objectName: "mapLinesList"
            }

            delegate: MapPolyline {
                id: mapConnectionLine
                line.width: 2
                line.color: "red"
                path: [u,v]
            }
        }

        MapItemView{
            id:mapHeat
            model: ListModel{
                id: mapHeatList
                // @disable-check M16
                objectName: "mapHeatList"
            }
            delegate: MapRectangle{
                border.width: 0
                id:heatrectangle
                color: "#FF0000"
                opacity: __opacity
                topLeft: QtPositioning.coordinate(topleft.latitude,topleft.longitude)
                bottomRight: QtPositioning.coordinate(bottomright.latitude,bottomright.longitude)
            }
        }

        function setCenter(lat:double, lon:double)
        {
            myMap.center = QtPositioning.coordinate(lat,lon)
        }
        function addMarkerRed(lat:double, lon:double)
        {
            markRedList.append({"location":QtPositioning.coordinate(lat, lon)})
        }
        function addMarkerBlue(lat:double, lon:double)
        {
            markBlueList.append({"location":QtPositioning.coordinate(lat, lon)})
        }
        function addLine(x1:double, y1:double, x2:double, y2:double)
        {
            mapLinesList.append({"u":QtPositioning.coordinate(x1, y1),"v":QtPositioning.coordinate(x2, y2)})
        }
        function changeHeat(a:double, b:double, c:double, d:double, opa:double)
        {
            mapHeatList.append({"topleft":QtPositioning.coordinate(a, b),"bottomright":QtPositioning.coordinate(c, d),"__opacity":opa})
        }
    }
}
