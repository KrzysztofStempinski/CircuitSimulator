#pragma once

#include <qpoint.h>

class EditorObject {

	protected:

		QPoint _pos;

	public:

		bool selected;
		QPoint prevPos;

		EditorObject() {
			
			selected = false;
			prevPos = QPoint(0, 0);
		
		}

};