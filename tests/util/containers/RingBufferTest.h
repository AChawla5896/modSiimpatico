#ifndef RING_BUFFER_TEST_H
#define RING_BUFFER_TEST_H

#include <test/UnitTest.h>
#include <test/UnitTestRunner.h>

#include <util/containers/RingBuffer.h>
#include <util/archives/MemoryOArchive.h>
#include <util/archives/MemoryIArchive.h>
#include <util/archives/MemoryCounter.h>

using namespace Util;

class RingBufferTest : public UnitTest 
{
private:

   const static int capacity = 3;

   typedef std::complex<double> Data;

   RingBuffer<Data>  v;


public:

   void setUp() {}
   void tearDown() {}
  
   void testConstructor();
   void testAllocate();
   void testSubscript1();
   void testSubscript2();
   void testPack1();
   void testPack2();
   void testSerialize1();
   void testSerialize2();

};


void RingBufferTest::testConstructor()
{
   printMethod(TEST_FUNC);
   TEST_ASSERT(v.capacity() == 0);
   TEST_ASSERT(v.size()     == 0);
} 

void RingBufferTest::testAllocate()
{
   printMethod(TEST_FUNC);
   v.allocate(3);
   TEST_ASSERT(v.capacity() == 3);
   TEST_ASSERT(v.size()     == 0);
} 

void RingBufferTest::testSubscript1()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1), d(40,40.1), e(50,50.1), f(60,60.1);

   v.allocate(3);
   v.append(a);
   v.append(b);
   TEST_ASSERT(imag(v[0]) == 20.1);
   TEST_ASSERT(real(v[1]) == 10);
} 

void RingBufferTest::testSubscript2()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1), d(40,40.1), e(50,50.1), f(60,60.1);
   
   v.allocate(3);
   v.append(a);
   v.append(b);
   TEST_ASSERT(real(v[0]) == 20);
   TEST_ASSERT(imag(v[1]) == 10.1);
   TEST_ASSERT(!v.isFull());
   TEST_ASSERT(v.size() == 2);
   v.append(c);
   TEST_ASSERT(real(v[0]) == 30);
   TEST_ASSERT(imag(v[1]) == 20.1);
   TEST_ASSERT(real(v[2]) == 10);
   TEST_ASSERT(v.isFull());
   TEST_ASSERT(v.size() == 3);
   v.append(d);
   TEST_ASSERT(real(v[0]) == 40);
   TEST_ASSERT(imag(v[1]) == 30.1);
   TEST_ASSERT(real(v[2]) == 20);
   TEST_ASSERT(v.isFull());
   TEST_ASSERT(v.size() == 3);
} 

#if 0
void RingBufferTest::testPack1()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1), d(40,40.1), e(50,50.1), f(60,60.1);
   
   v.allocate(3);
   v.append(a);
   v.append(b);
   int size = v.packedSize();
   
   char* begin;
   char* current;
   char* end;
   begin = new char[size + 12];
   end = begin + size + 12;
   current = begin;
   
   v.pack(current, end);
   TEST_ASSERT(current == begin + size);
   TEST_ASSERT(real(v[0]) == 20.0);
   TEST_ASSERT(imag(v[0]) == 20.1);
   TEST_ASSERT(real(v[1]) == 10.0);
   TEST_ASSERT(imag(v[1]) == 10.1);
   TEST_ASSERT(v.capacity() == 3);
   TEST_ASSERT(v.size() == 2);
  
   RingBuffer< Data > u;
   u.allocate(3); 
   u.unpack(begin, end);
   TEST_ASSERT(real(u[0]) == 20.0);
   TEST_ASSERT(imag(u[0]) == 20.1);
   TEST_ASSERT(real(u[1]) == 10.0);
   TEST_ASSERT(imag(u[1]) == 10.1);
   TEST_ASSERT(u.capacity() == 3);
   TEST_ASSERT(u.size() == 2);

}

void RingBufferTest::testPack2()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1), d(40,40.1), e(50,50.1), f(60,60.1);
   
   v.allocate(3);
   v.append(a);
   v.append(b);
   int size = v.packedSize();
  
   PackedData buffer;
   buffer.allocate(size + 12); 
  
   buffer.beginPacking(); 
   v.pack(buffer);
   TEST_ASSERT(buffer.cursor() == buffer.begin() + size);

   TEST_ASSERT(real(v[0]) == 20.0);
   TEST_ASSERT(imag(v[0]) == 20.1);
   TEST_ASSERT(real(v[1]) == 10.0);
   TEST_ASSERT(imag(v[1]) == 10.1);
   TEST_ASSERT(v.capacity() == 3);
   TEST_ASSERT(v.size() == 2);
  
   RingBuffer< Data > u;
   u.allocate(3); 

   buffer.beginUnpacking();
   u.unpack(buffer);
   TEST_ASSERT(real(u[0]) == 20.0);
   TEST_ASSERT(imag(u[0]) == 20.1);
   TEST_ASSERT(real(u[1]) == 10.0);
   TEST_ASSERT(imag(u[1]) == 10.1);
   TEST_ASSERT(u.capacity() == 3);
   TEST_ASSERT(u.size() == 2);
}
#endif

