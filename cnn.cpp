#include "cnn.h"

#include <iostream> 

using namespace std;

int main(){ 
    
    cout << "Hello, World" << endl;

    int ax, ay, bx, by, cx, cy;

    ax = 3; ay = 2;
    bx = 3; by = 3;
    //cx = 6; cy = 2;
    cx = ax + bx - 1;
    cy = ay + by - 1;


    //float A[ay*ax] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    float A[ay*ax] = {4, 5, 6, 7, 8, 9};
    float B[by*bx] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    float C[cy*cx];

    //conv(A, B, C, ax, bx, cx);
    conv2d(A, B, C, ax, ay, bx, by, cx, cy);  

    cout << "{ ";
    for(int j = 0; j < cy; j++){
        for(int i = 0; i < cx; i++){
            cout << C[j*cx + i] << " ";
        }
        cout << endl;
    }
    cout << "}" << endl;


    return 0;

}
