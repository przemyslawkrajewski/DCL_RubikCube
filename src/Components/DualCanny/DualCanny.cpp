/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "DualCanny.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DualCanny {

DualCanny::DualCanny(const std::string & name) : Base::Component(name), dilationSize("dilation.kernel.size",1), erosionSize("erosion.kernel.size",1),
												 lowerThreshold("histeresis.lowerThreshold",10),higherThreshold("histeresis.higherThreshold",20)
{
	registerProperty(dilationSize);
	dilationSize.addConstraint("0");
	registerProperty(erosionSize);
	erosionSize.addConstraint("0");

	registerProperty(lowerThreshold);
	erosionSize.addConstraint("0");
	registerProperty(higherThreshold);
	erosionSize.addConstraint("0");
}

DualCanny::~DualCanny() {
}

void DualCanny::prepareInterface() {
	CLOG(LTRACE) << "DualCanny::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&DualCanny::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
}

bool DualCanny::onInit() {

	return true;
}

bool DualCanny::onFinish() {
	return true;
}

bool DualCanny::onStop() {
	return true;
}

bool DualCanny::onStart() {
	return true;
}

void DualCanny::onNewImage()
{
	LOG(LTRACE) << "DualCanny::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();

		Canny( out, out, lowerThreshold,higherThreshold);

		Mat dilationElement = getStructuringElement( MORPH_RECT,Size(dilationSize, dilationSize),Point( (int)(dilationSize/2), (int)(dilationSize/2) ) );
		dilate( out, out, dilationElement );

		Mat erosionElement = getStructuringElement( MORPH_RECT,Size(erosionSize, erosionSize),Point( (int)(erosionSize/2), (int)(erosionSize/2) ) );
		erode( out, out, erosionElement );

		//It doesn't matter what thresholds are because this is now binary image
		Canny( out, out, lowerThreshold,higherThreshold);

		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "DualCanny	::onNewImage failed\n";
	}
}

} //: namespace DualCanny
} //: namespace Processors
