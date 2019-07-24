#pragma once

#include <qcolor.h>
#include <Qpen.h>

namespace Config {
	const QColor colorGrid = Qt::darkGray;
	const QColor colorNormal = Qt::cyan;
	const QColor colorHighlighted = Qt::blue;
	const QColor colorSelected = Qt::red;

	namespace Pens {
		const QPen grid(Qt::darkGray, 1);
		const QPen selected(Qt::red, 2);
		const QPen highlighted(Qt::blue, 2);
		const QPen normal(Qt::cyan, 1);
		const QPen outline(Qt::lightGray, 1);
	}
};