#include "kalman.h"

KalmanFilter generateKalman() {
	KalmanFilter Filter(4, 2, 0);
	Filter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	Filter.statePre.at<float>(0) = 0;        //Położenie x
	Filter.statePre.at<float>(1) = 0;        //Położenie y
	Filter.statePre.at<float>(2) = 0;        //Prędkosc x
	Filter.statePre.at<float>(3) = 0;
	setIdentity(Filter.measurementMatrix);
	setIdentity(Filter.processNoiseCov, Scalar::all(1e-3));        //Kowariancja Q
	setIdentity(Filter.measurementNoiseCov, Scalar::all(1e-2));    //Kowariancja R
	setIdentity(Filter.errorCovPost, Scalar::all(.1));

	return Filter;
}