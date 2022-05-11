 // a skeleton implementation of a tokeniser

#include "tokeniser.h"
#include <iostream>
#include <ctype.h>
#include <string>

// to shorten the code
using namespace std ;

////////////////////////////////////////////////////////////////////////

namespace Assignment_Tokeniser
{

    // is the token of the given kind or does it belong to the given grouping?
    bool token_is_in(Token token,TokenKind kind_or_grouping)
    {
        TokenKind kind = token_kind(token) ;

        // check identity first
        if ( kind == kind_or_grouping ) 
        {
            return true ;
        }

        switch(kind_or_grouping)
        // to recognise the group tokens tk_number, tk_keyword, and tk_symbol
        {
            case tk_number:
                if ( kind == tk_integer || kind == tk_double )
                {
                    return true;
                }
                else
                {
                 return false;
                }
            break;

            case tk_keyword:
                if ( kind == tk_if || kind == tk_while || kind == tk_else || kind == tk_class || kind == tk_int)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            break;

            case tk_symbol:
                if ( kind == tk_at || kind == tk_semi || kind == tk_colon || kind == tk_not || kind == tk_comma || kind == tk_stop || kind == tk_eq || kind == tk_spaceship ||
                    kind == tk_rcb || kind == tk_lcb || kind == tk_rrb || kind == tk_lrb || kind == tk_rsb || kind == tk_lsb || kind == tk_div)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            break;
          
            default:
                return false ;
        }
    }

    // the current input character, initiliased to ' ' which we ignore
    // it is an int so that the EOF marker is not confused with a legal character
    static int ch = ' ' ;

    // the current line number and column, initialised to line 1 column 0
    static int line_num = 1 ;
    static int column_num = 0 ;

    // the line number and column for the first character in the current token
    static int start_line = 0 ;
    static int start_column = 0 ;

    string ch_1 = "";
    string ch_2 = "";

    // generate a context string for the given token
    // it shows the line before the token,
    // the line containing the token, and
    // a line with a ^ marking the token's position
    // tab stops are every 8 characters
    // in the context string, tabs are replaced by spaces (1 to 8)
    // so that the next character starts on an 8 character boundary
    string token_context(Token token)
    {
        
       int t_l1 = token_line(token); // line number 
       int t_c = token_column(token); // column position
       int t_ll1 = t_l1 - 1; // the previous line number

       string t_l2 = "";
       string t_c1 = "";
       string t_ll2 = "";

       t_l2 = to_string(t_l1);
       t_ll2 = to_string(t_ll1);

       int len = t_c + 3 + t_l2.length() ;

       for (int i = 1; i < len; i++)
       {
            t_c1 += " ";
       }

       t_c1 += "^";

       if ( t_l1 > 1 )
       {
        return " " + t_ll2 + ": " + ch_2 + '\n' + " " + t_l2 + ": " + ch_1 + '\n' + t_c1 + '\n';
       }
       else
       {
        return " " + t_l2 + ": " + ch_1 + '\n' + t_c1  + '\n';
       }     
    }

