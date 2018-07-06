#include "cnn.h"

#include <iostream> 
#include <iomanip> 
#include <string>
#include <cstdlib> //rand
#include <ctime> 
#include <fstream> 
#include <cmath> 

using namespace std;

////////////////////////////
/// Reusable Definitions ///
////////////////////////////

void print_file(std::ofstream& ofile, float * arr_ptr, int size){
    for(int i = 0; i < size; i++){
        ofile << std::fixed << std::setprecision(5) << arr_ptr[i];
        ofile << "\n";
    }
}

void rot180(float * newArr, int size){
}

/////////////////////////////
/// ConvLayer Definitions ///
/////////////////////////////

ConvLayer::ConvLayer(int x_weight_in, 
                     int y_weight_in, 
                     int depth_in, 
                     float * input_in, 
                     int x_in_in, 
                     int y_in_in, 
                     int channel_in_in,
                     std::string layer_name_in){
    // Layer Definition
    layer_name = layer_name_in;
    x_weight = x_weight_in;
    y_weight = y_weight_in;
    depth = depth_in;
    weights = new float[depth*x_weight*y_weight*channel_in_in]; 
    biases = new float[depth];
    
    // Input Definition
    x_in = x_in_in;
    y_in = y_in_in;
    channel_in = channel_in_in;
    input = input_in;

    // Output Definition
    x_out = x_in - x_weight + 1;
    y_out = y_in - y_weight + 1;
    output = new float[x_out*y_out*depth];
    for(int i = 0; i < x_out*y_out*depth; i++)
        output[i] = 0; // Initialize output to 0

    // scratch pad to calculate intermediate convolutions
    calc_arr = new float[x_out*y_out];
}

ConvLayer::~ConvLayer(){
    delete []weights;
    delete []biases;
    delete []output;
    delete []calc_arr;
}

void ConvLayer::initialize(bool load_values, const std::string path){
    if(load_values){
        load_weights_biases(path);
    }else{ // Randomly generate weights (0-1)
        srand(static_cast <unsigned> (time(0)));
        for(int i = 0; i < x_weight*y_weight*depth*channel_in; i++){
            weights[i] = (static_cast <float> (rand())) / (static_cast <float> (RAND_MAX)); 
        }
        for(int i = 0; i < depth; i++){
            biases[i] = (static_cast <float> (rand())) / (static_cast <float> (RAND_MAX)); 
        }
    }
}

void ConvLayer::load_weights_biases(const std::string path){
    //std::ifstream my_file("network/layer0_conv_W");
    std::ifstream weights_file( (path+"_W").c_str() );

    std::string line, num;
    int pos;
    int i;
                                                        
    // Load weights
    if(weights_file.is_open()){
        i = 0;
        while(std::getline(weights_file,line)){
            while(line.length()>0){
                pos = line.find(",");
                if(pos != string::npos){
                    num = line.substr(0,pos);
                    line = line.substr(pos+1);
                    weights[i++] = atof(num.c_str());
                }else{
                    num = line;
                    line = string("");
                    weights[i++] = atof(num.c_str());
                }
            }
        }

    weights_file.close();

    }

    // Load biases 
    std::ifstream bias_file( (path+"_B").c_str() );
                                                        
    if(bias_file.is_open()){
        i = 0;
        while(std::getline(bias_file,line)){
            while(line.length()>0){
                pos = line.find(",");
                if(pos != string::npos){
                    num = line.substr(0,pos);
                    line = line.substr(pos+1);
                    biases[i++] = atof(num.c_str());
                }else{
                    num = line;
                    line = string("");
                    biases[i++] = atof(num.c_str());
                }
            }
        }

    bias_file.close();

    }
}

