/*   Old School Square Root Algorithm Based on the Formula
     For Square of Binomial (10a + b)^2 = (10a)^2 + 2(10a)b + b^2

    This version handles decimal fractions with integer part 0
    It also implements helper function, including zahl, which was
    originally named COUT_NumberTo_SRA_String since it takes a number (double)
    as input, converts it to long integer, feeds it into a deque similar
    to tokens, named Zahl, and then uses Zahl to build and return
    a string formatted for being displayed in the "By Hand" form of the
    Digit By Digits Square Root Algorithm
*/

#include <iostream>
#include <deque>
#include <string>
#include <limits>
#include <climits>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <istream>  // to use cin.get() so as to use function "pause"



/* Obtaining square root using the decimal method */

class square_root      {
   double result, tmp, remainder, acceptedDiff;
   double input_double;
   long input_long;
   std::deque<double> tokens; //Will contain the tokenized 'input' value.
   int digits;
   int Efactor;   // We multiply input by 10^Efactor to get LONG integer
   int Dfactor;  // Dfactor = 2*H - 2 where H = tokens.size() - scale_factor
                // decimal_places[0] = 10^Dfactor, then each succeeding place
                // values has Dfactor -= 2
   int phase;  // Crucial for organzing and connecting different stages
   int itr;    // This is an iterator which is "phase - 1"
               // It serves the phase variable in the same capacity as
               // the variable "count" serves variable 'k'; that is,
               // It reduces confusion between our tendency in mathematics
               // to iterate beginning with 1 and computer-based iterations
               // beginning with 0

   // Some arrays for displaying the work
   double a_integers[25];  // Each "phase" has a result, one single digit of root
                        // a_integers[] is the digit by digit that appear in
                        // the final solution, one digit per element

   double difference[25]; // stores each remainder BEFORE it transforms
   double remainders[25];  // stores each "remainder" AFTER it transforms

   double subtrahends[25]; // The actual values that add up to the square

   double pairs[25];  // array of doubles from tokens, for adding to
                      // 100 times the remainder

   double sub_result[25];  // Snapsshots of result as digits are added
                        // Each sub_result was used in computing each
                        // approximation:
                        // tmp*[ (2)(10)(result) + result]  <= remainders
                      // called by: tmp = approximate(result, remainder)
   double a[25];    // used for Final Analysis
                    // and for sub_result[i] = a[i] + a[a-1]
                    // Let sub_result[0] = a[0]

   int scale_factor;  //  if number of digits after point are an even
                      // scale_factor = digits_after_radix/2;
                      // if odd, scale_factor = (digits_after_radix + 1)/2
                      // This factor is used to determine how many zeros
                      // must be added after a decimal point
                      // It is used to determine the value of the decimal
                      // place represented by each position in all arrays
                      // It will generally be around Efactor/2

   double s;  // temporary memory for storing "subtradends" (a+b)*b
   int point_location;  // necessary to be able to link with place 10^(-2)
                        // so as to insert point before that place in
                        // the display of the algorithm
                        // This is a great aid to making sense of the numbers
                        // which are often handled as integers.
   int digits_before_radix;
   int digits_after_radix;
   double decimal_places[25];  // decimal_places[i] = pow(10, Dfactor)
   std::string input_str;  // input_double is used to build input_long,
                  // which, in turn, is used for building the deque,
                  // tokens.  We actually use the tokens deque to building
                  // this string for printing purposes and to fill the
                  // array of doubles in pairs[].  See create_input_string
                  // and the sophisticated use of ostringstream.

  double pow10[25];   // pow10[i] = pow(10, Dfactor/2)
  int leading_zeros_after_radix;  // Very crucial for allowing input_double < 1
  char hyphen;   // line above radical symbol
  char space;    // used for displaying uniform amount of spaces
  int tab;   // custom-defined number of spaces
  int point_;  // To keep the display algorithm in order
               // after the radix makes its appearance
  int radix_index;  // stores value of count when point_ is set to 1
  int display_step; // display explanation if display_step == k



   square_root ();

  /*
     Approximates the biggest number 'n' given as input 'a' and 'x'
     such that ( y = (a*20) + n | x - (y*n) < y )
  */
  /***********************************************************************/
  double approximate (double a, double x)  {

    double b = 1;
     a *= 20;

  if ( (pairs[0] != 0) || (phase != 1) )  {
    double d = 1;  // for additional commentary to help reduce confusion

  /********************************************************************/

  std::cout << "\n\n Working with only integers, we notice that:\n";

         while ((a + d)*d <= x)  {
           d++;
         }
         d--;

         std::cout << "\n (" << a << " + " << d << ")(" << d << ") = ("
                   << (a + d)  << ")(" << d << ") = " << (a+d)*d;

         std::cout << "\n\n Extracting a single digit to be in the "
                   << pow10[itr]
                   << " place in our square root,\n";
         std::cout << "\n\tWe choose " << d << " since "
                   << (a+d)*d << " <= " << x << "\n\n";

    /***********************************************************/
      pause();
     std::cout << "\n For clarity, consider the more exact but less compact form:\n";
   }
     while ((a + b)*b <= x)  {
       b++;
     }
     b--;

   if ( (pairs[0] != 0) || (phase != 1) )   {
     std::cout << "\n [(" << a << ")(" << pow10[itr] << ") + ("
               << b << ")(" << pow10[itr] << ")] times [("
               << b << ")("
               << pow10[itr] << ")] = ";

     std::cout << "(" << a*pow10[itr] << " + "
               << b*pow10[itr]
               << ")(" << b*pow10[itr] << ")\n"
               << std::string(11*tab, space) << " = ("
               << a*pow10[itr] + b*pow10[itr] << ")(" << b*pow10[itr]
               << ") = " << (a*pow10[itr] + b*pow10[itr])*b*pow10[itr]
               << "\n";

     std::cout << "\n\n Regardless, " << b << " is the integer we choose "
               << "for decimal place " << pow10[itr] << ",\n\n since ("
               << a*pow10[itr] << " + " << b*pow10[itr]
               << ") times (" << b*pow10[itr] << ") = "
               << (a*pow10[itr] + b*pow10[itr])*(b*pow10[itr]) << " <= ";

   if (phase == 1) {
     std::cout << input_double;
     std::cout << "\n whereas (" << a*pow10[itr] << " + " << (b+1)*pow10[itr]
     << ") times (" << (b+1)*pow10[itr] << ") = "
     << (a*pow10[itr] + (b+1)*pow10[itr])*((b+1)*pow10[itr]) << " > "
     << input_double;
   }
   else if (decimal_places[itr] >= 0.01) std::cout << remainders[itr-1]*decimal_places[itr];
        else std::cout << remainders[itr-1];


     std::cout << "\n\n Do you see that the numeral " << b
               << ", when in decimal place " << pow10[itr]
               << ", has a real value of " << b*pow10[itr]
               << " ?\n\n";

      pause();
    }               // I block out the display when the first digit is 0.x
     return b;
   }

/***********************************************************************/

