/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "CvMedianBlur.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvMedianBlur {

CvMedianBlur::CvMedianBlur(const std::string & name) : Base::Component(name), kernelSize("kernel.size", 1)
{
	registerProperty(kernelSize);
	kernelSize.addConstraint("0");
}

CvMedianBlur::~CvMedianBlur() {
}

void CvMedianBlur::prepareInterface()
{

	CLOG(LTRACE) << "CvMedianBlur::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&CvMedianBlur::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
}

bool CvMedianBlur::onInit() {

	return true;
}

bool CvMedianBlur::onFinish() {
	return true;
}

bool CvMedianBlur::onStop() {
	return true;
}

bool CvMedianBlur::onStart() {
	return true;
}

void CvMedianBlur::onNewImage()
{
	LOG(LTRACE) << "CvMedianBlur::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		medianBlur(img,out,kernelSize);
		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvMedianBlur::onNewImage failed\n";
	}
}

} //: namespace CvMedianBlur
} //: namespace Processors