void RingBufferTest::testSerialize1()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1);
   Data  d(40,40.1), e(50,50.1), f(60,60.1);
  
   // Fill RingBuffer v
   v.allocate(5);
   v.append(a);
   v.append(b);
   v.append(d);
   TEST_ASSERT(v.capacity() == 5);
   TEST_ASSERT(v.size() == 3);
  
   MemoryOArchive oArchive;
   int size = memorySize(v);
   oArchive.allocate(size + 2);

   // Save to archive
   oArchive << v;

   TEST_ASSERT(oArchive.cursor() == oArchive.begin() + size);

   // Show that v is unchanged
   TEST_ASSERT(real(v[0]) == 40.0);
   TEST_ASSERT(imag(v[0]) == 40.1);
   TEST_ASSERT(real(v[1]) == 20.0);
   TEST_ASSERT(imag(v[1]) == 20.1);
   TEST_ASSERT(real(v[2]) == 10.0);
   TEST_ASSERT(imag(v[2]) == 10.1);
   TEST_ASSERT(v.capacity() == 5);
   TEST_ASSERT(v.size() == 3);
  
   RingBuffer< Data > u;
   u.allocate(5); 

   // Load from archive
   MemoryIArchive iArchive;
   iArchive = oArchive;
   iArchive >> u;
 
   // Test object loaded from iArchive
   TEST_ASSERT(real(u[0]) == 40.0);
   TEST_ASSERT(imag(u[0]) == 40.1);
   TEST_ASSERT(real(u[1]) == 20.0);
   TEST_ASSERT(imag(u[1]) == 20.1);
   TEST_ASSERT(real(u[2]) == 10.0);
   TEST_ASSERT(imag(u[2]) == 10.1);
   TEST_ASSERT(u.capacity() == 5);
   TEST_ASSERT(u.size() == 3);

}

void RingBufferTest::testSerialize2()
{
   printMethod(TEST_FUNC);
   Data a(10, 10.1), b(20,20.1), c(30,30.1);
   Data  d(40,40.1), e(50,50.1), f(60,60.1);
  
   // Fill RingBuffer v
   v.allocate(5);
   v.append(a);
   v.append(b);
   v.append(d);
   TEST_ASSERT(v.capacity() == 5);
   TEST_ASSERT(v.size() == 3);
  
   MemoryOArchive oArchive;
   int size = memorySize(v);
   oArchive.allocate(size + 2);

   // Save to archive
   oArchive << v;

   TEST_ASSERT(oArchive.cursor() == oArchive.begin() + size);

   // Show that v is unchanged
   TEST_ASSERT(real(v[0]) == 40.0);
   TEST_ASSERT(imag(v[0]) == 40.1);
   TEST_ASSERT(real(v[1]) == 20.0);
   TEST_ASSERT(imag(v[1]) == 20.1);
   TEST_ASSERT(real(v[2]) == 10.0);
   TEST_ASSERT(imag(v[2]) == 10.1);
   TEST_ASSERT(v.capacity() == 5);
   TEST_ASSERT(v.size() == 3);
  
   RingBuffer< Data > u;

   // Did NOT allocate u prior to loading.
   // This is the only difference from testSerialize1.

   // Load from archive
   MemoryIArchive iArchive;
   iArchive = oArchive;
   iArchive >> u;
 
   // Test object loaded from iArchive
   TEST_ASSERT(u.capacity() == 5);
   TEST_ASSERT(u.size() == 3);
   TEST_ASSERT(real(u[0]) == 40.0);
   TEST_ASSERT(imag(u[0]) == 40.1);
   TEST_ASSERT(real(u[1]) == 20.0);
   TEST_ASSERT(imag(u[1]) == 20.1);
   TEST_ASSERT(real(u[2]) == 10.0);
   TEST_ASSERT(imag(u[2]) == 10.1);

}
TEST_BEGIN(RingBufferTest)
TEST_ADD(RingBufferTest, testConstructor)
TEST_ADD(RingBufferTest, testAllocate)
TEST_ADD(RingBufferTest, testSubscript1)
TEST_ADD(RingBufferTest, testSubscript2)
//TEST_ADD(RingBufferTest, testPack1)
//TEST_ADD(RingBufferTest, testPack2)
TEST_ADD(RingBufferTest, testSerialize1)
TEST_ADD(RingBufferTest, testSerialize2)
TEST_END(RingBufferTest)

#endif
