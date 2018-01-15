/******************************************************************************
Copyright (c) 2017, Alexander W. Winkler, ETH Zurich. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of ETH ZURICH nor the names of its contributors may be
      used to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ETH ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef TOWR_TOWR_H_
#define TOWR_TOWR_H_

#include <vector>

#include <Eigen/Dense>

#include <ifopt/problem.h>

#include <towr/variables/spline_holder.h>
#include <towr/models/robot_model.h>

#include "height_map.h"
#include "optimization_parameters.h"


namespace towr {


/**
 * @brief TOWR - Trajectory Optimizer for Walking Robots.
 *
 * Interface to the motion optimization framework. The user sets the initial
 * state and the desired motion parameters, then and NLP is constructed and
 * solved with the chosen solver and finally the solution splines can be
 * retrieved.
 */
class TOWR {
public:
  using FeetPos = std::vector<Eigen::Vector3d>;

  TOWR () = default;
  virtual ~TOWR () = default;

  /**
   * @brief The current state of the robot where the optimization starts from.
   *
   * @param base  The linear and angular position and velocity of the 6D- base.
   * @param feet  The current position of the end-effectors.
   */
  void SetInitialState(const BaseState& base, const FeetPos& feet);

  /**
   * @brief  The parameters that determine the type of motion produced.
   *
   * @param final_base  The desired final position and velocity of the base.
   * @param params      The parameters defining the optimization problem.
   * @param model       The kinematic and dynamic model of the system.
   * @param terrain     The height map of the terrain to walk over.
   */
  void SetParameters(const BaseState& final_base,
                     const OptimizationParameters& params,
                     const RobotModel& model,
                     HeightMap::Ptr terrain);

  enum Solver { Ipopt, Snopt };
  /**
   * @brief Constructs the problem and solves it with the chosen solver.
   * @param solver  Any solver implemented in ifopt can be used to solve the problem.
   */
  void SolveNLP(Solver solver);

  /**
   * @returns the optimized motion for base, feet and force as splines.
   *
   * The can then be queried at specific times to get the positions, velocities,
   * or forces.
   */
  SplineHolder GetSolution() const;

  /**
   * @brief Sets the solution to a previous iteration of solver.
   * @param solver_iteration  The iteration to be inspected.
   *
   * This can be helpful when trying to understand how the NLP solver reached
   * a particular solution. The initialization of the NLP can also be inspected
   * by setting the iteration to zero.
   */
  void SetSolution(int solver_iteration);

  /**
   * @returns The number of iterations the solver took to find the solution.
   */
  int GetIterationCount() const;

private:
  /**
   * @brief The solver independent optimization problem formulation.
   *
   * This object holds ownership of the optimization variables, so must
   * exist to query the spline_holder values.
   */
  ifopt::Problem nlp_;

  /**
   * @brief Holds the splines constructed from pointer to the current variables.
   *
   * These variables must exist in memory for the spline_holder to be valid.
   */
  SplineHolder spline_holder_;

  // initial state
  FeetPos foot_pos_;
  BaseState initial_base_;

  // motion parameters
  RobotModel model_;
  HeightMap::Ptr terrain_;
  OptimizationParameters params_;
  BaseState final_base_;

  /**
   * @returns the solver independent optimization problem.
   *
   * @param[in/out] splines  Links object to the current optimization variables.
   */
  ifopt::Problem BuildNLP(SplineHolder& splines) const;
};

} /* namespace towr */

#endif /* TOWR_TOWR_H_ */
