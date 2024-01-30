#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

const int P = 1e9 + 7;

int qpow(int a, int n) {
	int res = 1;
	while (n) {
		if (n & 1) {
			res = (1LL * res * a % P + P) % P;
		}
		a = 1LL * a * a % P;
		n >>= 1;
	}
	return (res + P) % P;
}
// 项
struct Term {
	// x0, x1, ... x100
	long long coef;
	int times[101];
	Term() {
		coef = 0;
		memset(times, 0, sizeof(times));
	}
	// 可以是非同类项
	Term operator*(const Term& rhs) {
		Term newTerm;
		newTerm.coef = ((this->coef * rhs.coef) % P + P) % P;
		for (int i = 0; i <= 100; i++) {
			newTerm.times[i] = this->times[i] + rhs.times[i];
		}
		return newTerm;
	}
	Term& operator*=(const Term& rhs) {
		*this = (*this) * rhs;
		return *this;
	}

	// 必须是同类项
	Term operator+(const Term& rhs) {

		Term newTerm;
		newTerm.coef = (this->coef + rhs.coef) % P;
		for (int i = 0; i <= 100; i++) {
			newTerm.times[i] = this->times[i];
		}
		return newTerm;
	}
	Term& operator+=(const Term& rhs) {
		*this = (*this) + rhs;
		return *this;
	}
	Term& operator-() {
		this->coef = -this->coef;
		return *this;
	}
	// 必须是同类项
	Term operator-(const Term& rhs) {

		Term newTerm;
		newTerm.coef = (this->coef - rhs.coef + P) % P;
		for (int i = 0; i <= 100; i++) {
			newTerm.times[i] = this->times[i];
		}
		return newTerm;
	}
	Term& operator-=(const Term& rhs) {
		*this = (*this) - rhs;
		return *this;
	}

	Term& operator=(const Term& rhs) {
		coef = rhs.coef;
		memcpy(times, rhs.times, sizeof(times));
		return *this;
	}

	// 判定同类项
	bool operator==(const Term& rhs) {
		for (int i = 0; i <= 100; i++) {
			if (times[i] != rhs.times[i]) {
				return false;
			}
		}
		return true;
	}
	bool operator==(const int rhs) {
		for (int i = 0; i <= 100; i++) {
			if (times[i] != rhs) {
				return false;
			}
		}
		return coef == 0;
	}
};

struct Poly {
	vector<Term> terms; // 一共有几项
	Poly() {

	}
	Poly(string _s) { // 多项式初始化
		terms.resize(1);
		// cout << _s << "\n";
		if (_s[0] == 'x') {
			// 含有x的项，对应的次数为1，系数为1
			int idx = std::stoi(_s.substr(1), nullptr, 10);
			terms[0].times[idx] = 1;
			terms[0].coef = 1;
		}
		else {
			// 常数项， 次数为0， 系数为本身
			terms[0].times[0] = 0;
			terms[0].coef = std::stoi(_s, nullptr, 10);
		}
	}
	Poly(Term term) {
		terms.resize(1);
		terms[0] = term;
	}

	Poly& operator= (const Poly& rhs) {
		this->terms = rhs.terms;
		return *this;
	}
	// x1 + x1 * 2 + x1 ** 2 =	3x1 + x1 ^ 2
	// x1 + x2 * 2 =			x1 + x2 ^ 2
	Poly operator+ (const Poly& _r) {
		Poly newPoly, rhs = _r;

		// 检查第一个多项式的每一项的“所有未知数”的次数是否与第二个多项式的相同，若相同则合并，否则添加在第二个后面
		for (int i = 0; i < (int)terms.size(); i++) {
			bool same = false;
			for (int j = 0; j < (int)rhs.terms.size(); j++) {
				if (terms[i] == rhs.terms[j] || terms[i] == 0 || rhs.terms[j] == 0) {
					rhs.terms[j] += terms[i];
					same = true;
					break;
				}
			}
			// 无法合并，放进去 
			if (!same) {
				newPoly.terms.push_back(terms[i]);
			}
		}
		// rhs的放进去
		for (int j = 0; j < (int)rhs.terms.size(); j++) {
			newPoly.terms.push_back(rhs.terms[j]);
		}

		return newPoly;
	}

	Poly& operator+= (const Poly& _r) {
		*this = *this + _r;
		return *this;
	}

