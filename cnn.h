#ifndef CNN_H
#define CNN_H

#include <iostream>
#include <string>

// Convolution functions
void conv(float*, float* , float*, int, int, int);
void conv2d(float*, float*, float*,  int, int, int, int, int, int);
void conv2d_valid(float*, float*, float*,  int, int, int, int, int, int);


void print_file();

// CNN layers
class ConvLayer{
    public:

    std::string layer_name;

    int x_in;
    int y_in;
    int channel_in;

    int x_weight;
    int y_weight;
    int depth;

    int x_out;
    int y_out;
    
    float * input; 
    float * weights; 
    float * biases; 
    float * output; 

    ConvLayer(int x_weight_in, 
              int y_weight_in, 
              int depth_in, 
              float * input_in, 
              int x_in_in, 
              int y_in_in, 
              int channel_in_in,
              std::string layer_name);
    ~ConvLayer();
    void initialize(bool load_values, const std::string path);
    void inference();


    //Print functions
    void save_network();
    void save_output();

    private: 
    float * calc_arr;

    void load_weights_biases(std::string path);
    void accumulate(float *arg, float *acc, int x_dim, int y_dim);
    void add_biases(float arg, float *acc, int x_dim, int y_dim);
};

class PoolLayer{
    public: 

    std::string layer_name;

    int x_in, y_in, depth;
    int pool_dim;
    int x_out, y_out;

    float * input;
    float * output;
    
    PoolLayer(int pool_dim_in, float * input_in, int x_in_in, int y_in_in, int depth_in, std::string layer_name_in);
    ~PoolLayer();
    void inference();
    void print();
    void save_output();

};

class FullLayer{
    public:

    std::string layer_name;

    int categories;
    int size;
    
    float * weights;
    float * biases;
    float * input;
    float * output;
    float * probs;

    int answer;

    FullLayer(int cat_in, float * input, int size, std::string layer_name_in);
    ~FullLayer();
    
    void initialize(bool load_values, const std::string path);
    void inference();
    void softmax();

    //Print functions
    void save_network();
    void save_output();

    private:
    void load_weights_biases(const std::string path);
    void load_biases(std::string path);
};

class CNN{

    int num_layers;

    ConvLayer * conv_layers;
    PoolLayer * pool_layers; 
    FullLayer * full_layer;
   
    CNN();
    ~CNN(); 
    
    
    
};

// Image holds the image [28x28 bytes] and label [1 byte] 
class Image{
    public: 
    char label;         // 0-9
    char image[784];    // 28x28 image
    void print_image(); // Print image to iostream
    void print_label(); // Print label
};

// Read mnist image and label  
void load_mnist(Image *, int, int, std::string);

#endif
