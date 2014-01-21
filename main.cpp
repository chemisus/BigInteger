/* 
 * File:   main.cpp
 * Author: chemisus
 *
 * Created on January 21, 2014, 4:18 PM
 */

#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;

typedef union {
    int integer;
    char c[];
} IntegerAndChar;

typedef union {
    short s;
    char c[];
} ShortAndChar;

class BigInteger {
private:
    unsigned char* values;
    unsigned int length;

public:

    BigInteger(unsigned char* values, unsigned int length) {
        this->values = values;
        this->length = length;
    }

    ~BigInteger() {
        delete values;
    }

    unsigned int getLength() {
        return length;
    }
    
    int compareTo(BigInteger *rhs) {
        for (int i = length - 1; i >= 0; i--) {
            if (values[i] != rhs->values[i]) {
                return values[i] > rhs->values[i] ? 1 : -1;
            }
        }
        
        return 0;
    }

    BigInteger* add(BigInteger* rhs) {
        unsigned char* bytes = (unsigned char*) malloc(sizeof (unsigned char*) * length);

        unsigned char carry = 0;

        for (int i = 0; i < length; i++) {
            ShortAndChar sc;
            sc.s = values[i] + rhs->values[i] + carry;

            bytes[i] = sc.c[0];
            carry = sc.s >> 8;
        }

        return new BigInteger(bytes, length);
    }
    
    BigInteger* times(BigInteger rhs) {
        unsigned char* bytes = (unsigned char*) malloc(sizeof (unsigned char*) * length);

        unsigned char carry = 0;

        for (int i = 0; i < length; i++) {
            ShortAndChar sc;
            sc.s = values[i] + rhs->values[i] + carry;

            bytes[i] = sc.c[0];
            carry = sc.s >> 8;
        }

        return new BigInteger(bytes, length);
    }

    void print() {
        for (int i = 0; i < length; i++) {
            printf("%d ", values[i]);
        }
        printf("\n");
    }
};

class BigIntegerFactory {
public:

    BigInteger* make(int value) {
        unsigned int length = sizeof (value);
        unsigned char* values = (unsigned char*) malloc(sizeof (unsigned char) * length);

        IntegerAndChar ic;
        ic.integer = value;

        for (int i = 0; i < length; i++) {
            values[i] = ic.c[i];
        }

        return new BigInteger(values, length);
    }
};

/*
 * 
 */
int main(int argc, char** argv) {
    //    int length = 57885161;
    // new BigInteger((unsigned char*)malloc(sizeof(unsigned char) * length), length);

    BigIntegerFactory big_integer_factory;
    BigInteger* a = big_integer_factory.make(7);
    BigInteger* b = big_integer_factory.make(7);
    BigInteger* c = a->add(b);

    a->print();
    b->print();
    c->print();
    
    printf("a compare to b: %d\n", a->compareTo(b));
    printf("a compare to c: %d\n", a->compareTo(c));

    delete a;
    delete b;
    delete c;

    return 0;
}

