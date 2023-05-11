// FASTFilter_TEST_tINT128.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

#define tINT32 signed int
#define tUINT32 unsigned int
#define tINT64  long long signed
#define tUINT64 unsigned long long

struct tDecimalStruct {
	tINT32 m_Exponent;
	tINT64 m_Mantissa;
};

#define tDECIMAL tDecimalStruct
#define tASCII char
#define tUNICODE unsigned char
#define tBYTEVECTOR unsigned char

class tINT128 {
public:
	tINT128() : m_Second(0), m_First(0) {};
	tINT128( tUINT32 pInt ) : m_First(pInt), m_Second(0) {}
	tINT128( tUINT64 pInt ) : m_Second(0), m_First(pInt) {};
	tINT128( tINT32 pInt ) : m_First(0), m_Second(0)
	{
		if( pInt & 0x80000000 ) {
			m_Second = 0xFFFFFFFFFFFFFFFFLL;
			m_First = 0xFFFFFFFF00000000ULL;
		}

		m_First |= pInt;
	}
	tINT128( tINT64 pInt ) : m_Second(0), m_First(0) 
	{
		if( pInt & 0x8000000000000000 )
			m_Second = 0xFFFFFFFFFFFFFFFFLL;

		m_First |= pInt;
	};
	tINT128 operator ~()const {
		tINT128 l_result(*this);
		l_result.m_First = ~l_result.m_First;
		l_result.m_Second = ~l_result.m_Second;
		return l_result;
	}
	tINT128 operator ^(const tINT128 &pOther)const {
		tINT128 l_result(pOther);
		l_result.m_Second ^= m_Second;
		l_result.m_First ^= m_First;
		return l_result;
	}
	tINT128 operator|(const tINT128 &pOther)const {
		tINT128 l_result(pOther);
		l_result.m_Second |= m_Second;
		l_result.m_First |= m_First;
		return l_result;
	}
	tINT128 &operator|=(const tINT128 &pOther) {
		m_Second |= pOther.m_Second;
		m_First |= pOther.m_First;
		return *this;
	}
	tINT128 &operator ^=(const tINT128 &pOther) {
		m_Second ^= pOther.m_Second;
		m_First ^= pOther.m_First;
		return *this;
	}
	tINT128 &operator &=(const tINT128 &pOther) {
		m_Second &= pOther.m_Second;
		m_First &= pOther.m_First;
		return *this;
	}
	tINT128 operator&(const tINT128 &pOther)const {
		tINT128 l_result(pOther);
		l_result.m_Second &= m_Second;
		l_result.m_First &= m_First;
		return l_result;
	}
	tINT128 operator<<(const unsigned int pDist)const {
		tINT128 l_result;
		
		tUINT64 l_tmp = 0xFFFFFFFFFFFFFFFFULL;
		if( pDist <= 64 ) {
			int l_shiftLength = 64 - pDist;

			l_tmp <<= l_shiftLength;
			l_tmp &= m_First;
			l_tmp >>= l_shiftLength;
			l_result.m_First = m_First << pDist;
			l_result.m_Second = m_Second << pDist;
		}
		else
		if( pDist <= 128 )
		{
			l_tmp &= m_First << (pDist - 64);
			l_result.m_First = 0;
			l_result.m_Second = m_Second << (pDist - 64);
		}
		else
		{
			l_result.m_First = 0;
			l_result.m_Second = 0;
			return l_result;
		}

		l_result.m_Second |= l_tmp;

		return l_result;
	}
	tINT128 &operator<<=(const unsigned int pDist) {		
		tUINT64 l_tmp = 0xFFFFFFFFFFFFFFFFULL;
		if( pDist <= 64 ) {
			int l_shiftLength = 64 - pDist;
			l_tmp <<= l_shiftLength;
			l_tmp &= m_First;
			l_tmp >>= l_shiftLength;
			m_First <<= pDist;
			m_Second <<= pDist;
			m_Second |= l_tmp;
		}
		else
		if( pDist <= 128 )
		{
			l_tmp &= m_First << (pDist - 64);
			m_First = 0;
			m_Second <<= (pDist - 64);
			m_Second |= l_tmp;
		}
		else
		{
			m_First = 0;
			m_Second = 0;
		}

		return *this;
	}
	tINT128 &operator>>=(const unsigned int pDist) {		
		tUINT64 l_tmp = 0xFFFFFFFFFFFFFFFFULL;
		if( pDist <= 64 ) {
			const int l_shiftLength = 64 - pDist;
			l_tmp >>= l_shiftLength;
			l_tmp &= m_Second;
			l_tmp <<= l_shiftLength;

			m_First >>= pDist;
			m_Second >>= pDist;
		}
		else
		if( pDist <= 128 )
		{
			m_Second >>= (pDist - 64);
			l_tmp &= m_Second;
			m_First >>= (pDist - 64);
			m_Second = 0;
		}
		else
		{
			m_First = 0;
			m_Second = 0;
			return *this;
		}
		m_First |= l_tmp;
		return *this;
	}
	tINT128 operator>>(const unsigned int pDist)const {
		tINT128 l_result;
		
		tUINT64 l_tmp = 0xFFFFFFFFFFFFFFFFULL;
		if( pDist <= 64 ) {
			int l_shiftLength = 64 - pDist;
			l_tmp >>= l_shiftLength;
			l_tmp &= m_Second;
			l_tmp <<= l_shiftLength;

			l_result.m_First = m_First >> pDist;
			l_result.m_Second = m_Second >> pDist;
		}
		else
		if( pDist <= 128 )
		{
			l_tmp &= m_Second >> (pDist - 64);
			l_result.m_Second = 0;
			l_result.m_First = m_First >> (pDist - 64);
		}
		else
		{
			l_result.m_First = 0;
			l_result.m_Second = 0;
			return l_result;
		}

		l_result.m_First |= l_tmp;

		return l_result;
	}

