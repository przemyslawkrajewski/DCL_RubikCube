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



} //: namespace CubeInfo
} //: namespace Processors
