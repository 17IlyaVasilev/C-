#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BigInteger {
private:
	vector <int> v;
	bool minus;
	void swap(BigInteger&);
	void zero_destroy();

public:
	BigInteger();
	BigInteger(int);
	friend std::ostream& operator <<(std::ostream& out, const BigInteger& s);
	friend std::istream& operator >>(std::istream& in, BigInteger& s);

	friend bool operator==(const BigInteger&, const BigInteger&);
	friend bool operator==(const BigInteger&, const int&);
	friend bool operator==(const int&, const BigInteger&);

	friend bool operator!=(const BigInteger&, const BigInteger&);
	friend bool operator!=(const BigInteger&, const int&);
	friend bool operator!=(const int&, const BigInteger&);

	friend bool operator<=(const BigInteger&, const BigInteger&);
	friend bool operator<=(const BigInteger&, const int&);
	friend bool operator<=(const int&, const BigInteger&);

	friend bool operator>=(const BigInteger&, const BigInteger&);
	friend bool operator>=(const BigInteger&, const int&);
	friend bool operator>=(const int&, const BigInteger&);

	friend bool operator<(const BigInteger&, const BigInteger&);
	friend bool operator<(const BigInteger&, const int&);
	friend bool operator<(const int&, const BigInteger&);

	friend bool operator>(const BigInteger&, const BigInteger&);
	friend bool operator>(const BigInteger&, const int&);
	friend bool operator>(const int&, const BigInteger&);

	BigInteger& operator=(BigInteger a) {
		swap(a);
		return *this;
	}

	BigInteger& operator=(int a) {
		BigInteger b(a);
		swap(b);
		return *this;
	}

	BigInteger& operator-=(const BigInteger& a) {
		BigInteger b = 0;
		BigInteger s = 0;
		if (a.minus != minus) {
			BigInteger k = *this;
			BigInteger q = a;
			k.minus = false;
			q.minus = false;
			if (q > k) {
				b = a;
				s = *this;
			}
			else {
				b = *this;
				s = a;
			}
			size_t sz = b.v.size();
			for (size_t i = 0; i < sz; ++i) {
				if (i < s.v.size()) {
					b.v[i] += s.v[i];
					if (b.v[i] > 9) {
						if (i + 1 == sz) b.v.push_back(0);
						b.v[i] -= 10;
						++b.v[i + 1];
					}
				}
			}
			size_t i = sz;
			b.v.push_back(0);
			while (b.v[i] < 0 && b.v.size() > i + 1) {
				b.v[i] += 10;
				--b.v[i + 1];
				++i;
			}
			if (a.minus) b.minus = false; else b.minus = true;
			b.zero_destroy();
			*this = b;
			return *this;
		}
		else {
			BigInteger k = *this;
			BigInteger q = a;
			k.minus = false;
			q.minus = false;
			if (q > k) {
				b = a;
				s = *this;
				if (a.minus) b.minus = false; else b.minus = true;
			}
			else {
				b = *this;
				s = a;
			}
			size_t sz = s.v.size();
			for (size_t i = 0; i < sz; ++i) {
				if (b.v[i] < s.v[i]) {
					b.v[i] += 10;
					--b.v[i + 1];
				}
				b.v[i] -= s.v[i];
			}
			size_t i = sz;
			b.v.push_back(0);
			while (b.v[i] < 0 && b.v.size() > i + 1) {
				b.v[i] += 10;
				--b.v[i + 1];
				++i;
			}
			b.zero_destroy();
			*this = b;
			return *this;
		}
	}


	BigInteger& operator+=(const BigInteger& a) {
		BigInteger b = 0;
		BigInteger s = 0;
		if (a.minus == minus) {
			BigInteger k = *this;
			BigInteger q = a;
			k.minus = false;
			q.minus = false;
			if (q > k) {
				b = a;
				s = *this;
			}
			else {
				b = *this;
				s = a;
			}
			size_t sz = b.v.size();
			for (size_t i = 0; i < sz; ++i) {
				if (i < s.v.size()) {
					b.v[i] += s.v[i];
					if (b.v[i] > 9) {
						if (i + 1 == sz) b.v.push_back(0);
						b.v[i] -= 10;
						++b.v[i + 1];
					}
				}
			}
			size_t i = sz;
			b.v.push_back(0);
			while (b.v[i] < 0 && b.v.size() > i + 1) {
				b.v[i] += 10;
				--b.v[i + 1];
				++i;
			}
			b.zero_destroy();
			*this = b;
			return *this;
		}
		else {
			BigInteger k = *this;
			BigInteger q = a;
			k.minus = false;
			q.minus = false;
			if (q > k) {
				b = a;
				s = *this;
			}
			else {
				b = *this;
				s = a;
			}
			size_t sz = s.v.size();
			for (size_t i = 0; i < sz; ++i) {
				if (b.v[i] < s.v[i]) {
					b.v[i] += 10;
					--b.v[i + 1];
				}
				b.v[i] -= s.v[i];
			}
			size_t i = sz;
			b.v.push_back(0);
			while (b.v[i] < 0 && b.v.size() > i + 1) {
				b.v[i] += 10;
				--b.v[i + 1];
				++i;
			}
			b.zero_destroy();
			*this = b;
			return *this;
		}
	}

	BigInteger& operator*=(const BigInteger& a) {
		BigInteger b = 0;
		BigInteger s = 0;
		BigInteger k = *this;
		BigInteger q = a;
		k.minus = false;
		q.minus = false;
		if (q > k) {
			b = a;
			s = *this;
		}
		else {
			b = *this;
			s = a;
		}
		vector<int> bb = b.v;
		vector<int> ss = s.v;
		int rank = 0;
		int degree = 0;
		BigInteger res = 0;
		for (size_t i = 0; i < ss.size(); ++i) {
			vector<int> temp;
			int t = degree;
			degree++;
			while (t > 0) {
				temp.push_back(0);
				t--;
			}
			for (size_t j = 0; j < bb.size(); ++j) {
				temp.push_back((ss[i] * bb[j] + rank) % 10);
				rank = (ss[i] * bb[j] + rank) / 10;
			}
			if (rank > 0) temp.push_back(rank);
			rank = 0;
			BigInteger temp_big;
			temp_big.v = temp;
			res += temp_big;
		}
		if (minus != a.minus) res.minus = true; else res.minus = false;
		res.zero_destroy();
		*this = res;
		return *this;
	}

	BigInteger& operator/=(const BigInteger& a) {
		BigInteger k = *this;
		BigInteger q = a;
		k.minus = false;
		q.minus = false;
		if (q > k) {
			*this = 0;
			minus = false;
			return *this;
		}
		else {
			bool sign;
			if (minus == a.minus) sign = false; else sign = true;
			BigInteger copy;
			BigInteger s = a;
			BigInteger b = *this;
			b.minus = false;
			s.minus = false;
			BigInteger st = 0;
			vector<int> temp;
			vector<int> res;
			size_t i = 0, jj = v.size();
			--jj;
			while (i < s.v.size()) {
				temp.push_back(v[jj]);
				++i;
				--jj;
			}
			reverse(temp.begin(), temp.end());
			copy.v = temp;
			copy.minus = false;
			int j = 0;
			for (j = 0; j < 10; ++j) {
				if (copy < st) {
					if (!(j == 1 && res.size() == 0))
						res.push_back(j - 1);
					for (int l = 0; l < j - 1; ++l) {
						copy -= s;
					}
					break;
				}
				st += s;
			}
			if (j == 10) {
				res.push_back(j - 1);
				for (int l = 0; l < j - 1; ++l) {
					copy -= s;
				}
			}
			for (int i = v.size() - s.v.size(); i > 0; --i) {
				copy *= 10;
				copy += v[i - 1];
				j = 0;
				st = 0;
				for (j = 0; j < 10; ++j) {
					if (copy < st) {
						if (!(j == 1 && res.size() == 0))
							res.push_back(j - 1);
						for (int l = 0; l < j - 1; ++l) {
							copy -= s;
						}
						break;
					}
					st += s;
				}
				if (j == 10) {
					res.push_back(j - 1);
					for (int l = 0; l < j - 1; ++l) {
						copy -= s;
					}
				}
			}
			reverse(res.begin(), res.end());
			v = res;
			minus = sign;
			(*this).zero_destroy();
			return *this;
		}
	}

	BigInteger& operator%=(const BigInteger& a) {
		BigInteger temp = *this;
		BigInteger copy = *this;
		temp /= a;
		temp *= a;
		copy -= temp;
		copy.zero_destroy();
		*this = copy;
		return *this;
	}

	BigInteger& operator-=(const int& a) {
		BigInteger temp = BigInteger(a);
		*this -= temp;
		return *this;
	}

	BigInteger& operator+=(const int& a) {
		BigInteger temp = BigInteger(a);
		*this += temp;
		return *this;
	}

	BigInteger& operator*=(const int& a) {
		BigInteger temp = BigInteger(a);
		*this *= temp;
		return *this;
	}

	BigInteger& operator/=(const int& a) {
		BigInteger temp = BigInteger(a);
		*this /= temp;
		return *this;
	}

	BigInteger& operator%=(const int& a) {
		BigInteger temp = BigInteger(a);
		*this %= temp;
		return *this;
	}

	BigInteger& operator-() {
		if (*this != 0) {
			if (minus) minus = false; else minus = true;
		}
		return *this;
	}

	operator bool() {
		if (v.size() == 0)return false;
		return true;
	}

	BigInteger& operator++() {
		BigInteger copy = *this;
		copy += BigInteger(1);
		copy.zero_destroy();
		*this = copy;
		return *this;
	}

	BigInteger operator++(int) {
		BigInteger copy = *this;
		++(*this);
		return copy;
	}

	BigInteger& operator--() {
		BigInteger copy = *this;
		copy -= BigInteger(1);
		copy.zero_destroy();
		*this = copy;
		return *this;
	}

	BigInteger operator--(int) {
		BigInteger copy = *this;
		--(*this);
		return copy;
	}

	string toString();
	string toString() const;
};

