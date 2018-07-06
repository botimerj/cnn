% Convolution Practice
clear all;

%function conv_practice
    new_test = 'N';

    %Special cases
    %One Element test
    f = [1];
    g = [1];
    h = conv2(f,g);
    dlmwrite('f.txt',f); dlmwrite('f.txt',new_test,'-append');
    dlmwrite('g.txt',g); dlmwrite('g.txt',new_test,'-append');
    dlmwrite('h.txt',h); dlmwrite('h.txt',new_test,'-append');
    
    %One dimension test
    f = [1, 2, 3];
    g = [3, 4, 5];
    h = conv2(f,g);
    dlmwrite('f.txt',f,'-append'); dlmwrite('f.txt',new_test,'-append');
    dlmwrite('g.txt',g,'-append'); dlmwrite('g.txt',new_test,'-append');
    dlmwrite('h.txt',h,'-append'); dlmwrite('h.txt',new_test,'-append');
    
    %One dimension (alternating dimensions) test
    f = [1, 2, 3];
    g = [3; 4; 5];
    h = conv2(f,g);
    dlmwrite('f.txt',f,'-append'); dlmwrite('f.txt',new_test,'-append');
    dlmwrite('g.txt',g,'-append'); dlmwrite('g.txt',new_test,'-append');
    dlmwrite('h.txt',h,'-append'); dlmwrite('h.txt',new_test,'-append');
    
    %Random testing
    for i = 1:2
        f_dims = randi(10,1,2);
        g_dims = randi(10,1,2);

        f = rand(f_dims(1), f_dims(2));
        g = rand(g_dims(1), g_dims(2));

        h = conv2(f,g);
 
        dlmwrite('f.txt',f,'-append'); dlmwrite('f.txt',new_test,'-append');
        dlmwrite('g.txt',g,'-append'); dlmwrite('g.txt',new_test,'-append');
        dlmwrite('h.txt',h,'-append'); dlmwrite('h.txt',new_test,'-append');
              
    end
    
    %conv2_valid testing
    f = [1.1,2,3; 1,2,3; 1,2,3];
    g = [1,2; 1,2.2];
    h = conv2(f,g,'valid');
    dlmwrite('f.txt',f,'-append'); dlmwrite('f.txt',new_test,'-append');
    dlmwrite('g.txt',g,'-append'); dlmwrite('g.txt',new_test,'-append');
    dlmwrite('h.txt',h,'-append'); dlmwrite('h.txt',new_test,'-append');
    
    load ../../mnist/cnn_4904_000638
    load ../../mnist/images1.mat
    f = images1;
    g = cnn.layers{1}.W(:,:,1);
    h = conv2(f,g,'valid');
    
    dlmwrite('f.txt',f,'-append'); dlmwrite('f.txt',new_test,'-append');
    dlmwrite('g.txt',g,'-append'); dlmwrite('g.txt',new_test,'-append');
    dlmwrite('h.txt',h,'-append'); dlmwrite('h.txt',new_test,'-append');
    
%end
