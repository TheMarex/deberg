#include "../line_reader.hpp"
#include "../point_reader.hpp"
#include "../gml_check.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_SUITE(reader_tests)

BOOST_AUTO_TEST_CASE(line_reader_test)
{
    std::string line_data = "13:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">-7984749.593824852,5368675.690126911 -7984392.035620423,5367570.791302501</gml:coordinates></gml:LineString>\n";
    std::stringstream test_stream;
    test_stream << line_data;
    line_reader reader(test_stream);

    std::vector<line> result = reader.read();
    BOOST_CHECK(result.size() == 1);
    BOOST_CHECK(result[0].id == 13);
    BOOST_CHECK(result[0].coordinates.size() == 2);
    BOOST_CHECK_LE(std::abs(result[0].coordinates[0].x - (-7984749.593824852)), 0.001);
    BOOST_CHECK_LE(std::abs(result[0].coordinates[0].y - 5368675.690126911), 0.001);
}

template<typename reader_t>
void test_reader(const char* line)
{
    std::stringstream test_stream;
    reader_t reader(test_stream);
    test_stream << line;
    BOOST_CHECK_THROW(reader.read(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(line_reader_invalid)
{
    test_reader<line_reader>("13:<foo></foo>");
    test_reader<line_reader>("13:<foo><bla></bla></foo>");
    test_reader<line_reader>("13:<foo><bla decimal=\",\" cs=\".\" ts=\" \"></bla></foo>");
    test_reader<line_reader>("13:<gml:LineString><bla decimal=\",\" cs=\".\" ts=\" \"></bla></gml:LineString>");
    test_reader<line_reader>("13:<gml:LineString><gml:coordinates decimal=\",\" cs=\".\" ts=\" \"></gml:coordinates></gml:LineString>");
}

BOOST_AUTO_TEST_CASE(point_reader_invalid)
{
    test_reader<line_reader>("13:<foo></foo>");
    test_reader<line_reader>("13:<foo><bla></bla></foo>");
    test_reader<line_reader>("13:<foo><bla decimal=\",\" cs=\".\" ts=\" \"></bla></foo>");
    test_reader<line_reader>("13:<gml:Point><bla decimal=\",\" cs=\".\" ts=\" \"></bla></gml:Point>");
    test_reader<line_reader>("13:<gml:Point><gml:coordinates decimal=\",\" cs=\".\" ts=\" \"></gml:coordinates></gml:Point>");
}

BOOST_AUTO_TEST_CASE(point_reader_test)
{
    std::string point_data = "3:<gml:Point srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">-7975939.10140652,5375174.29896494 </gml:coordinates></gml:Point>";
    std::stringstream test_stream;
    test_stream << point_data;
    point_reader reader(test_stream);

    std::vector<point> result = reader.read();
    BOOST_CHECK(result.size() == 1);
    BOOST_CHECK(result[0].id == 3);
    BOOST_CHECK_LE(std::abs(result[0].location.x - (-7975939.10140652)), 0.001);
    BOOST_CHECK_LE(std::abs(result[0].location.y - 5375174.29896494), 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
