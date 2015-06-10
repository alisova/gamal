#include "bint.h"

bint::bint()
{
	this->sign = false;
}

bint::bint(string str)
{
	if (str.length() == 0)
		this->sign = false;
	else
	{
		if (str[0] == '-')
		{
			str = str.substr(1);

			this->sign = true;
		}
		else
			this->sign = false;

		for (long long i = str.length(); i > 0; i -= 9)
			i < 9 ? this->elements.push_back(atoi(str.substr(0, i).c_str())) : this->elements.push_back(atoi(str.substr(i - 9, 9).c_str()));

		this->removezeros();
	}
}

bint::bint(int i)
{
	this->sign = i < 0 ? this->sign = true : this->sign = false;

	this->elements.push_back(abs(i) % bint::OSN);

	i /= bint::OSN;

	if (i != 0)
		this->elements.push_back(abs(i));
}

bint::operator string() const
{
	stringstream s;

	s << *this;

	return s.str();
}

string bint::toString()
{
	stringstream s;

	s << *this;

	return s.str();
}

string bint::__str__()
{
	return toString();
}

string bint::__repr__()
{
	return toString();
}

string bint::read(char* c)
{
	ifstream ifst(c);

	string str;

	ifst >> str;

	ifst.close();

	return str;
}

void bint::write(string str, char* c)
{
	ofstream ofst(c);

	ofst << str;

	ofst.close();
}

bint bint::readbin(char* c)
{
	ifstream ifst;
	ifst.open(c, ios::binary | ios::in);

	bint b;

	ifst.seekg(0, ifst.end);
	long long size = (long long)ifst.tellg();
	ifst.seekg(0);

	b.elements.resize(size / 9);

	for (int i = 0; i < size / 9; i++)
		ifst.read((char*)&b.elements[i], 9);

	ifst.read((char*)&b.sign, 1);

	ifst.close();

	return b;
}

void bint::writebin(const bint& b, char* c)
{
	ofstream ofst;
	ofst.open(c, ios::binary | ios::out);

	for (long long i = 0; i < (long long)b.elements.size(); i++)
		ofst.write((char*)&(b.elements[i]), 9);

	ofst.write((char*)&b.sign, 1);

	ofst.close();
}

ostream& operator <<(ostream& os, const bint& b)
{
	if (b.elements.empty())
		os << 0;
	else
	{
		if (b.sign)
			os << '-';

		os << b.elements.back();

		char old = os.fill('0');

		for (long long i = (long long)b.elements.size() - 2; i >= 0; --i)
			os << setw(9) << b.elements[i];

		os.fill(old);
	}

	return os;
}

istream& operator >>(istream& is, bint& b)
{
	string str;

	is >> str;

	bint a(str);

	b = a;

	return is;
}

bool bint::operator ==(const bint& right)
{
	if (this->sign != right.sign)
		return false;

	if (this->elements.empty())
		return (right.elements.empty() || (right.elements.size() == 1 && right.elements[0] == 0));

	if (right.elements.empty())
		return (this->elements.size() == 1 && this->elements[0] == 0);

	if (this->elements.size() != right.elements.size())
		return false;

	for (size_t i = 0; i < this->elements.size(); ++i)
	if (this->elements[i] != right.elements[i])
		return false;

	return true;
}

bool bint::operator !=(const bint& right)
{
	return !(*this == right);
}

bool bint::operator <(const bint& right)
{
	if (*this == right)
		return false;

	if (this->sign)
	{
		if (right.sign)
			return ((-right) < (-*this));
		else
			return true;
	}
	else if (right.sign)
		return false;
	else
	{
		if (this->elements.size() != right.elements.size())
			return this->elements.size() < right.elements.size();
		else
		{
			for (long long i = this->elements.size() - 1; i >= 0; --i)
			if (this->elements[i] != right.elements[i])
				return this->elements[i] < right.elements[i];

			return false;
		}
	}
}

bool bint::operator >(const bint& right)
{
	return !(*this <= right);
}

bool bint::operator <=(const bint& right)
{
	return (*this < right || *this == right);
}

bool bint::operator >=(const bint& right)
{
	return !(*this < right);
}

bint bint::operator +(const bint& right) const
{
	bint left(*this);

	if (left.sign && !right.sign)
		return right - (left.neg());
	else if (!left.sign && right.sign)
		return left - (-right);

	int carryflag = 0;

	for (size_t i = 0; i < max(left.elements.size(), right.elements.size()) || carryflag != 0; ++i)
	{
		if (i == left.elements.size())
			left.elements.push_back(0);

		left.elements[i] += carryflag + (i < right.elements.size() ? right.elements[i] : 0);

		carryflag = left.elements[i] >= bint::OSN;

		if (carryflag != 0)
			left.elements[i] -= bint::OSN;
	}

	return left;
}

