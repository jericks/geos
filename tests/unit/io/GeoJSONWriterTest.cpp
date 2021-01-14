//
// Test Suite for geos::io::GeoJSONWriter

// tut
#include <tut/tut.hpp>
// geos
#include <geos/io/WKTReader.h>
#include <geos/io/GeoJSONWriter.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateArraySequence.h>
// std
#include <sstream>
#include <string>
#include <memory>

namespace tut {

//
// Test Group
//

// dummy data, not used
struct test_geojsonwriter_data {
    geos::geom::PrecisionModel pm;
    geos::geom::GeometryFactory::Ptr gf;
    geos::io::WKTReader wktreader;
    geos::io::GeoJSONWriter geojsonwriter;
    typedef std::unique_ptr<geos::geom::Geometry> GeomPtr;

    test_geojsonwriter_data()
        :
        pm(1000.0),
        gf(geos::geom::GeometryFactory::create(&pm)),
        wktreader(gf.get())
    {}

};

typedef test_group<test_geojsonwriter_data> group;
typedef group::object object;

group test_geojsonwriter_group("geos::io::GeoJSONWriter");

template<>
template<>
void object::test<1>
()
{
    GeomPtr geom(wktreader.read("POINT(-117 33)"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"Point\",\"coordinates\":[-117.0,33.0]}");
}

template<>
template<>
void object::test<2>
()
{
    GeomPtr geom(wktreader.read("LINESTRING(102.0 0.0, 103.0 1.0, 104.0 0.0, 105.0 1.0)"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"LineString\",\"coordinates\":[[102.0,0.0],[103.0,1.0],[104.0,0.0],[105.0,1.0]]}");
}

template<>
template<>
void object::test<3>
()
{
    GeomPtr geom(wktreader.read("POLYGON((30 10, 40 40, 20 40, 10 20, 30 10))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"Polygon\",\"coordinates\":[[[30.0,10.0],[40.0,40.0],[20.0,40.0],[10.0,20.0],[30.0,10.0]]]}");
}

template<>
template<>
void object::test<4>
()
{
    GeomPtr geom(wktreader.read("POLYGON((35 10, 45 45, 15 40, 10 20, 35 10), (20 30, 35 35, 30 20, 20 30))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"Polygon\",\"coordinates\":[[[35.0,10.0],[45.0,45.0],[15.0,40.0],[10.0,20.0],[35.0,10.0]],[[20.0,30.0],[35.0,35.0],[30.0,20.0],[20.0,30.0]]]}");
}

template<>
template<>
void object::test<5>
()
{
    GeomPtr geom(wktreader.read("MULTIPOINT ((10 40), (40 30), (20 20), (30 10))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"MultiPoint\",\"coordinates\":[[10.0,40.0],[40.0,30.0],[20.0,20.0],[30.0,10.0]]}");
}

template<>
template<>
void object::test<6>
()
{
    GeomPtr geom(wktreader.read("MULTILINESTRING ((10 10, 20 20, 10 40),(40 40, 30 30, 40 20, 30 10))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"MultiLineString\",\"coordinates\":[[[10.0,10.0],[20.0,20.0],[10.0,40.0]],[[40.0,40.0],[30.0,30.0],[40.0,20.0],[30.0,10.0]]]}");
}

template<>
template<>
void object::test<7>
()
{
    GeomPtr geom(wktreader.read("MULTIPOLYGON (((30 20, 45 40, 10 40, 30 20)),((15 5, 40 10, 10 20, 5 10, 15 5)))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"MultiPolygon\",\"coordinates\":[[[[30.0,20.0],[45.0,40.0],[10.0,40.0],[30.0,20.0]]],[[[15.0,5.0],[40.0,10.0],[10.0,20.0],[5.0,10.0],[15.0,5.0]]]]}");
}

template<>
template<>
void object::test<8>
()
{
    GeomPtr geom(wktreader.read("GEOMETRYCOLLECTION(POINT(1 1),POINT(2 2))"));
    std::string result = geojsonwriter.write(geom.get());
    ensure_equals(result, "{\"type\":\"GeometryCollection\",\"geometries\":[{\"type\":\"Point\",\"coordinates\":[1.0,1.0]},{\"type\":\"Point\",\"coordinates\":[2.0,2.0]}]}");
}

}
