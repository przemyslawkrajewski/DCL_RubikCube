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
	LOG(LTRACE) << "GetTilesColors::onNewCubeFace\n";
	try {
		CubeFace cubeFace = in_cubeface.read();

		int numberOfChannels=img.channels();

		if(cubeFace.isFound() && !img.empty())
		for (int i=0;i<3;i++)
			for(int j=0;j<3;j++)
			{
				int x = cubeFace.getTile(i,j).getMiddle().x;
				int y = cubeFace.getTile(i,j).getMiddle().y;
				if(x >10 && x <1286 && y>10 && y < 1022)
				{
					int b = img.ptr<uchar>(y)[numberOfChannels*x+0]+img.ptr<uchar>(y+4)[numberOfChannels*(x+4)+0]+img.ptr<uchar>(y+4)[numberOfChannels*(x-4)+0]+img.ptr<uchar>(y-4)[numberOfChannels*(x+4)+0]+img.ptr<uchar>(y-4)[numberOfChannels*(x-4)+0];
					int g = img.ptr<uchar>(y)[numberOfChannels*x+1]+img.ptr<uchar>(y+4)[numberOfChannels*(x+4)+1]+img.ptr<uchar>(y+4)[numberOfChannels*(x-4)+1]+img.ptr<uchar>(y-4)[numberOfChannels*(x+4)+1]+img.ptr<uchar>(y-4)[numberOfChannels*(x-4)+1];
					int r = img.ptr<uchar>(y)[numberOfChannels*x+2]+img.ptr<uchar>(y+4)[numberOfChannels*(x+4)+2]+img.ptr<uchar>(y+4)[numberOfChannels*(x-4)+2]+img.ptr<uchar>(y-4)[numberOfChannels*(x+4)+2]+img.ptr<uchar>(y-4)[numberOfChannels*(x-4)+2];
					b=b/5;g=g/5;r=r/5;
					cubeFace.setTileColor(i,j,cv::Scalar(b,g,r));
				}
			}
		out_cubeface.write(cubeFace);

	} catch (...) {
		LOG(LERROR) << "GetTilesColors	::onNewCubeFace failed\n";
	}
}

void GetTilesColors::onNewImage()
{
	LOG(LTRACE) << "GetTilesColors::onNewImage\n";
	try {
		img = in_img.read();

	} catch (...) {
		LOG(LERROR) << "GetTilesColors	::onNewImage failed\n";
	}
}

} //: namespace GetTilesColors
} //: namespace Processors
