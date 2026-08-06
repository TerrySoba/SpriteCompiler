#include "exception.h"

Exception::Exception(const Exception& other)
{
    if (&other == this) return;

    m_msg = other.m_msg;
}

Exception::Exception(const char* msg)
{
    m_msg = msg;
}

Exception::Exception(const char* msg1, const char* msg2)
{
    m_msg = std::string(msg1) + std::string(msg2);
}

const char* Exception::what() const noexcept
{
    return m_msg.c_str();
}
