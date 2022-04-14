var rs = require('readline-sync');
var input = [
    rs.question("1st Number: "),
    rs.question("2nd Number: "),
    rs.question("3rd Number: "),
    rs.question("4th Number: ")]

console.log("Factorial of the 1st number is = " + factorial(input[0]));
console.log("The sum of all digits of the 2nd number is = " + sumDigits(input[1]));
console.log("The reverse of the 3rd number is = " + reverse(input[2]));
console.log("Is the 4th number a palindrome (True/False)? = " + isPalindrome(input[3]));

function factorial(num) {
    if (num == 0) {
        return 0;
    } else if (num == 1 || num == -1) {
        return num;
    } else {
        return num * factorial(+num + (num > 0 ? -1 : 1));
    }
}

function sumDigits(num) {
    if (num < 10) {
        return ~~num;
    } else {
        return sumDigits((num * 0.1)) + ~~(num % 10);
    }
}

function reverse(num) {
    if (num < 10) {
        return num;
    } else {
        return +(reverse(num.substring(1)) + num[0]);
    }
}

function isPalindrome(num) {
    if (num < 10) {
        return "True";
    } else if (num[0] == num % 10) {
        return isPalindrome(num.substring(1, num.length - 1));
    } else {
        return "False";
    }
}