 void process(double num) {

    remainder *= 100;
    remainder += num;

    if (phase != 1)
    {
      double TwiceR = 2*sub_result[itr-1]*(1/pow10[itr]);
      int estimate = (int)(remainder/TwiceR);

    std::cout << "\n  We want to find a single digit integer between 0 and 9.\n";
    if (( pairs[0] != 0)  || (phase != 2) )  {
      std::cout << "\n  By hand, we can estimate its value by"
              << " dividing " << remainder << " by " << TwiceR
              << ".\n\n  The integer part of dividing " << remainder
              << " by " << TwiceR << " is " << estimate << ".\n\n  Does [("
              << TwiceR  << " plus " << estimate << ") times " << estimate
              << "] come close to " << remainder << " without exceeding it?\n\n"
              << "  If " << (TwiceR + estimate)*estimate << " <= "
              << remainder << " then we choose " << estimate;

    if (estimate != 0)
      std::cout << "\n\n  If " << (TwiceR + estimate)*estimate << " > "
                << remainder << " then we choose " << estimate - 1;

      if ( (TwiceR + estimate-1)*(estimate-1) > remainder )
        std::cout << "\n\nBUT [(" << TwiceR  << " plus " << estimate-1 \
                  << ") times " << estimate-1 << "] = "  \
                  << (TwiceR + estimate-1)*(estimate-1) << " > " \
                  << remainder << ", so we must consider " << estimate - 2;

       }


     pause();

   }

    tmp = approximate(result,remainder);

    if (phase == 1) s = std::pow(tmp, 2);
    else s = ((result*20)+tmp)*tmp;


    // EXAMPLE:
    //  7  53.00    | 27.44
    //  4-          |___________________________________
    //  3=          | 2*2=4 < 7                                => 2
    //    353-      | (((2*2*10) + 7)*7 = 329 < 353            => 7
    //    329=      |
    //     24.00-   | (((2*27) + 0.4)*0.4 = 21.76 < 24         => .4
    //     21.76=   |
    //      2.24-   | (((2*27.4) + 0.04)*0.04 = 2.1936 < 2.24  => 0.04
    //      2.1936  |

    remainder -= ((result*20)+tmp)*tmp;
    result *= 10;    // This is the accumulated integer which gets adjusted
                     // by dividing to the apropriate value at the very end,
                     // as in:  result /= std::pow(10, scale_factor) ;
    result += tmp;

//DEBUG:

    // Update values of variables for independent debugging

 std::cout << std::string(7*tab, space) << "PHASE " << phase << "\n";

/*** uncomment to debug
     << ": AFTER PROCESSING \nremainder x = "
              << remainder << ",\t\ttmp b = " << tmp << ",\tresult a = "
              << result << "\nBefore displaying algorithm ...\n";
***/

    // This is where I attempt to transform this to display the process
    // The idea is to save the values which play a significant role
    // in the systematic way humans would carry this work out on paper
    // by hand, digit by digit

    subtrahends[itr] = s*decimal_places[itr];
     // would change to s
    a_integers[itr] = tmp;

    a[itr] = a_integers[itr]*pow10[itr];

    if (phase == 1)  sub_result[0] = a[0];
    else  sub_result[itr] = sub_result[phase - 2] + a[itr];

    if (phase == 1)  {
        difference[0] = pairs[0] - pow(a_integers[0], 2);
        remainders[0] = difference[0]*100+pairs[1];
      }
    else  {
      difference[itr] = remainder; //*decimal_places[itr];
      remainders[itr] = remainder*decimal_places[itr] + pairs[itr+1]*decimal_places[itr+1];
    }
    display_algorithm(phase);
    phase += 1;
    itr += 1;

  }

/***************************************************************************/


long prepare_input_long(double N)  {

   long output_long;

  // Since I want to deal with inputs which contain digits to the right of
  // the decimal point, I will have to determine the number of digits after
  // the decimal point, then output_long = real*pow(10, this_number)
  /*

  My approach uses a similar strategy:
     First I have to determine if the number of digits after decimal point
     is odd or even.  If odd, then I have to add a zero to the end;
     that is, multiply by 10 after transforming into long int.
     So, if it is odd, I multiply by one more power of 10 than when even.

     I also want to handle numbers less than 1; that is, decimal fractions
     with integer part equal to 0.  This entails counting the number of
     leading zeros after the radix, ie, after "the decimal point".

     If the number of digits after the radix is odd, and the number of
     leading zeros after the radix is also odd, an additional zero must
     be appended.  This amounts to multiplying the long integer representation
     by another 10; hence output_long = N*pow(10, digits_after_radix + 1)
  */

  // PRESERVED DEBUGGING COMMENTS are merely commented out
  // Simply uncomment them for some rather enlightening information about
  // the internal processes.  Of course, you have to recompile and maybe
  // rename it sra_debugger.

  //  std::cout << "\nTECHNICAL OBSERVATIONS:\n";
    digits_before_radix = numbers_before_decimal_point(N);
  //  std::cout << "\ndigits_before_radix = " << digits_before_radix << "\t.\t";
    digits_after_radix = numbers_after_decimal_point(N);
//    std::cout << "digits_after_radix = " << digits_after_radix << "\n\n";


    int D = digits_after_radix;
    int Z = 0;

    if ( (digits_after_radix == 0) || (N - 0.0 < acceptedDiff) )
            {  output_long = N; }
    else
     {
       if (N < 1)   {
          if (N < 0.1) {
          leading_zeros_after_radix = count_leading_zeros_after_radix(N);
          Z = leading_zeros_after_radix;
        }
        else if ( (digits_before_radix == 0) && (N >= 0.1) )  {
          output_long *= 100;
          digits_before_radix += 1;
          point_location = 1;
        }
      if ( (Z%2 == 1) && (D%2 == 1) )  {   //  0.00049
            output_long = N*std::pow(10, D + 1);
            scale_factor += (D+1)/2;
         // This is a crucial part of the code as far as precision goes
         // These adjustments had to be made for handling  0 < input < 1
         // since the iteger input_long stores no leading zeros

       }
       else if ( (Z%2 == 0) && (D%2 == 1))  {    //  0.5, 0.005, 0.111
             output_long = N*std::pow(10, D+1);
             scale_factor += (D+1)/2;

        }
        else if ( ( (D%2 == 0)  && (Z%2 == 1) )  \
                     || ( ( (D%2 == 0) && (Z%2 == 0 )) ) )  { //  0.0625
            output_long = N*std::pow(10, D);                // 0.702896
            scale_factor += D/2;
            //Z -= 1;
          }
        }
        else {
           if (D%2 == 0)  {
              output_long = N*pow(10, D);
              scale_factor = D/2;
          }
          else {
                output_long = N*pow(10, D + 1);
                scale_factor = (D + 1)/2;
              }

          }
      }
      return output_long;

  } // end prepare_input_long

/***********************************************************************/

  /* Tokenizes a number by grouping its digits in pairs of two
   * from back to front and stores them in the vector 'tokens'.
   *
   *		example: input_long = 3453545;
   *
   * Now the vector 'tokens' will contain four objects (45,35,45,3)
   *
   */

  void tokenize(long input)  {
    const long step = 100;
    long x = step;
    for (long x = step, y = 1; x <= input*step; x *= step, y *= step)  {
      double token = ((input%x - input%y) / y);
      tokens.push_front (token);
     }

  }

/***********************************************************************/
  std::string fix( double x, int p )
    {
        std::ostringstream strout ;
        strout << std::fixed << std::setprecision(p) << x ;
        std::string str = strout.str() ;
        size_t end = str.find_last_not_of( '0' ) + 1 ;
        return str.erase( end ) ;
    }

/***********************************************************************/

template <class T>
T please_round(T input, int digits) {
    T factor = pow(T(10), digits);

    return round(input * factor) / factor;
}
/*************************************************************************/

/*  If Tsize is the number of pairs (tokens),
    and scale is the scale_factor, which is half
    the number of digits to the right of the decimal
    point, I imagined a variable H, where

    H = tokens.size() - scale_factor

    In the function, this is H = Tsize - scale
    Dfactor = 2*H - 2

   The decimal place will begin with 10^(highest even power)
   Each succeeding power of 10 will be shrunk by a factor of 10^2

  */

  // We go to <= tokens.size() only to include references to the set
  // of values one beyond the "n + 1" digits in the result

   void create_decimal_places(int Tsize, int scale)  {
     int Dfactor = 2*(Tsize - scale) - 2;
     for (int i = 0; i <= Tsize; i++)  {
       decimal_places[i] = std::pow(10, Dfactor);
       pow10[i] = std::pow(10, Dfactor/2);
       if (fabs(decimal_places[i] - 0.01) < acceptedDiff) {
          point_location = i;
        }
       Dfactor -= 2;
     }

// For Creation and Debugging ... Save and rename file before uncommenting
/*
    std::cout << "Created decimal_places array of reals:\n";
    for (int i = 0; i < Tsize; i++)  {
       std::cout << "decimal_places[" << i << "] = " << decimal_places[i]
                << "\n";
     }
     std::cout << "\npoint_location = " << point_location << "\n";
*/

} // END function

/***********************************************************************/

