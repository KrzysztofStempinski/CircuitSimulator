#pragma once

#include <qcolor.h>
#include <Qpen.h>
#include <qicon.h>

namespace Config {

	namespace Paths {
		const QString Icons = "data\\icons\\";
		const QString Components = "data\\components\\";
	}

	namespace Icons {
		const QString OK = Paths::Icons + "ok.ico";
		const QString cancel = Paths::Icons + "cancel.ico";
	}

	namespace Pens {
		const QPen grid(Qt::darkGray, 1);
		const QPen selected(Qt::red, 2);
		const QPen highlighted(Qt::blue, 2);
		const QPen normal(Qt::cyan, 1);
		const QPen outline(Qt::lightGray, 1);
	}
};