	tINT128 &operator+=( const tINT128 &pOther ) {
		tINT128 l_a, l_b;
		l_a = *this;
		l_b = pOther;

		*this ^= l_b;
		while( !(l_a & l_b).zero() ) {
			l_b &= l_a;
			l_b <<= 1;
			l_a = *this;
			*this ^= l_b;
		}

		return *this;
	}	
	tINT128 &operator-=( const tINT128 &pOther ) {
		tINT128 l_a, l_b;
		l_a = *this;
		l_b = ~pOther;
		l_b += 1;

		*this ^= l_b;
		while( !(l_a & l_b).zero() ) {
			l_b &= l_a;
			l_b <<= 1;
			l_a = *this;
			*this ^= l_b;
		}

		return *this;
	}
	tINT128 &operator*=( const tINT128 &pOther ) {
		tINT128 l_tmp = pOther;
		tINT128 l_offset = *this;
		*this = 0;
		while( !l_tmp.zero() ) {
			if( l_tmp.m_First & 0x1 )
				*this += l_offset;
			l_offset <<= 1;
			l_tmp >>= 1;
		}
		return *this;
	}

	bool operator>( const tINT128 &pOther ) const {
		tINT128 l_tmp = *this;
		l_tmp -= pOther;

		if( l_tmp.negative() ) return false;
		else if( l_tmp.zero() ) return false;
		return true;		
	}
	bool operator>=( const tINT128 &pOther ) const {
		tINT128 l_tmp = *this;
		l_tmp -= pOther;

		if( l_tmp.negative() ) return false;
		return true;		
	}
	bool operator<( const tINT128 &pOther ) const {
		tINT128 l_tmp = *this;
		l_tmp -= pOther;

		if( l_tmp.negative() ) return true;
		return false;		
	}
	bool operator<=( const tINT128 &pOther ) const {
		tINT128 l_tmp = *this;
		l_tmp -= pOther;

		if( l_tmp.negative() ) return true;
		else if( l_tmp.zero() ) return true;
		return false;		
	}
	bool unsignedLessThanEquals( const tINT128 &pOther ) const {
		
		if( pOther.m_Second > m_Second ) return true;
		if( pOther.m_Second < m_Second ) return false;
		if( pOther.m_First >= m_First ) return true;
		return false;
	}
	bool unsignedLessThan( const tINT128 &pOther ) const {
		
		if( pOther.m_Second > m_Second ) return true;
		if( pOther.m_Second < m_Second ) return false;
		if( pOther.m_First > m_First ) return true;
		return false;
	}
	/**
	 * exceptionally slow long division
     */
	tINT128 operator/( const tINT128 &pOther ) {
		if( pOther.zero() ) {
			return 0;
		}
		tINT128 l_D(*this);
		tINT128 l_Q(pOther);

		/**
		 * Convert Negatives To Positives And Remember
		 * if the result should be negative
		 */
		bool l_negative = false;
		if( l_D.negative() ) {
			l_D = ~l_D;
			l_D += 1;
			l_negative = !l_negative;
		}
		if( l_Q.negative() ) {
			l_Q = ~l_Q;
			l_Q += 1;
			l_negative = !l_negative;
		}

		/*
		 * now that operands are positive see if the
		 * dividend is smaller and return default zero
		 * if so.
		 */
		if( l_D.unsignedLessThan(l_Q) ) {
			return 0;
		}

		/**
		 * find how many bits are between the MSB of a tUINT128
		 * and the MSB of the dividend
		 */
		tINT32 l_SD = 1;
		{
			tINT128 l_MSBD(l_D);
			while( !(l_MSBD >>= 1).zero() )	l_SD++;
		}
		/**
		 * find how many bits are between the MSB of a tUINT128
		 * and the MSB of the divisor
		 */
		tINT32 l_SQ = 1;
		{
			tINT128 l_MSBQ(l_Q);
			while( !(l_MSBQ >>= 1).zero() ) l_SQ++;
		}
		/**
		 * shift the divisor left so that it's MSB is at the same
		 * spot as the MSB of the dividend
		 */
		l_Q <<= (l_SD - l_SQ);
		assert( l_SD >= l_SQ );

		tINT128 l_RESULT(0); // the quotient
		tINT128 l_BIT = tINT128(0x1) << (l_SD - l_SQ); // the current bit to be set in the quotient
		while( !l_BIT.zero() ) {
			// if the divisor is smaller than the
			// dividend we make that bit in the
			// quotient
			if( l_Q.unsignedLessThanEquals(l_D) ) 
			{
				l_D -= l_Q;
				l_RESULT |= l_BIT;
			}
			// move the current quotient bit 
			l_BIT >>= 1;
			// make the divisor smaller
			l_Q >>= 1;
		}

		// we need to negate the result if
		// one of the operands was negative
		if( l_negative ) {
			l_RESULT = ~l_RESULT;
			l_RESULT += 1;
		}

		return l_RESULT;
	}
	/**
	 * this code is a minor variation of operator/()
	 * see that function for comments about implementation
	 */
	tINT128 &operator/=( const tINT128 &pOther ) {
		if( pOther.zero() ) {
			// zero self out
			m_First = 0;
			m_Second = 0;
			return *this;
		}
		tINT128 l_D(*this);
		tINT128 l_Q(pOther);
		bool l_negative = false;
		if( l_D.negative() ) {
			l_D = ~l_D;
			l_D += 1;
			l_negative = !l_negative;
		}
		if( l_Q.negative() ) {
			l_Q = ~l_Q;
			l_Q += 1;
			l_negative = !l_negative;
		}
		if( l_D.unsignedLessThan(l_Q) ) {
			// zero self out
			m_First = 0;
			m_Second = 0;
			return *this;
		}
		tINT32 l_SD = 1;
		{
			tINT128 l_MSBD(l_D);
			while( !(l_MSBD >>= 1).zero() )	l_SD++;
		}
		tINT32 l_SQ = 1;
		{
			tINT128 l_MSBQ(l_Q);
			while( !(l_MSBQ >>= 1).zero() ) l_SQ++;
		}
		l_Q <<= (l_SD - l_SQ);
		assert( l_SD >= l_SQ );

		// zero self out
		m_First = 0;
		m_Second = 0;

		tINT128 l_BIT = tINT128(0x1) << (l_SD - l_SQ);
		while( !l_BIT.zero() ) {
			if( l_Q.unsignedLessThanEquals(l_D) ) 
			{
				l_D -= l_Q;
				*this |= l_BIT; // apply bit to self
			}
			l_BIT >>= 1;
			l_Q >>= 1;
		}

		// negate and return self
		if( l_negative ) {
			*this = ~*this;
			*this += 1;
		}
		return *this;
	}
	/**
	 * this code is a minor variation of operator/()
	 * see that function for comments about implementation
	 */
	tINT128 operator%( const tINT128 &pOther ) {		
		if( pOther.zero() ) {
			return 0;
		}
		tINT128 l_D(*this);
		tINT128 l_Q(pOther);
		bool l_negative = false;
		if( l_D.negative() ) {
			l_D = ~l_D;
			l_D += 1;
			l_negative = !l_negative;
		}
		if( l_Q.negative() ) {
			l_Q = ~l_Q;
			l_Q += 1;
			l_negative = !l_negative;
		}
		if( l_D.unsignedLessThan(l_Q) ) {
			// return value as remainder
			return l_D;
		}
		tINT32 l_SD = 1;
		{
			tINT128 l_MSBD(l_D);
			while( !(l_MSBD >>= 1).zero() )	l_SD++;
		}
		tINT32 l_SQ = 1;
		{
			tINT128 l_MSBQ(l_Q);
			while( !(l_MSBQ >>= 1).zero() ) l_SQ++;
		}
		l_Q <<= (l_SD - l_SQ);
		assert( l_SD >= l_SQ );

		// ignore result
		tINT128 l_BIT = tINT128(0x1) << (l_SD - l_SQ);
		while( !l_BIT.zero() ) {
			if( l_Q.unsignedLessThanEquals(l_D) ) 
			{
				l_D -= l_Q;
			}
			l_BIT >>= 1;
			l_Q >>= 1;
		}
		// return remainder
		return l_D;
	}
	bool zero() const {
		return m_Second == 0 && m_First == 0;
	}
	bool negative() const {
		return (m_Second & 0x8000000000000000) == 0x8000000000000000;
	}

