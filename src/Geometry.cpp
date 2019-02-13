/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Geometry.cpp
// 
//  ---------------------------------------------

#include "Geometry.h"

QPoint snapPointToGrid(const QPoint &pointToSnap, int gridSize, bool performSnap) {

	// checking whether we really want to snap the point to grid is better done here,
	// as saves us writing multiple identical ifs
	if (performSnap) {

		int snappedX = ((pointToSnap.x() + gridSize / 2) / gridSize) * gridSize;
		int snappedY = ((pointToSnap.y() + gridSize / 2) / gridSize) * gridSize;

		return QPoint(snappedX, snappedY);

	} else {
		return pointToSnap;
	}
}

QPoint rotatePoint(const QPoint &point, const QPoint &pivot, int angle) {

	double angle_r = PI / 180.0 * angle; // degrees to radians

	int rotatedX = (point.x() - pivot.x()) * cos(angle_r) - (point.y() - pivot.y()) * sin(angle_r) + pivot.x();
	int rotatedY = (point.x() - pivot.x()) * sin(angle_r) + (point.y() - pivot.y()) * cos(angle_r) + pivot.y();

	return QPoint(rotatedX, rotatedY);

}

float distanceSquaredBetweenPoints(const QPoint& p1, const QPoint& p2) {

	return (p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y());

}
 
float distanceBetweenPoints(const QPoint& p1, const QPoint& p2) {

	return sqrt(distanceSquaredBetweenPoints(p1, p2));

}

// TODO this is HORRIBLY inefficient, but has one major advantage over all the other methods 
// I tried in this two-hour-long crusade towards decent mouseOverLink detection
//
// the advantage is that
// *IT* *JUST* *WORKS*
bool isPointOnLine(const QPoint& lineStart, const QPoint& lineEnd, const QPoint& point, double precision){

	// S -- P -- E
	float distSE = distanceBetweenPoints(lineEnd, lineStart);
	float distSP = distanceBetweenPoints(point, lineStart);
	float distPE = distanceBetweenPoints(point, lineEnd);

	return (abs(distSE - (distSP + distPE)) <= precision);

}

void Circle::draw(QPainter& painter, const QPoint& parentPos, int) {

	painter.drawEllipse(center + parentPos, radius / 2, radius / 2);

}

void Line::draw(QPainter& painter, const QPoint& parentPos, int parentRotationAngle) {

	painter.drawLine(rotatePoint(parentPos + begin, parentPos, parentRotationAngle), rotatePoint(parentPos + end, parentPos, parentRotationAngle));

}

void loadGeometryFromJSON(const rapidjson::Value& JSONdata, GeometryData& geometryData) {

	geometryData.clear();

	for (rapidjson::Value::ConstValueIterator itr = JSONdata.Begin(); itr != JSONdata.End(); ++itr) {
		const rapidjson::Value& attribute = *itr;

		if (attribute["type"] == "line") {

			const rapidjson::Value& point1 = attribute["point1"];
			const rapidjson::Value& point2 = attribute["point2"];

			GeometryObject *line = new Line(QPoint(point1[0].GetInt(), point1[1].GetInt()),
				QPoint(point2[0].GetInt(), point2[1].GetInt()));

			geometryData.push_back(line);

		} else if (attribute["type"] == "circle") {

			const rapidjson::Value& center = attribute["center"];
			const rapidjson::Value& radius = attribute["radius"];

			GeometryObject *circle = new Circle(QPoint(center[0].GetInt(), center[1].GetInt()), radius.GetInt());

			geometryData.push_back(circle);

		} else if (attribute["type"] == "nametext") {

			const rapidjson::Value& pos = attribute["pos"];

		}
	
	}

}