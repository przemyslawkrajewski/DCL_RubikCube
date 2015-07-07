/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef CUBEPROXY_HPP_
#define CUBEPROXY_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"



namespace Processors {
namespace CubeProxy {

/*!
 * \class CubeProxy
 * \brief CubeProxy processor class.
 *
 * CubeProxy processor.
 */
class CubeProxy: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CubeProxy(const std::string & name = "CubeProxy");

	/*!
	 * Destructor
	 */
	virtual ~CubeProxy();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	

};

} //: namespace CubeProxy
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CubeProxy", Processors::CubeProxy::CubeProxy)

#endif /* CUBEPROXY_HPP_ */
