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
            if(line != "N"){
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
            }else{
                break;
            }
        }
    }
    *x = i / *y;
}

int main(){ 
    
    cout << "Convolution Testbench : ";
    
    ifstream f_file("f.txt");
    ifstream g_file("g.txt");
    ifstream h_file("h.txt");


    int num_normal_tests = 5;
    bool valid_test = false;
    //conv_2d testing
    for(int test_num = 0; test_num < 7; test_num++){

        if(test_num > num_normal_tests-1){
            valid_test = true; 
        }

        float A[1024];
        int ax, ay;
        read_fp_array(f_file, A, &ax, &ay);

        float B[1024];
        int bx, by;
        read_fp_array(g_file, B, &bx, &by);

        float C_check[4096];
        int cx_check, cy_check;
        read_fp_array(h_file, C_check, &cx_check, &cy_check);

        int cx, cy; 
        float C[4096];
        if(valid_test == true){
            cx = ax - bx + 1;
            cy = ay - by + 1;
            conv2d_valid(A, B, C, ax, ay, bx, by, cx, cy);  
        } else {
            cx = ax + bx - 1;
            cy = ay + by - 1;
            conv2d(A, B, C, ax, ay, bx, by, cx, cy);  
        }


        if(cx_check != cx || cy_check != cy) {
            cout << "Error: dimension mismatch" << endl;
            return 1;
        }

        for(int j = 0; j < cy; j++){
            for(int i = 0; i < cx; i++){
                if(abs((C[j*cx + i] - C_check[j*cx + i])) > 0.005){
                    cout << endl << "Error convolution mismatch : test " << test_num+1 << endl;
                    cout << "element (" << i << "," << j << ") = " << C[j*cx+i] << " vs " << C_check[j*cx+i] << endl;
                    return 1;
                }
            }
        }

        // print result 
        /*for(int j = 0; j < cy; j++){
            for(int i = 0; i < cx; i++){
                cout << C[j*cx + i] << " ";
                }
            cout << endl;
        }*/
    } // End test loop

    f_file.close();
    g_file.close();
    h_file.close();
    
    cout << "Pass" << endl;

    return 0;

}


