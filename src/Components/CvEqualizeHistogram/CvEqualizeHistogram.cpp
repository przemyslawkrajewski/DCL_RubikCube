/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "CvEqualizeHistogram.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvEqualizeHistogram {

CvEqualizeHistogram::CvEqualizeHistogram(const std::string & name) :
		Base::Component(name)  {

}

CvEqualizeHistogram::~CvEqualizeHistogram() {
}

void CvEqualizeHistogram::prepareInterface() {
	CLOG(LTRACE) << "CvEqualizeHistogram::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&CvEqualizeHistogram::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);

}

bool CvEqualizeHistogram::onInit() {

	return true;
}

bool CvEqualizeHistogram::onFinish() {
	return true;
}

bool CvEqualizeHistogram::onStop() {
	return true;
}

bool CvEqualizeHistogram::onStart() {
	return true;
}

void CvEqualizeHistogram::onNewImage()
{
	LOG(LTRACE) << "CvEqualizeHistogram::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		cv::equalizeHist(img,out);
		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvEqualizeHistogram::onNewImage failed\n";
	}
}

} //: namespace CvEqualizeHistogram
} //: namespace Processors
