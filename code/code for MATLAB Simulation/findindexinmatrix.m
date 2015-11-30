function [ found_index ] = findindexinmatrix( aim_value, aim_matrix)

[matrix_size_row, matrix_size_col] = size(aim_matrix);
matrix_index = 1;
found_index = 0;
while matrix_index <= matrix_size_col
    if aim_matrix(matrix_index) == aim_value
        found_index = matrix_index;
        break
    end
    matrix_index = matrix_index + 1;    
end
%UNTITLED14 Summary of this function goes here
%   Detailed explanation goes here

end