void ConvLayer::inference(){
    float * weights_rot = new float[x_weight*y_weight];
    for(int i = 0; i < depth; i++){             // Depth 
        float * output_idx = output+x_out*y_out*i;
        for(int j = 0; j < channel_in; j++){    // Channel
            float * input_idx = input+j*x_in*y_in; 
            float * weights_idx = weights+i*x_weight*y_weight*channel_in+j*x_weight*y_weight;
            
            //Stupid rotation needed for weight matrix
            int max_idx = x_weight*y_weight;
            for(int i = 0; i < max_idx; i++){
                weights_rot[i] = weights_idx[max_idx - i - 1];
            }

            // Perform convolution on input and weights
            conv2d_valid(input_idx, weights_rot, calc_arr, x_in, y_in, x_weight, y_weight, x_out, y_out); 
            // add element wise over channels
            accumulate(calc_arr, output_idx, x_out, y_out);                                   
        }
        add_biases(biases[i], output_idx, x_out, y_out);
    }
    int itt = 0;
    for(int k = 0; k < depth; k++){ 
        for(int j = 0; j < y_out; j++){
            for(int i = 0; i < x_out; i++){
                output[itt] = 1/(1+exp(-output[itt]));
                itt++;
            }
        }
    }
    delete []weights_rot;
}
    
void ConvLayer::accumulate(float *arg, float *acc, int x_acc, int y_acc){
    for(int j = 0; j < y_acc ; j++){
        for(int i = 0; i < x_acc ; i++){
            acc[j*x_acc + i] = acc[j*x_acc + i] + arg[j*x_acc + i];
        }
    }
}

void ConvLayer::add_biases(float arg, float *acc, int x_weight, int y_weight){
    for(int j = 0; j < y_weight; j++){
        for(int i = 0; i < x_weight; i++){
            acc[j*x_weight + i] = acc[j*x_weight + i] + arg;
        }
    }
}

void ConvLayer::save_network(){
    std::ofstream ofileW( (layer_name+"_W").c_str() );
    int size = x_weight * y_weight * depth;
    if(ofileW.is_open()){
        print_file(ofileW, weights, size);
        ofileW.close(); 
    }
    std::ofstream ofileB( (layer_name+"_B").c_str() );
    size = depth;
    if(ofileB.is_open()){
        print_file(ofileB, biases, size);
        ofileB.close();
    }
}

void ConvLayer::save_output(){
    std::ofstream ofile( (layer_name+"_O").c_str() );
    int size = x_out * y_out * depth;
    if(ofile.is_open()){
        print_file(ofile, output, size);
        ofile.close();
    }
}



/////////////////////////////
/// PoolLayer Definitions ///
/////////////////////////////

PoolLayer::PoolLayer(int pool_dim_in, float * input_in, int x_in_in, int y_in_in, int depth_in, std::string layer_name_in){
    layer_name = layer_name_in;
    pool_dim = pool_dim_in;
    input = input_in;
    x_in = x_in_in;
    y_in = y_in_in;
    depth = depth_in;

    x_out = x_in/pool_dim;
    y_out = y_in/pool_dim;
    output = new float[x_out*y_out*depth];

    if(x_in*y_in%pool_dim != 0){
        std::cout << "Error pool dimension mismatch" << std::endl;
    }
}

PoolLayer::~PoolLayer(){
    delete[] output;
}

void PoolLayer::inference(){
   
    std::string type("average"); 

    int output_y_it, output_x_it;
    int output_idx, input_idx;
    int max_in_idx = x_in*y_in*depth;
    int max_out_idx = x_out*y_out*depth;

    for(int d = 0; d < depth; d++){
    output_y_it = 0;
    // iterate through entire feature map
        for(int j = 0; j < y_in; j=j+pool_dim){ // y dim
            output_x_it = 0;
            for(int i = 0; i < x_in; i=i+pool_dim){ // x dim
                output_idx = d*x_out*y_out+output_y_it*x_out + output_x_it;
                input_idx = d*x_in*y_in+j*x_in + i;
                output[output_idx] = 0;
                float average = 0;
                for(int k = 0; k < pool_dim; k++){  // find max in a subset
                    for(int l = 0; l < pool_dim; l++){
                        int search_idx = input_idx+k*y_in+l;
                        if( search_idx < max_in_idx){
                            if(type == "max"){ // max pooling
                                if( input[search_idx] >= output[output_idx]) // Find max
                                    output[output_idx] = input[search_idx];
                            }else{ // average pooling
                                    average = input[search_idx]+average;
                            }
                        }else{
                            cout << "IDX Error during search" << endl;
                        }
                    }
                } // find max
                if(type == "average"){
                    output[output_idx] = average/(pool_dim*pool_dim);
                }
                output_x_it++;
            } // x dimension
            output_y_it++;
        } // y dimension

    } // depth
}

