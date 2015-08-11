/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "FindParallelograms.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FindParallelograms {

FindParallelograms::FindParallelograms(const std::string & name) :
		Base::Component(name)  {

}

FindParallelograms::~FindParallelograms() {
}

void FindParallelograms::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	CLOG(LTRACE) << "findParallelograms::prepareInterface\n";

	registerHandler("onNewContours", boost::bind(&FindParallelograms::onNewContours, this));

	registerStream("in_contours", &in_contours);

	registerStream("out_parallelograms", &out_parallelograms);

	addDependency("onNewContours", &in_contours);
}

bool FindParallelograms::onInit() {

	return true;
}

bool FindParallelograms::onFinish() {
	return true;
}

bool FindParallelograms::onStop() {
	return true;
}

bool FindParallelograms::onStart() {
	return true;
}

void FindParallelograms::onNewContours()
{
	LOG(LTRACE) << "FindParallelograms::onNewImage\n";
	try {
		vector<Contour> contours = in_contours.read();
		vector<Parallelogram> parallelograms;

        for (vector<Contour>::iterator c = contours.begin(); c!=contours.end(); c++)
        {
        	//Calculate middles of contours
            Point contourMiddle = Point(0,0);
            int sizeOfStep = c->size() / 8;
            contourMiddle.x = (*c)[0].x + (*c)[sizeOfStep].x + (*c)[sizeOfStep*2].x + (*c)[sizeOfStep*3].x + (*c)[sizeOfStep * 4].x + (*c)[sizeOfStep * 5].x + (*c)[sizeOfStep * 6].x + (*c)[sizeOfStep * 7].x;
            contourMiddle.y = (*c)[0].y + (*c)[sizeOfStep].y + (*c)[sizeOfStep*2].y + (*c)[sizeOfStep*3].y + (*c)[sizeOfStep * 4].y + (*c)[sizeOfStep * 5].y + (*c)[sizeOfStep * 6].y + (*c)[sizeOfStep * 7].y;
            contourMiddle.x = contourMiddle.x / 8;
            contourMiddle.y = contourMiddle.y / 8;

            //Calculate distance layout
            vector<double> distanceLayout;
            for (int i = 0; i < c->size(); i++)
            {
                Point p1 = (*c)[i];
                //Point p2 = listOfPoints.get((j + 1) % listOfPoints.size());

                double r = (p1.x - contourMiddle.x) * (p1.x - contourMiddle.x) + (p1.y - contourMiddle.y) * (p1.y - contourMiddle.y);
                r = sqrt(r);

                distanceLayout.push_back(r);
            }
            Parallelogram parallelogram;
            //Wyznaczamy najdalszy punkt czyli może jeden z wierzcholkow
            double maximalDistance = 0;
            int numberOfFurthestPoint = -1;
            for (int i = 0; i < distanceLayout.size(); i++)
            {
                if (distanceLayout[i] > maximalDistance || numberOfFurthestPoint == -1)
                {
                    maximalDistance = distanceLayout[i];
                    numberOfFurthestPoint = i;
                }
            }

            parallelogram.setCorner( (*c)[(numberOfFurthestPoint) % c->size()], 0);

            //Seting found point and opposite
            parallelogram.setCorner( (*c)[(numberOfFurthestPoint + 2 * c->size() / 4) % c->size()], 2);


            double biggestArea = 0;
            int numberOfFurthestPoint2 = -1;
            for (int j = 0; j < c->size(); j++) {
                Point p1 = (*c)[j];
                Point p2 = (*c)[(j + c->size() / 2) % c->size()];
                Point p0 = (*c)[numberOfFurthestPoint];

                double a01 = atan2(p0.y - p1.y, p0.x - p1.x);
                double a02 = atan2(p0.y - p2.y, p0.x - p2.x);
                double r01 = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
                double r02 = sqrt((p0.x - p2.x) * (p0.x - p2.x) + (p0.y - p2.y) * (p0.y - p2.y));

                if (sin(a01 - a02) * r01 * r02 > biggestArea) {
                    biggestArea = sin(a01 - a02) * r01 * r02;
                    numberOfFurthestPoint2 = j;
                }
            }

            parallelogram.setCorner((*c)[(numberOfFurthestPoint2) % c->size()], 1);
            parallelogram.setCorner((*c)[(numberOfFurthestPoint2 + 2 * c->size() / 4) % c->size()], 3);

            //Sprawdzamy czy dane rownolegloboki sa rzeczywiscie rownoleglobokami
            Point p0 = parallelogram.getCorner(0);
            Point p1 = parallelogram.getCorner(1);
            Point p2 = parallelogram.getCorner(2);
            Point p3 = parallelogram.getCorner(3);
            double a01 = atan2(p0.y - p1.y, p0.x - p1.x);
            double a03 = atan2(p0.y - p3.y, p0.x - p3.x);
            double a12 = atan2(p1.y - p2.y, p1.x - p2.x);
            double a32 = atan2(p3.y - p2.y, p3.x - p2.x);
            double r01 = sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
            double r12 = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
            double r23 = sqrt((p2.x - p3.x) * (p2.x - p3.x) + (p2.y - p3.y) * (p2.y - p3.y));
            double r30 = sqrt((p3.x - p0.x) * (p3.x - p0.x) + (p3.y - p0.y) * (p3.y - p0.y));

            double distanceThreshold;
            double minLength = 40;
            double similarAngleThreshold = 10 * 3.14 / 180;
            if (r01 < r30) distanceThreshold = r01 / 5;
            else distanceThreshold = r30 / 5;


            if (abs(a01 - a32) < similarAngleThreshold && abs(a03 - a12) < similarAngleThreshold &&
            		abs(r01 - r23) < distanceThreshold && abs(r30 - r12) < distanceThreshold &&
            		r01 > minLength && r12 > minLength
                   )
            {
            	parallelograms.push_back(parallelogram);
            }
        }

		out_parallelograms.write(parallelograms);
	} catch (...) {
		LOG(LERROR) << "FindParallelograms::onNewImage failed\n";
	}
}

} //: namespace findParallelograms
} //: namespace Processors