	bool truncatesINT64() const {
		return ((m_First & 0x8000000000000000) | (m_Second & 0x7FFFFFFFFFFFFFFF)) != 0;
	}
	bool truncatesUINT64() const {
		return m_Second != 0;
	}
	bool truncatesINT32() const {
		return ((m_First & 0xFFFFFFFF80000000) | (m_Second & 0x7FFFFFFFFFFFFFFF)) != 0;
	}
	bool truncatesUINT32() const {
		return (m_Second | ( m_First & 0xFFFFFFFF00000000)) != 0;
	}

	tINT64 toINT64() const {
		return (tINT64)(m_First & 0x7FFFFFFFFFFFFFFF | m_Second & 0x8000000000000000);
	}
	tUINT64 toUINT64() const {
		return (tUINT64)(m_First);
	}
	tINT32 toINT32() const {
		return (tINT32)( m_First & 0x7FFFFFFFF | m_Second & 0x8000000000000000 );
	}
	tUINT32 toUINT32() const {
		return (tUINT32)( m_First & 0xFFFFFFFF );
	}

public:
	tUINT64 m_Second;
	tUINT64 m_First;
};

std::ostream & operator<<(std::ostream &os, const tINT128&pObj) {
	if( pObj.zero() ) {
		return os << "0";
	}
	if( pObj.negative() )
		os << "-";

	std::stringstream l_rev;
	tINT128 l_obj(pObj);
	while( !l_obj.zero() ) {
		l_rev << (l_obj % 10).toUINT32();
		l_obj = l_obj / 10;
	}
	std::string l_tmp(l_rev.str());
	for( std::string::reverse_iterator l_i = l_tmp.rbegin() ;
			l_i != l_tmp.rend() ;
			l_i++ )
	{
		os << (*l_i);
	}

	return os;
}

