/**
 * Class to hold and do math on rational numbers
 * @author Alexander Harms
 * ajharms
 */
#ifndef RATIONAL_HH
#define RATIONAL_HH

class rational {
    private:
        // Numerator and denominator of the number
        long numer, denom;

    public:  
        /**
         * Creates a rational number
         * @param numerator
         * numerator of the rational number
         * @param denominator
         * denominator of the ratinoal number
         */
        rational(long numerator=0, long denominator=1);

        // Overloaded operators
        rational operator+ (rational & r);
        rational operator- (rational & r);
        rational operator* (rational & r);
        rational operator/ (rational & r);
        friend std::ostream &operator<< (std::ostream &output, const rational &r);
};

#endif