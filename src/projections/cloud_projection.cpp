// Copyright (C) 2017  I. Bogoslavskyi, C. Stachniss, University of Bonn

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "projections/cloud_projection.h"
#include <string>
#include <vector>

#include "utils/mem_utils.h"

namespace depth_clustering {

using mem_utils::make_unique;

CloudProjection::PointContainer::PointContainer() {}

CloudProjection::CloudProjection(const ProjectionParams& params)
    : _params(params) {
  if (!_params.valid()) {
    throw std::runtime_error("_params not valid for projection.");
  }
  _data = PointMatrix(_params.cols(), PointColumn(_params.rows()));
  _depth_image =
      cv::Mat::zeros(_params.rows(), _params.cols(), cv::DataType<float>::type);
}

RichPoint CloudProjection::UnprojectPoint(const cv::Mat& image, const int row,
                                          const int col ,const cv::Mat& bin_image) const {
  float depth = image.at<float>(row, col);
  Radians angle_z = this->_params.AngleFromRow(row);
  Radians angle_xy = this->_params.AngleFromCol(col);

    if (  bin_image.at<uchar>(row,col)==0) {


        RichPoint point{depth * cosf(angle_z.val()) * cosf(angle_xy.val()),
                        depth * cosf(angle_z.val()) * sinf(angle_xy.val()),
                        depth * sinf(angle_z.val())};
         point.flag_num() = 1;//在这里将的值赋值为255
        //std::cout<<"the value of  the point.flag_num() = 255;"<<std::endl;
        return point;
    }
    else{
        //std::cout<<"the value of  the point.flag_num() = 0;"<<std::endl;
        RichPoint point{depth * cosf(angle_z.val()) * cosf(angle_xy.val()),
                        depth * cosf(angle_z.val()) * sinf(angle_xy.val()),
                        depth * sinf(angle_z.val())};
        return point;
    }

}





void CloudProjection::CheckCloudAndStorage(
    const std::vector<RichPoint>& points) {
  if (this->_data.size() < 1) {
    throw std::length_error("_data size is < 1");
  }
  if (points.empty()) {
    throw std::runtime_error("cannot fill from cloud: no points");
  }
}

void CloudProjection::CheckImageAndStorage(const cv::Mat& image) {
  if (image.type() != CV_32F) {
    throw std::runtime_error("wrong image format");
  }
  if (this->_data.size() < 1) {
    throw std::length_error("_data size is < 1");
  }
  if (this->rows() != static_cast<size_t>(image.rows) ||
      this->cols() != static_cast<size_t>(image.cols)) {
    throw std::length_error("_data dimentions do not correspond to image ones");
  }
}

const cv::Mat& CloudProjection::depth_image() const {
  return this->_depth_image;
}

cv::Mat& CloudProjection::depth_image() { return this->_depth_image; }

}  // namespace depth_clustering
