#include "BigInt.h"
#include <iomanip>
#include <cmath>
//do i need this?
BigInt::BigInt() 
{
	lsb = 0;
}


BigInt::BigInt(const std::string& number)
{
	size_t len = 0;
	int sPos = number.length() - 1;
	
	//count number of digits, and also check if negative
	for (auto c : number) {
		if (len == 0 && c == '-') {
			isNegative = true;
		}
		else if (c >= '0' && c <= '9') {
			++len;
		}
	}
	long int nPos = len - 1;
	int extra = len % SAFEDIGITS;
	int_t n = 0;
	int d = 0;

	bool lsbSet = false;
	while (nPos >= extra) {
		char digit = number[sPos--];
		if (digit > '9' || digit < '0') {
			continue;
		}
		nPos -= 1;
		n += (digit - '0') * pow10(d++);
		if (d >= SAFEDIGITS) {
			if (!lsbSet) {
				lsb = n;
				lsbSet = true;
			}
			else {
				num.push_back(n);
			}
			d = 0;
			n = 0;
		}
	}
	for (int i = 0; i < extra; ++i) {
		char digit = number[sPos--];
		
		if (digit > '9' || digit < '0') {
			i -= 1; //doesnt count as one of the digits in extra
			continue;
		}
		n += (digit - '0') * pow10(i);
	}
	if (n || num.size() == 0) {
		if (!lsbSet) {
			lsb = n;
			lsbSet = true;
		}
		else {
			num.push_back(n);
		}
	}
	
	if (num.size() > 0) {
		while(num.size() && num[num.size() - 1] == 0) {
			num.pop_back();
		}
	}
	
	
}

BigInt::BigInt(const BigInt& x) : num(x.num), lsb{ x.lsb }{
	isNegative = x.isNegative;
}
BigInt::BigInt(BigInt&& x) : num(std::move(x.num)){
	lsb = x.lsb;
	x.lsb = 0;
	isNegative = x.isNegative;
}
BigInt& BigInt::operator=(const BigInt& rhs) {
	if (this == &rhs) {
		return *this;
	}
	isNegative = rhs.isNegative;
	num = rhs.num;
	lsb = rhs.lsb;

	return *this;
}

std::string BigInt::toString() {
	std::string res;

	return res;
	
}

bool conInv(bool condition, bool inverse) {
	return inverse ? !condition : condition;
}
bool operator> (const BigInt& lhs, const BigInt& rhs) {
	
	if (lhs.isNegative && !rhs.isNegative) return false;
	if (!lhs.isNegative && rhs.isNegative) return true;

	bool negative = lhs.isNegative;
	auto l = lhs.num.size();
	auto r = rhs.num.size();
	if (l > r) return (conInv(true,negative));
	if (r > l) return (conInv(false,negative));

	int i = l-1;
	while (i >= 0) {
		if (lhs.num[i] > rhs.num[i]) return (conInv(true, negative));
		if (lhs.num[i] < rhs.num[i]) return (conInv(false, negative));
		i -= 1;
	}
	return conInv(lhs.lsb > rhs.lsb,negative);
}
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
	if (lhs.isNegative && !rhs.isNegative) return false;
	if (!lhs.isNegative && rhs.isNegative) return true;
	bool negative = lhs.isNegative;
	auto l = lhs.num.size();
	auto r = rhs.num.size();
	if (l > r) return (conInv(true, negative));
	if (r > l) return (conInv(false, negative));

	int i = l - 1;
	while (i >= 0) {
		if (lhs.num[i] > rhs.num[i]) return (conInv(true, negative));
		if (lhs.num[i] < rhs.num[i]) return (conInv(false, negative));
		i -= 1;
	}
	if (lhs.lsb != rhs.lsb) {
		return conInv(lhs.lsb > rhs.lsb, negative);
	}
	else {
		return true;
	}
	
}
bool operator< (const BigInt& lhs, const BigInt& rhs) {
	return !(lhs >= rhs);
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs > rhs);
}
bool operator==(const BigInt& lhs, const BigInt& rhs) {
	
	if (lhs.isNegative != rhs.isNegative) return false;

	auto l = lhs.num.size();
	auto r = rhs.num.size();
	if (l != r) return false;
	
	int i = l-1;
	while (i >= 0) {
		if (lhs.num[i] != rhs.num[i]) return false;
		i -= 1;
	}
	return lhs.lsb == rhs.lsb;
}
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
	return !(lhs == rhs);
}


void BigInt::add_unchecked(const BigInt& other) {
	
	int_t both = lsb + other.lsb;
	int_t carry = both / SEGMOD;
	lsb = both % SEGMOD;
	int i = 0;
	for (; i < other.num.size(); ++i) {
		both = num[i] + other.num[i] + carry;
		carry = both / SEGMOD;
		num[i] = both % SEGMOD;
	}
	for (; i < num.size(); ++i) {
		both = num[i] + carry;
		carry = both / SEGMOD;
		num[i] = both % SEGMOD;
		if (!carry) {
			return;
		}
	}
	if (carry) {
		num.push_back(carry);
	}
}

void BigInt::sub_unchecked(const BigInt& other) {
	
	int_t both = lsb - other.lsb;
	int_t borrow = 0;
	if (both < 0) {
		borrow = 1;
		both = SEGMOD + both;
	}
	lsb = both;
	size_t i = 0;
	for (auto s = other.num.size(); i < s; ++i) {
		both = num[i] - other.num[i] - borrow;
		borrow = 0;
		if (both < 0) {
			borrow = 1;
			both = SEGMOD + both;
		}
		num[i] = both;
	}
	//num is max

	for (auto s = num.size(); i < s; ++i) {
		both = num[i] - borrow;
		borrow = 0;
		if (both < 0) {
			borrow = 1;
			both = SEGMOD + both;
		}
		num[i] = both;
		if (!borrow) {
			break;
		}
	}
	while (num.size() && num[num.size() - 1] == 0) {
		num.pop_back();
	}
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
	
	auto& max = lhs.num.size() > rhs.num.size() ? lhs : rhs;
	auto& min = lhs.num.size() <= rhs.num.size() ? lhs : rhs;
	auto res(max);
	if (max.isNegative == min.isNegative) {
		res.add_unchecked(min);
	}
	else {
		res.sub_unchecked(min);
	}
	return res;

}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
	
	auto& max = lhs.num.size() > rhs.num.size() ? lhs : rhs;
	auto& min = lhs.num.size() <= rhs.num.size() ? lhs : rhs;
	auto res(max);
	if (max.isNegative == min.isNegative) {
		res.sub_unchecked(min);
		if (&max == &rhs) {
			res.isNegative = !res.isNegative;
		}
	}
	else {
		res.add_unchecked(min);
		res.isNegative = lhs.isNegative;
	}
	return res;

}

void BigInt::print() {
	if (isNegative) {
		std::cout << '-';
	}
	if (num.size() > 0) {
		std::cout << num[num.size() - 1] << '|';
		for (int i = num.size() - 2; i >= 0; --i) {
			std::cout << std::setw(SAFEDIGITS) << std::setfill('0') << num[i] << '|';
		}
		std::cout << std::setw(SAFEDIGITS) << std::setfill('0') << lsb << std::endl;

	}
	else {
		std::cout << lsb << std::endl;
	}
	
}