	Poly operator- (const Poly& _r) {
		Poly newPoly, rhs = _r;

		// 检查第一个多项式的每一项的“所有未知数”的次数是否与第二个多项式的相同，若相同则合并，否则添加在第二个后面
		for (int i = 0; i < (int)terms.size(); i++) {
			bool same = false;
			for (int j = 0; j < (int)rhs.terms.size(); j++) {
				if (terms[i] == rhs.terms[j] || terms[i] == 0 || rhs.terms[j] == 0) {
					rhs.terms[j] -= terms[i];
					same = true;
					break;
				}
			}
			// 无法合并，放进去 
			if (!same) {
				newPoly.terms.push_back(terms[i]);
			}
		}
		// rhs的放进去
		for (int j = 0; j < (int)rhs.terms.size(); j++) {
			newPoly.terms.push_back((-rhs.terms[j]));
			(newPoly.terms.back().coef += P) %= P;
		}

		return newPoly;
	}

	Poly& operator-= (const Poly& _r) {
		*this = *this - _r;
		return *this;
	}

	Poly operator* (const Poly& rhs) {
		Poly newPoly;

		// 对应相乘
		for (int i = 0; i < (int)terms.size(); i++) {
			for (int j = 0; j < (int)rhs.terms.size(); j++) {
				newPoly += Poly(terms[i] * rhs.terms[j]);
			}
		}

		return newPoly;
	}

	Poly& operator*= (const Poly& _r) {
		*this = *this * _r;
		return *this;
	}

	int partial_derivative(int which, vector<int>& a) {
		int ans = 0;
		for (int i = 0; i < (int)terms.size(); i++) {
			int res = 0;
			if (terms[i].coef != 0 && terms[i].times[which] > 0) {
				res = (terms[i].times[which] * terms[i].coef) % P;
				terms[i].times[which] -= 1;
				for (int j = 1; j < a.size(); j++) {
					res = (1LL * res * qpow(a[j], terms[i].times[j]) % P);
				}
			}
			ans = (ans + res) % P;
		}
		return ans;
	}

	friend ostream& operator<<(ostream& os, const Poly& rhs) {
		cout << "terms.size() = " << rhs.terms.size() << "\n";
		for (int i = 0; i < rhs.terms.size(); i++) {
			if (i > 0) {
				cout << " + ";
			}
			int cnt = 0;

			for (int j = 0; j <= 10; j++) {
				if (rhs.terms[i].times[j] != 0) {
					if (cnt > 0) {
						cout << " * ";
					}
					else {
						os << rhs.terms[i].coef << " * ";
					}
					cnt++;
					cout << "x" << j << "^" << rhs.terms[i].times[j];
				}
			}

		}
		os << "\n";
		return os;
	}
};


Poly poly;

stack<Poly> poly_stack;

int main() {
	// ios::sync_with_stdio(0), cin.tie(0);

	int n, m;
	cin >> n >> m;
	getchar();
	string inputString;
	getline(cin, inputString);
	///cout << inputString << "\n";
	std::vector<std::string> tokens;
	size_t start = 0, end;

	while ((end = inputString.find(" ", start)) != std::string::npos) {
		tokens.push_back(inputString.substr(start, end - start));
		//cout << tokens.back() << "\n";
		start = end + 1;
	}

	tokens.push_back(inputString.substr(start));

	int cnt = 0;
	for (auto token : tokens) {
		if (token[0] == 'x') {
			poly_stack.push(Poly(token));
		}
		else if (token[0] == '+') {
			auto p1 = poly_stack.top();
			poly_stack.pop();
			auto p2 = poly_stack.top();
			poly_stack.pop();
			poly_stack.push(Poly(p1 + p2));
		}
		else if (token[0] == '-' && token.length() == 1) {
			auto p1 = poly_stack.top();
			poly_stack.pop();
			auto p2 = poly_stack.top();
			poly_stack.pop();
			poly_stack.push(Poly(p2 - p1));
		}
		else if (token[0] == '*') {
			auto p1 = poly_stack.top();
			poly_stack.pop();
			auto p2 = poly_stack.top();
			poly_stack.pop();
			poly_stack.push(Poly(p1 * p2));
		}
		else { // 数字
			poly_stack.push(Poly(token));
		}
			//cout << "cnt: " << (++cnt) << "\n";
			//cout << poly_stack.top() << "\n";
	}
	//cout << poly_stack.top() << "\n";


	while (m--) {
		poly = poly_stack.top();
		int which;
		cin >> which;
		vector<int> a(n + 1);
		for (int i = 1; i <= n; i++) {
			cin >> a[i];
		}
		cout << poly.partial_derivative(which, a) << "\n";
	}


	return 0;
}
