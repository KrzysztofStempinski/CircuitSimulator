#pragma once

#include <qstring>

#include "../eigen/Dense"

QString vectorToString(const Eigen::VectorXd& vector) {

	QString S = "[ ";
	for (int i = 0; i < vector.size(); ++i) 
		S += QString::number(vector(i)) + " ";
	//S += QString::number(vector(i), 'g', 15) + " ";
	return S + "]";;

}

QString matrixToString(const Eigen::MatrixXd& matrix) {

	QString S = "\n\t[";
	for (int i = 0; i < matrix.rows(); ++i) {

		S += "\t";

		for (int j = 0; j < matrix.cols(); ++j) 
			S += QString::number(matrix(i, j)) + "\t";

		S += ",\n";
	}

	return S + " ]";

}