/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "CubeInfo.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CubeInfo {

CubeInfo::CubeInfo(const std::string & name) :
		Base::Component(name)  {

}

CubeInfo::~CubeInfo() {
}

void CubeInfo::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerHandler("onNewCubeFace", boost::bind(&CubeInfo::onNewCubeFace, this));

	registerStream("in_cubeface", &in_cubeface);

	registerStream("out_object3d", &out_object3d);

	addDependency("onNewCubeFace", &in_cubeface);
}

bool CubeInfo::onInit() {

	return true;
}

bool CubeInfo::onFinish() {
	return true;
}

bool CubeInfo::onStop() {
	return true;
}

bool CubeInfo::onStart() {
	return true;
}

void CubeInfo::onNewCubeFace()
{
	LOG(LTRACE) << "CubeInfo::onNewCubeFace\n";
	try {
		RubikCube::CubeFace cubeFace = in_cubeface.read();

		std::vector< cv::Point2f > imagePoints;

		if(cubeFace.isFound())
		for (int i=0;i<3;i++)
			for(int j=0;j<3;j++)
			{
				imagePoints.push_back(cubeFace.getTile(i,j).getMiddle());
			}

		std::vector <cv::Point3f> modelPoints;

		for (int i=0;i<3;i++)
			for(int j=0;j<3;j++)
			{
				cv::Point3f p((i-1)*0.015,(j-1)*0.015,0);
				modelPoints.push_back(p);
				//imagePoints.push_back(cubeFace.getTile(i,j).getMiddle());
			}

		CubeModel object;
		object.setModelPoints(modelPoints);
		object.setImagePoints(imagePoints);
		out_object3d.write(object);

	} catch (...) {
		LOG(LERROR) << "CubeInfo::onNewCubeFace failed\n";
	}
}


} //: namespace CubeInfo
} //: namespace Processors