BigInteger::BigInteger() {

}

BigInteger::BigInteger(int i) {
	minus = false;
	if (i == 0) v.push_back(0);
	else {
		if (i < 0) {
			i *= -1;
			minus = true;
		}
		while (i > 0) {
			v.push_back(i % 10);
			i /= 10;
		}
	}
}

void BigInteger::swap(BigInteger& s) {
	std::swap(s.v, v);
	std::swap(s.minus, minus);
}

void BigInteger::zero_destroy() {
	while (v.size() > 1 && v.back() == 0) v.pop_back();
}

BigInteger operator ""_bi(unsigned long long a) {
	return BigInteger(a);
}

string BigInteger::toString() {
	string s = "";
	if (minus) s += "-";
	vector<int> ts = v;
	reverse(ts.begin(), ts.end());
	for (size_t i = 0; i < v.size(); ++i) s += char(48 + ts[i]);
	return s;
}

string BigInteger::toString() const {
	string s = "";
	if (minus) s += "-";
	vector<int> ts = v;
	reverse(ts.begin(), ts.end());
	for (size_t i = 0; i < v.size(); ++i) s += char(48 + ts[i]);
	return s;
}

std::istream& operator >> (std::istream& in, BigInteger& s) {
	char c;
	bool sign = false;
	s = 0;
	do {
		c = in.get();
	} while (isspace(c));
	while (!in.eof() && !isspace(c)) {
		if (c != '-') {
			s *= 10;
			s += BigInteger(int(c) - 48);
		}
		else sign = true;
		c = in.get();
	}
	s.minus = sign;
	return in;
}

