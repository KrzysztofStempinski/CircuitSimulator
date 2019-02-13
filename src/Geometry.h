/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Geometry.h
// 
//  ---------------------------------------------

#pragma once

#include <cmath>
#include <vector>

#include <qpoint.h>
#include <qrect.h>
#include <qpainter.h>

#include "../rapidjson/document.h"

const double PI = std::atan(1) * 4;

// TODO find a proper place to put this in
template <typename T> int sign(T val) {
	return (T(0) < val) - (val < T(0));
}

QPoint snapPointToGrid(const QPoint& pointToSnap, int gridSize, bool performSnap = true);
QPoint rotatePoint(const QPoint& point, const QPoint& pivot, int angle);

bool isPointOnLine(const QPoint& lineStart, const QPoint& lineEnd, const QPoint& point, double precision = 0.25);

float distanceBetweenPoints(const QPoint& p1, const QPoint& p2);
float distanceSquaredBetweenPoints(const QPoint& p1, const QPoint& p2);

struct GeometryObject {

	virtual void draw(QPainter &painter, const QPoint &parentPos, int parentRotationAngle = 0) = 0;

};

struct Circle : public GeometryObject {

	QPoint center;
	int radius;

	Circle(QPoint _center, int _radius) 
		: center(_center), radius(_radius) {};

	void draw(QPainter& painter, const QPoint& parentPos, int);

};

struct Line : public GeometryObject {

	QPoint begin, end;

	Line(QPoint _begin, QPoint _end) 
		: begin(_begin), end(_end) {};

	void draw(QPainter &painter, const QPoint &parentPos, int parentRotationAngle = 0);

};

using GeometryData = std::vector<GeometryObject*>;

void loadGeometryFromJSON(const rapidjson::Value& JSONdata, GeometryData& geometryData);