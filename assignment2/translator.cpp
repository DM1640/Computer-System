// convert an abstract syntax tree for Hack Assembly language into machine code
#include "iobuffer.h"
#include "symbols.h"
#include "tokeniser.h"
#include "abstract-syntax-tree.h"
#include <string>
#include <bitset>

// to simplify the code
using namespace std ;
using namespace CS_Symbol_Tables ;
using namespace Hack_Assembler ;

symbols alu_a_c;
symbols alu_c;
symbols dest_c;
symbols jump_c;
symbols name_p;
int k = 1;
int t = 0;;

//change to binary
string decTohex(int ivalue)
{
    int arr[50];

    string a_str = "";

    for(int i = 0 ; i < 16; i++)
    {
        arr[i] = ivalue % 2;
        ivalue = ivalue / 2;
    }


    for (int i = 16 - 1; i >= 0; i--)
    {
         a_str += to_string(arr[i]);  
    }

    return a_str;
}

void walk_program(ast the_program)
{
    //Create the new tables
    alu_a_c = create_strings();
    alu_c = create_strings();
    dest_c = create_strings();
    jump_c = create_strings();
    name_p = create_strings();

    //add the mapping
    if (!insert_strings(alu_a_c,"0","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"-1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"!D","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"!A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"-D","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"-A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D+1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"A+1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D-1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"A-1","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D+A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D-A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"A-D","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D&A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D|A","0")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"!M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"-M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"M+1","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"M-1","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D+M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D-M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"M-D","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D&M","1")) cout << "Error!" << endl ;
    if (!insert_strings(alu_a_c,"D|M","1")) cout << "Error!" << endl ;

    if (!insert_strings(alu_c,"0","101010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"1","111111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"-1","111010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D","001100")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"A","110000")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"!D","001101")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"!A","110001")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"-D","001111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"-A","110011")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D+1","011111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"A+1","110111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D-1","001110")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"A-1","110010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D+A","000010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D-A","010011")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"A-D","000111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D&A","000000")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D|A","010101")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"M","110000")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"!M","110001")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"-M","110011")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"M+1","110111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"M-1","110010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D+M","000010")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D-M","010011")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"M-D","000111")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D&M","000000")) cout << "Error!" << endl ;
    if (!insert_strings(alu_c,"D|M","010101")) cout << "Error!" << endl ;

    if (!insert_strings(dest_c,"NULL","000")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"M","001")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"D","010")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"MD","011")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"A","100")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"AM","101")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"AD","110")) cout << "Error!" << endl ;
    if (!insert_strings(dest_c,"AMD","111")) cout << "Error!" << endl ;

    if (!insert_strings(jump_c,"NULL","000")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JGT","001")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JEQ","010")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JGE","011")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JLT","100")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JNE","101")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JLE","110")) cout << "Error!" << endl ;
    if (!insert_strings(jump_c,"JMP","111")) cout << "Error!" << endl ;

    if (!insert_strings(name_p,"R0",bitset<16>(0).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R1",bitset<16>(1).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R2",bitset<16>(2).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R3",bitset<16>(3).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R4",bitset<16>(4).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R5",bitset<16>(5).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R6",bitset<16>(6).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R7",bitset<16>(7).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R8",bitset<16>(8).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R9",bitset<16>(9).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R10",bitset<16>(10).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R11",bitset<16>(11).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R12",bitset<16>(12).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R13",bitset<16>(13).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R14",bitset<16>(14).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"R15",bitset<16>(15).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"SCREEN",bitset<16>(16384).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"KBD",bitset<16>(24576).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"SP",bitset<16>(0).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"LCL",bitset<16>(1).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"ARG",bitset<16>(2).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"THIS",bitset<16>(3).to_string())) cout << "Error!" << endl ;
    if (!insert_strings(name_p,"THAT",bitset<16>(4).to_string())) cout << "Error!" << endl ;
    


    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program) ;
    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        ast instruction = get_program(the_program,i) ;
        switch(ast_node_kind(instruction))
        {

        case ast_label:

            insert_strings(name_p,get_label_name(instruction),bitset<16>(i-t).to_string());
            t ++;
            
            break ;

        case ast_a_name:

            break ;
            
        case ast_a_instruction:

            break ;

        case ast_c_instruction:

            break ;

        default:

            fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }
    }


}
 
// this function provides an example of how to walk an abstract syntax tree constructed by ast_parse_xml()
void walk_program1(ast the_program)
{

    // the_program contains a vector of instructions
    int ninstructions = size_of_program(the_program) ;
    for ( int i = 0 ; i < ninstructions ; i++ )
    {
        ast instruction = get_program(the_program,i) ;

        switch(ast_node_kind(instruction))
        {

        case ast_label:
            break ;

        case ast_a_name:

            if (insert_strings(name_p,get_a_name_unresolved(instruction),""))
            {
                update_strings(name_p,get_a_name_unresolved(instruction),bitset<16>(15 + k).to_string());
                k ++;

                //lookup and print value
                cout << lookup_strings(name_p,get_a_name_unresolved(instruction)) << endl;
            }

            else
            {
                //lookup and print value
                cout << lookup_strings(name_p,get_a_name_unresolved(instruction)) << endl;
            }

            break ;
            
        case ast_a_instruction:

            //lookup and print value
            cout << decTohex(get_a_instruction_value(instruction)) << endl;
            break ;

        case ast_c_instruction:

            //lookup and print value
            cout << "111" 
            << lookup_strings(alu_a_c,get_c_instruction_alu(instruction)) 
            << lookup_strings(alu_c,get_c_instruction_alu(instruction)) 
            << lookup_strings(dest_c,get_c_instruction_dest(instruction)) 
            << lookup_strings(jump_c,get_c_instruction_jump(instruction)) << endl;
            break ;

        default:
            fatal_error(0,"// bad node - expected ast_label,ast_a_name,ast_a_instruction or ast_c_instruction\n") ;
            break ;
        }
    }


}


// translate an abstract syntax tree representation of Hack Assembly language into Hack machine code
static void asm_translator(ast the_program)
{
    walk_program(the_program);
    walk_program1(the_program);
}

// main progra
int main(int argc,char **argv)
{
    // error messages
    config_errors(iob_buffer) ;

    // parse abstract syntax tree and pass to the translator
    asm_translator(ast_parse_xml()) ;

    // flush output and errors
    print_output() ;
    print_errors() ;
}
