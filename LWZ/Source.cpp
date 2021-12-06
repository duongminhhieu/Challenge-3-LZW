#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define BITS 12  
using namespace std;
vector<int> encoding(string s1)
{
    cout << "Encoding\n";
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < s1.length(); i++) {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {
            cout << p << "\t" << table[p] << "\t\t"
                << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    cout << p << "\t" << table[p] << endl;
    output_code.push_back(table[p]);
    return output_code;
}
//Viet so nguyen ra file output
void output_code1(ofstream& output, unsigned int code)
{
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;
    output_bit_buffer |= (unsigned long)code << (32 - BITS - output_bit_count);
    output_bit_count += BITS;
    while (output_bit_count >= 8)
    {
        output.put((char)(output_bit_buffer >> 24));
        output_bit_buffer <<= 8;
        output_bit_count -= 8;
    }
}
//Nhap so nguyen tu file input
int input_code(ifstream& input)
{
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    while (input_bit_count <= 24)
    {
        input_bit_buffer |=
            (unsigned long)input.get() << (24 - input_bit_count);
        input_bit_count += 8;
    }
    return_value = input_bit_buffer >> (32 - BITS);
    input_bit_buffer <<= BITS;
    input_bit_count -= BITS;
    return(return_value);
}
void decoding(vector<int> op)
{
    cout << "\nDecoding\n";
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    cout << s;
    int count = 256;
    for (int i = 0; i < op.size() - 1; i++) {
        n = op[i + 1];
        if (table.find(n) == table.end()) {
            s = table[old];
            s = s + c;
        }
        else {
            s = table[n];
        }
        cout << s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
}
int size(const char* filename) {
    ifstream f(filename, ios::binary);
    f.seekg(0, ifstream::end);
    int size1 = f.tellg();
    f.seekg(0);
    f.close();
    return size1;
}
int main()
{

    string s = "WYS*WYGWYS*WYSWYSG";
    vector<int> output_code = encoding(s);

    ofstream fout("output.lzw", ios::binary);
    cout << "Output Codes: " << endl;
    for (int i = 0; i < output_code.size(); i++) {
        cout << output_code[i] << " ";
        output_code1(fout, output_code[i]);
    }

    output_code1(fout, 1);
    output_code1(fout, 0);
    fout.close();



    vector<int> input_code1;
    ifstream fin("output.lzw", ios::binary);
    int n = -1;
    while (n != 1 && !fin.eof()) {
        n = input_code(fin);
        input_code1.push_back(n);
    }
    input_code1.pop_back();
    cout << endl;
    cout << "Input Codes: " << endl;
    for (int i = 0; i < input_code1.size(); i++)
        cout << input_code1[i] << " ";
    decoding(input_code1);
    fin.close();
    cout << endl << "Kich thuoc file goc: " << 8 * s.length() << " bits" << endl;
    cout << "Kich thuoc file .lzw: " << 8 * size("output.lzw") << " bits" << endl;
}