 int numbers_before_decimal_point(double x)  {
     int count = 0;
     int n = (int)x;
 if (n - 0.0 > acceptedDiff) {
     while (n > 0) {
       n /= 10;
       count++;
     }
   }
   return count;
  }

/***********************************************************************/

int numbers_after_decimal_point(double x)  {
  double acceptedDiff = 2e-13;
  if (x - 0.0 < acceptedDiff) return 0;     // acceptedDiff = 2.27898e-13;
   std::stringstream sso;
   sso << std::setprecision(9) << std::showpoint << std::fixed
       << std::abs( x - (int)x );   // 27.44 - 27 ---> 0.44
   std::string Zahl;
   sso >> Zahl;
   size_t end = Zahl.find_last_not_of( '0' ) + 1 ;
   Zahl.erase(end);
   int result;

   if ( std::abs( x - (int)x ) == 0) return 0;
  // if (x > 0.1)
    result = Zahl.length() - 2;  // We don't want to count 0 and .
   //else result = Zahl.length();

   if (result < 0) return 0;
   else return result;

 }
/***********************************************************************/

int count_leading_zeros_after_radix(double x)  {

  int zeros = 0;

  while ( (x < 0.1) && ( x > 0) )   {

      x *= 10;
      zeros += 1;
  }

  return zeros;
}

/***********************************************************************/

void create_input_string()  {

/*
  // List elements of tokens deque
  std::cout << "\nThe contents of tokens are: ";
  for (std::deque<double>::iterator i = tokens.begin(); i != tokens.end();++i)
  {
      std::cout << ' ' << *i;
  }
  std::cout << std::endl;
*/

  // Store in string for easy printing
  std::ostringstream convert;  // takes in pairs of double from tokens
  int count = 0;
  std::stringstream stream;  // stream << convert.str()
                            // stream >> pairs[count]
  for (std::deque<double>::iterator i = tokens.begin(); i != tokens.end();++i)
   {
       convert << *i;

       stream << convert.str();
       stream >> pairs[count];

    /***  The following was added so as to include fractional decimals,
          with 0 as the integer part, as soluable input.             ***/

       if (pairs[count] < acceptedDiff)   {
                if (count == 0)   {
                     point_location = 1;
                     input_str = '0';
                 }
                 else if (count == 1)  {
                    if (point_location == 1)  input_str += ".0" + convert.str();
                    else input_str += " 0" + convert.str();
                  }
                  else if (point_location == count)  input_str += ".0" + convert.str();
                       else input_str += " 0" + convert.str();
                 }
                 else if (pairs[count] < 10)  {
                         if (count == 0)  input_str +=  ' ' + convert.str();
                         else  if (count == point_location) {
                           input_str += ".0" + convert.str();
                         }
                         else if (count == 0) input_str += ' ' + convert.str();
                              else input_str += " 0" + convert.str();
                      }
                      else  if (count == point_location) {
                        input_str += '.' + convert.str();
                      }
                      else if (count == point_location) {
                        input_str += '.' + convert.str();
                      }
                      else input_str +=  ' ' + convert.str();

      /******     *****     ******    *****     *****     *****     *****/

             convert.str("");
             convert.clear();
             stream.str("");
             stream.clear();
             count +=1;      // our iterator for traversing through the deque
                             // as 'i' is not used since it is a kind of pointer

          }

    /* NO LONGER NEEDED but left here as a reference and clue for building
       functions which will transform numbers into strings for displaying
       the step by step details of the "By Hand Digit By Digit Algorithm"


    /*      if (tokens.size() < (digits) )   {
            input_str.erase(input_str.find_last_not_of(' ')+1);
            input_str += "0";
          }
************                                                     *********/
/*
            std::cout << "\nThe pairs array:\n";
            for (int i = 0; i < tokens.size(); i++)  {
              std::cout << "pairs[" << i << "] = "
                  << square_root::fix(, 2) << "\n\n";
             }
*/

/*
    No longer needed since I altered string to add space before numbers,
    not after.  As is my custom after hunting down such TECHGNOSIS, I leave
    the following here as a reference for "posterity".

          std::cout << "\n\nThe length of string is " << input_str.length() << "\n";

          input_str.erase(input_str.find_last_not_of(' ')+1);  // trims last " "

          std::cout << "input_str.erase(input_str.find_last_not_of(' ')+1);\n";
          std::cout << "The length of string is " << input_str.length();
          std::cout << "\n\nThe Input String is: " << input_str << "\n"
               << "----------------------------------------------------\n";

      std::cout << "input_str.erase(0,std::min(input_str.find_first_not_of('0'),"
                 << " input_str.size()-1));\n";
      input_str.erase(0, std::min(input_str.find_first_not_of('0'),\
             input_str.size()-1));

  */


    //  std::cout << "The length of string is " << input_str.length();
    //  std::cout << "\n\nThe Input String is: " << input_str << "\n";

    //  std::cout << "input_str.erase(0,std::min(input_str.find_first_not_of(' '),"
      //              << " input_str.size()-1));\n";

      input_str.erase(0, std::min(input_str.find_first_not_of(' '),\
              input_str.size()-1));


  //    std::cout << "The length of string is " << input_str.length();
  //    std::cout << "\n\nThe Input String is: " << input_str << "\n"
  //             << "----------------------------------------------------\n";

   }  // END function create_input_string

/***********************************************************************/

