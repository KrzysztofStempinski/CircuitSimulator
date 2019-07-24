#pragma once

#include "components/Resistor.h"
#include "components/Ground.h"
#include "components/VoltageSource.h"
#include "components/Diode.h"
#include "components/IdealOpAmp.h"

const QString COMPONENT_LIST[] = { "resistor", "voltage_source", "ground", "diode", "ideal_op_amp" };

inline Component* getComponentFromName(const QString componentName) {
	if (componentName == "resistor")
		return new Resistor();
	else if (componentName == "voltage_source")
		return new VoltageSource();
	else if (componentName == "ground")
		return new Ground();
	else if (componentName == "diode")
		return new Diode();
	else if (componentName == "ideal_op_amp")
		return new IdealOpAmp();
	else
		throw "TODO error messages!";
}