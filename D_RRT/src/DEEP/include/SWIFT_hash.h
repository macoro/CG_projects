/*************************************************************************\

  Copyright 2002 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify OR distribute the MODIFIED portion of this
  software and its documentation for educational, research and non-profit 
  purposes, without fee, and without a written agreement is hereby granted, 
  provided that the above copyright notice and the following three paragraphs
  appear in all copies.

  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             Y. Kim, M. Lin
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919) 962-1749

  EMail:               geom@cs.unc.edu
                       youngkim@cs.unc.edu
					   lin@cs.unc.edu

\**************************************************************************/

// SWIFT_hash.h was originally written by Thomans Wang (wang@cup.hp.com)

#ifndef SWIFT_HASH_H
#define SWIFT_HASH_H


/* explain file SWIFT_hash.h
 * Description:
 *   hash table declaration file
 *
 * Author: Thomas Wang
 *
 * History:
 * (ID)   (name)   (date)    (bug#)      (message)
 * 000TW  Thomas   07/01/97  0000-00000  Sample msg
 */


class SWIFT_hash; // forward declaration


#include <stddef.h>



#include <SWIFT_mesh.h>


#ifndef KVPAIRSWIFT_HASH_H
#define KVPAIRSWIFT_HASH_H

/* explain file kvpairSWIFT_hash.h
 *
 * Description: pair data structure
 *
 * Author: Thomas Wang
 *
 */


class kvpairSWIFT_hash; // forward declaration







/* explain class kvpairSWIFT_hash
 * The class kvpairSWIFT_hash provides for a storage for a pair
 * of data of type SWIFT_Tri_Vertex*, and SWIFT_Gauss*.
 */
class kvpairSWIFT_hash // from template class uuPair
{
  public:

    SWIFT_Tri_Vertex* a;
    SWIFT_Gauss* b;

    kvpairSWIFT_hash();
    kvpairSWIFT_hash(const kvpairSWIFT_hash &);
    kvpairSWIFT_hash(SWIFT_Tri_Vertex*, SWIFT_Gauss*);
    kvpairSWIFT_hash& operator = (const kvpairSWIFT_hash& cpy); /* assignment operator */
};


/* explain function
 * Description:
 *   default constructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvpairSWIFT_hash::kvpairSWIFT_hash()
{
}


/* explain function
 * Description:
 *   copy constructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvpairSWIFT_hash::kvpairSWIFT_hash(const kvpairSWIFT_hash & cpy)
  : a(cpy.a), b(cpy.b)
{
}


/* explain function
 * Description:
 *   alternate constructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvpairSWIFT_hash::kvpairSWIFT_hash(
   SWIFT_Tri_Vertex* a_val
  ,SWIFT_Gauss* b_val)
  : a(a_val), b(b_val)
{
}


/* explain function
 * Description:
 *   assignment operator
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvpairSWIFT_hash& kvpairSWIFT_hash::operator = (const kvpairSWIFT_hash& cpy)
{
  this->a = cpy.a;
  this->b = cpy.b;
  return *this;
}

#endif




#ifndef KVLINKSWIFT_HASH_H
#define KVLINKSWIFT_HASH_H

/* explain file kvlinkSWIFT_hash.h
 * Description:
 *   single linked list template
 *
 * Author: Thomas Wang
 *
 * History:
 * (ID)   (name)   (date)    (bug#)      (message)
 * 000TW  Thomas   07/01/93  0000-00000  Sample msg
 */


class kvlinkSWIFT_hash; // forward declaration


#include <stddef.h>







/* explain class kvlinkSWIFT_hash
 * The kvlinkSWIFT_hash data structure consists of an data item of
 * type kvpairSWIFT_hash, and the 'next' pointer. 
 *
 * If the data item is of pointer type, deleting a linked list node
 * will NOT cause the data item pointer to be deleted.  In any
 * case, it is not safe to delete the pointer, as it may be shared.
 */
class kvlinkSWIFT_hash // from template class uuSlink
{
  public:

    typedef kvpairSWIFT_hash value_type;

  protected:
    value_type data_item;  /* the data item in node */
    kvlinkSWIFT_hash *the_next;  /* the next pointer */

  public:
    value_type & Item();
    kvlinkSWIFT_hash*& Next();
    kvlinkSWIFT_hash* Reverse();

    kvlinkSWIFT_hash();
    kvlinkSWIFT_hash(const kvlinkSWIFT_hash&);
    kvlinkSWIFT_hash(value_type);
    kvlinkSWIFT_hash(value_type, kvlinkSWIFT_hash*);

    kvlinkSWIFT_hash& operator = (const kvlinkSWIFT_hash& cpy);
};


