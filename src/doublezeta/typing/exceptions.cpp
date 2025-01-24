#include "exceptions.h"
#include <string>

NotImplemented::NotImplemented() : NotImplemented("Not Implememented", __FUNCTION__) {}
NotImplemented::NotImplemented(const char *message) : NotImplemented(message, __FUNCTION__) {}
const char *NotImplemented::what() const throw() { return _text.c_str(); }

NotImplemented::NotImplemented(const char *message, const char *function) : std::logic_error("Not Implemented") {
  _text = message;
  _text += " : ";
  _text += function;
}

InvalidModel::InvalidModel() : InvalidModel("Invalid Model", __FUNCTION__) {}
InvalidModel::InvalidModel(const char *message) : InvalidModel(message, __FUNCTION__) {}
const char *InvalidModel::what() const throw() { return _text.c_str(); }

InvalidModel::InvalidModel(const char *message, const char *function) : std::logic_error("Invalid Model") {
  _text = message;
  _text += " : ";
  _text += function;
}
