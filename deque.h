#include <iostream>
#include <vector>


using namespace std;


template<class T>
class Deque {
private:
	const size_t bucket_size = 4;
	vector<T*> mas;
	size_t first_mas = 0;
	size_t last_mas = 0;
	size_t first_ind = 0;
	size_t last_ind = 0;
	size_t length_mas = 0;


	void grow_buffer() {
		//cerr << "GB" << endl;
		Deque<T> extra(*this);
		try {
			size_t sz = mas.size() * 2;
			vector<T*> newmas(sz - 1);
			for (size_t i = mas.size(); i < sz - 1; ++i) {
				newmas[i] = mas[i - mas.size() + 1];
			}
			mas = newmas;

			for (size_t i = mas.size(); i < sz - 1; ++i) {
				for (size_t j = 0; ((j < bucket_size && i < sz - 2) || (j <= last_ind && i == sz - 2)); ++j) {
					(newmas[i] + j)->~T();
				}
				delete[] newmas[i];
			}

			first_mas += sz / 2 - 1;
			last_mas += sz / 2 - 1;
			T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
			mas[first_mas - 1] = temp_mas;
		}
		catch (...) {
			this->mas = extra.mas;
			first_mas = extra.first_mas;
			last_mas = extra.last_mas;
			first_ind = extra.first_ind;
			last_ind = extra.last_ind;
			length_mas = extra.length_mas;
			throw;
		}
	}

public:
	template<bool IsConst>
	struct common_iterator {
		using T_type = std::conditional_t<IsConst, const T&, T&>;
		using T_ptr_type = std::conditional_t<IsConst, const T*, T*>;
		size_t bucket_size = 4;
		const vector<T*>* mas;
		size_t bucket = 0;
		size_t ind = 0;
		T_ptr_type ptr;
		common_iterator(T_ptr_type ptr, size_t bucket, size_t ind, const vector<T*>* mas) : mas(mas), bucket(bucket), ind(ind), ptr(ptr) {
			//cerr << "I4" << endl;
		}
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using pointer = T_ptr_type;
		using reference = T_type;
		using difference_type = std::ptrdiff_t;
		common_iterator(T_ptr_type ptr) : ptr(ptr) {
			//cerr << "I" << endl;
		}

		bool operator== (const common_iterator& iter) {
			//cerr << "==" << endl;
			return (ptr == iter.ptr);
		}

		bool operator!= (const common_iterator& iter) {
			//cerr << "!=" << endl;
			return !(ptr == iter.ptr);
		}

		bool operator<= (const common_iterator& iter) {
			//cerr << "<=" << endl;
			if (bucket == iter.bucket)
				return (ind <= iter.ind);
			else
				return (bucket <= iter.bucket);
		}

		bool operator>= (const common_iterator& iter) {
			//cerr << ">=" << endl;
			if (bucket == iter.bucket)
				return (ind >= iter.ind);
			else
				return (bucket >= iter.bucket);
		}

		bool operator< (const common_iterator& iter) {
			//cerr << "<" << endl;
			return (*this != iter) && (*this <= iter);
		}

		bool operator> (const common_iterator& iter) {
			//cerr << ">" << endl;
			return (*this != iter) && (*this >= iter);
		}

		common_iterator& operator++() {
			//cerr << "++" << endl;
			if (ind == bucket_size - 1) {
				bucket++;
				ind = 0;
				ptr = (*mas)[bucket];
			}
			else {
				ind++;
				++ptr;
			}

			return (*this);
		}

		common_iterator& operator--() {
			//cerr << "--" << endl;
			if (ind == 0) {
				bucket--;
				ind = bucket_size - 1;
				ptr = (*mas)[bucket] + ind;
			}
			else {
				ind--;
				--ptr;
			}

			return (*this);
		}

		common_iterator& operator++(int) {
			//cerr << "++i" << endl;
			common_iterator copy = *this;
			++(*this);

			return copy;
		}

		common_iterator& operator--(int) {
			//cerr << "--i" << endl;
			common_iterator copy = *this;
			--(*this);

			return (copy);
		}

