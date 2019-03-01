#pragma once

#include <qstring>

#include "../eigen/Dense"

// TODO this is a  horrible wrapper, hopefully just for the time being
void addStampEntry(Eigen::MatrixXd& matrix, const double value, const int row, const int column);
void addStampEntry(Eigen::VectorXd& matrix, const double value, const int row);

QString vectorToString(const Eigen::VectorXd& vector);

QString matrixToString(const Eigen::MatrixXd& matrix);