int main(int argc, char* argv[])
{
	using std::cout;
	using std::endl;

	cout << "tINT32: " << sizeof(tINT32) << endl;
	cout << "tINT64: " << sizeof(tINT64) << endl;
	cout << "tUINT32: " << sizeof(tUINT32) << endl;
	cout << "tUINT64: " << sizeof(tUINT64) << endl;
	cout << "tINT128: " << sizeof(tINT128) << endl;

	tINT64 l_slong = 0x8000000000000000;
	tUINT64 l_ulong = 0xFFFFFFFFFFFFFFFF;
	tINT32 l_sint =  0x80000000 ;
	tUINT32 l_uint = 0xFFFFFFFF;

	tINT128 l_diver = tINT128(0x800000);
	while( !l_diver.zero() ) {
		cout << "DIVISION 2: " << l_diver << " / 2 = ";
		l_diver = l_diver / 2;
		cout << l_diver << endl;
	}
	l_diver = tINT128(10000);
	while( !l_diver.zero() ) {
		cout << "DIVISION 10: " << l_diver << " / 10 = ";
		l_diver = l_diver / 10;
		cout << l_diver << endl;
	}
	l_diver = tINT128(-10000);
	while( !l_diver.zero() ) {
		cout << "DIVISION 10: " << l_diver << " / 10 = ";
		l_diver = l_diver / 10;
		cout << l_diver << endl;
	}

	tINT128 l_negOne(-1);
	tINT128 l_negTwo(-2);
	tINT128 l_negThree(-3);

	cout << "ulong: " << l_ulong << endl;
	cout << "tINT128 from ulong: " << tINT128(l_ulong) << endl;
	cout << "slong: " << l_slong << endl;
	cout << "tINT128 from slong: " << tINT128(l_slong) << endl;
	cout << "uint: " << l_uint << endl;
	cout << "tINT128 from uint: " << tINT128(l_uint) << endl;
	cout << "sint: " << l_sint << endl;
	cout << "tINT128 from sint: " << tINT128(l_sint) << endl;

	cout << std::boolalpha << (tINT128(100) > tINT128(-100)) << endl;
	cout << std::boolalpha << (tINT128(-100) > tINT128(100)) << endl;
	cout << std::boolalpha << (tINT128(100) > tINT128(100)) << endl;

	tINT128 l_adder = tINT128(0);
	for( int i = 0 ; i < 100 ; i++ ) {
		cout << "ADD: " << l_adder << " + 100 = ";
		l_adder += tINT128(100);		
		cout << l_adder << endl;
	}
	for( int i = 0 ; i < 100 ; i++ ) {
		cout << "SUBTRACT: " << l_adder << " - 100 = ";
		l_adder -= tINT128(100);		
		cout << l_adder << endl;
	}

	tINT128 l_shifter = 1;
	for( int l_i = 1 ; l_i < 128 ; l_i++ ) {
		l_shifter = l_shifter << 1;
		cout << "Shift Left (" << l_i << "): " << l_shifter << endl;
	}
	for( int l_i = 1 ; l_i < 128 ; l_i++ ) {
		l_shifter = l_shifter >> 1;
		cout << "Shift Left (" << l_i << "): " << l_shifter << endl;
	}
	for( int l_i = 1 ; l_i < 128 ; l_i++ ) {
		cout << "Multiply: " << l_shifter << " * 4 = ";
		l_shifter *= 4;
		cout << l_shifter << endl;
	}

	for( int i = 0 ; i < 16 ; i++ ) {
		
		int jz = i % 8;
		int jq = i >> 3;

		cout << jz << jq << endl;
	}


	
  cout << "Press any key to continue" << endl;
  getchar();

	return 0;
}