   void display_algorithm(int k)   {

      int p = digits + 1;

    int count;

    switch(k)  {

     case 1:
     {
   //if (k == 1)    {
   // PHASE 1
     count = k - 1;

     std::cout << "\n Notice the relationship between the decimal place values"
              << " of\n the original number given and those of its square root:"
              <<  "\n\n  " << decimal_places[itr] << " = (" << pow10[itr]
              << ") times (" << pow10[itr] << ")" << "\n";

     std::cout << "\n The systematic grouping of the numerals reduces "
      << "this initial step\n to finding the largest positive integer 'a' whose"
      << " square is not greater\n than " << pairs[0] << ".  We find that"
      << " this integer is " << a_integers[0] << ", so we write the numeral "
      << a_integers[0] << " directly above\n the 'least decimal place' of "
      << pairs[0] << " in our computation. We then subtract " << pow(a_integers[0], 2)
      << " from " << pairs[0] << "\n and then bring down the next two digits, ";

     print_pair(pairs[1], count);   // I want the 0 before numbers less than 10
     std::cout << ":\n\n";

    initialize(tokens.size(), digits_after_radix, k, a_integers);
    double number = std::pow(a_integers[0], 2);

    // For the first phase we will do this manually, step by step.
    // For cases k >= 2, we will use functions subtract and explain.

    // Testing zahl, the COUT_NumberTo_SRA_String function
    //   --- a workhorse I am depending on to finish this "project".

    std::cout << std::string(tab, space) << '-';       // << zahl(number);
    check_1o2(number, pairs[0]);
    if (point_location == 1)  std::cout << '.';

 // Step 1 is different from all remaining steps:

    std::cout << std::string(5*tab, space) << '(' << a_integers[0]
             << ")^2 = " << pow(a_integers[0], 2)
             << " < " << pairs[0] << line(2*tab, 0, 1);

    check_1o2(remainder, pairs[0]);
    print_pair(pairs[1], 0);
   pause();
   if (pairs[0] != 0)  {

     std::cout << "\n\n  Do you see that what we have done is equivalent to"
               << " subtracting\n  (" << a_integers[0] << " times "
               << pow10[itr] << ")^2 from " << input_double << " ?\n"
               << "\n  The remainder, " << remainders[0]
              << ", is not less than b times (2 times " << a_integers[0]*10
              << " plus b), or simply, b(" << 2*a_integers[0]*10
              << " + b) <= " << remainders[0];
    }

    std::cout << "\n\n  Find the largest integer 'b' in decimal place "
        << pow10[1] << " such that\n\n  [(" << pow10[1] << ")(b)] times [(2)("
        << (a_integers[0]) << ")(" << pow10[0] << ") + (" << pow10[1] << ")(b)]"
        << " = (" << pow10[1] << ")(b)*[" << 2*sub_result[0]
        << " + (" << pow10[1] << ")(b)] <= "
      //  << remainders[0]*100+pairs[1]*decimal_places[1] << "\n";
         << difference[0]*decimal_places[0]+pairs[1]*decimal_places[1] << "\n";

  if (decimal_places[1] != 1) {
  std::cout << "\n  By hand, on a separate sheet of paper, one may wish to "
            << "perform some operations.\n  First, we can multiply both "
            << "sides of the inequality by " << 1/decimal_places[1];

  std::cout << ":\n\n ";
  std::cout << " b(" << 2*sub_result[0]/pow10[1] << " + b) <= "
            << difference[0]*decimal_places[0]/decimal_places[1] + pairs[1];
  }

    if (point_location == 1)  {
        radix_index = 0;
        point_ = 1;
    }

    display_step += 1;
    break;
  }

  case 2:
//    if (k == 2) {
{
    count = k - 1;

    // We display the first step:

    initialize(tokens.size(), digits_after_radix, k, a_integers);
    std::cout << std::string(tab-1, space) << '-'
              << zahl(std::pow(a_integers[0], 2));
    if (point_location == 1)  std::cout << '.';
    std::cout << line(tab+2, 1, 0) << zahl(difference[0]);

    print_pair(pairs[1], 0);

    if (count == point_location)  {
      radix_index = count - 1;
      point_ = 1;
    }

   if (point_location == 1) {
     if (pairs[0] == 0) subtract(count, -3, -3, 2, -4, point_);
     else subtract(count, 0, -3, 2, -4, point_);
   }
   else subtract(count, -1, -3, 2, -1, point_);
   print_pair(pairs[k], k);

    std::cout << "\n\n\t" << "Next find the largest integer 'c' for the "
              << square_root::fix(pow10[k], p)
              << " digit such that\n\n\t\t [(2)(" << sub_result[count] << ") + ("
              << square_root::fix(pow10[k], p) << ")(c)] times ("
              << square_root::fix(pow10[k], p) << ")(c) <= "
              << remainders[count] << "\n\n\t\t [ " << 2*sub_result[count]
              << " + (" << square_root::fix(pow10[k], p) << ")(c) ] ("
              << square_root::fix(pow10[k], p) << ")(c) <= "
              << remainders[count] << std::endl;


      if (decimal_places[2] != 1) {
              std::cout << "\n  By hand, we can multiply both sides of the "
                        << "inequality by " << 1/decimal_places[2];

              std::cout << ":\n\n ";
              std::cout << "c(" << 2*sub_result[count]/pow10[k] << " + c) <= "
              << remainders[count]/decimal_places[2] << "\n";

            }

 display_step += 1;
 pause();
 break;

//    }   // END if (k == 2)
}
  case 3:
  //  if (k == 3) {
  {      count = k - 1;

        initialize(tokens.size(), digits_after_radix, k, a_integers);
        std::cout << std::string(tab-1, space) << '-'
                  << zahl(std::pow(a_integers[0], 2));
        if (point_location == 1)  std::cout << '.';
        std::cout << line(7, 0, 0) << zahl(difference[0]);

        print_pair(pairs[1], 0);

        if (count == point_location)  {
            radix_index = count - 1;
            point_ = 1;
        }


        if (point_location == 1) {
          if (pairs[0] == 0)  {
            if (pairs[1] >= 10) subtract(1, -3, -3, 2, -4, point_);
            else subtract(1, -3, -3, 2, -4, point_);
          }
          else subtract(1, 0, -3, 2, -4, point_);
        }
        else subtract(1, -1, -3, 2, -1, point_);
        print_pair(pairs[count], count);

        if (point_location == 1) {
          if (pairs[0] == 0)  {
            if (pairs[1] >= 10) subtract(count, 0, -3, 3, -4, point_);
            else subtract(count, 0, -3, 5, -4, point_);
          }
          else subtract(count, 0, -3, 6, -4, point_);  // pairs[0] > 0
        }
        else subtract(count, 2, -1, 4, -1, point_);
        print_pair(pairs[k], k);

        std::cout << "\n\n\t" << "We now seek an integer 'd' for the "
                 << square_root::fix(pow10[k], p)
                 << " digit such that\n\n\t\t\t [(2)(" << sub_result[count]
                 << ") + (" << square_root::fix(pow10[k], p)
                 << ")(d)] times (" << square_root::fix(pow10[k], p)
                 << ")(d) <= " << remainders[count] << "\n";

      if (decimal_places[k] != 1) {
       std::cout << "\n  By hand, we can multiply both sides of the "
                  << "inequality by " << 1/decimal_places[k];
       std::cout << ":\n\n ";
       std::cout << "d(" << 2*sub_result[count]/pow10[k] << " + d) <= "
               << remainders[count]/decimal_places[k] << "\n";

    }

        display_step += 1;
         pause();
        break;
  //  }  // END if (k == 3)
}
   case 4:
  //   if (k == 4) {
  {      count = k - 1;

    initialize(tokens.size(), digits_after_radix, k, a_integers);
    std::cout << std::string(tab-1, space) << '-'
              << zahl(std::pow(a_integers[0], 2));
    if (point_location == 1)  std::cout << '.';
    std::cout << line(7, 0, 0) << zahl(difference[0]);

    print_pair(pairs[1], 0);

    if (count == point_location)  {
        radix_index = count - 1;
        point_ = 1;
    }


    if (point_location == 1) {
      if (pairs[0] == 0) subtract(1, -3, -3, 2, -4, point_);
      else subtract(1, 0, -3, 2, -4, point_);
    }
    else subtract(1, -1, -3, 2, -1, point_);
    print_pair(pairs[2], 2);


    if (point_location == 1) {
      if (pairs[0] == 0)  {
        if (pairs[1] >= 10) subtract(2, 0, -3, 4, -4, point_);
        else subtract(2, 0, -3, 5, -4, point_);
      }
      else subtract(2, 0, -3, 4, -4, point_);  // pairs[0] > 0
    }
    else subtract(2, 2, -1, 3, -1, point_);
    print_pair(pairs[count], count);

    if (point_location == 1) {
      if (pairs[0] == 0)  {
        if (pairs[1] >= 10) subtract(count, 0, -3, 3, -4, point_);
        else subtract(count, 0, -3, 7, -4, point_);
      }
      else subtract(count, 0, -3, 7, -4, point_);  // pairs[0] > 0
    }
    else subtract(count, 2, -1, 6, -1, point_);
    print_pair(pairs[k], k);

        std::cout << "\t\t" << "We now seek an integer 'e' for the "
                  << square_root::fix(pow10[k], p)
                  << " digit such that\n\n\t\t\t [(2)(" << sub_result[count] << ") + ("
                  << square_root::fix(pow10[k], p) << ")(e)] times ("
                  << square_root::fix(pow10[k], p) << ")(e) <= "
                  << remainders[count] << "\n";

       if (decimal_places[k] != 1) {
           std::cout << "\n  By hand, we can multiply both sides of the "
                    << "inequality by " << 1/decimal_places[k];
           std::cout << ":\n\n ";
           std::cout << "e(" << 2*sub_result[count]/pow10[k] << " + e) <= "
                    << remainders[3]/decimal_places[k] << "\n";

       }

        display_step += 1;
         pause();
        break;
  //  }  // END if (k == 4)
}

case 5:
//   if (k == 5) {
{      count = k - 1;

 initialize(tokens.size(), digits_after_radix, k, a_integers);
 std::cout << std::string(tab-1, space) << '-'
           << zahl(std::pow(a_integers[0], 2));
 if (point_location == 1)  std::cout << '.';
 std::cout << line(7, 0, 0) << zahl(difference[0]);

 print_pair(pairs[1], 0);

 if (count == point_location)  {
     radix_index = count - 1;
     point_ = 1;
 }


 if (point_location == 1) {
   if (pairs[0] == 0) subtract(1, -3, -3, 2, -4, point_);
   else subtract(1, 0, -3, 2, -4, point_);
 }
  else subtract(1, -1, -3, 2, -1, point_);
  print_pair(pairs[2], 2);

  if (point_location == 1) {
     if (pairs[0] == 0)  {
       if (pairs[1] >= 10) subtract(2, 0, -3, 3, -4, point_);
       else subtract(2, 0, -3, 5, -4, point_);
     }
     else subtract(2, 0, -3, 2, -4, point_);  // pairs[0] > 0
   }
   else subtract(2, 2, -1, 3, -1, point_);
   print_pair(pairs[3], 3);

   if (point_location == 1) {
     if (pairs[0] == 0)  {
       if (pairs[1] >= 10) subtract(count-1, 0, -3, 3, -4, point_);
       else subtract(count-1, 0, -3, 7, -4, point_);
     }
     else subtract(count-1, 0, -3, 2, -4, point_);  // pairs[0] > 0
   }
   else subtract(count-1, 2, -1, 3, -1, point_);
   print_pair(pairs[count], count);


   if (point_location == 1) {
     if (pairs[0] == 0)  {
       if (pairs[1] >= 10) subtract(count, 0, -3, 3, -4, point_);
       else subtract(count, 0, -3, 7, -4, point_);
     }
     else subtract(count, 0, -3, 2, -4, point_);  // pairs[0] > 0
   }
   else subtract(count, 5, -1, 3, -1, point_);
   print_pair(pairs[k], k);


     std::cout << "\t\t" << "\n\nWe now seek an integer 'e' for the "
               << square_root::fix(pow10[k], p)
               << " digit such that\n\n\t\t\t [(2)(" << sub_result[count] << ") + ("
               << square_root::fix(pow10[k], p) << ")(e)] times ("
               << square_root::fix(pow10[k], p) << ")(e) <= "
               << remainders[count] << "\n";

     if ( (decimal_places[k] != 1) && (phase < digits) ) {
         std::cout << "\n  By hand, we can multiply both sides of the "
                   << "inequality by " << 1/decimal_places[k];
         std::cout << ":\n\n ";
         std::cout << "e(" << 2*sub_result[count]/pow10[k] << " + e) <= "
                  << remainders[count]/decimal_places[k] << "\n";
     }


     display_step += 1;
      pause();
     break;
//  }  // END if (k == 5)
}

  default:
{
   if (k == 6)  {
   std::cout << "\n\nAt this point we will only display the edges of "
             << "the systematic work you will see on \npaper when "
             << "doing this work with pencil ...\n\n";
    }
   count = k - 1;

   initialize(tokens.size(), digits_after_radix, k, a_integers);
   std::cout << std::string(tab-1, space) << '-'
             << zahl(std::pow(a_integers[0], 2));
   if (point_location == 1)  std::cout << '.';
   std::cout << line(7, 0, 0) << zahl(difference[0]);

   print_pair(pairs[1], 0);

   if (count == point_location)  {
       radix_index = count - 1;
       point_ = 1;
   }


    if (point_location == 1) {
      if (pairs[0] == 0) subtract(1, -3, -3, 2, -4, point_);
      else subtract(1, 0, -3, 2, -4, point_);
    }
     else subtract(1, -1, -3, 2, -1, point_);
     print_pair(pairs[2], 2);

     if (point_location == 1) {
        if (pairs[0] == 0)  {
          if (pairs[1] >= 10) subtract(2, 0, -3, 3, -4, point_);
          else subtract(2, 0, -3, 5, -4, point_);
        }
        else subtract(2, 0, -3, 2, -4, point_);  // pairs[0] > 0
      }
      else subtract(2, 2, -1, 3, -1, point_);
      print_pair(pairs[3], 3);

      if (point_location == 1) {
        if (pairs[0] == 0)  {
          if (pairs[1] >= 10) subtract(3, 0, -3, 3, -4, point_);
          else subtract(3, 0, -3, 7, -4, point_);
        }
        else subtract(3, 0, -3, 2, -4, point_);  // pairs[0] > 0
      }
      else subtract(3, 2, -1, 3, -1, point_);
      print_pair(pairs[4], 4);

    if (point_location == 1) {
      if (pairs[0] == 0)  {
        if (pairs[1] >= 10) subtract(4, 0, -3, 3, -4, point_);
        else subtract(4, 0, -3, 7, -4, point_);
      }
      else subtract(4, 0, -3, 2, -4, point_);  // pairs[0] > 0
     }
    else subtract(4, 5, -1, 3, -1, point_);
    print_pair(pairs[5], 5);

   if (k > 6) {
     std::cout << "\n     -------------------\t step "
               << k << " displayed below.....\n";
    }

// REST IN PEACE Michael William Hentrich (1967 - )

   if (point_location == 1) {
     if (pairs[0] == 0)  {
       if (pairs[1] >= 10) subtract(count-1, 0, -3, 3, -4, point_);
       else subtract(count-1, 0, -3, 7, -4, point_);
     }
     else subtract(count-1, 0, -3, 2, -4, point_);  // pairs[0] > 0
    }
   else subtract(count - 1, 5, -3, 2, -1, point_);
   print_pair(pairs[count], count);

   if (point_location == 1) {
     if (pairs[0] == 0)  {
       if (pairs[1] >= 10) subtract(count, 0, -3, 3, -4, point_);
       else subtract(count, 0, -3, 7, -4, point_);
     }
     else subtract(count, 0, -3, 2, -4, point_);  // pairs[0] > 0
    }
   else subtract(count, 5, -1, 3, -1, point_);
   print_pair(pairs[k], k);




   std::cout << "\t\t" << "\n\n We now seek an integer X for the "
             << square_root::fix(pow10[k], p)
             << " digit such that\n\n\t[(2)(" << sub_result[count] << ") + ("
             << square_root::fix(pow10[k], p) << ")(X)] times ("
             << square_root::fix(pow10[k], p) << ")(X) <= "
             << remainders[count] << "\n";

    if ( (decimal_places[k] != 1) && (phase < digits) )  {
    std::cout << "\n  By hand, we can multiply both sides of the "
             << "inequality by " << 1/decimal_places[k];
     std::cout << ":\n\n ";
     std::cout << "c(" << 2*sub_result[count]/pow10[k] << " + c) <= "
              << remainders[count]/decimal_places[k] << "\n";
    }


  display_step += 1;
   pause();
 }
 }  // END switch

} // END display_algorithm function

/***************************************************************************/

// function to subtract subtrahend[count] from pairs[count]
// placing minus symbol in place, drawing the line, and placing
// the result, remainders[count]

// Before the subtraction we would display an explanation to the right
// ONLY if step == k

// Create global variable display_step initialized to 1
// In phase 1 (k == 1), after displaying custom explanation (in k == 1) phase,
// increment display_step += 1

// In (k >= 2):  before calling subtract(int i, int ds, int dl, int l, dr, int point)
// Yes, it's that complicated.
//  I will display explanation if display_step == k
//
// To keep the display algorithm in order after the radix makes its appearance,
// introduce global variable point_ intitialized to 0.
//
// if (count == point_location)  {
//   radix_index = count;
//   point_ = 1;
// }
//
//  if (point_ == 1) set value of tab to have uniformity for the rest of the display

// After the first step, the result of the multiplication is the subtrahend,
// so, after this initial step, the explanations will be "called from" subtract.

// Only step 1 requires a custom hard coded explanation since it is the square_root
// of the first result.

// Think of j as count = k-1 = [i]

/*****************************************************************************/
/****                                                                     ****/
/****             We cannot access sub_result[j - 1]                      ****/
/****             j <---| count = k - 1 = 1 - 1 = 0                       ****/
/****             so (j - 1) <---| -1                                     ****/
/****             There is no location sub_result[-1] !!!                 ****/
/****                                                                     ****/
/****             DO NOT USE subtract function in First PHASE             ****/
/****             as subtract calls explain.                              ****/
/*****************************************************************************/

void subtract (int i, int ds, int dl, int l, int dr, int point)   {

  int t = tokens.size();
  int dar = digits_after_radix;

 if ( (pairs[0] == 0) && (i == 1) )  std::cout << "   ";

 if ( (pairs[0] > 0) && (i == 1) && (point_location == 1) && (subtrahends[i] >= 1) )
    std::cout << '\n' << std::string(tab+ds-1, space);
 else if ( (subtrahends[i] == 0) && (point_location != 1) )
         std::cout << '\n' << std::string(tab+ds-3, space);
      else std::cout << '\n' << std::string(tab+ds, space);

  if ( (pairs[0] == 0) && (i == 1) )  std::cout << "   ";

  std::cout << '-' ;

  if ( (subtrahends[i] == 0) && (point_location == 1) ) std::cout << " 0.00";
  else {
        if ( (point_location == 1) && (subtrahends[i] < 1) )
              std::cout << " 0";

        if ( (point == 1) && (i >= radix_index) )
              std::cout << zahl(subtrahends[i]);

        else std::cout << zahl(subtrahends[i]/decimal_places[i]);

        if ( (subtrahends[i] == 0) && (point_location != 1) ) std::cout << ".00";
     }

  if ( display_step == (i + 1) ) explain(i);

  std::cout << line(7 + l,tab+dl, tab+dr);
  if ( (remainders[i] < 1) && (remainders[i] > 0)  )  std::cout << "  0";
  if ( (point == 1) && (i >= radix_index) )
        std::cout << zahl(difference[i]*decimal_places[i]);
  else  std::cout << zahl(difference[i]);



}
/***************************************************************************/


