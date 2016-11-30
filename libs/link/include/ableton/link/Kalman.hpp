/* Copyright 2016, Ableton AG, Berlin. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like to incorporate Link into a proprietary software application,
 *  please contact <link-devs@ableton.com>.
 */

#pragma once

#include <array>
#include <cfloat>
#include <cmath>
#include <limits>

#if LINK_PLATFORM_WINDOWS
// Windows.h (or more specifically, minwindef.h) define the max(a, b) macro
// which conflicts with the symbol provided by std::numeric_limits.
#ifdef max
#undef max
#endif
#endif

namespace ableton
{
namespace link
{

template <std::size_t n>
struct Kalman
{
  Kalman()
    : mValue(0)
    , mGain(0)
    , mVVariance(1)
    , mWVariance(1)
    , mCoVariance(1)
    , mVarianceLength(n)
    , mCounter(mVarianceLength)
  {
  }

  double getValue()
  {
    return mValue;
  }

  double calculateVVariance()
  {
    auto vVar = 0.;
    auto meanOfDiffs = 0.;

    for (size_t k = 0; k < (mVarianceLength); k++)
    {
      meanOfDiffs += (mMeasuredValues[k] - mFilterValues[k]);
    }

    meanOfDiffs /= (mVarianceLength);

    for (size_t i = 0; i < (mVarianceLength); i++)
    {
      vVar += (pow(mMeasuredValues[i] - mFilterValues[i] - meanOfDiffs, 2.0));
    }

    vVar /= (mVarianceLength - 1);

    return vVar;
  }

  double calculateWVariance()
  {
    auto wVar = 0.;
    auto meanOfDiffs = 0.;

    for (size_t k = 0; k < (mVarianceLength); k++)
    {
      meanOfDiffs += (mFilterValues[(mCounter - k - 1) % mVarianceLength]
                      - mFilterValues[(mCounter - k - 2) % mVarianceLength]);
    }

    meanOfDiffs /= (mVarianceLength);

    for (size_t i = 0; i < (mVarianceLength); i++)
    {
      wVar += (pow(mFilterValues[(mCounter - i - 1) % mVarianceLength]
                     - mFilterValues[(mCounter - i - 2) % mVarianceLength] - meanOfDiffs,
        2.0));
    }

    wVar /= (mVarianceLength - 1);

    return wVar;
  }

  void iterate(const double value)
  {
    const std::size_t currentIndex = mCounter % mVarianceLength;
    mMeasuredValues[currentIndex] = value;

    if (mCounter < (mVarianceLength + mVarianceLength))
    {
      if (mCounter == mVarianceLength)
      {
        mValue = value;
      }
      else
      {
        mValue = (mValue + value) / 2;
      }
    }
    else
    {
      // prediction equations
      const double prevFilterValue = mFilterValues[(mCounter - 1) % mVarianceLength];
      mFilterValues[currentIndex] = prevFilterValue;
      mWVariance = calculateWVariance();
      const double coVarianceEstimation = mCoVariance + mWVariance;

      // update equations
      mVVariance = calculateVVariance();
      if ((coVarianceEstimation + mVVariance) != 0)
      {
        mGain = coVarianceEstimation / (coVarianceEstimation + mVVariance);
      }
      else
      {
        mGain = std::numeric_limits<double>::max();
      }
      mValue = prevFilterValue + mGain * (value - prevFilterValue);
      mCoVariance = (1 - mGain) * coVarianceEstimation;
    }
    mFilterValues[currentIndex] = mValue;

    ++mCounter;
  }

  double mValue;
  double mGain;
  double mVVariance;
  double mWVariance;
  double mCoVariance;
  size_t mVarianceLength;
  size_t mCounter;
  std::array<double, n> mFilterValues;
  std::array<double, n> mMeasuredValues;
};

} // namespace link
} // namespace ableton
