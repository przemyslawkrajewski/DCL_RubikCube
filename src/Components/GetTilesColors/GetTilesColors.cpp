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
	// Register data streams, events and event handlers HERE!

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



} //: namespace GetTilesColors
} //: namespace Processors
