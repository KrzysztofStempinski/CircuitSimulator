#pragma once

#include <qpoint.h>
#include <qrect.h>

class EditorObject {

	protected:

		QPoint _pos;
		QRect _boundingRect;

	public:

		bool selected;
		QPoint prevPos;

		EditorObject() {
			
			selected = false;
			prevPos = QPoint(0, 0);
		
		}

		const QPoint pos() const {
			return _pos;
		}

		const bool isWithinRectangle(const QRect& rect) const {
		
			return _boundingRect.translated(_pos).intersects(rect);

		}

		const bool isMouseOver(const QPoint& mousePos) {

			return _boundingRect.translated(_pos).contains(mousePos);

		}

};