std::ostream& operator <<(std::ostream& out, const BigInteger& s) {
	BigInteger copy = s;
	reverse(copy.v.begin(), copy.v.end());
	if (s.minus) copy.v[0] = -copy.v[0];
	for (auto i : copy.v) {
		out << i;
	}
	return out;
}

bool operator==(const BigInteger& a, const BigInteger& b) {
	if (a.v.size() != b.v.size())return false;
	for (size_t i = 0; i < a.v.size(); i++)if (a.v[i] != b.v[i])return false;
	return true;
}

bool operator==(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	if (a.v.size() != temp.v.size())return false;
	for (size_t i = 0; i < a.v.size(); i++)if (a.v[i] != temp.v[i])return false;
	return true;
}

bool operator==(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	if (temp.v.size() != b.v.size())return false;
	for (size_t i = 0; i < temp.v.size(); i++)if (temp.v[i] != b.v[i])return false;
	return true;
}

bool operator!=(const BigInteger& a, const BigInteger& b) {
	return !(a == b);
}

bool operator!=(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	return !(a == temp);
}

bool operator!=(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	return !(temp == b);
}

bool operator<=(const BigInteger& aa, const BigInteger& bb) {
	BigInteger a = aa;
	BigInteger b = bb;
	reverse(a.v.begin(), a.v.end());
	reverse(b.v.begin(), b.v.end());
	if ((a.minus || a == 0) && (!b.minus || b == 0))return true;
	if ((!a.minus || a == 0) && (b.minus || b == 0))return false;
	bool f = false;
	if ((a.minus || a == 0) && (b.minus || b == 0))f = true;
	if (a.v.size() < b.v.size()) {
		if (!f)return true;
		return false;
	}
	if (a.v.size() > b.v.size()) {
		if (!f)return false;
		return true;
	}
	for (size_t i = 0; i < a.v.size(); i++) {
		if (a.v[i] < b.v[i]) {
			if (!f)return true;
			return false;
		}
		if (a.v[i] > b.v[i]) {
			if (!f)return false;
			return true;
		}
	}
	return true;
}

