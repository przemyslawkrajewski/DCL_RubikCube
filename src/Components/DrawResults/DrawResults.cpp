/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "DrawResults.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DrawResults {

DrawResults::DrawResults(const std::string & name) : Base::Component(name),
		contourColor(255,0,255)
{

}

DrawResults::~DrawResults() {
}

void DrawResults::prepareInterface() {
	CLOG(LTRACE) << "DrawResults::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&DrawResults::onNewImage, this));
	registerHandler("onNewContours", boost::bind(&DrawResults::onNewContours, this));

	registerStream("in_img", &in_img);
	registerStream("in_contours", &in_contours);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
	addDependency("onNewContours", &in_contours);
}

bool DrawResults::onInit() {

	return true;
}

bool DrawResults::onFinish() {
	return true;
}

bool DrawResults::onStop() {
	return true;
}

bool DrawResults::onStart() {
	return true;
}

void DrawResults::onNewContours()
{
	LOG(LTRACE) << "DrawResults::onNewContours\n";
	try {
		contours.clear();
		contours = in_contours.read();
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewContours failed\n";
	}
}

void DrawResults::drawContours(Mat &img)
{
	for(vector<Contour>::iterator contour=contours.begin();contour!=contours.end();contour++)
	{
		for(Contour::iterator point=contour->begin();point!=contour->end();point++)
		{
			RGB& rgb = img.ptr<RGB>(point->y)[point->x];
			rgb=contourColor;
			//line( out,contours[i][j],contours[i][j+1],Scalar( 255, 255, 255 ),2,8 );
		}
	}
}

void DrawResults::onNewImage()
{
	LOG(LTRACE) << "DrawResults::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();

		drawContours(out);

		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewImage failed\n";
	}
}

} //: namespace DrawResults
} //: namespace Processors
