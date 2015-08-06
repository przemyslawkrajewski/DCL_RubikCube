/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef CUBEINFO_HPP_
#define CUBEINFO_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include "Types/Objects3D/Object3D.hpp"
#include "Types/HomogMatrix.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "Types/Parallelogram.hpp"
#include "Types/CubeFace.hpp"
#include "Types/CubeModel.hpp"

using namespace RubikCube;

namespace Processors {
namespace CubeInfo {

/*!
 * \class CubeInfo
 * \brief CubeInfo processor class.
 *
 * CubeInfo processor.
 */
class CubeInfo: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CubeInfo(const std::string & name = "CubeInfo");

	/*!
	 * Destructor
	 */
	virtual ~CubeInfo();

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

	void onNewCubeFace();


	Base::DataStreamIn < CubeFace , Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_cubeface;
	Base::DataStreamOut <Types::Objects3D::Object3D> out_object3d;
	

};

} //: namespace CubeInfo
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CubeInfo", Processors::CubeInfo::CubeInfo)

#endif /* CUBEINFO_HPP_ */