bint bint::operator -(const bint& right) const
{
	bint left(*this);

	if (right.sign)
		return left + (-right);
	else if (left.sign)
		return (left.neg() + right).neg();
	else if (left < right)
		return (right - left).neg();

	int carryflag = 0;

	for (size_t i = 0; i < right.elements.size() || carryflag != 0; ++i)
	{
		left.elements[i] -= carryflag + (i < right.elements.size() ? right.elements[i] : 0);

		carryflag = left.elements[i] < 0;

		if (carryflag != 0)
			left.elements[i] += bint::OSN;
	}

	left.removezeros();

	return left;
}

bint bint::operator *(const bint& right) const
{
	bint result;

	result.elements.resize(this->elements.size() + right.elements.size());

	for (size_t i = 0; i < this->elements.size(); ++i)
	{
		int carryflag = 0;

		for (size_t j = 0; j < right.elements.size() || carryflag != 0; ++j)
		{
			long long cur = result.elements[i + j] + this->elements[i] * 1LL * (j < right.elements.size() ? right.elements[j] : 0) + carryflag;

			result.elements[i + j] = (int)(cur % bint::OSN);

			carryflag = (int)(cur / bint::OSN);
		}
	}

	result.sign = this->sign != right.sign;

	result.removezeros();

	return result;
}

bint bint::operator /(const bint& right) const
{
	bint b(right), result, current;

	b.sign = false;

	if (b == 0)
		throw bint::divzero();

	result.elements.resize(this->elements.size());

	for (long long i = (long long)this->elements.size() - 1; i >= 0; --i)
	{
		current.shiftright();

		current.elements[0] = this->elements[i];

		current.removezeros();

		int x = 0, l = 0, r = bint::OSN;

		while (l <= r)
		{
			int m = (l + r) / 2;

			bint t = b * m;

			if (t <= current)
			{
				x = m;
				l = m + 1;
			}
			else
				r = m - 1;
		}

		result.elements[i] = x;

		current = current - b * x;
	}

	result.sign = this->sign != right.sign;

	result.removezeros();

	return result;
}

bint bint::operator %(const bint& right) const
{
	bint result = *this - (*this / right) * right;

	if (result.sign)
		result += right;

	return result;
}

bint bint::operator ^(const bint& right) const
{
	bint a(*this), b(right), result(1);

	while (b != 0)
	{
		if (b.odd())
			result *= a;

		a *= a;

		b /= 2;
	}

	return result;
}

bint bint::powmod(bint& a, bint& k, bint& n)
{
    bint res(1), aa = a, kk = k;

    while (kk != 0)
    {
        if ((kk % 2) == 1)
			res = (res * aa) % n;
        
		aa = (aa * aa) % n;

        kk /= 2;
    }

    return res;
}

bint bint::operator +=(const bint& right)
{
	return *this = *this + right;
}

bint bint::operator -=(const bint& right)
{
	return *this = *this - right;
}

bint bint::operator *=(const bint& right)
{
	return *this = *this * right;
}

bint bint::operator /=(const bint& right)
{
	return *this = *this / right;
}

bint bint::operator %=(const bint& right)
{
	return *this = *this % right;
}

bint bint::operator ^=(const bint& right)
{
	return *this = *this ^ right;
}

bint bint::operator -() const
{
	bint copy(*this);

	return copy.neg();
}

bint bint::operator ++()
{
	return *this += 1;
}

bint bint::operator ++(int)
{
	*this += 1;

	return *this - 1;
}

bint bint::operator --()
{
	return *this -= 1;
}

bint bint::operator --(int)
{
	*this -= 1;

	return *this + 1;
}

bool bint::odd() const
{
	if (this->elements.size() == 0)
		return false;

	return this->elements[0] & 1;
}

bool bint::even() const
{
	return !this->odd();
}

void bint::removezeros()
{
	while (this->elements.size() > 1 && this->elements.back() == 0)
		this->elements.pop_back();

	if (this->elements.size() == 1 && this->elements[0] == 0)
		this->sign = false;
}

void bint::shiftright()
{
	if (this->elements.size() == 0)
	{
		this->elements.push_back(0);

		return;
	}

	this->elements.push_back(this->elements[this->elements.size() - 1]);

	for (size_t i = this->elements.size() - 2; i > 0; --i)
		this->elements[i] = this->elements[i - 1];

	this->elements[0] = 0;
}

bint bint::neg()
{
	this->sign = !this->sign;

	return *this;
}
