/*

	This file is part of CircuitSimulator
	Copyright (C) 2018 Krzysztof Stempinski

	Refer to main.cpp or License.md for licensing info

*/

//  ---------------------------------------------
//
//	Math.h
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
template <typename T> 
int sign(T val) {
	return (T(0) < val) - (val < T(0));
}

QPoint snapPointToGrid(const QPoint& pointToSnap, int gridSize, bool performSnap = true);
QPoint rotatePoint(const QPoint& point, const QPoint& pivot, int angle);

bool isPointOnLine(const QPoint& lineStart, const QPoint& lineEnd, const QPoint& point, const double precision = 0.25);

float distanceBetweenPoints(const QPoint& p1, const QPoint& p2);
float distanceSquaredBetweenPoints(const QPoint& p1, const QPoint& p2);