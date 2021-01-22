//
// Test Suite for geos::io::GeoJSONReader

// tut
#include <tut/tut.hpp>
// geos
#include <geos/io/GeoJSONReader.h>
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
struct test_geojsonreader_data {
    geos::geom::PrecisionModel pm;
    geos::geom::GeometryFactory::Ptr gf;
    geos::io::GeoJSONReader geojsonreader;
    typedef std::unique_ptr<geos::geom::Geometry> GeomPtr;

    test_geojsonreader_data()
        :
        pm(1000.0),
        gf(geos::geom::GeometryFactory::create(&pm)),
        geojsonreader(*(gf.get()))
    {}

};

typedef test_group<test_geojsonreader_data> group;
typedef group::object object;

group test_geojsonreader_group("geos::io::GeoJSONReader");

template<>
template<>
void object::test<1>
()
{
    std::string geojson { "{\"type\":\"Point\",\"coordinates\":[-117.0,33.0]}" };
    GeomPtr geom(geojsonreader.read(geojson));
    ensure_equals("POINT (-117.000 33.000)", geom->toText());
}

template<>
template<>
void object::test<2>
()
{
    std::string geojson { "{\"type\":\"LineString\",\"coordinates\":[[102.0,0.0],[103.0,1.0],[104.0,0.0],[105.0,1.0]]}" };
    GeomPtr geom(geojsonreader.read(geojson));
    ensure_equals("LINESTRING (102.000 0.000, 103.000 1.000, 104.000 0.000, 105.000 1.000)", geom->toText());
}

template<>
template<>
void object::test<3>
()
{
    std::string geojson { "{\"type\":\"Polygon\",\"coordinates\":[[[30,10],[40,40],[20,40],[10,20],[30,10]]]}" };
    GeomPtr geom(geojsonreader.read(geojson));
    ensure_equals("POLYGON ((30.000 10.000, 40.000 40.000, 20.000 40.000, 10.000 20.000, 30.000 10.000))", geom->toText());
}

template<>
template<>
void object::test<4>
()
{
    std::string geojson { "{\"type\":\"Polygon\",\"coordinates\":[[[35,10],[45,45],[15,40],[10,20],[35,10]],[[20,30],[35,35],[30,20],[20,30]]]}" };
    GeomPtr geom(geojsonreader.read(geojson));
    ensure_equals("POLYGON ((35.000 10.000, 45.000 45.000, 15.000 40.000, 10.000 20.000, 35.000 10.000), (20.000 30.000, 35.000 35.000, 30.000 20.000, 20.000 30.000))", geom->toText());
}

}
