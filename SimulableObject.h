#pragma once

class SimulableObject {

	public:

		int currentIndex;
		double currentValue;

		int voltageIndex;
		double voltageValue;

		SimulableObject() {
			reset();
		}

		void reset() {

			currentIndex = -1;
			voltageIndex = -1;

			currentValue = double(0);
			voltageValue = double(0);

		}

};