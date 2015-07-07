/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "GetTilesColors.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace GetTilesColors {

GetTilesColors::GetTilesColors(const std::string & name) :
		Base::Component(name)  {

}

GetTilesColors::~GetTilesColors() {
}

void GetTilesColors::prepareInterface() {
	registerHandler("onNewImage", boost::bind(&GetTilesColors::onNewImage, this));
	registerHandler("onNewCubeFace", boost::bind(&GetTilesColors::onNewCubeFace, this));

	registerStream("in_img", &in_img);
	registerStream("in_cubeface", &in_cubeface);

	registerStream("out_cubeface", &out_cubeface);

	addDependency("onNewImage", &in_img);
	addDependency("onNewCubeFace", &in_cubeface);
}

bool GetTilesColors::onInit() {

	return true;
}

bool GetTilesColors::onFinish() {
	return true;
}

bool GetTilesColors::onStop() {
	return true;
}

bool GetTilesColors::onStart() {
	return true;
}

void GetTilesColors::onNewCubeFace()
{
	LOG(LTRACE) << "DrawResults::onNewCubeFace\n";
	try {
		CubeFace cubeFace = in_cubeface.read();

		out_cubeface.write(cubeFace);

	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewCubeFace failed\n";
	}
}

void GetTilesColors::onNewImage()
{
	LOG(LTRACE) << "DrawResults::onNewImage\n";
	try {
		img = in_img.read();

	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewImage failed\n";
	}
}

} //: namespace GetTilesColors
} //: namespace Processors
