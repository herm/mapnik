/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2011 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/
//$Id$

#include <mapnik/agg_renderer.hpp>
#include <mapnik/agg_rasterizer.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/svg/svg_converter.hpp>
#include <mapnik/svg/svg_renderer.hpp>
#include <mapnik/svg/svg_path_adapter.hpp>

#include <mapnik/symbolizer_helpers.hpp>


// boost
#include <boost/make_shared.hpp>

namespace mapnik {

template <typename T>
void  agg_renderer<T>::process(shield_symbolizer const& sym,
                               Feature const& feature,
                               proj_transform const& prj_trans)
{
    shield_symbolizer_helper<face_manager<freetype_engine>,
            label_collision_detector4> helper(
                sym, feature, prj_trans,
                width_, height_,
                scale_factor_,
                t_, font_manager_, *detector_);

    text_renderer<T> ren(pixmap_, font_manager_, *(font_manager_.get_stroker()));

    text_placement_info_ptr placement;
    while ((placement = helper.get_placement())) {
        for (unsigned int ii = 0; ii < placement->placements.size(); ++ii)
        {
            std::pair<int, int> marker_pos = helper.get_marker_position(placement->placements[ii]);
            render_marker(marker_pos.first, marker_pos.second, helper.get_marker(), helper.get_transform(), sym.get_opacity());

            double x = placement->placements[ii].starting_x;
            double y = placement->placements[ii].starting_y;
            ren.prepare_glyphs(&(placement->placements[ii]));
            ren.render(x, y);
        }
    }
}


template void agg_renderer<image_32>::process(shield_symbolizer const&,
                                              Feature const&,
                                              proj_transform const&);

}