    // EXAMPLE:
    //  7  53.00    | 27.44
    //  4-          |___________________________________
    //  3=          | 2*2=4 < 7                                => 2
    //    353-      | (((2*2)*10 + 7)*7 = 329 < 353            => 7
    //    329=      |
    //     24.00-   | (((2*27) + 0.4)*0.4 = 21.76 < 24         => .4
    //     21.76=   |
    //      2.24-   | (((2*27.4) + 0.04)*0.04 = 2.1936 < 2.24  => 0.04
    //      2.1936  |

void explain (int j)  {   // Think of j as count = k-1 = [i]
                         // So we cannot access sub_result[j - 1]
                         // j <---| count = k - 1 = 1 - 1 = 0
                         // so (j - 1) <---| -1
                         // There is no location sub_result[-1] !!!
                         // DO NOT USE subtract function in First PHASE
                         // as subtract calls explain.

 if (j <= 2)  std::cout << std::string(tab+2, space);
 else  std::cout << std::string(tab+1, space);

 if (j > 4)  std::cout << "\t\tArithmetic Drudgery .....";
 else {
   std::cout << "[(2)(" << sub_result[j - 1]
            << ") + " << a[j] << "]*(" << a[j] << ") = ("
            << 2*sub_result[j - 1] << " + " << a[j] << ")("
            << a[j] << ") = ("
            << 2*sub_result[j - 1] + a[j] << ")("
            << a[j] << ") = " << subtrahends[j];
 }
}

/***************************************************************************/

void check_1o2(double rem, double pair)  {
  if ( ( (rem < 10) && (pair < 10) ) || (rem < 10) && (pair > 9))  {
    std::cout << ' ' << rem;
  }
  if ( (rem > 9)  && (pair > 9) )  {
    std::cout << rem;
  }
  // Since I use the subtract procedure for all steps after the first phase,
  // I only use this sparingly.  I do not code for the case when the first
  // number is greater than 9 and the second is less.  It just would not occur.

}

void print_pair (double pair, int index)  {   // I want the 0 before numbers less than 10
      if (point_location == 1) {
        if ( (remainders[index] >= 1) || \
              ((difference[index-1] == 0) && (pair == 0) ) \
            ) std::cout << '.';
        else std::cout << ' ';
      }
      else if (point_location == index)  std::cout << '.';
           else std::cout << ' ';

      if (pair < 10)  std::cout << '0' << pair;
      else std::cout << pair;
}

void print_results (double r[], int upto)  {

  for (int j = 0; j < upto; j++)  {
    std::cout << r[j]; // With initialization starting at tab+1
    if ( j == (point_location - 1))  std::cout << ". ";
    else std::cout << "  ";
   }
}

int pause ()  {

   do  {
       std::cout << std::endl << std::endl;
       std::cout << std::string(tab, space) << "Press ENTER "
                 << "  [ <-----||| ]  to continue.\n";
      } while (std::cin.get() != '\n');

   return 0;
}

/***********************************************************************/

void initialize (int tsize, int dar, int j, double r[])  {

 // I have print_results handling how to print the results (no shit?)
 // So, all we must do initially get to the point where These
 // results will be displayed above the vinculum.
// Since we want the print_results function's point of view
// to be at tab+3, we want to jump ahead tab+2 spaces:

 std::cout << "\n" << std::string(tab+2, space);

  print_results(r, j);
  std::cout << symbol(tsize, dar, j) << input_str << std::endl;

}

/***********************************************************************/
// Define symbol:  radical sign and hyphen
// Required inputs?  distance and length
// example:  symbol(tab+1, vsize)

std::string symbol(int tsize, int dar, int step)  {  // dar --> digits after radix

  std::string str = "";
  str += '\n' + std::string(tab, space) + '/' \
      + vinculum(vsize(tsize, dar, step)) + "\n  \\/";

  if (pairs[0] < 10) str += "  ";  // We want to line up the Input Square
  else str += ' ';                 // with its Square Root above it
  return str;            // so that the "units" digits line up
}                        // Each digit in the result is from 0 to 9


std::string vinculum(int v)  {
  std::string vinculum = std::string(v, hyphen);
  return vinculum;
}

int vsize (int tsize, int dar, int step)  {     // tsize ---> tokens.size()
  int v = (int)(3*tsize + digits);                // dar ---> digits_after_radix
  if (step > tsize) v += 4*(step - tsize);     // step ---> phase k

  return v;
}

std::string line (int len, int dl, int dr)  {
  std::string L = "";

  L += '\n' + std::string(tab + dl, space);

  if (display_step < 5)  {
     L += std::string(len, '-') + '\n' + std::string(tab + dr, space);
  }
  else  L += std::string((int)(5*display_step*len/12), '_')
          + '\n' + std::string(tab + dr, space);

  return L;
}

/***************************************************************************/
/*******  The original name of this function was:                     ******/
/*******              COUT_NumberTo_SRA_String                        ******/
/*******                                                              ******/
/*******    It is a miniture version of the Mother Class Square Root. ******/
/****   I created it simply to display the subtrahends and remainders   ***/
/****   in an aesthetically beautiful and orderly manner, following    ****/
/****  the manner of grouping in twos.                                *****/
/**************************************************************************/


std::string zahl (double N)  {

   long io_long;
   int Da, Db, Lfactor = 0, radix_location = -1, efactor;
   double dval[25];
   double p[25];
   double p10[25];
   std::deque<double> Zahl;
   std::string result;
   double const acceptedDiff = 2.27898e-13;

  //  std::cout << "\nTECHNICAL OBSERVATIONS in ConvertToInteger:\n";
    Db = numbers_before_decimal_point(N);
//    std::cout << "\nTEMP: digits before radix = " << Db << "\t.\t";
    Da = numbers_after_decimal_point(N);
//    std::cout << "\nTEMP: digits after radix = " << Da << "\n\n";

 int z = 0;

 if (N < 0.1) { z = count_leading_zeros_after_radix(N); }
 else { z = 0; }

    if ( (Da == 0) || (N - 0.0 < acceptedDiff) ) io_long = N;
    else
     {
       if (N < 1)   {

            if ( (z%2 == 1) && (Da%2 == 1) )  {   //  0.00049
            io_long = N*std::pow(10, Da + 1);
            Lfactor += (Da+1)/2;
         // This is a crucial part of the code as far as precision goes
         // These adjustments had to be made for handling  0 < input < 1
         // since the iteger input_long stores no leading zeros

          }
          else if ( (z%2 == 0) && (Da%2 == 1))  {     // 0.5, 0.111
               io_long = N*std::pow(10, Da + 1);
               Lfactor += (Da + 1)/2;
          }
          else if ( ( (Da%2 == 0)  && (z%2 == 1) )  \
                       || ( ( (Da%2 == 0) && (z%2 == 0 )) ) )  { //  0.0625

                io_long = N*std::pow(10, Da);
                Lfactor += Da/2;
            //  z -= 1;       // not necessary in the zahl function
            }
          }

          else {

           if (Da%2 == 0)  {
              io_long = N*pow(10, Da);
              Lfactor = Da/2;
          }
           else {
                io_long = N*pow(10, Da + 1);
                Lfactor = (Da + 1)/2;
              }

          }
      }

// DEBUG
//std::cout << "\nio_long = " << io_long << std::endl;

    /*********** Now we have Long Integer to be "tokenized" ****************/

if (io_long <= std::numeric_limits<long>().max())  {

    const long step = 100;
    long x = step;
    for (long x = step, y = 1; x <= io_long*step; x *= step, y *= step)  {
      double t = ((io_long%x - io_long%y) / y);
      Zahl.push_front(t);
     }

     if ( (Da > 0) && (N < 0.1) ) {
       if (z%2 == 0) {
            if (z == 2) {
                 while (z > 0) {
                   Zahl.push_front(0);
                   z -= 1;    // for 0.00625
                  }
            }  // END if z == 2
            else if (z == 4)    {          // z == 4
                    while (z > 1) {
                       Zahl.push_front(0);
                       z -= 1;    // for 0.0000625
                     }
               }
               else {                   // z > 6
                     while (z >= 0) {
                          Zahl.push_front(0);
                          z -= 2;    // for 0.00000000625
                      }
                    }
        }  // END if z is even
        else { // BEGIN if z is odd
          while (z > 0) {
              Zahl.push_front(0);
              z -= 2;    // subtract 1 ?  No, subtract 2
           }
        }   // END if z is odd
      }
      else if (io_long == 0)  Zahl.push_front(0);



  /************************************** Not necessary for function Zahl
  /************************************** Left here just to show where the
  /************************************** rest of zahl comes from.
  //   if (Zahl.size() < 5 )   {
  //       efactor = 2*(5 - Zahl.size());
  //       io_long *=std::pow(10, efactor);
  //       Lfactor += efactor/2;
  //       for (int i = 0; i < (efactor/2); i++)  {
  //          Zahl.push_back(0);
  //       }
  //     }
  /************************************************************************/


   // CREATE temporary decimal places

   int dfactor = 2*(Zahl.size() - Lfactor) - 2;
   for (int i = 0; i < Zahl.size(); i++)  {
     dval[i] = std::pow(10, dfactor);
     p10[i] = std::pow(10, dfactor/2);
     if  ((fabs(dval[i] - 0.01) < acceptedDiff))  {
        radix_location = i;
      }
     dfactor -= 2;
   }
  if ( (radix_location == -1) && (N < 1) ) radix_location = 0;

  /*****  Now we have "tokenized" deque Zahl to convert to std::string  *****/
  /****                                                                 *****/
  /****   This is done last but is included at the tail end of          *****/
  /****    a very long IF statement in a function which is a replica    *****/
  /****    of the Mother Class square_root, using many of it TOOLS      *****/
  /****                                                                 *****/
  /****   We have prepared an integer, io_long, from input (double),    *****/
  /****  We created deque Zahl <double" from this io_long, checking     *****/
  /**** for leading zeros just as we did in the main class; we even     *****/
  /****  check size for future zero padding, create its decimal place   *****/
  /****  values. and final return a string that we can use in our       *****/
  /***   display of the Square Root Algorithm ....                      *****/
  /****                                                                 *****/
  /**************************************************************************/

     // Store in string for easy printing
     std::ostringstream convert;  // takes in pairs of double from Zahl
     int count = 0;
     std::stringstream stream;  // stream << convert.str()
                               // stream >> p[count]



     for (std::deque<double>::iterator i = Zahl.begin(); i != Zahl.end();++i)
      {
          convert << *i;

          stream << convert.str();
          stream >> p[count];

       /***  The following was added so as to include fractional decimals,
             with 0 as the integer part, as soluable input.             ***/

             if (p[count] < acceptedDiff)   {
                      if (count == 0)   {
                           radix_location = 1;
                           result = "  0";
                       }
                       else if (count == 1)  {
                          if (radix_location == 1)  result += ".0" + convert.str();
                          else result += " 0" + convert.str();
                        }
                        else if (radix_location == count)  result += ".0" + convert.str();
                             else result += " 0" + convert.str();
                       }
                       else if (p[count] < 10)  {
                               if (count == 0)  result +=  "  " + convert.str();
                               else  if (count == radix_location) {
                                 result += ".0" + convert.str();
                               }
                               else result += " 0" + convert.str();
                            }
                            else  if (count == radix_location) {
                              result += '.' + convert.str();
                            }
                            else result +=  ' ' + convert.str();

         /******     *****     ******    *****     *****     *****     *****/

                convert.str("");
                convert.clear();
                stream.str("");
                stream.clear();
                count +=1;      // our iterator for traversing through the deque
                                // as 'i' is not used since it is a kind of pointer

             }

      } // END of very long if (io_long <= std::numeric_limits<long>().max())


  if (N < 1) result.erase(0, std::min(result.find_first_not_of("  0"), \
             result.size()-1));

  // For now, I want spaces in front.  This can be altered easily when necessary
  //    result.erase(0, std::min(result.find_first_not_of(' '), result.size()-1));

  // In the SRA, the next pair in the input string is brought down after
  // each successive subtraction.  I don't want remainder having extra 00.
  // size_t end = result.find_last_not_of( " 00" ) + 1 ;
 // result.erase( end );

      // if (Da%2 == 1) result += '0';
      if (N < acceptedDiff)  result = "  0";
      return result;

} // END function zahl

/***************************************************************************/

void analyze (double a[], int d) {

  int LAST = 2;
  int NEXT = 3;
  int MAX = d;


  std::cout << "\n\n We have shown HOW the Digit by Digit Square Root Algorithm"
            << " works.\n\n Now we will show WHY it works, expressed in algebraic"
            << " terms.\n\n Let the variables ";
  int a_ = 1;
  for (int i = 0; i < d - 1; i++)  {
    std::cout << 'a' << i+1 << ", ";
    a_ += 1;
  }
  std::cout << "and a" << a_ << " represent the real values of each\n digit in the"
            << " square root we've extracted from your given number.\n\n"
            << " That is,\n let (";
  a_ = 1;
  for (int i = 0; i < d - 1; i++)  {
      std::cout << 'a' << i+1 << " + ";
      a_ += 1;
    }
  std::cout << 'a' << a_ << ") correspond to (";
  a_ = 0;
  for (int i = 0; i < d - 1; i++)  {
      std::cout << a[i] << " + ";
      a_ += 1;
    }
    std::cout << a[a_] << ")\n\n";

    std::cout << " While learning HOW this Square Root Algorithm works may "
              << "have\n seemed tedious, the following explanation of WHY it works "
              << "\n is refreshingly straight-forward since we start with "
              << "the result\n and show how the sum of these values squared "
              << "is as close to\n the number you gave without exceeding it.\n\n";

    std::cout << " First we express this in algebraic terms:\n\n";
pause();
    std::cout << "(";
    a_ = 1;
    for (int i = 0; i < d - 1; i++)  {
        std::cout << 'a' << i+1 << " + ";
        a_ += 1;
      }
    std::cout << 'a' << a_ << ")^2 \n\n\t\t = ";

    for (int i = 1; i < d; i++) {
      std::cout << "(a" << i << ")^2 + 2(";

      for (int j = 1; j < i+1; j++)  {
        std::cout << 'a' << j;
        for (int k = (j+1); k < (i+1); k++) {
          std::cout << " + a" << k;
        }
        std::cout << ")a" << i + 1;
        if (i < (d - 1))  std::cout << "\n\t\t + ";
        break;
      }

    }
    std::cout << " + (a" << d << ")^2\n\n\t\t = ";


std::cout << "(a1)^2 + [ 2(a1) + a2 ](a2)";
for (int i = 1; i < (LAST + 1); i++)  {
   if (NEXT > MAX)  break;
   std::cout << "\n\t\t + [ 2(";
   for (int j = 1; j < NEXT; j++)  {
     std::cout << 'a' << j;
     if (j < LAST) std::cout << " + ";
     else std::cout << ") + a" << NEXT << ']';
      }
   std::cout << "(a" << NEXT << ')';
   LAST += 1;
   NEXT += 1;
 }
 std::cout << "\n\n";

pause();

std::cout << "For our particular case:\n\n";

std::cout << "(";
 a_ = 0;
 for (int i = 0; i < MAX - 1; i++)  {
     std::cout << a[i] << " + ";
     a_ += 1;
   }
   std::cout << a[a_] << ")^2\n\n\t\t = ";


   for (int i = 0; i < (MAX - 1); i++) {
     std::cout << '(' << a[i] << ")^2 + 2(";

     for (int j = 0; j < i+1; j++)  {
       std::cout << a[j];
       for (int k = (j+1); k < (i+1); k++) {
         std::cout << " + " << a[k];

       }
       std::cout << ")(" << a[i + 1] << ')';
       if (i < (MAX - 2))  std::cout << "\n\t\t + ";
       break;
     }

   }
   std::cout << " + (" << a[MAX-1] << ")^2\n\n\t\t = ";


std::cout << '(' << a[0] << ")^2 + [ 2(" << a[0] << ") + " << a[1]
          << " ](" << a[1] << ")";
LAST = 1;
NEXT = 2;

for (int i = 0; i < (LAST + 1); i++)  {
  if (NEXT > (MAX - 1))  break;
  std::cout << "\n\t\t + [ 2(";
  for (int j = 0; j < NEXT; j++)  {
    std::cout << a[j];
    if (j < LAST) std::cout << " + ";
    else std::cout << ") + " << a[NEXT] << ']';
     }
  std::cout << "(" << a[NEXT] << ')';
  LAST += 1;
  NEXT += 1;
}
std::cout << "\n\n\t\t = ";

std::cout << std::pow(a[0],2) << " + [ 2(" << a[0] << ") + " << a[1]
          << " ](" << a[1] << ")";

LAST = 1;
NEXT = 2;
double sum = 0;

for (int i = 0; i < (LAST + 1); i++)  {
  if (NEXT > (MAX - 1))  break;
  std::cout << "\n\t\t + [ 2(";
  sum = 0;
  for (int j = 0; j < NEXT; j++)  {
      sum += a[j];
      if (j == LAST) {
        std::cout << sum << ") + " << a[NEXT] << " ]";
       }
    }
  std::cout << "(" << a[NEXT] << ')';
  LAST += 1;
  NEXT += 1;
}
std::cout << "\n\n\t\t = ";

std::cout << std::pow(a[0],2) << " + (" << 2*a[0] << " + " << a[1]
          << " )(" << a[1] << ")";

LAST = 1;
NEXT = 2;

for (int i = 0; i < (LAST + 1); i++)  {
  if (NEXT > (MAX - 1))  break;
  std::cout << "\n\t\t + (";
  sum = 0;
  for (int j = 0; j < NEXT; j++)  {
      sum += a[j];
      if (j == LAST) {
        std::cout << 2*sum << " + " << a[NEXT] << ')';
       }
    }
  std::cout << "(" << a[NEXT] << ')';
  LAST += 1;
  NEXT += 1;
}
pause();

std::cout << "\n\n\t\t = ";

std::cout << std::pow(a[0],2) << " + (" << 2*a[0] + a[1]
          << ")(" << a[1] << ")";

LAST = 1;
NEXT = 2;

for (int i = 0; i < (LAST + 1); i++)  {
  if (NEXT > (MAX - 1))  break;
  std::cout << "\n\t\t + (";
  sum = 0;
  for (int j = 0; j < NEXT; j++)  {
      sum += a[j];
      if (j == LAST) {
        std::cout << 2*sum + a[NEXT] << ')';
       }
    }
  std::cout << "(" << a[NEXT] << ')';
  LAST += 1;
  NEXT += 1;
}
std::cout << "\n\n\t\t = ";

std::cout << std::pow(a[0],2) << " + " << (2*a[0] + a[1])*a[1];
LAST = 1;
NEXT = 2;
double total_sum = std::pow(a[0], 2) + (2*a[0] + a[1])*a[1];

for (int i = 0; i < (LAST + 1); i++)  {
  if (NEXT > (MAX - 1))  break;
  std::cout << " + ";
  sum = 0;
  for (int j = 0; j < NEXT; j++)  {
      sum += a[j];
      if (j == LAST) {
        std::cout << (2*sum + a[NEXT])*a[NEXT];
       }
    }
  total_sum += (2*sum + a[NEXT])*a[NEXT];
  LAST += 1;
  NEXT += 1;
}

std::cout << " = " << total_sum << "\n\n";
std::cout << " We first showed HOW we calculated " << sub_result[d - 1] << " as the largest "
          << "\n " << d << " digit number whose square is not greater than "
          << input_double << ".\n\n  Do you see WHY this algorithm works?\n\n It is based upon the"
          << " formula for the square of a binomial.\n\n ";

std::cout << "(";
 a_ = 0;
 for (int i = 0; i < MAX - 1; i++)  {
     std::cout << a[i] << " + ";
     a_ += 1;
   }
   std::cout << a[a_] << ")^2 = " << total_sum << " <= " << input_double;
   std::cout << "\n\n";

}  // END function analyze;


/***************************************************************************/
public:
  explicit square_root(double input_d, int digits_) :  \
                  input_double(input_d),
                  result(0),
                  remainder(0),
                  tmp(0),
                  digits(digits_ + 1),  // find root to (n+1) digits and let
                                        // user round off the last digit
                  phase(1),
                  itr(0),
                  scale_factor(0),  // for adjusting result due to parsing
                  s(0),  // for storing values of subtrahend in approximate
                  point_location(-1),  // if no decimal point, it remains -1
                                     // if there is a decimal_place values
                                     // of 10^(-2), its index is point_location
                 acceptedDiff(2.27898e-13),  // for comparing floats to zero