bool operator<=(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	return a <= temp;
}

bool operator<=(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	return temp <= b;
}

bool operator>=(const BigInteger& aa, const BigInteger& bb) {
	BigInteger a = aa;
	BigInteger b = bb;
	reverse(a.v.begin(), a.v.end());
	reverse(b.v.begin(), b.v.end());
	if ((a.minus || a == 0) && (!b.minus || b == 0))return false;
	if ((!a.minus || a == 0) && (b.minus || b == 0))return true;
	bool f = false;
	if ((a.minus || a == 0) && (b.minus || b == 0))f = true;
	if (a.v.size() < b.v.size()) {
		if (f)return true;
		return false;
	}
	if (a.v.size() > b.v.size()) {
		if (f)return false;
		return true;
	}
	for (size_t i = 0; i < a.v.size(); i++) {
		if (a.v[i] < b.v[i]) {
			if (f)return true;
			return false;
		}
		if (a.v[i] > b.v[i]) {
			if (f)return false;
			return true;
		}
	}
	return true;
}

bool operator>=(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	return a >= temp;
}

bool operator>=(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	return temp >= b;
}

bool operator<(const BigInteger& aa, const BigInteger& bb) {
	return (aa <= bb && aa != bb);
}

bool operator<(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	return (a <= temp && a != temp);
}

bool operator<(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	return (temp <= b && temp != b);
}

bool operator>(const BigInteger& aa, const BigInteger& bb) {
	return (aa >= bb && aa != bb);
}

bool operator>(const BigInteger& a, const int& b) {
	const BigInteger temp = BigInteger(b);
	return (a >= temp && a != temp);
}

bool operator>(const int& a, const BigInteger& b) {
	BigInteger temp = BigInteger(a);
	return (temp >= b && temp != b);
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
	BigInteger copy = a;
	copy -= b;
	return copy;
}

BigInteger operator-(const int& a, const BigInteger& b) {
	BigInteger copy = BigInteger(a);
	copy -= b;
	return copy;
}

BigInteger operator-(const BigInteger& a, const int& b) {
	BigInteger copy = a;
	copy -= BigInteger(b);
	return copy;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
	BigInteger copy = a;
	copy += b;
	return copy;
}

BigInteger operator+(const int& a, const BigInteger& b) {
	BigInteger copy = BigInteger(a);
	copy += b;
	return copy;
}

BigInteger operator+(const BigInteger& a, const int& b) {
	BigInteger copy = BigInteger(b);
	copy += a;
	return copy;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
	BigInteger copy = a;
	copy *= b;
	return copy;
}

BigInteger operator*(const int& a, const BigInteger& b) {
	BigInteger copy = BigInteger(a);
	copy *= b;
	return copy;
}

BigInteger operator*(const BigInteger& a, const int& b) {
	BigInteger copy = BigInteger(b);
	copy *= a;
	return copy;
}


BigInteger operator/(const BigInteger& a, const BigInteger& b) {
	BigInteger copy = a;
	copy /= b;
	return copy;
}

BigInteger operator/(const int& a, const BigInteger& b) {
	BigInteger copy = BigInteger(a);
	copy /= b;
	return copy;
}

BigInteger operator/(const BigInteger& a, const int& b) {
	BigInteger copy = a;
	copy /= BigInteger(b);
	return copy;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
	BigInteger copy = a;
	copy %= b;
	return copy;
}

BigInteger operator%(const int& a, const BigInteger& b) {
	BigInteger copy = BigInteger(a);
	copy %= b;
	return copy;
}

BigInteger operator%(const BigInteger& a, const int& b) {
	BigInteger copy = a;
	copy %= BigInteger(b);
	return copy;
}

