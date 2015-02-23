/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "FindContours.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FindContours {

FindContours::FindContours(const std::string & name) :
		Base::Component(name)  {

}

FindContours::~FindContours() {
}

void FindContours::prepareInterface() {
	CLOG(LTRACE) << "DualCanny::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&FindContours::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_contours", &out_contours);

	addDependency("onNewImage", &in_img);
}

bool FindContours::onInit() {

	return true;
}

bool FindContours::onFinish() {
	return true;
}

bool FindContours::onStop() {
	return true;
}

bool FindContours::onStart() {
	return true;
}

void FindContours::onNewImage()
{
	LOG(LTRACE) << "FindContours::onNewImage\n";
	try {
		cv::Mat img = (in_img.read()).clone();
		vector<Contour> contours;

		findContours(img, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		out_contours.write(contours);
	} catch (...) {
		LOG(LERROR) << "FindContours::onNewImage failed\n";
	}
}

} //: namespace FindContours
} //: namespace Processors
