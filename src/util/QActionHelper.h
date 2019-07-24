#pragma once

#include <qaction.h>
#include <qstring.h>

QAction* createAction(const QString& title, const char* slot, const QString& shortcut = "", const QString& iconName = "");