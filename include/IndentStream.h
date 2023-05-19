#ifndef INDENT_STREAM_H
#define INDENT_STREAM_H

#include <fstream>
#include <iostream>
#include <memory>
#include <streambuf>
using std::ostream;
using std::streambuf;

namespace mpcs51045 {
using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::streambuf;

class IndentStreamBuf : public streambuf {
   public:
    IndentStreamBuf(ostream &stream)
        : wrappedStreambuf(stream.rdbuf()), isLineStart(true), myIndent(0) {}
    virtual int overflow(int outputVal) override {
        if (outputVal == traits_type::eof())
            return traits_type::eof();
        if (outputVal == '\n') {
            isLineStart = true;
        } else if (isLineStart) {
            for (size_t i = 0; i < myIndent; i++) {
                wrappedStreambuf->sputc(' ');
            }
            isLineStart = false;
        }
        wrappedStreambuf->sputc(static_cast<char>(outputVal));
        return outputVal;
    }

   protected:
    streambuf *wrappedStreambuf;
    bool isLineStart;

   public:
    size_t myIndent;
};

class IndentStream : public ostream {
   public:
    IndentStream(ostream &wrappedStream)
        : ostream(new IndentStreamBuf(wrappedStream)) {
    }
    ~IndentStream() { delete this->rdbuf(); }
};

ostream &indent(ostream &ostr) {
    IndentStreamBuf *out = dynamic_cast<IndentStreamBuf *>(ostr.rdbuf());
    if (nullptr != out) {
        out->myIndent += 4;
    }
    return ostr;
}

ostream &unindent(ostream &ostr) {
    IndentStreamBuf *out = dynamic_cast<IndentStreamBuf *>(ostr.rdbuf());
    out->myIndent -= 4;
    return ostr;
}

}  // namespace mpcs51045
#endif
