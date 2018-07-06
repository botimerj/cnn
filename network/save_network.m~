delete layer0_conv_W
delete layer0_conv_B
delete layer1_conv_W
delete layer1_conv_B
delete layer_full_W
delete layer_full_B


%Save network
for i = 1:cnn.layers{1}.numFilters
    transform = flip(rot90( cnn.layers{1}.W(:,:,1,i) ));
    transform = reshape(transform,[],1);
    dlmwrite('layer0_conv_W',transform,'-append')
    dlmwrite('layer0_conv_B',cnn.layers{1}.b(i),'-append')
end

for i = 1:cnn.layers{3}.numFilters
    for j = 1:6
        dlmwrite('layer1_conv_W',cnn.layers{3}.W(:,:,j,i),'-append')
    end
    dlmwrite('layer1_conv_B',cnn.layers{3}.b(i),'-append')
end

for i = 1:cnn.numClasses
    dlmwrite('layer_full_W',cnn.Wd(i,:),'-append')
    dlmwrite('layer_full_B',cnn.bd(i),'-append')
end

% for i = 1:cnn.layers{1}.filterDim
%     dlmwrite('layer1_conv_W',cnn.layers{3}.W(:,:,1,i),'-append')
% end
% 
% for i = 1:cnn.layers{1}.filterDim
%     dlmwrite('layer0_conv_W',cnn.layers{1}.W(:,:,1,i),'-append')
% end