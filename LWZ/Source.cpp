#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define BITS 12  


using namespace std;

// encode
vector<int> encoding(string str,bool order)
{
    // order=1 - print dictionary mode
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += str[0];
    int code = 256;
    vector<int> output_code;
    for (int i = 0; i < str.length(); i++) {
        if (i != str.length() - 1)
            c += str[i + 1];
        if (table.find(p + c) != table.end()) {
            p = p + c;
        }
        else {

            if (order==1){
            cout<<p+c<<"\t"<<code<<endl;
                }

            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }

    output_code.push_back(table[p]);
    return output_code;
}

//decode
string decoding(vector<int> op,bool order)
{
    string str = "";

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
    str += s;
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

        str += s;
        c = "";
        c += s[0];
        table[count] = table[old] + c;

        if (order == 1) {
            cout << table[count] << ": "<<count << endl;
        }
        count++;
        old = n;
    }

    return str;
}

//write binary file
void output_bit(ofstream& output, unsigned int code)
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


//read binary file
int input_bit(ifstream& input)
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

//determine the size of a file
int size(const char* filename) {
    ifstream f(filename, ios::binary);
    f.seekg(0, ifstream::end);
    int size1 = f.tellg();
    f.seekg(0);
    f.close();
    return size1;
}

//read text file
string input_txt(string file){
    ifstream f;
    f.open(file);
    
    string s="";
    if (f.is_open()){
        string line;
        while (getline(f, line)) {
            if (s != "") {
                s += '\n' + line;
            }
            else {
                s += line;
            }
        }
    }
    f.close();
    return s;
}

//write text file
void output_txt(string file,string s){
    ofstream f;
    f.open(file);

    if(f.is_open()){
        f<<s;
    }
    f.close();

}


int main(int argc,char *argv[])
{
    if (argc != 5) {
		cout << "Input is invalid!";
		return 0;
	}

    string action=argv[1];
    string inputPath=argv[2];
    string outputPath=argv[3];
    string outputInfo=argv[4];

    //string action = "-d";
    //string inputPath = "Output.lwz";
    //string outputPath = "Output.txt";
    //string outputInfo = "-d";


    if (action=="-e"){
        string s=input_txt(inputPath);
        cout << "Encoding\n";
        cout << "Input Text: " << s << endl;

        vector<int> output_code;

        if (outputInfo=="-i"){

            output_code=encoding(s,0);


            ofstream fout(outputPath, ios::binary);
            cout << "Output Codes: " << endl;
            for (int i = 0; i < output_code.size(); i++) {
            cout << output_code[i] << " ";
            output_bit(fout, output_code[i]);

            }

            output_bit(fout, 1);
            output_bit(fout, 0);
            fout.close();

            cout << "Input Size: " << 8 * s.length() << " bits" << endl;
            cout << "Output Size: " << 8 * size(outputPath) << " bits" << endl;
            cout<<"Space saved: "<<(1-1.0*size(outputPath)/s.length())*100<<" %"<<endl;
        }
        else{
            cout << "----Dictionary----" << endl;
            output_code=encoding(s,1);
            cout << "------------------" << endl;


            ofstream fout(outputPath, ios::binary);
            cout << "Output Codes: " << endl;
            for (int i = 0; i < output_code.size(); i++) {
            cout << output_code[i] << " ";
            output_bit(fout, output_code[i]);

            }
            output_bit(fout, 1);
            output_bit(fout, 0);
            fout.close();

        }

        if (outputInfo=="-ind"){
            cout << "\nInput Size: " << 8 * s.length() << " bits" << endl;
            cout << "Output Size: " << 8 * size(outputPath) << " bits" << endl;
            cout<<"Space saved: "<<(1-1.0*size(outputPath)/s.length())*100<<" %"<<endl;
        }

    }
    else{

        vector<int> input_code1;
        ifstream fin(inputPath, ios::binary);
        int n = -1;
        while (n != 1 && !fin.eof()) {
            n = input_bit(fin);
            input_code1.push_back(n);
        }
        input_code1.pop_back();
        fin.close();

        cout << "Decoding\n";

        cout << "Input Codes: " << endl;
        for (int i = 0; i < input_code1.size(); i++)
            cout << input_code1[i] << " ";
        cout << endl;

        string s;
        if (outputInfo == "-i") {

            s = decoding(input_code1, 0);
            output_txt(outputPath, s);

            cout << "\nOutput Text: " <<s<<endl;
            cout << "Input Size: " << 8 * size(inputPath) << " bits" << endl;
            cout << "Output Size: " << 8 * s.length() << " bits" << endl;
        }
        else {

            cout << "----Dictionary----" << endl;
            s = decoding(input_code1, 1);
            cout << "------------------" << endl;
            output_txt(outputPath, s);
            cout << "Output Text: " << s << endl;
        }

        if (outputInfo == "-ind") {

            cout << "Input Size: " << 8 * size(inputPath) << " bits" << endl;
            cout << "Output Size: " << 8 * s.length() << " bits" << endl;
        }

    }
    return 1;

}