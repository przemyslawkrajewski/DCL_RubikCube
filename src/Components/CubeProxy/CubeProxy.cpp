/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "CubeProxy.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CubeProxy {

CubeProxy::CubeProxy(const std::string & name) :
		Base::Component(name)  {

}

CubeProxy::~CubeProxy() {
}

void CubeProxy::prepareInterface() {
	// Register data streams, events and event handlers HERE!

}

bool CubeProxy::onInit() {

	return true;
}

bool CubeProxy::onFinish() {
	return true;
}

bool CubeProxy::onStop() {
	return true;
}

bool CubeProxy::onStart() {
	return true;
}



} //: namespace CubeProxy
} //: namespace Processors
