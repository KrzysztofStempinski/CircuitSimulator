#pragma once

#include "components/Resistor.h"
#include "components/Ground.h"
#include "components/VoltageSource.h"
#include "components/Diode.h"

const QString COMPONENT_LIST[] = { "resistor", "voltage_source", "ground", "diode" };

//TODO this is absolutely horrible. We should create components differently
inline Component* getComponentFromName(const QString componentName) {
	if (componentName == "resistor")
		return new Resistor();
	else if (componentName == "voltage_source")
		return new VoltageSource();
	else if (componentName == "ground")
		return new Ground();
	else if (componentName == "diode")
		return new Diode();
	else
		throw "TODO error messages!";
}