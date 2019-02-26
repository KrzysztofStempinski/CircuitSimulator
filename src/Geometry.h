#pragma once

#include <qpainter.h>

#include "../rapidjson/document.h"

#include "Math.h"

struct GeometryObject {

	virtual void draw(QPainter &painter, const QPoint &parentPos, const int parentRotationAngle = 0) const = 0;

};

struct Circle : public GeometryObject {

	QPoint center;
	int radius;

	Circle(QPoint _center, int _radius)
		: center(_center), radius(_radius) {};

	void draw(QPainter& painter, const QPoint& parentPos, const int) const {

		painter.drawEllipse(center + parentPos, radius / 2, radius / 2);

	}

};

struct Line : public GeometryObject {

	QPoint begin, end;

	Line(QPoint _begin, QPoint _end)
		: begin(_begin), end(_end) {};

	void draw(QPainter& painter, const QPoint &parentPos, const int parentRotationAngle = 0) const {

		painter.drawLine(rotatePoint(parentPos + begin, parentPos, parentRotationAngle), rotatePoint(parentPos + end, parentPos, parentRotationAngle));

	}

};

struct GeometryList {

	std::vector<GeometryObject*> objects;

	GeometryList() {}

	GeometryList(const rapidjson::Value& JSONdata) {
		loadFromJSON(JSONdata);
	}

	void loadFromJSON(const rapidjson::Value& JSONdata){

		objects.clear();

		for (rapidjson::Value::ConstValueIterator itr = JSONdata.Begin(); itr != JSONdata.End(); ++itr) {
			const rapidjson::Value& attribute = *itr;

			if (attribute["type"] == "line") {

				const rapidjson::Value& point1 = attribute["point1"];
				const rapidjson::Value& point2 = attribute["point2"];

				GeometryObject *line = new Line(QPoint(point1[0].GetInt(), point1[1].GetInt()),
					QPoint(point2[0].GetInt(), point2[1].GetInt()));

				objects.push_back(line);

			}
			else if (attribute["type"] == "circle") {

				const rapidjson::Value& center = attribute["center"];
				const rapidjson::Value& radius = attribute["radius"];

				GeometryObject *circle = new Circle(QPoint(center[0].GetInt(), center[1].GetInt()), radius.GetInt());

				objects.push_back(circle);

			}
			else if (attribute["type"] == "nametext") {

				const rapidjson::Value& pos = attribute["pos"];

			}

		}

	}

};