#include "cnn.h"
#include <fstream>
#include <string>

void Image::print_image(){
    for(int y = 0; y < 28; y++){
        for(int x = 0; x < 28; x++){
            if((unsigned int)image[28*y + x]<128)
                std::cout << '-';
            else
                std::cout << '0';
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Image::print_label(){
    std::cout << "Label: " << (int)label << std::endl;
}
    
// Takes the first 4 bytes and packs them into an int (if sizeof(int)==4) using big endian
int char4_to_int(char *char_arr){
    int num = 0;
    if(sizeof(int)==4){
        num = ((int)char_arr[0]<<24) + ((int)char_arr[1]<<16) + ((int)char_arr[2]<<8) + ((int)char_arr[3]);
    }
    return num;
} 

void load_mnist(Image * buffer, int idx, int num_elements, std::string data_set){
    std::ifstream f_labels;
    std::ifstream f_images;
    if(data_set == "test"){
        f_labels.open("mnist_dataset/t10k-labels.idx1-ubyte",std::ios::binary);
        f_images.open("mnist_dataset/t10k-images.idx3-ubyte",std::ios::binary);
    }else if(data_set == "train"){
        f_labels.open("mnist_dataset/train-labels.idx1-ubyte",std::ios::binary);
        f_images.open("mnist_dataset/train-images.idx3-ubyte",std::ios::binary);
    }else{
        std::cout << "Not a valid data set" << std::endl;
        return;
    }

    // Read labels
    if(f_labels.is_open()){
        f_labels.seekg(0, f_labels.end);
        int length = f_labels.tellg();
        //Check if inbounds 
        if(idx + num_elements <= (length-8)){               
            f_labels.seekg(8+idx, f_labels.beg);            //labels start at byte 9
            for(int i = 0; i < num_elements; i++){
                f_labels.read(&buffer[i].label, 1); // assume auto increment
            }
        }else{
            std::cout << "ERROR: MNIST READ OUT OF BOUNDS" << std::endl;
        }
        f_labels.close();
    }else{
        std::cout << "ERROR: File not found" << std::endl;
    }

    // Read images
    if(f_images.is_open()){
        f_images.seekg(0, f_images.end);
        int length = f_images.tellg();
        //Check if inbounds 
        if((idx*784 + num_elements*784) <= (length-16)){
            f_images.seekg(16+idx*784, f_images.beg);       //images start at byte 17
            for(int i = 0; i < num_elements; i++){
                f_images.read(buffer[i].image, 784);
            }
        }else{
            std::cout << "ERROR: MNIST READ OUT OF BOUNDS" << std::endl;
        }
        f_images.close();
    }else{
        std::cout << "ERROR: File not found" << std::endl;
    }
}


/*int main(){ 
    
    int batch_size = 3;
    int idx = 9998;
    Image test_image[batch_size];
    
    load_mnist(test_image, idx, batch_size, "train");
   
    test_image[0].print_label(); 
    test_image[0].print_image(); 

    return 0;
}*/