/* explain function
 * Description:
 *   Default constructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash::kvlinkSWIFT_hash()
  : the_next(NULL)
{
}


/* explain function
 * Description:
 *   Copy constructor
 *
 * Parameters:
 *   cpy - node to be copied
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash::kvlinkSWIFT_hash(const kvlinkSWIFT_hash& cpy)
  : data_item(cpy.data_item), the_next(cpy.the_next)
{
}


/* explain function
 * Description:
 *   Constructor with a data value
 *
 * Parameters:
 *   obj - data value of the node
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash::kvlinkSWIFT_hash(value_type obj)
  : data_item(obj), the_next(NULL)
{
}


/* explain function
 * Description:
 *   Constructor with a data value, and next pointer.
 *
 * Parameters:
 *   obj      - data value of the node
 *   next_ptr - next pointer value
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash::kvlinkSWIFT_hash(value_type obj,kvlinkSWIFT_hash* next_ptr)
  : data_item(obj), the_next(next_ptr)
{
}


/* explain function
 * Description:
 *   Item() would return the content of the linked list node.
 *
 * Parameters: none
 *
 * Return Value: content of linked list node
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash::value_type& kvlinkSWIFT_hash::Item()
{
  return this->data_item;
}


/* explain function
 * Description:
 *   Next() would return the next uuSlink node.
 *
 * Parameters: none
 *
 * Return Value: next uuSlink node
 *
 * Assumes: none
 */
inline kvlinkSWIFT_hash*& kvlinkSWIFT_hash::Next()
{
  return this->the_next;
}

#endif




#ifndef KVARRAYSWIFT_HASH_H
#define KVARRAYSWIFT_HASH_H

/* explain file kvarraySWIFT_hash.h
 *
 * Description: expansion array template declaration file
 *
 * Author: Thomas Wang
 *
 */


class kvarraySWIFT_hash; // forward declaration


#include <stddef.h>







#define UUARRAY_INITIAL_SIZE 15
#define UUARRAY_MAX_SLOT 28

/*  15 * (2 ^ 28) does not overflow 32 bits, while 15 * (2^ 29) does.
 *  Therefore 28 slots will cover the increasing slice sizes for 32 bit
 *  range.
 */


/* explain class kvarraySWIFT_hash
 * The class kvarraySWIFT_hash provides expansion array capability
 * over data item of class kvlinkSWIFT_hash*.
 *
 * An array is created initially with length of 0.  The length of the
 * array can be increased with IncreaseLength(), and decreased with
 * DecreaseLength().  An array of increasing length of buffers is used
 * for the storage of data.
 *
 * If the data item is of pointer type, deleting the array
 * will NOT cause the data item pointer to be deleted.  In any
 * case, it is not safe to delete the pointer, as it may be shared.
 */
class kvarraySWIFT_hash // from template class uuArray
{
  public:

    typedef kvlinkSWIFT_hash* value_type;

  protected:

    size_t total_elem; // number of elements in the array
    size_t num_buffer; // number of allocated buffer
    value_type* the_desc[UUARRAY_MAX_SLOT];
    // array buffer, in increasing size sequence

    char GrowBuffer(size_t the_length);

  public:

    size_t Length() const;
    kvlinkSWIFT_hash*& operator [] (size_t the_indx) const;
    kvarraySWIFT_hash& operator = (const kvarraySWIFT_hash& cpy); /* assignment operator */

    size_t Append(const value_type the_obj);
    size_t IncreaseLength(size_t amount, const value_type init_value);
    size_t DecreaseLength(size_t amount);

    kvarraySWIFT_hash(); /* default constructor */
    kvarraySWIFT_hash(const kvarraySWIFT_hash& cpy); /* copy constructor */
    ~kvarraySWIFT_hash(); /* destructor */
};



/* explain function
 * Description:
 *   Length() will return the number of elements in the array.
 *
 * Parameters: none
 *
 * Return Value: unsigned 32 bit integer
 *
 * Assumes: none
 */
inline size_t kvarraySWIFT_hash::Length() const
{
  return this->total_elem;
}


/* explain function
 * Description:
 *   Default constructor of uuArray.
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvarraySWIFT_hash::kvarraySWIFT_hash()
  : total_elem(0), num_buffer(0)
{
}


/* explain function
 * Description:
 *   Copy constructor of uuArray.
 *   If out of memory, the new array will have length of 0.
 *   If error occurred while copying objects, the length of the
 *   array will be at the place where error occurred.
 *
 * Parameters:
 *   cpy - uuArray to be copied
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline kvarraySWIFT_hash::kvarraySWIFT_hash(const kvarraySWIFT_hash& cpy)
  : total_elem(0), num_buffer(0)
{
  (*this) = cpy;
}


#endif






/* explain class SWIFT_hash
 * The SWIFT_hash data structure provides mapping capability
 * from a SWIFT_Tri_Vertex* type
 * to a SWIFT_Gauss* type, given a key
 * identity data structure Gauss_Comp.
 *
 * SWIFT_hash is implemented using hash table.
 *
 * The class Gauss_Comp should have three member functions
 * as follows:
 *
 *   struct Gauss_Comp
 *   {
 *     char equal(SWIFT_Tri_Vertex* x, SWIFT_Tri_Vertex* y) const; // x == y
 *     char less(SWIFT_Tri_Vertex* x, SWIFT_Tri_Vertex* y) const; // x < y
 *     size_t hash(SWIFT_Tri_Vertex* x) const;
 *   };
 *
 * At file generation time, just give the name
 * of the class with the above three member functions at
 * the parameter position for Keyid.
 *
 * The hash table algorithm used is 'linear hashing' discussed in
 * CACM, Apr 1988.  This algorithm features hash table expansion
 * capability with amortized expansion processing.
 *
 */