		common_iterator& operator+=(const int& a) {
			//cerr << "+=" << endl;
			auto temp = bucket * bucket_size + ind + a;

			bucket = temp / bucket_size;
			ind = temp % bucket_size;
			ptr = (*mas)[bucket] + ind;

			return (*this);
		}

		common_iterator& operator-=(const int& a) {
			//cerr << "-=" << endl;
			*this += -a;

			return (*this);
		}

		int operator- (const common_iterator& iter) {
			//cerr << "-it" << endl;
			return ((bucket - iter.bucket + 1) * bucket_size - iter.ind - bucket_size + ind);
		}

		common_iterator operator+ (const int& n) {
			//cerr << "+" << endl;
			auto temp = *this;
			temp += n;

			return temp;
		}

		common_iterator operator- (const int& n) {
			//cerr << "-" << endl;
			auto temp = *this;
			temp -= n;

			return temp;
		}

		T_type operator*() {
			//cerr << "*" << endl;
			return *ptr;
		}

		T_ptr_type operator->() {
			//cerr << "->" << endl;
			return ptr;
		}
	};


	using iterator = common_iterator<false>;
	using const_iterator = common_iterator<true>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	Deque() {
		//cerr << "D" << endl;
		size_t extra = 0;
		try {
			mas.resize(4);
			for (size_t i = 0; i < 4; ++i) {
				extra++;
				mas[i] = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
			}
			first_mas = 2;
			last_mas = 2;
			first_ind = 0;
			last_ind = 0;
			length_mas = 0;
		}
		catch (...) {
			for (size_t i = 0; i <= extra; ++i) {
				delete[] mas[i];
			}
			throw;
		}
	}

	Deque(const Deque& deque) {
		//cerr << "DD" << endl;
		first_mas = deque.first_mas;
		last_mas = deque.last_mas;
		first_ind = deque.first_ind;
		last_ind = deque.last_ind;
		length_mas = deque.length_mas;
		mas.resize(deque.mas.size());
		size_t i = 0;
		size_t j = 0;
		try {
			for (i = first_mas; i <= last_mas; ++i) {
				T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
				mas[i] = temp_mas;
				for (j = 0; j < bucket_size; ++j)
					new(mas[i] + j)T(deque.mas[i][j]);
			}
		}
		catch (...) {
			for (size_t ii = first_mas; ii <= i; ++ii) {
				for (size_t jj = 0; ((jj < bucket_size && ii < i) || (jj < j && ii == i)); ++jj) {
					(mas[ii] + jj)->~T();
				}
				delete[] mas[ii];
			}
			throw;
		}
	}

	Deque(const int& n, const T& x = T()) {
		//cerr << "DIT " << n << endl;
		size_t temp = (n - 1) / bucket_size + 1;
		auto nn = size_t(n);
		mas.resize(temp + 2);
		first_mas = 1;
		last_mas = temp;
		first_ind = 0;
		last_ind = 0;
		if (nn % bucket_size == 0) last_ind = bucket_size - 1;
		else last_ind = nn % bucket_size - 1;
		length_mas = nn;
		size_t i = first_mas;
		size_t j = 0;
		try {
			for (i = first_mas; i <= last_mas; ++i) {
				T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
				mas[i] = temp_mas;
				for (j = 0; ((j < bucket_size) && (((i - first_mas) * bucket_size + j) < nn)); ++j)
					new(mas[i] + j)T(x);
			}
			T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
			mas[last_mas + 1] = temp_mas;
		}
		catch (...) {
			for (size_t ii = first_mas; ii <= i; ++ii) {
				for (size_t jj = 0; ((jj < bucket_size && ii < i) || (jj < j && ii == i)); ++jj) {
					(mas[ii] + jj)->~T();
				}
				delete[] mas[ii];
			}
			throw;
		}
	}

