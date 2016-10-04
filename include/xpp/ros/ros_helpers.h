/*
 * ros_helpers.h
 *
 *  Created on: Apr 8, 2016
 *      Author: winklera
 */

#ifndef USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_UTILS_ROS_HELPERS_H_
#define USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_UTILS_ROS_HELPERS_H_

#include <ros/ros.h>

#include <xpp_opt/PhaseInfo.h>
#include <xpp_opt/Contact.h>
#include <xpp_opt/Spline.h>

#include <xpp/zmp/phase_info.h>
#include <xpp/utils/polynomial_helpers.h>

#include <xpp_msgs/ros_helpers.h>

namespace cmo {
namespace ros {

/**
 * Ros specific functions that only depend on utils folder, ros messages,
 * ros services.
 */
struct RosHelpers {

using VecComPoly   = std::vector<xpp::utils::ComPolynomial>;
using SplineMsg    = xpp_opt::Spline;

using ContactXpp   = xpp::zmp::Contact;
using PhaseInfoXpp = xpp::zmp::PhaseInfo;

using ContactMsg   = xpp_opt::Contact;
using PhaseInfoMsg = xpp_opt::PhaseInfo;

static ContactMsg
XppToRos(const ContactXpp& xpp)
{
  ContactMsg msg;
  msg.id = xpp.id;
  msg.ee = static_cast<int>(xpp.ee);

  return msg;
}

static ContactXpp
RosToXpp(const ContactMsg& msg)
{
  ContactXpp xpp;
  xpp.id = msg.id;
  xpp.ee = static_cast<xpp::zmp::EndeffectorID>(msg.ee);

  return xpp;
}

static PhaseInfoMsg
XppToRos(const PhaseInfoXpp& xpp)
{
  PhaseInfoMsg msg;
  msg.n_completed_steps = xpp.n_completed_steps_;
  for (auto c : xpp.free_contacts_)  msg.free_contacts.push_back(XppToRos(c));
  for (auto f : xpp.fixed_contacts_) msg.fixed_contacts.push_back(xpp::ros::RosHelpers::XppToRos(f));
  msg.id                = xpp.id_;
  msg.duration          = xpp.duration_;

  return msg;
}

static PhaseInfoXpp
RosToXpp(const PhaseInfoMsg& msg)
{
  PhaseInfoXpp xpp;
  xpp.n_completed_steps_ = msg.n_completed_steps;
  for (auto c : msg.free_contacts)  xpp.free_contacts_.push_back(RosToXpp(c));
  for (auto f : msg.fixed_contacts) xpp.fixed_contacts_.push_back(xpp::ros::RosHelpers::RosToXpp(f));
  xpp.id_                = msg.id;
  xpp.duration_          = msg.duration;

  return xpp;
}

static std::vector<PhaseInfoMsg>
XppToRos(const std::vector<PhaseInfoXpp>& xpp)
{
  std::vector<PhaseInfoMsg> msg;

  for (const auto& phase : xpp)
    msg.push_back(XppToRos(phase));

  return msg;
}

static std::vector<PhaseInfoXpp>
RosToXpp(const std::vector<PhaseInfoMsg>& msg)
{
  std::vector<PhaseInfoXpp> xpp;

  for (auto phase : msg)
    xpp.push_back(RosToXpp(phase));

  return xpp;
}

static std::vector<SplineMsg>
XppToRos(const VecComPoly& opt_splines)
{
  using namespace xpp::zmp;

  int n_splines = opt_splines.size();
  std::vector<SplineMsg> msgs(n_splines);

  for (uint i=0; i<opt_splines.size(); ++i)
  {

    for (auto coeff : xpp::utils::AllSplineCoeff) {
      msgs.at(i).coeff_x[coeff] = opt_splines.at(i).GetCoefficient(xpp::utils::X,coeff);
      msgs.at(i).coeff_y[coeff] = opt_splines.at(i).GetCoefficient(xpp::utils::Y,coeff);
    }

    msgs.at(i).duration = opt_splines.at(i).GetDuration();
    msgs.at(i).id       = opt_splines.at(i).GetId();
  }

  return msgs;
}

static VecComPoly
RosToXpp(const std::vector<SplineMsg>& msgs)
{
  using namespace xpp::zmp;

  uint n_splines = msgs.size();
  VecComPoly xpp(n_splines);

  for (uint i=0; i<n_splines; ++i)
  {
    for (auto coeff : xpp::utils::AllSplineCoeff) {
      xpp.at(i).SetCoefficients(xpp::utils::X, coeff, msgs.at(i).coeff_x[coeff]);
      xpp.at(i).SetCoefficients(xpp::utils::Y, coeff, msgs.at(i).coeff_y[coeff]);
    }
    xpp.at(i).SetDuration(msgs.at(i).duration);
    xpp.at(i).SetId(msgs.at(i).id);
  }
  return xpp;
}

}; // RosHelpers

} // namespace ros
} // namespace cmo

#endif /* USER_TASK_DEPENDS_XPP_OPT_INCLUDE_XPP_UTILS_ROS_HELPERS_H_ */
