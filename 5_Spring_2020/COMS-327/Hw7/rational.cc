/**
 * Class to hold and do math on rational numbers
 * @author Alexander Harms
 * ajharms
 */
#include <iostream>
#include "rational.hh"

rational::rational(long n, long d) : numer(n), denom(d) {
    numer *= n < 0 || d < 0 && !(n < 0 && d < 0) ? -1 : 1;
    denom *= d < 0 ? -1 : 1;
    
    for (long i = (denom < numer ? denom : numer); i > 1; i--) {
        if (n % i == 0 && d % i == 0) {
            numer /= i;
            denom /= i;
            i = (denom < numer ? denom : numer);
        }
    }
}

rational rational::operator+ (rational & r) {
    return rational((numer * r.denom) + (r.numer * denom), denom * r.denom);
}

rational rational::operator- (rational & r) {
    return rational((numer * r.denom) - (r.numer * denom), denom * r.denom);
}

rational rational::operator* (rational & r) {
    return rational(numer * r.numer, denom * r.denom);
}

rational rational::operator/ (rational & r) {
    return rational(numer * r.denom, denom * r.numer);
}

std::ostream &operator<< (std::ostream &output, const rational &r) {
    output << r.numer;
    if (r.denom > 1) output << "/" << r.denom;
    return output;
}