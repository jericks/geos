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

std::string GeoJSONWriter::write(const geom::Geometry* geometry, GeoJSONType type) {
    json j;
    encode(geometry, type, j);
    std::string geojson = j.dump();
    return geojson;
}

std::string GeoJSONWriter::writeFormatted(const geom::Geometry* geometry, GeoJSONType type, int indent) {
    json j;
    encode(geometry, type, j);
    std::string geojson = j.dump(indent);
    return geojson;
}

std::string GeoJSONWriter::write(const GeoJSONFeature feature) {
    json j;
    encodeFeature(feature, j);
    std::string geojson = j.dump();
    return geojson;
}

void GeoJSONWriter::encodeGeoJSONValue(std::string key, GeoJSONValue value, nlohmann::ordered_json& j) {
    if (value.type == GeoJSONValueType::NUMBER) {
        if (j.is_object()) {
            j[key] = value.numberValue;
        } else {
            j.push_back(value.numberValue);
        }
    } else if (value.type == GeoJSONValueType::STRING) {
        if (j.is_object()) {
            j[key] = value.stringValue;
        } else {
            j.push_back(value.stringValue);
        }
    } else if (value.type == GeoJSONValueType::BOOLEAN) {
        if (j.is_object()) {
            j[key] = value.booleanValue;
        } else {
            j.push_back(value.booleanValue);
        }
    } else if (value.type == GeoJSONValueType::NULLTYPE) {
        if (j.is_object()) {
            j[key] = nullptr;
        } else {
            j.push_back(nullptr);
        }
    } else if (value.type == GeoJSONValueType::ARRAY) {
        j[key] = json::array();
        for (GeoJSONValue v : value.arrayValue) {
            encodeGeoJSONValue("", v, j[key]);
        }
    } else if (value.type == GeoJSONValueType::OBJECT) {
        j[key] = json::object();
        for (auto entry : value.objectValue) {
            encodeGeoJSONValue(entry.first, entry.second, j[key]);
        }
    }
}

std::string GeoJSONWriter::write(const GeoJSONFeatureCollection features) {
    json j;
    j["type"] = "FeatureCollection";
    json featuresJson = json::array();
    for(auto const feature : features.getFeatures()) {
        json featureJson;
        encodeFeature(feature, featureJson);
        featuresJson.push_back(featureJson);
    }
    j["features"] = featuresJson;
    std::string geojson = j.dump();
    return geojson;
}

void GeoJSONWriter::encodeFeature(const GeoJSONFeature feature, nlohmann::ordered_json& j) {
    j["type"] = "Feature";
    json geometryJson;
    encodeGeometry(feature.getGeometry(), geometryJson);
    j["geometry"] = geometryJson;
    json propertiesJson;
    for(auto property : feature.getProperties()) {
        std::string key = property.first;
        GeoJSONValue value = property.second;
        encodeGeoJSONValue(key, value, propertiesJson);
    }
    j["properties"] = propertiesJson;
}

void GeoJSONWriter::encode(const geom::Geometry* geometry, GeoJSONType geojsonType, nlohmann::ordered_json& j) {
    if(geojsonType == GeoJSONType::GEOMETRY) {
        encodeGeometry(geometry, j);
    } else if (geojsonType == GeoJSONType::FEATURE) {
        encodeFeature(geometry, j);
    } else if (geojsonType == GeoJSONType::FEATURE_COLLECTION) {
        encodeFeatureCollection(geometry, j);
    }
}

void GeoJSONWriter::encodeFeature(const geom::Geometry* g, nlohmann::ordered_json& j) {
    json geometryJson;
    encodeGeometry(g, geometryJson);
    j["type"] = "Feature";
    j["geometry"] = geometryJson;
}

void GeoJSONWriter::encodeFeatureCollection(const geom::Geometry* g, nlohmann::ordered_json& j) {
    json featureJson;
    encodeFeature(g, featureJson);
    j["type"] = "FeatureCollection";
    j["features"] = std::vector<json>{featureJson};
}

void GeoJSONWriter::encodeGeometry(const geom::Geometry* geometry, nlohmann::ordered_json& j) {
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
    } else if (type == GEOS_MULTIPOINT) {
        auto multiPoint = static_cast<const geom::MultiPoint*>(geometry);
        encodeMultiPoint(multiPoint, j);
    } else if (type == GEOS_MULTILINESTRING) {
        auto multiLineString = static_cast<const geom::MultiLineString*>(geometry);
        encodeMultiLineString(multiLineString, j);
    } else if (type == GEOS_MULTIPOLYGON) {
        auto multiPolygon = static_cast<const geom::MultiPolygon*>(geometry);
        encodeMultiPolygon(multiPolygon, j);
    } else if (type == GEOS_GEOMETRYCOLLECTION) {
        auto geometryCollection = static_cast<const geom::GeometryCollection*>(geometry);
        encodeGeometryCollection(geometryCollection, j);
    }
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

void GeoJSONWriter::encodeMultiPoint(const geom::MultiPoint* multiPoint, nlohmann::ordered_json& j) {
    j["type"] = "MultiPoint";
    j["coordinates"] = convertCoordinateSequence(multiPoint->getCoordinates().get());
}

void GeoJSONWriter::encodeMultiLineString(const geom::MultiLineString* multiLineString, nlohmann::ordered_json& j) {
    j["type"] = "MultiLineString";
    std::vector<std::vector<std::pair<double, double>>> lines;
    for(size_t i = 0; i < multiLineString->getNumGeometries(); i++) {
        lines.push_back(convertCoordinateSequence(multiLineString->getGeometryN(i)->getCoordinates().get()));
    }
    j["coordinates"] = lines;
}

void GeoJSONWriter::encodeMultiPolygon(const geom::MultiPolygon* multiPolygon, nlohmann::ordered_json& json) {
    json["type"] = "MultiPolygon";
    std::vector<std::vector<std::vector<std::pair<double, double>>>> polygons;
    for(size_t i = 0; i < multiPolygon->getNumGeometries(); i++) {
        const Polygon* polygon = multiPolygon->getGeometryN(i);
        std::vector<std::vector<std::pair<double, double>>> rings;
        auto ring = polygon->getExteriorRing();
        auto coords = ring->getCoordinates();
        rings.push_back(convertCoordinateSequence(ring->getCoordinates().get()));
        for(size_t j = 0; j < polygon->getNumInteriorRing(); j++) {
            rings.push_back(convertCoordinateSequence(polygon->getInteriorRingN(i)->getCoordinates().get()));
        }
        polygons.push_back(rings);
    }
    json["coordinates"] = polygons;
}

void GeoJSONWriter::encodeGeometryCollection(const geom::GeometryCollection* g, nlohmann::ordered_json& j) {
    j["type"] = "GeometryCollection";
    auto geometryArray = j.array();
    for(size_t i = 0; i < g->getNumGeometries(); i++) {
        auto geometryObj = j.object();
        encodeGeometry(g->getGeometryN(i), geometryObj);
        geometryArray.push_back(geometryObj);
    }
    j["geometries"] = geometryArray;
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