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

#include <towr/variables/spline_holder.h>

namespace towr{


SplineHolder::SplineHolder (NodeVariables::Ptr base_lin_nodes,
                            NodeVariables::Ptr base_ang_nodes,
                            const std::vector<double>& base_poly_durations,
                            std::vector<NodeVariables::Ptr> ee_motion_nodes,
                            std::vector<NodeVariables::Ptr> ee_force_nodes,
                            std::vector<ContactSchedule::Ptr> contact_schedule,
                            bool durations_change)
{
  base_linear_ = std::make_shared<Spline>(base_lin_nodes.get(), base_poly_durations);
  base_angular_ = std::make_shared<Spline>(base_ang_nodes.get(), base_poly_durations);

  for (uint ee=0; ee<ee_motion_nodes.size(); ++ee) {

    if (durations_change) {
      // spline without changing the polynomial durations
      ee_motion_.push_back(std::make_shared<Spline>(ee_motion_nodes.at(ee).get(), contact_schedule.at(ee).get()));
      ee_force_.push_back(std::make_shared<Spline>(ee_force_nodes.at(ee).get(), contact_schedule.at(ee).get()));
    } else {
      // spline that changes the polynomial durations (affects Jacobian)
      ee_motion_.push_back(std::make_shared<Spline>(ee_motion_nodes.at(ee).get(), contact_schedule.at(ee)->GetDurations()));
      ee_force_.push_back(std::make_shared<Spline>(ee_force_nodes.at(ee).get(), contact_schedule.at(ee)->GetDurations()));
    }

  }
}

} /* namespace towr */
