/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef FINDCUBEFACE_HPP_
#define FINDCUBEFACE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Types/Parallelogram.hpp"
#include "Types/CubeFace.hpp"

using namespace std;
using namespace cv;
using namespace RubikCube;

namespace Processors {
namespace FindCubeFace {

/*!
 * \class FindCubeFace
 * \brief FindCubeFace processor class.
 *
 * FindCubeFace processor.
 */
class FindCubeFace: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FindCubeFace(const std::string & name = "FindCubeFace");

	/*!
	 * Destructor
	 */
	virtual ~FindCubeFace();

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

	/*!
	 * Event handler function.
	 */
	void onNewParallelograms();

	vector< vector<Parallelogram> > groupParallelograms( vector<Parallelogram> parallelograms);
	vector<CubeFace> determineCubeFaces( vector< vector<Parallelogram> > groupOfParallelograms );
	vector<CubeFace> fillNullTiles(vector<CubeFace> cubeFaces);

	double measureDistance(Point p1, Point p2);
	double measureAngle(Point p1, Point p2);

	Base::DataStreamIn < vector<Parallelogram> > in_parallelograms;
	Base::DataStreamOut < CubeFace> out_cubeface;
	
	double angleThresholdGroupSquares;
	double angleThresholdInLine;


};

} //: namespace FindCubeFace
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FindCubeFace", Processors::FindCubeFace::FindCubeFace)

#endif /* FINDCUBEFACE_HPP_ */