    // read next character if not at the end of input
    // and update the line and column numbers
    static void nextch()
    {
        extern int read_char() ;


        if ( ch == EOF ) 
        {
            return ;
        }

        if ( ch == '\n' )           // if last ch was newline ...
        {
            line_num++ ;            // increment line number
            column_num = 0 ;        // reset column number

            ch_2 = ch_1;
            ch_1 = "";
        }

        // calculate the position of ch after 'tab'
        if ( ch == '\t')
        {
            if ( column_num % 8 == 0 )
            {
                column_num += 7;
            }
            else
            {
                column_num += 8 - column_num % 8;
            }
        }

        ch = getchar() ;            // read the next character from stdin
        column_num++ ;              // increment the column number

        if ( ch != '\n' && ch != '\t' && ch != '\r')
        {
            ch_1 += ch;
        }

        // in 'tab' situation
        if ( ch == '\t')
        {
            if ( column_num % 8 == 0 )
            { 
               for (int i = 0; i <= 8 - column_num % 8; i++)
                {
                    ch_1 += " ";
                }
            }
            else
            {
                
                for (int i = 0; i <= 8 - column_num % 8; i++)
                {
                    ch_1+= " ";
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////

    

    // called when we find end of input or we have a bad token
    Token parse_eoi()
    {
        // simulate end of input in case this is handling a bad token rather than a real end of input
        ch = EOF ;

        // return an eoi token
        return new_token(tk_eoi,"",start_line,start_column) ;
    }

    // tk_identifier
    Token identifier()
    {
        string iden = "";
        iden += ch;
        nextch();
        while( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z') || (ch >= '0' && ch <= '9') || (ch == '_' || ch == '$'))
        {
            iden += ch ;
            nextch();
            if ( string_to_token_kind(iden) == tk_if || string_to_token_kind(iden) == tk_while || string_to_token_kind(iden) == tk_else || string_to_token_kind(iden) == tk_class || string_to_token_kind(iden) == tk_int)
            {
                return new_token(string_to_token_kind(iden),iden,start_line,start_column) ;
            }
        }
        return new_token(tk_identifier,iden,start_line,start_column) ;    
    }

    // tk_eol_comment
    Token eol_comment()
    {
        string com = "";
        while(( ch != '\n') && ( ( ch >= 32 && ch <= 126 ) || ( ch == '\t' ) || ( ch == '\r') ) )
        {
            com += ch;
            nextch();
            if ( com == "")
            {
                nextch();
                return parse_eoi() ; 
            }
        }
        if ( ch == '\n')
        {
            return new_token(tk_eol_comment,com,start_line,start_column) ;
        }
        else
        {
            return parse_eoi() ; 
        }
        
    }

    // tk_adhoc_comment
    Token adhoc_comment()
    {
        string a_com = "";

        while (( ch == '\n') || ( ch >= 32 && ch <= 126 ) || ( ch == '\t' ) || ( ch == '\r' ) )
        {
            if ( ch == '*')
            {
                nextch();

                if ( ch == '/')
                {
                    if ( a_com == "")
                    {
                        nextch();
                        return parse_eoi() ; 
                    }

                    else
                    {
                        nextch();
                        return new_token(tk_adhoc_comment,a_com,start_line,start_column) ;
                    }
                }

                else
                {
                    a_com += '*';
                    a_com += ch;
                    nextch();
                }

            }

            else
            {
                a_com += ch;
                nextch();
            }
        }

        return parse_eoi() ; 
    }

    // tk_double
    Token isDouble(string doub)
    {
        string d = "";
        d = doub;
        nextch();

        if (!isdigit(ch))
        {
            return parse_eoi() ;
        }

        while (ch >= '0' && ch <= '9' )
        {                                                                                               
            d += ch;
            nextch();
        }

        if ( ch == 'e' || ch == 'E')
        {
            nextch();

            if ( ch == '-')
            {
                nextch();
                if ( isdigit(ch) )
                {
                    d += 'e';
                    d += '-';
                    while ( ch >= '0' && ch <= '9' )
                    {
                        d += ch;
                        nextch();
                    }
                    return new_token(tk_double,d,start_line,start_column) ;
                }
                else
                {
                    return new_token(tk_double,d,start_line,start_column) ;
                }
            }

            else if ( isdigit(ch) )
            {
                d += 'e';
                d += '+';
                while ( ch >= '0' && ch <= '9' )
                {
                    d += ch;
                    nextch();
                }
                return new_token(tk_double,d,start_line,start_column) ;
            }

            else
            {
                return new_token(tk_double,d,start_line,start_column) ;
            }
        }

        else
        {
            return new_token(tk_double,d,start_line,start_column) ;   
        }
    }

    // tk_integer
    Token integer()
    {
        string inte = "";
        string doub = "";
        inte += ch;
        nextch();
        if ( ch == '.')
        {
            doub = inte;
            doub += ch;
            return isDouble(doub);
        }
        else
        {
            while( ch >= '0' && ch <= '9' )
            {
                inte += ch;
                nextch();
            }

            if ( ch == 'e' || ch == 'E' )
            {
                nextch();

                if ( ch == '-')
                {
                    nextch();
                    if ( isdigit(ch) )
                    {
                        doub = inte;
                        doub += 'e';
                        doub += '-';
                        while ( ch >= '0' && ch <= '9' )
                        {
                            doub += ch;
                            nextch();
                        }
                        return new_token(tk_double,doub,start_line,start_column) ;
                    }
                    else
                    {
                        return new_token(tk_integer,inte,start_line,start_column) ;
                    }
                }

                else if ( isdigit(ch) )
                {
                    doub = inte;
                    doub += 'e';
                    doub += '+';
                    while ( ch >= '0' && ch <= '9' )
                    {
                        doub += ch;
                        nextch();
                    }
                    return new_token(tk_double,doub,start_line,start_column) ;
                }

                else
                {
                    return new_token(tk_integer,inte,start_line,start_column) ;
                }
            }

            else if ( ch == '.' )
            {
                doub = inte; 
                doub += ch;
                return isDouble(doub);
            }
            return new_token(tk_integer,inte,start_line,start_column) ;
        }
    }

    // tk_string
    Token isString()
    {
        string str = ""; 
        nextch();
        while (( ch == ' ') || ( ch == '!' ) || ( ch >= 35 && ch <= 126 ) )
        {
            str += ch;
            nextch();
            if (ch == '"')
            {
                nextch();
                if (str == "")
                {
                    return parse_eoi();
                }
                else
                {
                    return new_token(tk_string,str,start_line,start_column) ;
                }
            }
        }
        if ( ch == '"')
        {
            return new_token(tk_string,str,start_line,start_column) ;
        }
        else
        {
            return parse_eoi() ; 
        }
    }

    // return the next token object by reading more of the input
    Token next_token()
    {
        // you must read input using the nextch() function
        // the last character read is in the static variable ch
        // always read one character past the end of the token being returned

        // this loop reads one character at a time until it reaches end of input
        while ( ch != EOF )
        {
            start_line = line_num ;                 // remember current position in case we find a token
            start_column = column_num ;

            switch(ch)                              // ch is always the next char to read
            {
            case ' ':                               // ignore space, tab, CR and LF
            case '\t':
            case '\r':
            case '\n':
                nextch() ;                          // read one more character and try again
                break ;
            case '@':
                nextch() ;
                return new_token(tk_at,"@",start_line,start_column) ;
                break;
            case ';':
                nextch() ;
                return new_token(tk_semi,";",start_line,start_column) ;
                break;
            case ':':
                nextch() ;
                return new_token(tk_colon,":",start_line,start_column) ;
                break;
            case '!':
                nextch() ;
                return new_token(tk_not,"!",start_line,start_column) ;
                break;
            case ',':
                nextch() ;
                return new_token(tk_comma,",",start_line,start_column) ;
                break;
            case '.':
                nextch() ;
                return new_token(tk_stop,".",start_line,start_column) ;
                break;
            case '}':
                nextch() ;
                return new_token(tk_rcb,"}",start_line,start_column) ;
                break;
            case '{':
                nextch() ;
                return new_token(tk_lcb,"{",start_line,start_column) ;
                break;
            case ')':
                nextch() ;
                return new_token(tk_rrb,")",start_line,start_column) ;
                break;
            case '(':
                nextch() ;
                return new_token(tk_lrb,"(",start_line,start_column) ;
                break;
            case ']':
                nextch() ;
                return new_token(tk_rsb,"]",start_line,start_column) ;
                break;
            case '[':
                nextch() ;
                return new_token(tk_lsb,"[",start_line,start_column) ;
                break;
            case '/':
                nextch();
                if (ch == '/')
                {
                    nextch();
                    return eol_comment() ;
                }
                else if (ch == '*')
                {
                    nextch();
                    return adhoc_comment() ;
                }
                else
                {
                    return new_token(tk_div,"/",start_line,start_column) ;
                }
                break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':
            case '$':
                return identifier() ;
                break ;
            case '0':
                nextch();
                if ( ch == '.')
                {
                    string doub = "";
                    doub += '0' ;
                    doub += ch;
                    return isDouble(doub) ;
                }
                else
                {
                    return new_token(tk_integer,"0",start_line,start_column) ;
                }
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return integer() ;
                break ;
            case '"':
                return isString() ;
                break ;
            case '=':
                nextch();
                if ( ch == '=')
                {
                    return new_token(tk_eq,"==",start_line,start_column) ;
                }
                else
                {
                    return parse_eoi() ;  
                }
                break ;
            case '<':
                nextch();
                if ( ch == '=')
                {
                    nextch();
                    if ( ch == '>' )
                    {
                        return new_token(tk_spaceship,"<=>",start_line,start_column) ;
                    }
                    else
                    {
                        return parse_eoi() ; 
                    }
                }
                else
                {
                    return parse_eoi() ; 
                }
                break ;



                                                    // read one more character and try again
                                                    // add additional case labels here for characters that can start tokens
                                                    // call a parse_* function to complete and return each kind of token
            default:
                return parse_eoi() ;                // the next character cannot start a token, return an EOI token
            }
        }

        start_line = line_num ;                     // remember current position so EOI token is correct
        start_column = column_num ;

        return parse_eoi() ;                        // return an EOI token
    }
}

