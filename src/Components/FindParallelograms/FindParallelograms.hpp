/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef FINDPARALLELOGRAMS_HPP_
#define FINDPARALLELOGRAMS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include "Types/Contour.hpp"
#include "Types/Parallelogram.hpp"

namespace Processors {
namespace FindParallelograms {

using namespace cv;
using namespace std;
using namespace RubikCube;

/*!
 * \class findParallelograms
 * \brief findParallelograms processor class.
 *
 * findParallelograms processor.
 */
class FindParallelograms: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FindParallelograms(const std::string & name = "findParallelograms");

	/*!
	 * Destructor
	 */
	virtual ~FindParallelograms();

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
	void onNewContours();

	/// Input data stream
	Base::DataStreamIn < vector<Contour> > in_contours;

	/// Output data stream - found contours
	Base::DataStreamOut < vector<Parallelogram> > out_parallelograms;
	

};

} //: namespace findParallelograms
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FindParallelograms", Processors::FindParallelograms::FindParallelograms)

#endif /* FINDPARALLELOGRAMS_HPP_ */
