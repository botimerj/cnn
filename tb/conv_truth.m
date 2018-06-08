% Convolution Truth
function conv_truth 
    f_dims = randi(10,1,2);
    g_dims = randi(10,1,2);
    
    f = rand(f_dims(1), f_dims(2));
    g = rand(g_dims(1), g_dims(2));
    
    h = conv2(f,g);
    
    dlmwrite('f.txt',f);
    dlmwrite('g.txt',g);
    dlmwrite('h.txt',h);
end
