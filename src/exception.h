#ifndef _EXCEPTION_H_INCLUDED
#define _EXCEPTION_H_INCLUDED

#ifndef __WATCOMC__
#include <exception> 
#endif

class Exception
#ifndef __WATCOMC__
    : public std::exception
#endif
{
public:
    Exception(const Exception& other);
    Exception(const char* msg);
    Exception(const char* msg1, const char* msg2);

#ifdef __WATCOMC__
    // const char* what() const;
#else
    const char* what() const noexcept override;
#endif

    ~Exception();

private:
    char* m_msg;
};



#define toStringHelper(x) #x
#define toStringHelper2(x) toStringHelper(x)

// Macro to throw an exception with a message and the file and line number

#ifdef __WATCOMC__
    // In DOS we don't want the overhead of the file and line number in the exception message
    #define THROW_EXCEPTION(...) \
        throw Exception(__VA_ARGS__)
#else
    #define THROW_EXCEPTION(msg, ...) \
        throw Exception("file:" __FILE__ " line:" toStringHelper2(__LINE__) " - " msg,  ##__VA_ARGS__)
#endif

#endif
