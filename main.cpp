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
    unsigned short s;
    unsigned char c[];
} ShortAndChar;

class BigInteger {
private:
    bool _negative;
    unsigned char* _values;
    unsigned int _length;

public:

    BigInteger(unsigned char* values, unsigned int length, bool negative) {
        this->_values = values;
        this->_length = length;
        this->_negative = negative;
    }

    ~BigInteger() {
        delete _values;
    }

    unsigned int length() {
        return _length;
    }
    
    int compareTo(BigInteger *rhs) {
        if (_length > rhs->_length) {
            return 1;
        } else if (_length < rhs->_length) {
            return -1;
        }
        
        for (int i = _length - 1; i >= 0; i--) {
            if (_values[i] != rhs->_values[i]) {
                return _values[i] > rhs->_values[i] ? 1 : -1;
            }
        }
        
        return 0;
    }
    
    BigInteger* clone() {
        unsigned char* values = (unsigned char*)malloc(sizeof(unsigned char*) * _length);
        
        for (int i = 0; i < _length; i++) {
            values[i] = this->_values[i];
        }
        
        return new BigInteger(values, _length, _negative);
    }

    bool isNegative() {
        return _negative;
    }
    
    BigInteger* add(BigInteger* rhs) {
        unsigned char carry = 0;

        for (int i = 0; i < _length; i++) {
            ShortAndChar sc;
            sc.s = _values[i] + rhs->_values[i] + carry;

            _values[i] = sc.c[0];
            carry = sc.s >> 8;
        }
        
        return this;
    }
    
    BigInteger* minus(BigInteger* rhs) {
        BigInteger* high = compareTo(rhs) > 0 ? this : rhs;
        BigInteger* low = high == this ? rhs : this;
        
        for (int i = 0; i < _length; i++) {
            if (high->_values[i] >= low->_values[i]) {
                this->_values[i] = high->_values[i] - low->_values[i];
            } else {
                high->_values[i + 1]--;
                
                ShortAndChar sc;
                
                sc.c[0] = high->_values[i];
                sc.c[1] = 1;
                sc.s -= low->_values[i];
                
                this->_values[i] = sc.c[0];
            }
        }
        
        _negative = low == this;
        
        return this;
    }
    
    BigInteger* print() {
        if (_negative) {
            printf("-");
        }
        
        for (int i = 0; i < _length; i++) {
            printf("%d ", _values[i]);
        }
        printf("\n");
        
        return this;
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

        return new BigInteger(values, length, false);
    }
};

/*
 * 
 */
int main(int argc, char** argv) {
    //    int length = 57885161;
    // new BigInteger((unsigned char*)malloc(sizeof(unsigned char) * length), length);

    BigIntegerFactory big_integer_factory;
    BigInteger* a = big_integer_factory.make(1);
    BigInteger* b = big_integer_factory.make(32);
    BigInteger* c = big_integer_factory.make(34);
    
    b->minus(c);

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

