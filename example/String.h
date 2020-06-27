#pragma once

class String
{
public:
    String() {}
    ~String() {}
    String(const char *str);
    String(const String &other);
    String& operator=(const String &other);
    String& operator+(const String &other);
    bool operator==(const String &other);
    int getLength();

private:
    char *m_data;
};


String::String() {}

String::~String() { 
    if (m_data) {
        delete m_data; 
        m_data = NULL;
    }
}

String::String(const char *str) {
    if (str == NULL) {
        m_data = new char[1];
        *m_data = '\0';
    } else {
        int len = strlen(str);
        m_data = new char[len+1];
        strcpy(m_data, str);
    }
}

String::String(const String &other) {
    if (!other.m_data) m_data = 0;
    m_data = new char[strlen(other.m_data)+1];
    strcpy(m_data, other.m_data);
}

String & String::operator=(const String &other) {
    if (this != other) {
        delete[] m_data;
        if (!other.m_data) {
            m_data = 0;
        } else {
            m_data = new char[strlen(other.m_data)+1];
            strcpy(m_data, other.m_data);
        }
    }
    return *this;
}