void PoolLayer::print(){
    for(int y = 0; y < y_out; y++){
        for(int x = 0; x < x_out; x++){
            cout << static_cast<int>(output[x_out*y+x]) << " ";
        }
        std::cout << std::endl;
    }
}

void PoolLayer::save_output(){
    std::ofstream ofile( (layer_name+"_O").c_str() );
    int size = x_out * y_out * depth;
    if(ofile.is_open()){
        print_file(ofile, output, size);
        ofile.close();
    }
}


/////////////////////////////
/// FullLayer Definitions ///
/////////////////////////////

FullLayer::FullLayer(int cat_in, float * input_in, int size_in, std::string layer_name_in){
    layer_name = layer_name_in;
    categories = cat_in;
    input = input_in;
    size = size_in;
    weights = new float[size*categories];
    biases  = new float[categories];
    output  = new float[categories];
    probs   = new float[categories];
    answer  = -1;
}
FullLayer::~FullLayer(){
    delete[] weights;
    delete[] biases;
    delete[] output;
    delete[] probs;
}

void FullLayer::initialize(bool load_values, const std::string path){
    if(load_values){
        load_weights_biases(path);
    }else{ // Randomly generate weights (0-1)
        srand(static_cast <unsigned> (time(0)));
        for(int i = 0; i < size*categories; i++){
            weights[i] = (static_cast <float> (rand())) / (static_cast <float> (RAND_MAX)); 
        }
        for(int i = 0; i < categories; i++){
            biases[i] = (static_cast <float> (rand())) / (static_cast <float> (RAND_MAX)); 
        }
    }
}

void FullLayer::load_weights_biases(const std::string path){

    //std::ifstream my_file("network/layer0_conv_W");
    std::ifstream weights_file( (path+"_W").c_str() );

    std::string line, num;
    int pos;
    int i;
                                                        
    // Load weights
    if(weights_file.is_open()){
        i = 0;
        while(std::getline(weights_file,line)){
            while(line.length()>0){
                pos = line.find(",");
                if(pos != string::npos){
                    num = line.substr(0,pos);
                    line = line.substr(pos+1);
                    weights[i++] = atof(num.c_str());
                }else{
                    num = line;
                    line = string("");
                    weights[i++] = atof(num.c_str());
                }
            }
        }

    weights_file.close();

    }

    // Load biases 
    std::ifstream bias_file( (path+"_B").c_str() );
                                                        
    if(bias_file.is_open()){
        i = 0;
        while(std::getline(bias_file,line)){
            while(line.length()>0){
                pos = line.find(",");
                if(pos != string::npos){
                    num = line.substr(0,pos);
                    line = line.substr(pos+1);
                    biases[i++] = atof(num.c_str());
                }else{
                    num = line;
                    line = string("");
                    biases[i++] = atof(num.c_str());
                }
            }
        }

    bias_file.close();

    }
}

void FullLayer::inference(){
    //matrix multiplication and add biases
    int offset = 0;
    for(int j = 0; j < categories; j++){
        output[j] = biases[j];
        //output[j] = 0;
        offset = j*size;
        
        //stupid transform because of matlab reshape function takes elements column-wise
        int p = -1; int m = -1; int n = 0;
        for(int i = 0; i < size; i++){
            if(i%4 == 0) { n = 0; m++;}
            if(i%16 == 0){ p++; m = 0;}
            int idx = p*16+m+n*4; 
            n++;
            output[j] = output[j] + weights[offset+i]*input[idx]; 
        }
    }
}

void FullLayer::softmax(){
    float sum_probs = 0; 
    for(int i = 0; i < categories; i++){
        probs[i] = exp(output[i]);
        sum_probs = sum_probs + probs[i];
    }

    float max_prob = 0;
    for(int i = 0; i < categories; i++){
        probs[i] = probs[i]/sum_probs;
        if(probs[i] > max_prob){
            answer = (i+1)%10;
            max_prob = probs[i];
        }
    }
}


