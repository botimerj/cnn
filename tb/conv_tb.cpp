#include "../cnn.h"

#include <iostream> 
#include <fstream> 
#include <stdlib.h> 
#include <cmath> 
#include <string> 
#include <cstring> 

using namespace std;


//void read_fp_array(ifstream*, float*, int*, int*);

void read_fp_array(ifstream& my_file, float* float_arr, int* x, int* y){
    string line, num;
    int pos;
    int i;
                                                        
    if(my_file.is_open()){
        i = 0; *y = 0;
        while(getline(my_file,line)){
            while(line.length()>0){
                pos = line.find(",");
                if(pos != string::npos){
                    num = line.substr(0,pos);
                    line = line.substr(pos+1);
                    float_arr[i++] = atof(num.c_str());
                }else{
                    num = line;
                    line = string("");
                    float_arr[i++] = atof(num.c_str());
                }
            }
            (*y)++;
        }
    }
    *x = i / *y;
}

int main(){ 
    
    cout << "Convolution Testbench : ";

    float A[100];
    int ax, ay;
    ifstream f_file("f.txt");
    read_fp_array(f_file, A, &ax, &ay);
    f_file.close();

    float B[100];
    int bx, by;
    ifstream g_file("g.txt");
    read_fp_array(g_file, B, &bx, &by);
    g_file.close();



    float C_check[361];
    int cx_check, cy_check;
    ifstream h_file("h.txt");
    read_fp_array(h_file, C_check, &cx_check, &cy_check);
    h_file.close();

    int cx = ax + bx - 1;
    int cy = ay + by - 1;
    float C[cx*cy];
    conv2d(A, B, C, ax, ay, bx, by, cx, cy);  




    if(cx_check != cx || cy_check != cy) {
        cout << "Error: dimension mismatch" << endl;
        return 1;
    }

    for(int j = 0; j < cy; j++){
        for(int i = 0; i < cx; i++){
            if(abs((C[j*cx + i] - C_check[j*cx + i])) > 0.0001){
                cout << "Error: convolution mismatch" << endl;
                cout << "element (" << i << "," << j << ") = " << C[j*cx+i] << " vs " << C_check[j*cx+i] << endl;
                return 1;
            }
        }
    }

    cout << "Pass" << endl;
    return 0;

}