	Deque& operator= (const Deque& deque) {
		//cerr << "D=" << endl;
		first_mas = deque.first_mas;
		last_mas = deque.last_mas;
		first_ind = deque.first_ind;
		last_ind = deque.last_ind;
		length_mas = deque.length_mas;
		size_t sz = mas.size();
		Deque<T> extra = *this;
		for (size_t i = 0; i < sz; ++i) {
			for (size_t j = 0; j < bucket_size; ++j)
				(mas[i] + j)->~T();
			delete[] reinterpret_cast<int8_t*>(mas[i]);
		}
		mas.clear();
		mas.resize(deque.mas.size());
		try {
			for (size_t i = first_mas; i <= last_mas; ++i) {
				T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
				mas[i] = temp_mas;
				for (size_t j = 0; j < bucket_size; ++j)
					new(mas[i] + j)T(deque.mas[i][j]);
			}
		}
		catch (...) {
			first_mas = extra.first_mas;
			last_mas = extra.last_mas;
			first_ind = extra.first_ind;
			last_ind = extra.last_ind;
			length_mas = extra.length_mas;
			mas = extra.mas;
			throw;
		}

		return *this;
	}

	size_t size() const {
		//cerr << "size" << endl;
		return (length_mas);
	}

	T& operator[] (const size_t& i) {
		//cerr << "[]" << endl;
		return (mas[first_mas + (i + first_ind) / bucket_size][(i + first_ind) % bucket_size]);
	}

	T& at(const size_t& i) {
		//cerr << "at" << endl;
		if (i >= length_mas) throw out_of_range("....");

		return (mas[first_mas + (i + first_ind) / bucket_size][(i + first_ind) % bucket_size]);
	}

	const T& operator[] (const size_t& i) const {
		//cerr << "[]c" << endl;
		return (mas[first_mas + (i + first_ind) / bucket_size][(i + first_ind) % bucket_size]);
	}

	const T& at(const size_t& i) const {
		//cerr << "atc" << endl;
		if (i >= length_mas) throw out_of_range("....");

		return (mas[first_mas + (i + first_ind) / bucket_size][(i + first_ind) % bucket_size]);
	}

	void push_back(const T& x) {
		//cerr << "pub" << endl;
		try {
			if (length_mas > 0) {
				if (last_ind + 1 == bucket_size) {
					last_ind = 0;
					last_mas++;
					if (last_mas + 1 == mas.size()) {
						T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
						mas.push_back(temp_mas);
					}
				}
				else last_ind++;
			}
			new(mas[last_mas] + last_ind)T(x);
			length_mas++;
		}
		catch (...) {
			throw;
		}
	}

	void pop_back() {
		//cerr << "pob" << endl;
		if (length_mas > 0) {
			(mas[last_mas] + last_ind)->~T();
			length_mas--;
			if (length_mas > 0) {
				if (last_ind == 0) {
					last_ind = bucket_size - 1;
					last_mas--;
				}
				else last_ind--;
			}
			else {
				last_mas = first_mas;
				last_ind = first_ind;
			}
		}
		else throw;
	}

	void push_front(const T& x) {
		//cerr << "puf" << endl;
		try {
			if (length_mas == 0) {
				new(mas[first_mas] + first_ind)T(x);
				length_mas++;
			}
			else {
				if (first_ind == 0 && first_mas == 1) {
					grow_buffer();
				}
				if (first_ind == 0) {
					first_ind = bucket_size - 1;
					first_mas--;
					T* temp_mas = reinterpret_cast<T*>(new int8_t[sizeof(T) * bucket_size]);
					mas[first_mas] = temp_mas;
				}
				else first_ind--;
				new(mas[first_mas] + first_ind)T(x);
				length_mas++;
			}
		}
		catch (...) {
			throw;
		}
	}

	void pop_front() {
		//cerr << "pof" << endl;
		if (length_mas > 0) {
			(mas[first_mas] + first_ind)->~T();
			length_mas--;
			if (length_mas > 0) {
				if (first_ind + 1 == bucket_size) {
					first_ind = 0;
					first_mas++;
				}
				else first_ind++;
			}
			else {
				first_mas = last_mas;
				first_ind = last_ind;
			}
		}
		else throw;
	}

