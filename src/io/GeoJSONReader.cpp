/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2005-2006 Refractions Research Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************/

#include <geos/io/GeoJSONReader.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/MultiPoint.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/PrecisionModel.h>

#include <algorithm>
#include <ostream>
#include <sstream>
#include <cassert>

#include <json.hpp>

#undef DEBUG_GEOJSON_READER

using namespace geos::geom;
using json = nlohmann::json;

namespace geos {
namespace io { // geos.io

GeoJSONReader::GeoJSONReader(): GeoJSONReader(*(GeometryFactory::getDefaultInstance())) {}

GeoJSONReader::GeoJSONReader(const geom::GeometryFactory& gf) : geometryFactory(gf) {}

std::unique_ptr<geom::Geometry> GeoJSONReader::read(const std::string& geoJsonText) {
    json j = json::parse(geoJsonText);
    std::string type = j["type"];
    if (type == "Point") {
        return readPoint(j);
    } else if (type == "LineString") {
        return readLineString(j);
    }
    return std::unique_ptr<geom::Geometry>(geometryFactory.createEmptyGeometry());
}

std::unique_ptr<geom::Point> GeoJSONReader::readPoint(nlohmann::json& j) {
    std::vector<double> coords = j["coordinates"].get<std::vector<double>>();
    geom::Coordinate coord = {coords[0], coords[1]};
    return std::unique_ptr<geom::Point>(geometryFactory.createPoint(coord));
}

std::unique_ptr<geom::LineString> GeoJSONReader::readLineString(nlohmann::json& j) {
    std::vector<std::pair<double,double>> coords = j["coordinates"].get<std::vector<std::pair<double,double>>>();
    std::vector<geom::Coordinate> coordinates;
    for(int i = 0; i < coords.size(); i++) {
        coordinates.push_back(geom::Coordinate{coords[i].first, coords[i].second});
    }
    geom::CoordinateArraySequence coordinateSequence { std::move(coordinates) };
    return std::unique_ptr<geom::LineString>(geometryFactory.createLineString(coordinateSequence));
}

} // namespace geos.io
} // namespace geos