#pragma once

#include <qstring>

#include "../eigen/Dense"

QString vectorToString(const Eigen::VectorXd& vector) {

	QString S = "[ ";

	for (int i = 0; i < vector.size(); ++i) 
		S += QString::number(vector(i), 'g', 15) + " ";
	
	S += "]";

	return S;

}

QString matrixToString(const Eigen::MatrixXd& matrix) {

	//TODO implement
	return "Matrix to string not implemented";
}