#pragma once

#include <vector>
#include <string>
#include <iostream>
class BigInt
{

private:
	enum nums
	{
		SAFEDIGITS = 9,
		SEGMOD = 1000000000,
	};
	using int_t = int;
	constexpr int_t numBits() const { return 30; } //2**30 always fits 9 digits it works out nice

	int_t pow10(int x) const {
		int_t res = 1;
		for (int i = 0; i < x; ++i) {
			res *= 10;
		}
		return res;
	}
	int_t lsb;
	std::vector<int_t> num;
	bool isNegative = false;

	void add_unchecked(const BigInt& other);
	void sub_unchecked(const BigInt& other);


public:
	BigInt();
	explicit BigInt(const std::string& number);
	template<typename T,
		std::enable_if_t<std::is_integral<T>::value, bool>>
	BigInt(T integral)
	{
		isNegative = -1 * (integral < 0);
		integral = std::abs(integral);
		
		lsb = integral % SEGMOD;
		integral /= SEGMOD;
		while(integral){
			num.emplace_back(integral % SEGMOD);
			integral /= SEGMOD;
		}
	}
	BigInt(const BigInt& x);
	BigInt(BigInt&& x);
	BigInt& operator=(const BigInt& rhs);
	~BigInt() = default;

	std::string toString();
	void print();

	//operator overloads
	//math
	friend BigInt operator+(const BigInt& lhs , const BigInt& rhs);
	friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);

	//compare
	friend bool operator> (const BigInt& lhs, const BigInt& rhs);
	friend bool operator>=(const BigInt& lhs, const BigInt& rhs);
	friend bool operator< (const BigInt& lhs, const BigInt& rhs);
	friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
	friend bool operator==(const BigInt& lhs, const BigInt& rhs);
	friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
};


