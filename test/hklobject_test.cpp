#include "hklobject_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION( HKLObjectTest );

void
HKLObjectTest::setUp(void)
{}

void 
HKLObjectTest::tearDown(void) 
{}

void
HKLObjectTest::constructors(void)
{
  CPPUNIT_ASSERT_THROW(HKLObject("", ""), HKLException);
  CPPUNIT_ASSERT_THROW(HKLObject("titi", ""), HKLException);
  CPPUNIT_ASSERT_NO_THROW(HKLObject("titi", "toto"));

  // 1st constructor
  HKLObject hklObject("titi", "toto");
  CPPUNIT_ASSERT_EQUAL(MyString("titi"), hklObject.get_name());
  CPPUNIT_ASSERT_EQUAL(MyString("toto"), hklObject.get_description());

  // copy constructor
  HKLObject hklObject1(hklObject);

  CPPUNIT_ASSERT_EQUAL(hklObject, hklObject1);
}

void
HKLObjectTest::persistanceIO(void)
{
  HKLObject hklObject_ref("toto","titi");
  HKLObject hklObject1_ref("titi","toto");
  
  HKLObject hklObject("tutu","tata");
  HKLObject hklObject1("tata","tutu");
  
  stringstream flux;
  hklObject_ref.toStream(flux);
  hklObject1_ref.toStream(flux);
  hklObject.fromStream(flux);
  hklObject1.fromStream(flux);
  
  CPPUNIT_ASSERT_EQUAL(hklObject_ref, hklObject);
  CPPUNIT_ASSERT_EQUAL(hklObject1_ref, hklObject1);
}