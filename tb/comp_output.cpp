#include <iostream> 
#include <fstream> 
#include <string> 
#include <cstdlib> 
#include <cmath> 

using namespace std;

int main(){
    
    cout << " Comparing output : ";
    const int num_tests = 4;

//    compare(file1, file2);
    string test_names[num_tests]; 
    test_names[0] = "layer0_conv_O";
    test_names[1] = "layer0_pool_O";
    test_names[2] = "layer1_conv_O";
    test_names[3] = "layer1_pool_O";
    
    for(int i = 0; i < num_tests; i++){
        ifstream file1(("network/" + test_names[i]).c_str());
        ifstream file2(("test_network/" + test_names[i]).c_str());

        float var1, var2;
        string var1_str, var2_str;
        if(file1.is_open() && file2.is_open()){
            while(getline(file1,var1_str) && getline(file2,var2_str)){
                if(0.001 < abs(atof(var1_str.c_str()) - atof(var2_str.c_str()))){
                    cout << test_names[i] << " differ" << endl; 
                    return 1;
                }
            }
            file1.close(); 
            file2.close(); 
        }
    }
    cout << "Passed" << endl;
    return 0;

}

