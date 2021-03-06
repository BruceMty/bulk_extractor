/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/* sbuf_flex_scanner.h:
 * Used to build a C++ class that can interoperate with sbufs.
 * Previously we used the flex c++ option, but it had cross-platform problems.
 */

#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wmissing-noreturn"

#define YY_NO_INPUT

/* Needed for flex: */
#define ECHO {}                   /* Never echo anything */
#define YY_SKIP_YYWRAP            /* Never wrap */
#define YY_NO_INPUT

class sbuf_scanner {
public:
    class sbuf_scanner_exception: public std::exception {
    public:
        const char *msg;
        sbuf_scanner_exception(const char *m):msg(m){}
        virtual const char *what() const throw() {
            return msg;
        }
    };
    explicit sbuf_scanner(const sbuf_t *sbuf_): sbuf(sbuf_),pos(0),point(0){}
    virtual ~sbuf_scanner(){}
    const sbuf_t *sbuf;
    // pos & point may be redundent.
    // pos counts the number of bytes into the buffer and is incremented by the flex rules
    // point counts the point where we are removing characters
    size_t pos;            
    size_t point;

    size_t get_input(char *buf,size_t max_size){
        if((int)max_size < 0) return 0;
        int count=0;
        while(max_size > 0 && point < sbuf->bufsize && pos<sbuf->pagesize){
            *buf++ = (char)sbuf->buf[point++];
            max_size--;
            count++;
        }
        return count;
    };
};

#define YY_INPUT(buf,result,max_size) result = get_extra(yyscanner)->get_input(buf,max_size);
#define POS  s.pos
#define SBUF (*s.sbuf)
#define YY_FATAL_ERROR(msg) {throw sbuf_scanner::sbuf_scanner_exception(msg);}

#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wsign-compare"
