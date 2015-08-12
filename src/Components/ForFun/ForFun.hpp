/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef FORFUN_HPP_
#define FORFUN_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"



namespace Processors {
namespace ForFun {

/*!
 * \class ForFun
 * \brief ForFun processor class.
 *
 * ForFun processor.
 */
class ForFun: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ForFun(const std::string & name = "ForFun");

	/*!
	 * Destructor
	 */
	virtual ~ForFun();

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

} //: namespace ForFun
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ForFun", Processors::ForFun::ForFun)

#endif /* FORFUN_HPP_ */
