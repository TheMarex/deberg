#include "../line_reader.hpp"

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

BOOST_AUTO_TEST_SUITE_END()
