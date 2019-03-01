#include "MatrixHelper.h"

void addStampEntry(Eigen::MatrixXd& matrix, const double value, const int row, const int column) {

	if (row >= 0 && column >= 0)
		matrix(row, column) += value;

}

void addStampEntry(Eigen::VectorXd& matrix, const double value, const int row) {

	if (row >= 0)
		matrix(row) += value;

}

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