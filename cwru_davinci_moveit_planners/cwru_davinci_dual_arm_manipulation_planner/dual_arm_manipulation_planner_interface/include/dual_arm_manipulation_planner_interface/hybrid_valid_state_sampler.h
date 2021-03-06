/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2018, Case Western Reserve University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of SRI International nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Su Lu <sxl924@case.edu>
 * Description: This is the derived valid state sampler inherited from ompl::base::StateValidityChecker
 */

#ifndef CWRU_DAVINCI_DUAL_ARM_MANIPULATION_PLANNER_HYBRID_VALID_STATE_SAMPLER_H
#define CWRU_DAVINCI_DUAL_ARM_MANIPULATION_PLANNER_HYBRID_VALID_STATE_SAMPLER_H

#include <dual_arm_manipulation_planner_interface/parameterization/hybrid_object_state_space.h>
#include <dual_arm_manipulation_planner_interface/hybrid_state_validity_checker.h>
#include <ompl/base/ValidStateSampler.h>
#include <ompl/base/StateSampler.h>

namespace dual_arm_manipulation_planner_interface
{
class HybridValidStateSampler : public ompl::base::ValidStateSampler
{
public:
  HybridValidStateSampler(const std::string &robot_name,
                          const ompl::base::SpaceInformation* si);

  bool sample(ompl::base::State *state) override;

  bool sampleNear(ompl::base::State *state, const ompl::base::State *near, double distance) override
  {
    throw ompl::Exception("HybridValidStateSampler::sampleNear", "not implemented");
    return false;
  }

private:
  robot_model::RobotModelConstPtr kmodel_;

  robot_model_loader::RobotModelLoader robot_model_loader_;

  std::string robot_name_;
};
}

#endif //CWRU_DAVINCI_DUAL_ARM_MANIPULATION_PLANNER_HYBRID_VALID_STATE_SAMPLER_H