                 input_str(""),
                 hyphen('-'),
                 tab(4),
                 space(' '),
                 display_step(1),
                 point_(0)


  /* The purpose of this is for printing square root result
     in an experimental yet somewhat primitive display to
     show one how to compute by hand with pencil and paper
  */
{

  // Initialize subtrahends array to 0 for displaying cleaner results
  for (int i = 0; i < 25; i++)  {
     subtrahends[i] = 0;
     remainders[i] = 0;
     pairs[i] = 0;
   }

 input_long = prepare_input_long(input_double);

// Set precision for displaying input_double

 int p = digits + 1;

/*
    //DEBUG:
 std::cout << "input_double = " << square_root::fix(input_double, p)
       << "   <----->   input_long = "  << input_long << "\n\n";
*/

if (input_long <= std::numeric_limits<long>().max())  {
    tokenize(input_long);
    if ( (digits_after_radix > 0) && (input_double < 1) ) {
      leading_zeros_after_radix = count_leading_zeros_after_radix(input_double);
      int Z = leading_zeros_after_radix;
      while (Z > 0) {
         tokens.push_front(0);
         Z -= 2;
       }
     }
    if (point_location == 1)  tokens.push_front(0);

    if (tokens.size() < (digits - 1) )   {
        Efactor = 2*(digits - tokens.size());
        input_long *=std::pow(10, Efactor);
        scale_factor += Efactor/2;
        for (int i = 0; i < (Efactor/2); i++)  {
           tokens.push_back(0);
        }
      }

/*
// DEBUG
 std::cout << " The INPUT has been altered as follows:\n";
 std::cout << " input_double = " << square_root::fix(input_double, p)
	         << "   <----->   input_long = "  << input_long << "\n\n";
      // << std::flush;
*/

 create_decimal_places(tokens.size(), scale_factor);
 create_input_string();

 // Initialize pairs[] array so as to prevent unpredicabale display
 // when areas of pairs[] are accessed at the limits of digits
 // in tokens deque and Input String


 //*************************************************************************/

 if (phase == 1)
 {
  std::cout  << "\n\n We want to find the " << digits - 1 << "-digit number closest to"
      << " the square root of " << square_root::fix(input_double, p)
      << "\n that is,\n"
      << symbol((int)2*tokens.size()/3, (int)digits_after_radix/2, 1)
      << square_root::fix(input_double, p) << "\n\n We will find " << digits
      << " digits and let you round off to " << digits - 1 << " digits.\n\n"
      << " We first rewrite " << input_str << " to the right of the radical symbol\n and under the vinculum,"
      << " grouping the numbers in pairs:  " << "\n\n";

 std::cout << symbol(tokens.size(), digits_after_radix, 1) << input_str
    << "\n\n If there is an odd number of digits to the right of "
    << "\n the decimal point, add a zero to the rightmost end. \n If there is an odd"
    << " number of digits to the left\n of the decimal point, the leftmost"
    << " digit will be isolated." << std::flush;

   pause();
// DEBUG:
/*** uncomment to debug
std::cout << "\nBEFORE PROCESSING PHASE " << phase << "\nremainder x = "
      << remainder << "\t\t\t" << "\ntmp b = " << tmp << "\nresult a = "
      << result << "\n\n";
***/

}  //END if phase == 1

// THE DRIVING ENGINE:  ***************************************************/
  for (std::deque<double>::iterator i = tokens.begin(); i != tokens.end();++i)  {
               process(*i);
  }
  /***********************************************************************/
/*
   std::cout << "\n\n----------------------------------\n";
   std::cout << "Size of tokens deque: " << tokens.size()
          << "\nScale Factor: " << scale_factor;

   std::cout << "\n----------------------------------\n";
*/
        /* Dividing to the apropriate value */
       result /= std::pow(10, scale_factor) ;
  }
 pause();
 /******************************* VERY HELPFUL WHEN BUILDING ******************/

  //DEBUG:
  /*** uncomment to debug
 std::cout << "\n\n----------------------------------\n";

      for (int i = 0; i < itr; i++)   {
           std::cout << "\na_integers[" << i << "] = " << a_integers[i]
                     << "\nsub_result[" << i << "] = " << sub_result[i]
                     << "\nsubtrahends[" << i << "] = " << subtrahends[i]
                     << "\nremainders[" << i << "] = " << remainders[i]
                     << "\ndecimal_places[" << i << "] = " << decimal_places[i]
                     << "\npow10[" << i << "] = " << pow10[i]
                     << "\na[" << i << "] = " << a[i]
                     << "\npairs[" << i << "] = "
                     << pairs[i]
                    // << square_root::fix(pairs[i], 2)
                    << "\ndifference[" << i << "] = " << difference[i]
                     << std::endl;
      }

***/
      // THIS IS IMPORTANT.  I WANT TO PRESERVE THIS OBSERVATION:
      // The sum of the subtrahends approaches the value of the number eroded

     // Show that the sum of subtrahends approaches value of input

      std::cout << "\n----------------------------------------------------------------\n";
      std::cout << "\n The sum of the subtrahends approach or equal the value of "
                << square_root::fix(input_double, p) << "\n\n";
      double sum_of_subtrahends = 0;
      for (int i = 0; i < digits; i++)  {
        std::cout << "s[" << i << "] = " << subtrahends[i] << "\n";
        sum_of_subtrahends += subtrahends[i];
      }

      std::cout << "\n The sum of the subtrahends is " << sum_of_subtrahends;
      std::cout << "\n-----------------------------------------------------------------\n\n";

      analyze(a, digits - 1);

}

