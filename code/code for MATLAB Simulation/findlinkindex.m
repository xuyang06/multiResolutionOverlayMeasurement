function [ found ] = findlinkindex( link_matrix, src_index, dst_index)
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
link_index = 1;
[row_link_m, column_link_m] = size(link_matrix);
if src_index > dst_index
    tmp_index = dst_index;
    dst_index = src_index;
    src_index = tmp_index;
end
found = 0;
%row_link_m
while link_index <= row_link_m
    if (src_index == link_matrix(link_index,1)) && (dst_index == link_matrix(link_index,2))
        found = link_index;
        break
    end
    link_index = link_index + 1;
%     if src_index < link_matrix(link_index,1)
%         break
%     elseif src_index > link_matrix(link_index,1)
%         link_index = link_index + 1;
%         continue
%     else
%         if dst_index == link_matrix(link_index,2)
%             found = link_index;
%             break
%         else
%             
%         end
%     end
end


end


