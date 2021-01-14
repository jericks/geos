/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2011 Sandro Santilli <strk@kbt.io>
 * Copyright (C) 2005-2006 Refractions Research Inc.
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation.
 * See the COPYING file for more information.
 *
 **********************************************************************/

#ifndef GEOS_IO_GEOJSONWRITER_H
#define GEOS_IO_GEOJSONWRITER_H

#include <geos/export.h>

#include <string>
#include <cctype>

#include <json.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // warning C4251: needs to have dll-interface to be used by clients of class
#endif

// Forward declarations
namespace geos {
namespace geom {
class Coordinate;
class CoordinateSequence;
class Geometry;
class GeometryCollection;
class Point;
class LineString;
class LinearRing;
class Polygon;
class MultiPoint;
class MultiLineString;
class MultiPolygon;
class PrecisionModel;
}
namespace io {
class Writer;
}
}


namespace geos {
namespace io {

/**
 * \class GeoJSONWriter
 *
 * \brief Outputs the GeoJSON representation of a Geometry.
 * See also GeoJSONReader for parsing.
 *
 */
class GEOS_DLL GeoJSONWriter {
public:
    GeoJSONWriter() = default;
    ~GeoJSONWriter() = default;

    /// Returns WKT string for the given Geometry
    std::string write(const geom::Geometry* geometry);

    // Send Geometry's WKT to the given Writer
    //void write(const geom::Geometry* geometry, Writer* writer);

    //std::string writeFormatted(const geom::Geometry* geometry);

    //void writeFormatted(const geom::Geometry* geometry, Writer* writer);

private:

    std::pair<double, double> convertCoordinate(const geom::Coordinate* c);

    std::vector<std::pair<double, double>> convertCoordinateSequence(const geom::CoordinateSequence* c);

    void encodeGeometry(const geom::Geometry* g, nlohmann::ordered_json& j);

    void encodePoint(const geom::Point* p, nlohmann::ordered_json& j);

    void encodeLineString(const geom::LineString* l, nlohmann::ordered_json& j);

    void encodePolygon(const geom::Polygon* p, nlohmann::ordered_json& j);

    void encodeMultiPoint(const geom::MultiPoint* p, nlohmann::ordered_json& j);

    void encodeMultiLineString(const geom::MultiLineString* l, nlohmann::ordered_json& j);

    void encodeMultiPolygon(const geom::MultiPolygon* m, nlohmann::ordered_json& j);

    void encodeGeometryCollection(const geom::GeometryCollection* g, nlohmann::ordered_json& j);

};

} // namespace geos::io
} // namespace geos

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // #ifndef GEOS_IO_GEOJSONWRITER_H
