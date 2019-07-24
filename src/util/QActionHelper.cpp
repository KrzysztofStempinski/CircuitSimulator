#include "QActionHelper.h"

#include "../Config.h"

QAction* createAction(const QString& title, const char* slot, const QString& shortcut, const QString& iconName) {
	QAction* action = new QAction(title);

	if (!iconName.isEmpty())
		action->setIcon(QIcon(Config::Paths::Icons + iconName));

	if (!shortcut.isEmpty())
		action->setShortcut(QKeySequence(shortcut));

	return action;
};
