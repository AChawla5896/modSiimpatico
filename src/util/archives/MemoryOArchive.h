#ifndef MEMORY_O_ARCHIVE_H
#define MEMORY_O_ARCHIVE_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010 - 2012, David Morse (morse012@umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include "Byte.h"
#include "serialize.h"

#include <util/space/Vector.h>
#include <util/space/IntVector.h>

#include <complex>
#include <string>

namespace Util
{

   class MemoryIArchive;

   /**
   * Save archive for packed heterogeneous binary data.
   *
   * \ingroup Archive_Module
   */
   class MemoryOArchive
   {

   public:

      /// Returns true;
      static bool is_saving();

      /// Returns false;
      static bool is_loading();

      /**
      * Constructor.
      */
      MemoryOArchive();

      /**
      * Destructor.
      */
      virtual ~MemoryOArchive();

      /**
      * Allocate memory.
      *
      * \param capacity size of memory block in bytes
      */
      virtual void allocate(size_t capacity);

      /**
      * Resets the cursor to the beginning.
      */
      void clear();

      /**
      * Save one object.
      */
      template <typename T>
      void operator & (T& data);

      /**
      * Save one object.
      */
      template <typename T>
      MemoryOArchive& operator << (T& data);

      /**
      * Pack a T object.
      */
      template <typename T> 
      void pack(const T& data);

      /**
      * Pack a C array.
      *
      * \param array C array
      * \param n     number of elements
      */
      template <typename T> 
      void pack(const T* array, int n);

      /**
      * Pack a 2D C array.
      *
      * \param array poiner to [0][0] element of 2D array 
      * \param m     number of rows
      * \param n     number of columns
      */
      template <typename T> 
      void pack(const T* array, int m, int n);

      #ifdef UTIL_MPI
      /**
      * Send packed data via MPI.
      *
      * \param comm  MPI communicator
      * \param dest  rank of processor to which data is sent
      */
      void send(MPI::Intracomm& comm, int dest);

      /**
      * Send packed data via MPI (non-blocking)
      *
      * \param comm  MPI communicator
      * \param req   MPI request
      * \param dest  rank of processor to which data is sent
      */
      void iSend(MPI::Intracomm& comm, MPI::Request& req, int dest);
      #endif

      /**
      * Return pointer to beginning of block.
      */
      Byte* begin() const;

      /**
      * Return pointer to current position (cursor).
      */
      Byte* cursor() const;

      /**
      * Return capacity in Bytes.
      */
      size_t capacity() const;

      /**
      * Has memory been allocated?
      */
      bool isAllocated() const;

   private:

      /// Pointer to first Byte of allocated memory.
      Byte* buffer_;

      /// Pointer to first element in block. 
      Byte* begin_;

      /// Current element (read/write cursor).
      Byte* cursor_;

      /// Pointer to one Byte past last in allocated block.
      Byte* endAllocated_;

      /// Allocated size of send and recv buffers, in Bytes.
      size_t capacity_;

      /// Archive version number.
      unsigned int version_;

      /// An OArchive is locked while it is being read.
      bool  isLocked_;

      /// Did this archive allocate the memory block?
      bool ownsData_;

      /**
      * Copy constructor (not implemented).
      */
      MemoryOArchive (const MemoryOArchive& other);

      /**
      * Assignment (not implemented).
      */
      MemoryOArchive& operator = (const MemoryOArchive& other);

   // friends:

      friend class MemoryIArchive;

   };

   // Inline methods

   inline bool MemoryOArchive::is_saving()
   { return true; }

   inline bool MemoryOArchive::is_loading()
   { return false; }

   // Inline non-static methods

   /*
   * Has a memory block been allocated?
   */
   inline bool MemoryOArchive::isAllocated() const
   {  return (bool) begin_; }

   /*
   * Return capacity in Bytes.
   */
   inline size_t MemoryOArchive::capacity() const
   {  return capacity_; }

   /*
   * Return pointer to beginning of block.
   */
   inline Byte* MemoryOArchive::begin() const
   {  return begin_; }

   /*
   * Return pointer to cursor position.
   */
   inline Byte* MemoryOArchive::cursor() const
   {  return cursor_; }

   /*
   * Save one object.
   */
   template <typename T>
   inline void MemoryOArchive::operator & (T& data)
   {  serialize(*this, data, version_); }

   /*
   * Save one object.
   */
   template <typename T>
   inline MemoryOArchive& MemoryOArchive::operator << (T& data)
   {   
      serialize(*this, data, version_); 
      return *this;
   }

   // Method templates

   /*
   * Save a single object of type T.
   */
   template <typename T>
   inline void MemoryOArchive::pack(const T& data)
   {
      if (isLocked_) {
         UTIL_THROW("Locked archive");
      }
      if (cursor_ + sizeof(data) > endAllocated_) {
         UTIL_THROW("Attempted write past end of allocated block");
      }
      T* ptr = (T *)cursor_;
      *ptr = data;
      ++ptr;
      cursor_ = (Byte *)ptr;
   }

   /*
   * Save a C-array of objects of type T.
   */
   template <typename T>
   inline void MemoryOArchive::pack(const T* array, int n)
   {
      if (isLocked_) {
         UTIL_THROW("Locked archive");
      }
      if (cursor_ + n*sizeof(T) > endAllocated_) {
         UTIL_THROW("Attempted write past end of allocated block");
      }
      T* ptr = (T *)cursor_;
      for (int i=0; i < n; ++i) {
         *ptr = array[i];
         ++ptr;
      }
      cursor_ = (Byte *)ptr;
   }

   /*
   * Bitwise pack a 2D C-array of objects of type T.
   */
   template <typename T>
   inline void MemoryOArchive::pack(const T* array, int m, int n)
   {
      if (isLocked_) {
         UTIL_THROW("Locked archive");
      }
      if (cursor_ + m*n*sizeof(T) > endAllocated_) {
         UTIL_THROW("Attempted write past end of allocated block");
      }
      int i, j;
      T* ptr = (T *)cursor_;
      for (i=0; i < m; ++i) {
         for (j=0; j < n; ++j) {
            *ptr = array[i*n + j];
            ++ptr;
         }
      }
      cursor_ = (Byte *)ptr;
   }

   // Explicit serialize functions for primitive types

   /*
   * Save a bool to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, bool& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a char to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, char& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save an unsigned int to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, unsigned int& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save an int to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, int& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a unsigned long int to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, unsigned long& data,  
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a long int to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, long& data,  
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a float to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, float& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save an double to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, double& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   // Explicit serialize functions for standard library types

   /*
   * Save a std::complex<float> to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, std::complex<float>& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a std::complex<double> to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, std::complex<double>& data, 
                         const unsigned int version)
   {  ar.pack(data); }

   /*
   * Save a std::string to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, std::string& data, 
                         const unsigned int version)
   {
      size_t size = data.size() + 1; // the +1 is for the NULL
      ar.pack(size);
      const char* temp = data.c_str();
      ar.pack(temp, size);
   }

   // Explicit serialize functions for namespace Util

   /*
   * Save a Util::Vector to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, Vector& data, 
                         const unsigned int version)
   {  ar.pack(data); } 

   /*
   * Save a Util::IntVector to a MemoryOArchive.
   */
   template <>
   inline void serialize(MemoryOArchive& ar, IntVector& data, 
                         const unsigned int version)
   {  ar.pack(data); }

}
#endif
