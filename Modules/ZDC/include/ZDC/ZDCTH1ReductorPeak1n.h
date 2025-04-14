// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file   ZDCTH1ReductorPeak1n.h
/// \author Piotr Konopka and ZDC code added by Stefan Cristi Zugravel
///
#ifndef QUALITYCONTROL_TH1REDUCTOR_H
#define QUALITYCONTROL_TH1REDUCTOR_H

#include "QualityControl/ReductorTObject.h"

namespace o2::quality_control_modules::zdc
{

/// \brief A Reductor which obtains the most popular characteristics of TH1.
///
/// A Reductor which obtains the most popular characteristics of TH1.
/// It produces a branch in the format: "mean/D:stddev:entries"
class ZDCTH1ReductorPeak1n : public quality_control::postprocessing::ReductorTObject
{
 public:
 ZDCTH1ReductorPeak1n() = default;
  ~ZDCTH1ReductorPeak1n() = default;

  void* getBranchAddress() override;
  const char* getBranchLeafList() override;
  void update(TObject* obj) override;

 private:
  struct {
    Double_t mean;
    Double_t stddev;
    Double_t entries;
    Double_t position1n;
  } mStats;
};

} // namespace o2::quality_control_modules::zdc

#endif //QUALITYCONTROL_TH1REDUCTOR_H
