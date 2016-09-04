/**
 @file    optimization_variables_interpreter.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Jun 3, 2016
 @brief   Brief description
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_OPTIMIZATION_VARIABLES_INTERPRETER_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_OPTIMIZATION_VARIABLES_INTERPRETER_H_

#include <xpp/hyq/foothold.h>
#include <xpp/hyq/support_polygon_container.h>
#include "com_spline.h"

namespace xpp {
namespace zmp {

/** @brief Holds the context information of the optimization variables.
  *
  * Holds information that helps to interpret, what the values in the optimization
  * variables vector represent. These are usually initial conditions.
  */
class OptimizationVariablesInterpreter {
public:
  typedef std::vector<xpp::hyq::Foothold> VecFoothold;
  typedef ComSpline::VecPolynomials VecSpline;
  typedef std::vector<xpp::hyq::LegID> VecLegID;
  typedef xpp::hyq::SupportPolygonContainer SupportPolygonContainer;
  typedef xpp::utils::StdVecEigen2d FootholdPositionsXY;
  typedef ComSpline::PtrS ComSplinePtr;
  typedef Eigen::VectorXd VectorXd;
  typedef Eigen::Vector2d Vector2d;

  OptimizationVariablesInterpreter ();
  virtual ~OptimizationVariablesInterpreter ();

  void Init(const ComSplinePtr& splines,
            const SupportPolygonContainer& support_polygon_container,
            double robot_height);

  void SetSplineCoefficients(const VectorXd& x_spline_coeff_abcd);
  void SetFootholds(const FootholdPositionsXY& x_feet);

  double GetRobotHeight() const;
  ComSplinePtr GetSplineStructure() const;
  SupportPolygonContainer GetSuppPolygonContainer() const;
  VecFoothold GetStartStance() const;
  VecFoothold GetFootholds() const;
  VecSpline GetSplines() const;

private:
  ComSplinePtr spline_structure_;
  SupportPolygonContainer supp_polygon_container_;
  double robot_height_;

  bool initialized_ = false; // checks if the Init() method has been called
};

} /* namespace zmp */
} /* namespace xpp */

#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_ZMP_OPTIMIZATION_VARIABLES_INTERPRETER_H_ */