BigInteger nod(BigInteger a, BigInteger b) {
	if (a < 0) {
		a *= -1;
	}
	if (b < 0) {
		b *= -1;
	}
	while (a > 0 && b > 0) {
		if (a > b)a %= b;
		else b %= a;
	}
	a += b;
	return a;
}

class Rational {
private:
	BigInteger up = 0;
	BigInteger down = 1;

public:
	Rational() = default;
	Rational(const BigInteger&);
	Rational(const int&);
	~Rational() {};

	Rational& operator= (const Rational& a) {
		up = a.up;
		down = a.down;
		return *this;
	}

	Rational& operator= (const BigInteger& a) {
		up = a;
		down = 1;
		return *this;
	}

	Rational& operator= (const int& a) {
		up = a;
		down = 1;
		return *this;
	}

	Rational& operator-= (const Rational& a) {
		up = up * a.down - down * a.up;
		down = down * a.down;
		BigInteger Nod = nod(down, up);
		up /= Nod;
		down /= Nod;
		if (down < 0) {
			up = -up;
			down = -down;
		}
		return *this;
	}

	Rational& operator-= (const BigInteger& a) {
		Rational copy = *this;
		copy -= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator-= (const int& a) {
		Rational copy = *this;
		copy -= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator+= (const Rational& a) {
		up = up * a.down + down * a.up;
		down = down * a.down;
		BigInteger Nod = nod(down, up);
		up = up / Nod;
		down = down / Nod;
		if (down < 0) {
			up = -up;
			down = -down;
		}
		return *this;
	}

	Rational& operator+= (const BigInteger& a) {
		Rational copy = *this;
		copy += Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator+= (const int& a) {
		Rational copy = *this;
		copy += Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator*= (const Rational& a) {
		up = up * a.up;
		down = down * a.down;
		BigInteger Nod = nod(down, up);
		up = up / Nod;
		down = down / Nod;
		if (down < 0) {
			up = -up;
			down = -down;
		}
		return *this;
	}

	Rational& operator*= (const BigInteger& a) {
		Rational copy = *this;
		copy *= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator*= (const int& a) {
		Rational copy = *this;
		copy *= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator/= (const Rational& a) {
		up = up * a.down;
		down = down * a.up;
		BigInteger Nod = nod(down, up);
		up = up / Nod;
		down = down / Nod;
		if (down < 0) {
			up = -up;
			down = -down;
		}
		return *this;
	}

	Rational& operator/= (const BigInteger& a) {
		Rational copy = *this;
		copy /= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator/= (const int& a) {
		Rational copy = *this;
		copy /= Rational(a);
		*this = copy;
		return *this;
	}

	Rational& operator- () {
		up = -up;
		return *this;
	}

	explicit operator double () {
		string copy = asDecimal(310);
		size_t beg = copy.find(".");
		double k = 1, sum = 0;
		for (int i = beg - 1; i >= 0; --i) {
			char c = copy[i];
			double x = int(c) - 48;
			sum = sum + x * k;
			k = k * 10;
		}
		k = 0.1;
		for (size_t i = beg + 1; i <= 307; ++i) {
			char c = copy[i];
			double x = int(c) - 48;
			sum = sum + x * k;
			k = k / 10;
		}
		return sum;
	}

	friend bool operator==(const Rational&, const Rational&);
	friend bool operator==(const Rational&, const BigInteger&);
	friend bool operator==(const Rational&, const int&);
	friend bool operator==(const BigInteger&, const Rational&);
	friend bool operator==(const int&, const Rational&);

	friend bool operator!=(const Rational&, const Rational&);
	friend bool operator!=(const Rational&, const BigInteger&);
	friend bool operator!=(const Rational&, const int&);
	friend bool operator!=(const BigInteger&, const Rational&);
	friend bool operator!=(const int&, const Rational&);

	friend bool operator>(const Rational&, const Rational&);
	friend bool operator>(const Rational&, const BigInteger&);
	friend bool operator>(const Rational&, const int&);
	friend bool operator>(const BigInteger&, const Rational&);
	friend bool operator>(const int&, const Rational&);

	friend bool operator<(const Rational&, const Rational&);
	friend bool operator<(const Rational&, const BigInteger&);
	friend bool operator<(const Rational&, const int&);
	friend bool operator<(const BigInteger&, const Rational&);
	friend bool operator<(const int&, const Rational&);

	friend bool operator>=(const Rational&, const Rational&);
	friend bool operator>=(const Rational&, const BigInteger&);
	friend bool operator>=(const Rational&, const int&);
	friend bool operator>=(const BigInteger&, const Rational&);
	friend bool operator>=(const int&, const Rational&);

	friend bool operator<=(const Rational&, const Rational&);
	friend bool operator<=(const Rational&, const BigInteger&);
	friend bool operator<=(const Rational&, const int&);
	friend bool operator<=(const BigInteger&, const Rational&);
	friend bool operator<=(const int&, const Rational&);

	friend Rational operator/(const BigInteger&, const Rational&);
	friend Rational operator/(const int&, const Rational&);

	string toString();
	string asDecimal(size_t);
};


Rational::Rational (const BigInteger& a) {
	up = a;
	down = 1;
}

Rational::Rational (const int& a) {
	up = a;
	down = 1;
}

Rational operator+ (const Rational& a, const Rational& b) {
	Rational copy = a;
	copy += b;
	return copy;
}
Rational operator+ (const Rational& a, const BigInteger& b) {
	Rational copy = a;
	copy += b;
	return copy;
}
Rational operator+ (const Rational& a, const int& b) {
	Rational copy = a;
	copy += b;
	return copy;
}

Rational operator+ (const BigInteger& b, const Rational& a) {
	Rational copy = a;
	copy += b;
	return copy;
}

Rational operator+ (const int& b, const Rational& a) {
	Rational copy = a;
	copy += b;
	return copy;
}

Rational operator- (const Rational& a, const Rational& b) {
	Rational copy = a;
	copy -= b;
	return copy;
}

Rational operator- (const Rational& a, const BigInteger& b) {
	Rational copy = a;
	copy -= b;
	return copy;
}

Rational operator- (const Rational& a, const int& b) {
	Rational copy = a;
	copy -= b;
	return copy;
}

Rational operator- (const BigInteger& b, const Rational& a) {
	Rational copy = a;
	copy *= -1;
	copy += b;
	return copy;
}
Rational operator- (const int& b, const Rational& a) {
	Rational copy = a;
	copy *= -1;
	copy += b;
	return copy;
}

Rational operator* (const Rational& a, const Rational& b) {
	Rational copy = a;
	copy *= b;
	return copy;
}
Rational operator* (const Rational& a, const BigInteger& b) {
	Rational copy = a;
	copy *= b;
	return copy;
}

Rational operator* (const Rational& a, const int& b) {
	Rational copy = a;
	copy *= b;
	return copy;
}

Rational operator* (const BigInteger& b, const Rational& a) {
	Rational copy = a;
	copy *= b;
	return copy;
}

Rational operator* (const int& b, const Rational& a) {
	Rational copy = a;
	copy *= b;
	return copy;
}

Rational operator/ (const Rational& a, const Rational& b) {
	Rational copy = a;
	copy /= b;
	return copy;
}
Rational operator/ (const Rational& a, const BigInteger& b) {
	Rational copy = a;
	copy /= b;
	return copy;
}
Rational operator/ (const Rational& a, const int& b) {
	Rational copy = a;
	copy /= b;
	return copy;
}
Rational operator/ (const BigInteger& b, const Rational& a) {
	Rational copy;
	copy.up = a.down;
	copy.down = a.up;
	copy *= b;
	BigInteger Nod = nod(copy.down, copy.up);
	copy.up /= Nod;
	copy.down /= Nod;
	return copy;
}

Rational operator/ (const int& b, const Rational& a) {
	Rational copy;
	copy.up = a.down;
	copy.down = a.up;
	copy *= b;
	BigInteger Nod = nod(copy.down, copy.up);
	copy.up /= Nod;
	copy.down /= Nod;
	return copy;
}

bool operator==(const Rational& a, const Rational& b) {
	return (a.up == b.up && a.down == b.down);
}

bool operator==(const Rational& a, const BigInteger& b) {
	return (a.up == b && a.down == 1);
}

bool operator==(const Rational& a, const int& b) {
	return (a.up == b && a.down == 1);
}

bool operator==(const BigInteger& b, const Rational& a) {
	return (a.up == b && a.down == 1);
}

bool operator==(const int& b, const Rational& a) {
	return (a.up == b && a.down == 1);
}

bool operator!=(const Rational& a, const Rational& b) {
	return !(a.up == b.up && a.down == b.down);
}

bool operator!=(const Rational& a, const BigInteger& b) {
	return !(a.up == b && a.down == 1);
}

bool operator!=(const Rational& a, const int& b) {
	return !(a.up == b && a.down == 1);
}

bool operator!=(const BigInteger& b, const Rational& a) {
	return !(a.up == b && a.down == 1);
}

bool operator!=(const int& b, const Rational& a) {
	return !(a.up == b && a.down == 1);
}

bool operator>(const Rational& a, const Rational& b) {
	return (a.up * b.down > a.down * b.up);
}

bool operator>(const Rational& a, const BigInteger& b) {
	return (a.up > a.down * b);
}

bool operator>(const Rational& a, const int& b) {
	return (a.up > a.down * b);
}

bool operator>(const BigInteger& b, const Rational& a) {
	return (a.up < a.down* b);
}

bool operator>(const int& b, const Rational& a) {
	return (a.up < a.down* b);
}

bool operator<(const Rational& a, const Rational& b) {
	return (a.up * b.down < a.down* b.up);
}

bool operator<(const Rational& a, const BigInteger& b) {
	return (a.up < a.down* b);
}

bool operator<(const Rational& a, const int& b) {
	return (a.up < a.down* b);
}

bool operator<(const BigInteger& b, const Rational& a) {
	return (a.up > a.down * b);
}

bool operator<(const int& b, const Rational& a) {
	return (a.up > a.down * b);
}

bool operator>=(const Rational& a, const Rational& b) {
	return (a.up * b.down >= a.down * b.up);
}

bool operator>=(const Rational& a, const BigInteger& b) {
	return (a.up >= a.down * b);
}

bool operator>=(const Rational& a, const int& b) {
	return (a.up >= a.down * b);
}

bool operator>=(const BigInteger& b, const Rational& a) {
	return (a.up <= a.down * b);
}

bool operator>=(const int& b, const Rational& a) {
	return (a.up <= a.down * b);
}

bool operator<=(const Rational& a, const Rational& b) {
	return (a.up * b.down <= a.down * b.up);
}

bool operator<=(const Rational& a, const BigInteger& b) {
	return (a.up <= a.down * b);
}

bool operator<=(const Rational& a, const int& b) {
	return (a.up <= a.down * b);
}

bool operator<=(const BigInteger& b, const Rational& a) {
	return (a.up >= a.down * b);
}

bool operator<=(const int& b, const Rational& a) {
	return (a.up >= a.down * b);
}

string Rational::toString() {
	string copy;
	BigInteger x = up;
	if (x < 0) {
		copy += "-";
		x *= -1;
	}
	if (x == 0) {
		copy += "0";
		return copy;
	}
	else {
		string str = x.toString();
		copy += str;
		if (down == 1) {
			return copy;
		}
		else {
			BigInteger x = down;
			copy += "/";
			string str = x.toString();
			copy += str;
			return copy;
		}
	}
}

string Rational::asDecimal (size_t precision = 0) {
	BigInteger a = up / down;
	BigInteger b = up % down;
	BigInteger c = down;
	string copy;
	if (up < 0) copy = "-"; else copy = "";
	string ccopy = a.toString();
	if (up < 0) copy = "-"; else copy = "";
	copy += ccopy;
	if (precision == 0) {
		return copy;
	}
	else {
		precision++;
		copy += ".";
		for (size_t i = 0; i < precision; ++i) {
			a = b * 10;
			b = a % c;
			a /= c;
			if (a < 0) a *= -1;
			if (a == 0) copy += "0"; else
				if (a == 1) copy += "1"; else
					if (a == 2) copy += "2"; else
						if (a == 3) copy += "3"; else
							if (a == 4) copy += "4"; else
								if (a == 5) copy += "5"; else
									if (a == 6) copy += "6"; else
										if (a == 7) copy += "7"; else
											if (a == 8) copy += "8"; else
												if (a == 9) copy += "9";
		}
		if (int(copy.back()) - 48 >= 5) {
			copy.pop_back();
			int i = copy.size() - 1;
			while (true) {
				if (copy[i] != '9') {
					copy[i] = char(int(copy[i]) + 1);
					break;
				}
				copy[i] = '0';
				i--;
				if (copy[i] == '.')break;
			}
			if (copy[i] == '.')copy = (up / down + 1).toString();
			return copy;
		}
		else {
			copy.pop_back();
			return copy;
		}
	}
}