struct Gauss_Comp
{
	char equal(SWIFT_Tri_Vertex* x, SWIFT_Tri_Vertex* y) const {return x==y;} ; // x == y
	char less(SWIFT_Tri_Vertex* x, SWIFT_Tri_Vertex* y) const {return x<y; }; // x < y
    size_t hash(SWIFT_Tri_Vertex* x) const { 
		return (size_t)x;
		/*
		size_t key=(size_t)x;
	    key += (key << 12);
		key ^= (key >> 22);
		key += (key << 4);
		key ^= (key >> 9);
		key += (key << 10);
		key ^= (key >> 2);
		key += (key << 7);
		key ^= (key >> 12);
		return key;
		*/
		};
};

class SWIFT_hash // from template class uuMap
{
  public:
    typedef SWIFT_Tri_Vertex* key_type;
    typedef SWIFT_Gauss* val_type;
    typedef Gauss_Comp keyid_type;

    typedef kvlinkSWIFT_hash link_type;

  protected:
    size_t key_count;  /* number of records in the table */
    size_t split_ptr;  /* next bucket to be split */
    size_t max_split;  /* upper bound on 'split_ptr' during this expansion */
    keyid_type  ident; /* Key's identity data structure */
    kvarraySWIFT_hash slots; /* bucket table */

    size_t BucketOffset(size_t maxsplit,
                        size_t splitptr,
                        size_t hashval) const;

    void SplitBucket();
    void MergeBucket();

  public:
    size_t Length() const;
    void ClearAll();
    char Exists(const key_type& the_key) const;
    val_type& operator [] (const key_type& the_key) const;
    key_type GetAnyKey(const size_t offset = 0) const;
    key_type GetNextKey(const key_type& the_key) const;

    size_t Insert(const key_type& the_key, const val_type& the_val);
    size_t RemoveWith(const key_type& the_key);

    SWIFT_hash& operator = (const SWIFT_hash& cpy);

    SWIFT_hash(); // default constructor
    SWIFT_hash(const SWIFT_hash& cpy); // copy constructor
    ~SWIFT_hash(); // destructor

  private:
	SWIFT_Array<SWIFT_Gauss*> Gauss_Array;
	
  public:
  	void Insert_Gauss(SWIFT_Tri_Vertex* the_key, SWIFT_Gauss* the_val);
  
};


inline void SWIFT_hash::Insert_Gauss(SWIFT_Tri_Vertex* the_key, SWIFT_Gauss* the_val)
{
	Gauss_Array.Add_Grow(the_val, 10);
	Insert(the_key, the_val);
}


/* explain function
 * Description:
 *   Length() will return the number of elements in the hash table.
 *
 * Parameters: none
 *
 * Return Value: unsigned 32 bit integer
 *
 * Assumes: none
 */
inline size_t SWIFT_hash::Length() const
{
  return this->key_count;
}


/* explain function
 * Description:
 *   This function will return a bucket number suitable with linear
 *   hashing.
 *
 * Parameters:
 *   maxsplit - max split value
 *   splitptr - split pointer value
 *   hashval  - a hash value
 *
 * Return Value: bucket number
 *
 * Assumes: none
 */
inline size_t
  SWIFT_hash::BucketOffset(size_t maxsplit,
                         size_t splitptr,
                         size_t hashval) const
{
  return (hashval % maxsplit < splitptr) ?
         hashval % (maxsplit + maxsplit) :
         hashval % maxsplit;
}


/* explain function
 * Description:
 *   default constructor
 *
 * Parameters: none
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline SWIFT_hash::SWIFT_hash()
  : key_count(0), split_ptr(0), max_split(1)
{
}


/* explain function
 * Description:
 *   copy constructor
 *
 * Parameters:
 *   cpy - hash table to be copied
 *
 * Return Value: none
 *
 * Assumes: none
 */
inline SWIFT_hash::SWIFT_hash(const SWIFT_hash& cpy)
  : key_count(0), split_ptr(0), max_split(1)
{
  (*this) = cpy;
}


#endif

