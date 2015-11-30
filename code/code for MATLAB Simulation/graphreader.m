function [ graph_matrix, link_matrix ] = graphreader1( file_path, dimension_m, aim_link_num )
graph_matrix = zeros(dimension_m);
link_matrix = [];
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
file_fid = fopen(file_path, 'r');
count = 0;
initial = 1;
while ~feof(file_fid)
    tline = fgetl(file_fid);
    if ~isempty(tline)
        if strcmp(tline,'EDGES (from-node to-node length a b):')
            count = 1;
            continue
        end
        if count == 1
            [c1,c2,c3,c4] = strread(tline, '%d %d %d %d');
            %link_matrix
            %[c1+1 c2+1]
            link_matrix(initial,:) = [(c1+1) (c2+1) c3];
            graph_matrix(c1+1,c2+1) = c3;
            graph_matrix(c2+1,c1+1) = c3;
            initial = initial + 1;
        end
    end
end

if initial < aim_link_num
    while initial <= aim_link_num
        src_c1 = int8(rand()*(dimension_m-1)) + 1;
        dst_c2 = int8(rand()*(dimension_m-1)) + 1;
        if src_c1 == dst_c2
            continue
        else
            if graph_matrix(src_c1, dst_c2) == 0
                cost_c3 = int8(rand()*9) + 1;
                if src_c1 < dst_c2
                    link_matrix(initial,:) = [src_c1 dst_c2 cost_c3];
                else
                    link_matrix(initial,:) = [dst_c2 src_c1 cost_c3];
                end
                graph_matrix(src_c1,dst_c2) = cost_c3;
                graph_matrix(dst_c2,src_c1) = cost_c3;
            else
                continue
            end
        end
        
        
        initial = initial + 1;
    end
end




end



