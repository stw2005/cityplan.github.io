#ifndef CUSTOM_EXCEPTION_HPP
#define CUSTOM_EXCEPTION_HPP

#include <exception>

class OutOfBoundException: public std::exception {
private:
    const char *message;

public:
    OutOfBoundException(): message("The input/computed parameters are out of bound.") {}

    const char *what() const noexcept override {
        return message;
    }
};

class FileNotOpeningException: public std::exception {
private:
    const char *message;

public:
    FileNotOpeningException(): message("Error occured while opening the file\n") {}

    const char *what() const noexcept override {
        return message;
    }
};

class OperatingSystemNotSupported: public std::exception {
private:
    const char *message;

public:
    OperatingSystemNotSupported(): message("Operating system not supported\n") {}

    const char *what() const noexcept override {
        return message;
    }
};

class ContainerIsFullException: public std::exception {
private:
    const char *message;

public:
    ContainerIsFullException(): message("The container is full.\n") {}

    const char *what() const noexcept override {
        return message;
    }
};

#endif