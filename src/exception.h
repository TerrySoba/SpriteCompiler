#ifndef _EXCEPTION_H_INCLUDED
#define _EXCEPTION_H_INCLUDED

#include <exception>


class Exception : public std::exception
{
public:
    Exception(const Exception& other);
    Exception(const char* msg);
    Exception(const char* msg1, const char* msg2);

    const char* what() const noexcept override;

    ~Exception();

private:
    char* m_msg;
};


#define toStringHelper(x) #x
#define toStringHelper2(x) toStringHelper(x)

// Macro to throw an exception with a message and the file and line number

#define THROW_EXCEPTION(msg, ...) \
    throw Exception("file:" __FILE__ " line:" toStringHelper2(__LINE__) " - " msg,  ##__VA_ARGS__)

#endif
