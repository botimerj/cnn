
#include <iostream>


void conv(float* f, float* g, float* h, int fx, int gx, int hx){
    std::cout << "performing convolutions" << std::endl;    

    for(int n = 0; n < hx; n++){
        h[n] = 0;
        int m = 0;
        while(m < gx || m < fx){
            if( (n-m >= 0) && (m < fx) && (n-m < gx) ) // if its inbounds
                h[n] = h[n] + f[m]*g[n-m]; 
            m++;
        }
    }

}

void conv2d(float* f, float* g, float* h, int fx, int fy, int gx, int gy, int hx, int hy){

    int nindex = 0;
    int mindex = 0;

    for(int ny = 0; ny < hy; ny++){
    for(int nx = 0; nx < hx; nx++){
        nindex = ny*hx + nx;
        h[nindex] = 0;
        int my = 0; 
        while(my < gy || my < fy){
            int mx = 0;
            while(mx < gx || mx < fx){
                bool xinbounds = (nx-mx >= 0) && (mx < fx) && (nx-mx < gx);
                bool yinbounds = (ny-my >= 0) && (my < fy) && (ny-my < gy);
                if(xinbounds && yinbounds){ 
                    mindex = (ny-my)*gx + (nx-mx);
                    h[nindex] = h[nindex] + f[my*fx + mx]*g[mindex]; 
                }
                mx++;
            }
            my++;
        }
    }
    }
}