void FullLayer::save_network(){
    std::ofstream ofileW( (layer_name+"_W").c_str() );
    int weights_size = categories*size;
    if(ofileW.is_open()){
        print_file(ofileW, weights, weights_size);
        ofileW.close(); 
    }
    std::ofstream ofileB( (layer_name+"_B").c_str() );
    int biases_size = categories;
    if(ofileB.is_open()){
        print_file(ofileB, biases, biases_size);
        ofileB.close();
    }
}

void FullLayer::save_output(){
    std::ofstream ofile( (layer_name+"_O").c_str() );
    int output_size = categories;
    if(ofile.is_open()){
        print_file(ofile, output, output_size);
        ofile.close(); 
    }
}


float cost(float * probs, int size){

    float * logp = new float[size];
    for(int i = 0; i < size; i++){
        logp[i] = log(probs[i]);
        
    }
    delete []logp;

}


int main(){ 
    
    std::cout << "Hello, World" << std::endl;

    int batch_size = 10;
    int idx = 0;
    Image test_image[batch_size];
    
    load_mnist(test_image, idx, batch_size, "train");
   
    //test_image[0].print_label(); 
    //test_image[0].print_image(); 
    float input_image[28*28];
//    for(int i = 0; i < 28*28; i++){
//        input_image[i] = static_cast<float> (static_cast<unsigned char>(test_image[0].image[i])/255.0);
//    }

    
    int size;

    // Create a conv layer with 5x5 frame w/ 6 dimensions
    // x_weight, y_weight, depth, input array, x_in, y_in, channel_in
    ConvLayer layer0_conv(5,5,6,input_image,28,28,1,"test_network/layer0_conv");   

    // Create a pooling layer that does max pooling with dims of 2
    // pool_dim, input array, x_in, y_in, depth_in, layer_name
    PoolLayer layer0_pool(2, layer0_conv.output, layer0_conv.x_out, layer0_conv.y_out, layer0_conv.depth, "test_network/layer0_pool");

    ConvLayer layer1_conv(5,5,8,layer0_pool.output,layer0_pool.x_out,layer0_pool.y_out,layer0_pool.depth,"test_network/layer1_conv");   

    PoolLayer layer1_pool(2, layer1_conv.output, layer1_conv.x_out, layer1_conv.y_out, layer1_conv.depth, "test_network/layer1_pool");
    size = layer1_pool.x_out*layer1_pool.y_out*layer1_pool.depth;

    // Create a fully connected layer at the output
    // categories, input arr, input size
    FullLayer layer_full(10, layer1_pool.output, size, "test_network/layer_full");

    layer0_conv.initialize(true, "network/layer0_conv");    //Initialize with pretrained network 
    layer1_conv.initialize(true, "network/layer1_conv");    //Initialize with pretrained network 
    layer_full.initialize(true, "network/layer_full");      //Initialize with pretrained network  


    int correct = 0;
    for(int j = 0; j < batch_size; j++){
        for(int i = 0; i < 28*28; i++){
            input_image[i] = static_cast<float> (static_cast<unsigned char>(test_image[j].image[i])/255.0);
        }

        layer0_conv.inference();                            //input image, x_in, y_in, num_channels
        layer0_pool.inference();
        layer1_conv.inference();                            //input image, x_in, y_in, num_channels
        layer1_pool.inference();
        layer_full.inference();
        layer_full.softmax();
        
        // Print output
        //cout << "The label is " << (int)test_image[j].label;
        //cout << " and my guess is " << layer_full.answer << endl;
        if((int)test_image[j].label == layer_full.answer){
            correct++;
        }
    }
    float accuracy;
    accuracy = static_cast<float>(correct)/static_cast<float>(batch_size);
    cout << "Accuracy: " << accuracy << endl;
    
    //for(int i = 0; i < 10; i++){
    //    cout << layer_full.output[i] << " | ";
    //    cout << layer_full.probs[i] << endl;
    //}
    
    return 0;

}
