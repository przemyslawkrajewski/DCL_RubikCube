/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "ForFun.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace ForFun {

ForFun::ForFun(const std::string & name) :
		Base::Component(name)  {

}

ForFun::~ForFun() {
}

void ForFun::prepareInterface() {
	// Register data streams, events and event handlers HERE!

}

bool ForFun::onInit() {

	return true;
}

bool ForFun::onFinish() {
	return true;
}

bool ForFun::onStop() {
	return true;
}

bool ForFun::onStart() {
	return true;
}



} //: namespace ForFun
} //: namespace Processors