	void insert(iterator iter, const T& x) {
		//cerr << "in1" << endl;
		try {
			iterator it(mas[last_mas] + last_ind, last_mas, last_ind, &mas);
			T last = *it;
			push_back(last);
			while (it > iter) {
				iterator it1(it);
				(it1.ptr)->~T();
				new(it1.ptr)T(*(--it));
			}
			(it.ptr)->~T();
			new(it.ptr)T(x);
		}
		catch (...) {
			throw;
		}
	}

	void erase(iterator iter) {
		//cerr << "er1" << endl;
		if (length_mas > 0) {
			iterator it(iter);
			iterator i(--end());
			while (it != i) {
				iterator it1(it);
				(it1.ptr)->~T();
				new(it1.ptr)T(*(++it));
			}
			pop_back();
		}
		else throw;
	}

	void insert(const_iterator iter, const T& x) {
		//cerr << "in2" << endl;
		try {
			iterator it(mas[last_mas] + last_ind, last_mas, last_ind, &mas);
			T last = *it;
			push_back(last);
			while (it > iter) {
				iterator it1(it);
				(it1.ptr)->~T();
				new(it1.ptr)T(*(--it));
			}
			(it.ptr)->~T();
			new(it.ptr)T(x);
		}
		catch (...) {
			throw;
		}
	}

	void erase(const_iterator iter) {
		//cerr << "er2" << endl;
		if (length_mas > 0) {
			iterator it(iter);
			iterator i(--end());
			while (it != i) {
				iterator it1(it);
				(it1.ptr)->~T();
				new(it1.ptr)T(*(++it));
			}
			pop_back();
		}
		else throw;
	}

	const_iterator begin() const {
		//cerr << "cbc" << endl;
		return const_iterator(mas[first_mas] + first_ind, first_mas, first_ind, &mas);
	}

	const_iterator end() const {
		//cerr << "cec" << endl;
		if (last_ind < bucket_size - 1)
			return const_iterator(mas[last_mas] + (last_ind + 1), last_mas, last_ind + 1, &mas);
		else
			return const_iterator(mas[last_mas + 1], last_mas + 1, 0, &mas);
	}

	const_iterator cbegin() const {
		//cerr << "ccbc" << endl;
		return const_iterator(mas[first_mas] + first_ind, first_mas, first_ind, &mas);
	}

	const_iterator cend() const {
		//cerr << "ccec" << endl;
		if (last_ind < bucket_size - 1)
			return const_iterator(mas[last_mas] + (last_ind + 1), last_mas, last_ind + 1, &mas);
		else
			return const_iterator(mas[last_mas + 1], last_mas + 1, 0, &mas);
	}

	iterator begin() {
		//cerr << "b" << endl;
		return iterator(mas[first_mas] + first_ind, first_mas, first_ind, &mas);
	}

	iterator end() {
		//cerr << "e" << endl;
		if (last_ind < bucket_size - 1)
			return iterator(mas[last_mas] + (last_ind + 1), last_mas, last_ind + 1, &mas);
		else
			return iterator(mas[last_mas + 1], last_mas + 1, 0, &mas);
	}

	const_iterator cbegin() {
		//cerr << "ccb" << endl;
		return const_iterator(mas[first_mas] + first_ind, first_mas, first_ind, &mas);
	}

	const_iterator cend() {
		//cerr << "cce" << endl;
		if (last_ind < bucket_size - 1)
			return const_iterator(mas[last_mas] + (last_ind + 1), last_mas, last_ind + 1, &mas);
		else
			return const_iterator(mas[last_mas + 1], last_mas + 1, 0, &mas);
	}

	reverse_iterator crend() const {
		return const_reverse_iterator(cbegin() - 1);
	}

	reverse_iterator crbegin() const {
		return const_reverse_iterator(cend() - 1);
	}

	const_reverse_iterator rend() {
		return reverse_iterator(begin() - 1);
	}

	const_reverse_iterator rbegin() {
		return reverse_iterator(end() - 1);
	}

	~Deque() {
		for (size_t i = 0; i < mas.size(); ++i) {
			for (size_t j = 0; ((j < bucket_size && i < mas.size() - 1) || (j <= last_ind && i == mas.size() - 1)); ++j) {
				(mas[i] + j)->~T();
			}
			delete[] mas[i];
		}
	}

};
