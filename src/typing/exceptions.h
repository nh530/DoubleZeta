#include <stdexcept>
#include <string>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

class NotImplemented : public std::logic_error {
public:
  NotImplemented();
  NotImplemented(const char *);
  virtual const char *what() const throw();

private:
  std::string _text;
  NotImplemented(const char *, const char *);
};

#endif