  ~square_root () {}

  operator double () const  {
     return result;
  }
};   // The class needs a semicolon after it

/***************************************************************************/
/*  PROMPT VERSION
/*
/**************************************************************************/
int main (int argc, char* argv[])  {

  double real_n = 0.0;
  int d;

  std::string line = "";
  std::stringstream stream;

/************* for command line version (better for printing)
  if (argc < 3) {   // We expect 3 arguments: the program name,
                    // the number to erode, and the desired digits,

    // Tell the user how to run the program
    std::cerr << "Usage: " << argv[0] << "  [number to erode] [desired digits]"
               << std::endl;
    return 1;
  }
 std::stringstream (argv[1]) >> real_n;
 std::stringstream (argv[2]) >> d;

***************    uncomment for commandline version  ********/

std::cout << "\n\n\n\n";
std::cout << "*******************************************************************\n";
std::cout << "****                                                           ****\n";
std::cout << "****        An Old School Square Root Algorithm Teacher        ****\n";
std::cout << "****                                                           ****\n";
std::cout << "****                                                           ****\n";
std::cout << "****          Preserving the Art of Computing By Hand          ****\n";
std::cout << "****                      Digit by Digit                       ****\n";
std::cout << "****                  Arranged and Explained                   ****\n";
std::cout << "****                  In a Systematic Manner                   ****\n";
std::cout << "****                                                           ****\n";
std::cout << "*******************************************************************\n";
std::cout << "\n\n";
std::cout << " Suppose we wish to find the largest n-digit number whose square\n"
          << " is as close as possible to a given number. This program takes\n "
          << "such a number, and finds the n-digit number, which when multiplied\n"
          << " by itself is as close as possible to the given number.\n\n";

 while (true) {
    std::cout << " Please enter any positive rational number you wish.\n"
       << " It may contain an integer part and a fractional part.\n\n"
       << " Please enter the number in decimal format: ";

 std::getline(std::cin, line);
 stream << std::fixed << std::setprecision(12) << line;
 if ( (stream >> real_n) && (real_n >= 0) )
     break;
 std::cout << " Invalid number.  Please try again." << "\n";
}
stream.str("");
stream.clear();

 std::cout << "\n\n How many digits of the square root of "
           << real_n << " do you wish to extract?\n (3 or 4 works best,"
           << " but we can handle more if you prefer more precision)\n";
 while(true) {
     std::cout << "\n Enter the number of digits you wish to extract: ";
     std::getline(std::cin, line);
     stream << std::fixed << std::setprecision(12) << line;
     if ( (stream >> d) && (d > 0) )
         break;
    std::cout << "\n Invalid number.  Please try again." << "\n\n";
 }
stream.str("");
stream.clear();

// END Prompt Version
// ADDED IN VERSION 3 to reject input < 0.01

if (real_n < 0.01) {

  std::cout << "\n This program can handle non-integral fractional input for values"
            << " greater than 0.01,\n but you have entered " << real_n
            << " < 0.01\n\n";
  std::cout << " Using std::sqrt(" << real_n << ") = " << std::sqrt(real_n)
            << "\n\n";
  return -1;
}

  std::cout.precision(12);
  std::cout << " " << square_root(real_n, d) << " is the square root of "
            << real_n << " to roughly " << d
            << " digits.\n\n";

std::cout << " To find a square root of a number to n digits, "
          << " we should find the\n square root to (n+1) digits and then"
          << " round off the last digit.\n\n";

  std::cout << " Using std::sqrt(" << real_n << ") = " << std::sqrt(real_n) << "\n\n";
//  std::cout << " Thank you for exploring the Old School Square Root Algorithm!\n\n";
 }
