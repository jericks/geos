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

#include <geos/io/GeoJSONWriter.h>
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

#undef DEBUG_GEOJSON_WRITER

using namespace geos::geom;
using json = nlohmann::ordered_json;

namespace geos {
namespace io { // geos.io

std::string GeoJSONWriter::write(const geom::Geometry* geometry) {
    json j;
    auto type = geometry->getGeometryTypeId();
    if (type == GEOS_POINT) {
        auto point = static_cast<const geom::Point*>(geometry);
        encodePoint(point, j);
    } else if (type == GEOS_LINESTRING) {
        auto line = static_cast<const geom::LineString*>(geometry);
        encodeLineString(line, j);
    } else if (type == GEOS_POLYGON) {
        auto poly = static_cast<const geom::Polygon*>(geometry);
        encodePolygon(poly, j);
    }
    std::string geojson = j.dump();
    return geojson;
}

void GeoJSONWriter::encodePoint(const geom::Point* point, nlohmann::ordered_json& j) {
    j["type"] = "Point";
    j["coordinates"] = convertCoordinate(point->getCoordinate());
}

void GeoJSONWriter::encodeLineString(const geom::LineString* line, nlohmann::ordered_json& j) {
    j["type"] = "LineString";
    j["coordinates"] = convertCoordinateSequence(line->getCoordinates().get());
}

void GeoJSONWriter::encodePolygon(const geom::Polygon* poly, nlohmann::ordered_json& j) {
    j["type"] = "Polygon";
    std::vector<std::vector<std::pair<double, double>>> rings;
    auto ring = poly->getExteriorRing();
    auto coords = ring->getCoordinates();
    rings.push_back(convertCoordinateSequence(ring->getCoordinates().get()));
    for(size_t i = 0; i < poly->getNumInteriorRing(); i++) {
        rings.push_back(convertCoordinateSequence(poly->getInteriorRingN(i)->getCoordinates().get()));
    }
    j["coordinates"] = rings;
}

std::pair<double, double> GeoJSONWriter::convertCoordinate(const Coordinate* c) {
    return std::make_pair(c->x,c->y);
}

std::vector<std::pair<double, double>> GeoJSONWriter::convertCoordinateSequence(const CoordinateSequence* coordinateSequence) {
    std::vector<std::pair<double, double>> coordinates;
    for(size_t i = 0; i<coordinateSequence->size(); i++) {
        const geom::Coordinate c = coordinateSequence->getAt(i);
        coordinates.push_back(convertCoordinate(&c));
    }
    return coordinates;
}

} // namespace geos.io
} // namespace geos