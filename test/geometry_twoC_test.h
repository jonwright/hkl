#ifndef _GEOMETRY_TWOC_TEST_H
#define _GEOMETRY_TWOC_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "geometry_twoC.h"
#include "crystal.h"

using namespace hkl;

class GeometryTwoCTest : public CppUnit::TestFixture  {
    CPPUNIT_TEST_SUITE( GeometryTwoCTest );
    CPPUNIT_TEST( equal );
    CPPUNIT_TEST( copyConstructor );
    CPPUNIT_TEST( otherConstructors );
    CPPUNIT_TEST( getAxesNames );
    CPPUNIT_TEST( getSampleQuaternion );
    CPPUNIT_TEST( getSampleRotationMatrix );
    CPPUNIT_TEST( getQ );
    CPPUNIT_TEST( getDistance );
    CPPUNIT_TEST( computeHKL );
    CPPUNIT_TEST( setFromGeometry );
    CPPUNIT_TEST( persistanceIO );

    CPPUNIT_TEST_SUITE_END();

    geometry::twoC::Vertical m_geometry;
    Crystal<geometry::twoC::Vertical> m_crystal;

  public:

    void setUp(void);
    void tearDown(void);

    void equal(void);
    void copyConstructor(void);
    void otherConstructors(void);
    void getAxesNames(void);
    void addSampleDetectorAxe(void);
    void operateurs(void);
    void getSampleQuaternion(void);
    void getSampleRotationMatrix(void);
    void getQ(void);
    void getDistance(void);
    void computeHKL(void);
    void setFromGeometry(void);
    void persistanceIO(void);
};

#endif /* _GEOMETRY_TWOC_TEST_H */