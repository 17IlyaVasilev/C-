#include <iostream>
#include <cstring>

class String {
private:
    size_t sz = 0;
    size_t buf_sz = 8;
    char* str = new char[buf_sz];
    void swap(String&);
    void growbuffer();
    void decreasebuffer();

public:
    String() = default;
    String(const char*);
    String(size_t, const char);
    String(const String&);
    String(char);
    ~String();

    String& operator=(String s) {
        swap(s);
        return *this;
    }

    String& operator=(char c) {
        String s(c);
        swap(s);
        return *this;
    }

    char& operator[](size_t i) {
        return str[i];
    }

    char operator[](size_t i) const {
        return str[i];
    }

    String& operator+=(const String& s) {
        for (size_t i = 0; i < s.sz; ++i) (*this).push_back(s[i]);
        return *this;
    }

    String& operator+=(const char& c) {
        (*this).push_back(c);
        return *this;
    }

    friend bool operator==(const String&, const String&);

    size_t length() const;
    void push_back(char);
    void pop_back();
    char& front();
    char front() const;
    char& back();
    char back() const;
    size_t find(const String&);
    size_t find(const String&) const;
    size_t rfind(const String&);
    size_t rfind(const String&) const;
    String substr(const size_t&, const size_t&);
    String substr(const size_t&, const size_t&) const;
    bool empty() const;
    void clear();
};

String::String(const char* carr) {
    sz = strlen(carr);
    if (buf_sz < sz) {
        buf_sz = 2 * sz;
        if (str)delete[] str;
        str = new char[buf_sz];
    }
    memcpy(str, carr, strlen(carr));
}

String::String(size_t ssz, const char c = '\0') {
    sz = ssz;
    if (buf_sz < sz) {
        buf_sz = 2 * sz;
        if (str)delete[] str;
        str = new char[buf_sz];
    }
    memset(str, c, sz);
}

String::String(const String& s) : String(s.sz) {
    if (buf_sz < sz) {
        buf_sz = 2 * sz;
        if (str)delete[] str;
        str = new char[buf_sz];
    }
    memcpy(str, s.str, sz);
}

String::String(char c) : sz(1), str(new char[1]) {
    memset(str, c, 1);
}

String::~String() {
    if (str)delete[] str;
}

bool operator ==(const String& s1, const String& s2) {
    if (s1.length() == s2.length()) {
        if (s1.length() > 0) return memcmp(s1.str, s2.str, s1.length()) == 0;
        return true;
    }
    return false;
}

String operator+(const String& s1, const String& s2) {
    String copy = s1;
    copy += s2;
    return copy;
}

String operator+(const String& s, const char& c) {
    String copy = s;
    copy += c;
    return copy;
}

String operator+(const char& c, const String& s) {
    String copy(c);
    copy += s;
    return copy;
}

std::istream& operator>>(std::istream& in, String& s) {
    char c;
    do {
        c = in.get();
    } while (isspace(c));
    while (!in.eof() && !isspace(c)) {
        s.push_back(c);
        c = in.get();
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const String& s) {
    for (size_t i = 0; i < s.length(); ++i) out << s[i];
    return out;
}

void String::growbuffer() {
    buf_sz *= 2;
    char* newstr = new char[buf_sz];
    memcpy(newstr, str, sz);
    delete[] str;
    str = newstr;
}

void String::decreasebuffer() {
    buf_sz = sz;
    char* newstr = new char[buf_sz];
    memcpy(newstr, str, sz);
    delete[] str;
    str = newstr;
}

void String::swap(String& s) {
    std::swap(sz, s.sz);
    std::swap(str, s.str);
}

size_t String::length() const {
    return sz;
}

void String::push_back(char x) {
    if (sz + 1 == buf_sz) growbuffer();
    str[sz++] = x;
}

void String::pop_back() {
    sz--;
    if (buf_sz / 2 > sz)decreasebuffer();
}

char& String::front() {
    return str[0];
}

char String::front() const {
    return str[0];
}

char& String::back() {
    return str[sz - 1];
}

char String::back() const {
    return str[sz - 1];
}

size_t String::find(const String& s) {
    for (size_t i = 0; i < (sz - s.sz) + 1; ++i)
        for (size_t j = i; j < s.sz + i; ++j) {
            if (s[j - i] != (*this)[j]) j = s.sz + i;
            if ((j - i) + 1 == s.sz) return i;
        }
    return sz;
}

size_t String::find(const String& s) const {
    for (size_t i = 0; i < (sz - s.sz) + 1; ++i)
        for (size_t j = i; j < s.sz + i; ++j) {
            if (s[j - i] != (*this)[j]) j = s.sz + i;
            if ((j - i) + 1 == s.sz) return i;
        }
    return sz;
}

size_t String::rfind(const String& s) {
    size_t ans = sz;
    for (size_t i = 0; i < (sz - s.sz) + 1; ++i)
        for (size_t j = i; j < s.sz + i; ++j) {
            if (s[j - i] != (*this)[j]) j = s.sz + i;
            if ((j - i) + 1 == s.sz) ans = i;
        }
    return ans;
}

size_t String::rfind(const String& s) const {
    size_t ans = sz;
    for (size_t i = 0; i < (sz - s.sz) + 1; ++i)
        for (size_t j = i; j < s.sz + i; ++j) {
            if (s[j - i] != (*this)[j]) j = s.sz + i;
            if ((j - i) + 1 == s.sz) ans = i;
        }
    return ans;
}

String String::substr(const size_t& start, const size_t& count) {
    String s(count);
    memcpy(s.str, str + start, count);
    return s;
}

String String::substr(const size_t& start, const size_t& count) const {
    String s(count);
    memcpy(s.str, str + start, count);
    return s;
}

bool String::empty() const {
    return !sz;
}

void String::clear() {
    sz = 0;
    String s(sz, '\0');
    str = nullptr;
    (*this).buf_sz = 8;
}